#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int
main(int argc, char *argv[]) {
    char buffer[100];
    FILE *fpt = fopen("output", "w");
    assert(fpt);

    while(fscanf(stdin, "%[^\n]", buffer) != -1) {
        fprintf(fpt, "%s \n", buffer);
    }

    fclose(fpt);
}