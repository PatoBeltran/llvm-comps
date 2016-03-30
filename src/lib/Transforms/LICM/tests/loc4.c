#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args) {
    int x = atoi(args[1]);
    int y = atoi(args[2]);
    int z = atoi(args[3]);

    int k;
    int i = 0;
    int j = 0;
    int l = 0;
    int o = 0;
    int q = 0;

    int thing[10];

    thing[1] = 10;

    do {
        k = z / x;
        i = i + 1;
    } while (i < 5);

    exit(0);

    for (i = 0; i < 2; i++) {
        for (int p = 0; p < 2; p++) {
            int a = x * y;
            int b = z << 2;
    
            j = b + a; 
            l = 3 * b;
            o = p + i; 
            q = y + thing[x];
        }
    }

    printf("%d %d %d %d %d\n", k, j, l, o, q);
}
