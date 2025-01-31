#include "codegen.hpp"
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

// ... Existing CodeGen methods ...

void CodeGen::runJIT() {
    // Initialize LLVM targets
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Create an ExecutorProcessControl
    auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
    if (!EPC) {
        std::cerr << "Failed to create ExecutorProcessControl: "
                  << llvm::toString(EPC.takeError()) << "\n";
        return;
    }

    // Create ExecutionSession
    llvm::orc::ExecutionSession execSession(std::move(*EPC));

    // Obtain the target triple from the current module
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    module.setTargetTriple(targetTriple);

    // Create JITTargetMachineBuilder
    llvm::orc::JITTargetMachineBuilder JTMB(targetTriple);

    // Create DataLayout
    auto DL = JTMB.getDefaultDataLayoutForTarget();
    if (!DL) {
        std::cerr << "Failed to get DataLayout: " << llvm::toString(DL.takeError()) << "\n";
        return;
    }

    // Create IRCompileLayer with ConcurrentIRCompiler
    llvm::orc::IRCompileLayer compileLayer(
        execSession,
        llvm::orc::ConcurrentIRCompiler(std::move(JTMB)));

    // Create main JITDylib and add dynamic library search
    auto &mainJD = execSession.createBareJITDylib("main");
    mainJD.addGenerator(llvm::cantFail(
        llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            (*DL)->getGlobalPrefix())));

    // Clone the module to transfer ownership to the JIT
    std::unique_ptr<llvm::Module> clonedModule = llvm::CloneModule(module);
    llvm::orc::ThreadSafeModule TSM(
        std::move(clonedModule),
        std::make_unique<llvm::LLVMContext>());

    // Add the module to the JIT
    if (auto err = compileLayer.add(mainJD, std::move(TSM))) {
        std::cerr << "Failed to add module to JIT: " << llvm::toString(std::move(err)) << "\n";
        return;
    }

    // Look up the 'main' function
    auto mainSym = execSession.lookup({&mainJD}, "main");
    if (!mainSym) {
        std::cerr << "Failed to lookup 'main': " << llvm::toString(mainSym.takeError()) << "\n";
        return;
    }

    // Cast the symbol to a function pointer and execute
    auto *mainFunc = (void (*)())(mainSym->getAddress());
    mainFunc();

    // Shutdown the execution session
    if (auto err = execSession.endSession()) {
        std::cerr << "Error shutting down session: " << llvm::toString(std::move(err)) << "\n";
    }
}