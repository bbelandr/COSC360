#include <stdio.h>
#include <string.h>

static int init(void)
{
    printf("FROM PLUGIN: Hello from %s\n", __FILE__);
    return 0;
}

static void fini(void)
{
    printf("FROM PLUGIN: Goodbye from %s\n", __FILE__);
}

static int cmd(char *str)
{
    printf("Running command '%s'\n", str);
    if (!strcmp("cosc360", str)) {
        printf("   -> Hello COSC360!\n");
        // Command Handled
        return 1;
    }
    // If it's not cosc360, say we didn't handle it.
    return 0;
}

struct {
    char name[65];
    int (*init)(void);
    void (*fini)(void);
    int (*cmd)(char *str);
} export = {"cosc360", init, fini, cmd};