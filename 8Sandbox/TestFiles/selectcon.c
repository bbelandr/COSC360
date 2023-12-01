#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
int
main(void)
{
    unsigned long tm = 0;
    char data[256];
    ssize_t bytes;
    int nfds;
    fd_set rds;
    
    while (tm < 30000000UL)
    {
        FD_ZERO(&rds);
        FD_SET(STDIN_FILENO, &rds);
        nfds = select(STDIN_FILENO + 1, &rds, 0, 0, &(struct timeval){0, 1000});
        if (nfds > 0 && FD_ISSET(STDIN_FILENO, &rds))
        {
            bytes = read(STDIN_FILENO, data, 255);
            if (bytes < 0)
            {
                perror("read");
                break;
            }
            else if (bytes == 0)
            {
                printf("EOF received, quitting....\n");
                break;
            }
            else
            {
                data[strcspn(data, "\n")] = '\0';
                printf("Data = \"%s\"\n", data);
                printf(">");

            }
        }
        tm += 1;
    }
    printf("Terminated with tm = %lu.\n", tm);
}
