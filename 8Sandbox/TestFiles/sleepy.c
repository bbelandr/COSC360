#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int seconds = atoi(argv[1]);
    sleep(seconds);

    return 0;
}