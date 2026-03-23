int main() {
    int x = 1;
    int y = 2;
    int* p;
    p = &x;
    *p = y + 3;
    return x;
}
