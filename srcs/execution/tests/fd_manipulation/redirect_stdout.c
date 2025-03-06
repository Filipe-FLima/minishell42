#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    int fd = open("teste.txt", O_WRONLY | O_CREAT | O_TRUNC , 0777);
    if (fd == -1)
        return (1);

    printf("Fd do arquivo test = %d\n", fd);
    int fd2 = dup2(fd, 1);
    close(fd);
    printf("aaaaa\n");
    return(0);
}