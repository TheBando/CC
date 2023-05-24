#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Flex & Bison
extern int yyparse();

int main(int argc, const char* argv[]) {
    // Validate arguments
    if (argc != 2) {
        printf("Usage: ./parse <file>\n");
        exit(1);
    }

    // Attempt to open the input file
    FILE* fp = freopen(argv[1], "r", stdin);
    if (fp == NULL) {
        printf("[ERROR] Unable to open file \"%s\"", argv[1]);
        exit(1);
    }

    // Attempt to parse the input file
    int ret = -1;
    if (yyparse() == 0) {
        ret = 0;
        printf("PASSED");
    } else {
        printf("FAILED");
    }
    printf("\n\n");

    // Close the input file
    fclose(fp);

    return ret;
}
