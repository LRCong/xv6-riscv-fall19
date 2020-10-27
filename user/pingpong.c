#include "user/user.h"

int main(int argc, char *argv)
{
    char buf[64];
    int p[2];
    int c[2];
    pipe(p);
    pipe(c);
    if (fork() == 0)
    {
        read(p[0], buf, 4);
        fprintf(2, "%d: received %s\n", getpid(), buf);
        write(c[1], "pong", 4);
    }
    else
    {
        write(p[1], "ping", 4);
        read(c[0], buf, 4);
        fprintf(2, "%d: received %s\n", getpid(), buf);
    }
    exit();
}
