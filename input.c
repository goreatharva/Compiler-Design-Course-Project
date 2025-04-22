

int add(int a, int b) {
    return a + b;
}

void printHello() {
    printf("Hello, World");
}

int main() {
    int result;
    result = add(5, 10);
    if (result > 10) {
        printHello();
    } else {
        printf("Result is small");
    }
    return 0;
}
