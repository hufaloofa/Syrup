/* NOTE: this is just a page for me to test functions */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// note isalpha takes in an **int** value

int main() {
    if (isalpha('a')) {
        printf("true");
    } else {
        printf("false");
    }
    return 0;
}