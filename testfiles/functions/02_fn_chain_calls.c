int add(int a, int b) {
    return a + b;
}

int mul3(int x) {
    return x * 3;
}

int main() {
    return mul3(add(3, 4));
}
