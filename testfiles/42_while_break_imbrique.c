int main() {
    int i = 0;
    int j = 0;
    while (i < 3) {
        j = 0;
        while (j < 10) {
            if (j == 2) {
                break;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return j;
}