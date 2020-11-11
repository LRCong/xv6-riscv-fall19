#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char whitespace[] = " \t\r\n\v";

int getcmd(char *buf, int nbuf)
{
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if (buf[0] == '\0')
        return -1;
    return 0;
}

void setargs(char *cmd, char *argv[], int *argc)
{
    int i, j;
    for (i = 0, j = 0; cmd[j] != '\n' && cmd[j] != '\0'; j++)
    {
        while (strchr(whitespace, cmd[j]))
        {
            j++;
        }
        argv[i++] = cmd + j;
        while (strchr(whitespace, cmd[j]) == 0)
        {
            j++;
        }
        cmd[j] = '\0';
        // fprintf(2, "%s\n", argv[i - 1]);
    }
    argv[i] = 0;
    *argc = i;
}

void runcmd(char *argv[], int argc);

void myPipe(char *argv[], int argc)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            argv[i] = 0;
            break;
        }
    }

    int pd[2];
    pipe(pd);

    if (fork() == 0)
    {
        close(1);
        dup(pd[1]);
        close(pd[0]);
        close(pd[1]);
        runcmd(argv, i);
    }
    else
    {
        close(0);
        dup(pd[0]);
        close(pd[0]);
        close(pd[1]);
        runcmd(argv + i + 1, argc - i - 1);
    }
}

void runcmd(char *argv[], int argc)
{
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            myPipe(argv, argc);
        }
    }
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], ">"))
        {
            close(1);
            open(argv[i + 1], O_CREATE | O_WRONLY);
            argv[i] = 0;
        }
        if (!strcmp(argv[i], "<"))
        {
            close(0);
            open(argv[i + 1], O_RDONLY);
            argv[i] = 0;
        }
    }
    exec(argv[0], argv);
    exit(0);
}

int main()
{
    char buf[100];
    while (getcmd(buf, sizeof(buf)) >= 0)
    {
        if (fork() == 0)
        {
            char *argv[10];
            int argc = -1;
            setargs(buf, argv, &argc);
            runcmd(argv, argc);
        }
        wait(0);
    }
    exit(0);
}