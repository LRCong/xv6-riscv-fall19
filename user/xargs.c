#include "user.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char buf2[32][32];
    char *pass[32];

    for (int i = 0; i < 31; i++)
    {
        pass[i] = buf2[i];
    }

    for (int i = 1; i < argc; i++)
    {
        strcpy(buf2[i - 1], argv[i]);
    }

    while (read(0, buf, sizeof(buf2)) > 0)
    {
        int pos = argc - 1;
        char *c = buf2[pos];
        for (char *p = buf; *p; p++)
        {
            if (*p == ' ' || *p == '\n')
            {
                *c = '\0';
                pos++;
                c = buf2[pos];
            }
            else
            {
                *c = *p;
                c++;
            }
        }
        pass[pos] = 0;

        if (fork())
        {
            wait();
        }
        else
        {
            exec(pass[0], pass);
            exit();
        }
    }

    exit();
}