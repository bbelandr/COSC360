#include <stdio.h>

static int init(void)
{
    printf("Initialized hello plugin.\n");
    return 0;
}

static void fini(void)
{
    printf("Unloading hello plugin.\n");
}

static int cmd(char *str)
{
    printf("Hello got command %s\n", str);
    return 0;
}

struct {
    char name[65];
    int  (*init)(void);
    void (*fini)(void);
    int  (*cmd)(char *str);
} export = {"hello", init, fini, cmd};
