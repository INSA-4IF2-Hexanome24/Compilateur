int main() {
    int i = 0;
    int s = 0;
    for (i=0 ; i<10 ; i=i+1) {
        if (i == 5) {
            s = s + 100;
        }
        s = s + 1;
    }
    return s;
}