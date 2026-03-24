int score(int a, int b, int c) {
    int x = a + b;
    int y = x * c;
    int z = y - a;
    return z + b;
}

int main() {
    return score(4, 5, 6);
}
