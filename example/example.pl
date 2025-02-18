// Simple program with function calls, conditionals, and loops

int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 5;
    int y = 10;
    
    if (x < y) {
        // Inside the if block
        int result = add(x, y);
        print(result); // Print the result of the add function
    } else {
        // Inside the else block
        print(0); // Print 0 if x is not less than y
    }

    // While loop example
    int counter = 0;
    while (counter < 5) {
        counter = add(counter, 1);
        print(counter);
    }

    return 0;
}
