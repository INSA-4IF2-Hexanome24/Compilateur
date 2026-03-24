int add(int a, int b) {
    return a + b;
}

int mul(int a, int b) {
    return a * b;
}

int sub(int a, int b) {
    return a - b;
}

int main() {
    return sub(mul(add(1, 2), 5), 3);
}
