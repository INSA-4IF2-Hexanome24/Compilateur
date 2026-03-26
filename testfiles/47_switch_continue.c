int main() {
    int i = 0;
    int sum = 0;
    continue;
    while (i < 5) {
        i = i + 1;
        switch (i) {
            case 3: continue; 
            default: break;
        }
        sum = sum + i;
    }
    return sum;
}