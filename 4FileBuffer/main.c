#include "bufread.h"
#include <stdio.h>

int main() {

    BufReader *br = br_open("input", 5, 3);
    printf("This is where the pointer is: %d\n", br_tell(br));


    br_seek(br, 5, SEEK_SET);



    for (int i = 0; i < 28; i++) {
        printf("%c", br_getchar(br));
    }
    printf("\n");


    // br_seek(br, 2, SEEK_SET);
    // printf("This is where the pointer is: %d\n", br_tell(br));
    
    
    // printf("This is the first char: %c\n", br_getchar(br));



    br_close(br);
    return 0;
}