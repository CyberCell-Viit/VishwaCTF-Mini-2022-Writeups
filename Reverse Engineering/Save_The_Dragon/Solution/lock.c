#include <stdio.h>
#include <string.h>
#include <conio.h>

void path2flag(char spell[])
{
    printf("Ghidra is freed, he would like to reward you with a flag !!!\n");
    for (int i = 0; i < strlen(spell); i++)
    {
        printf("%c", (char)((int)spell[i] + 4));
    }
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        int a = 0;
        int var[12] = {61, 108, 97, 110, 101, 76, 107, 110, 112, 93, 105};
        if (strlen(argv[1]) < 11)
        {
            printf("Think Big !!!!");
        }
        else
        {
            for (int i = 0; i < strlen(argv[1]); i++)
            {
                if ((int)argv[1][i] == var[i])
                {
                    a = a + 1;
                }
            }
            if (a == 11)
            {
                path2flag(argv[1]);
            }
            else
            {
                printf("Wrong Spell");
            }
        }
    }
    else
    {
        printf("Cast your spell in Command Line as an arguement.");
    }
    getch();
    return 0;
}