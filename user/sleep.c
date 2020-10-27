#include "user/user.h"
#include "kernel/types.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(0, "You have to input a number!");
    }
    int put = atoi(argv[1]);
    fprintf(0, "Now you have fall asleep.\n");
    sleep(put);
    fprintf(0, "Now you have wake up.\n");
    exit();
}