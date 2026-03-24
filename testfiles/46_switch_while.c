int main() {
    int i = 0;
    int sum = 0;
    while (i < 4) {
        switch (i) {
            case 0: sum = sum + 10; break;
            case 1: sum = sum + 20; break;
            case 2: sum = sum + 30; break;
            default: sum = sum + 99;
        }
        i = i + 1;
    }
    return sum;
}