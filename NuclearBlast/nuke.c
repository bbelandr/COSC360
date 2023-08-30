// Benjamin Belandres

#include <stdio.h>
#include <stdlib.h>

struct Point {  // Holds an x and y coordinate on the grid
    int x;
    int y;
};

struct Person {
    struct Point location;
    char name[65];  // Takes a maximum of 64 characters plus the null character at the end
};

static double distance(const struct Point *p1, const struct Point *p2); // Calculates the distance from point1 to point2

int main(int argc, char *argv[]) {
    
    // Read in the command line arguments
    if (argc != 5) {
        printf("Usage: ./nuke <blast strength> <attenuation> <blast x> <blast y>\n");
        return 1;
    }

    double blastStrength = strtod(argv[1]);
    double attenuation = strtod(argv[2]);
    int blastX = strtoi(argv[3]);
    int blastY = strtoi(argv[4]);



    
    struct Person currentPerson;
    // while Read in a person formatted like:
        // x y name


    return 0;
}

static double distance(const struct Point *p1, const struct Point *p2) {
    
    
    return -1;
}
