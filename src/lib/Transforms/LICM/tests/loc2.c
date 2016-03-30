#include <stdio.h>
#include <stdlib.h>

void maybe(int num) {
   if (num != 2) {
        exit(1);
   }
   return;
}

int main(int argc, char** args) {
    // pass in 1, 2, 1
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
        maybe(y); // this MIGHT call exit; should still do lifting
        k = z / x; // should lift this since it dominates all exits of while
        i = i + 1; // don't lift this; i changes in the loop
    } while (i < 5);

    i = 0;

    while (i < 5) {
        k = z / x; // don't lift this 1; doesn't dominate all exits of loop
        i = i + 1;
    }

    for (i = 0; i < 2; i++) {
        for (int p = 0; p < 2; p++) {
            int a = x * y; // x * y should be lifted
            int b = z << 2; // z << 2 should be lifted
    
            j = b + a; 
            l = 3 * b;
            o = p + i; 
            q = y + thing[x];
        }
    }

    // if pass in 1, 2, 1, k = 1, j = 6, l = 12, o = 2, q = 12
    printf("%d %d %d %d %d\n", k, j, l, o, q);
}
