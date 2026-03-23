int f(int a, int b) {
    int x = a ^ b;
    int y = a & b;
    int z = x | y;
    if (z != 0) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    return f(12, 10);
}
