int a;
a = 5;

while (a) {
    a = a - 1;

    if (a == 3) {
        continue;
    }

    if (a == 1) {
        break;
    }
}

return a;