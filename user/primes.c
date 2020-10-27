#include "user/user.h"

void primes()
{
    int p[2];
    pipe(p);
    int num;

    if (read(0, &num, sizeof(num)))
    {
        fprintf(1, "prime %d\n", num);
        if (fork() == 0)
        {
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);
            primes();
        }
        else
        {
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            int i;
            while (read(0, &i, sizeof(i)))
            {
                if (i % num != 0)
                {
                    write(1, &i, sizeof(i));
                }
            }
        }
    }
    exit();
}

int main(int argc, char *argv)
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        primes();
    }
    else
    {
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        int i;
        for (i = 2; i < 36; i++)
        {
            write(1, &i, sizeof(i));
        }
    }
    exit();
}