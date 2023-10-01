struct BufReader;
typedef struct BufReader BufReader;

BufReader *br_open(const char *path, int capacity, int fill_trigger);
void  br_close(BufReader *br);
void  br_seek(BufReader *br, int offset, int whence);
int   br_tell(BufReader *br);
char *br_getline(char s[], int size, BufReader *br);
int   br_getchar(BufReader *br);
int   br_read(BufReader *br, char *dest, int max_bytes);