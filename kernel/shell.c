
#include <types.h>
#include <debug.h>
#include <stdio.h>
#include <asm.h>
#include <kernel_head.h>
#include <string.h>


extern void prog1();
extern void prog2();
extern void prog3();
extern void prog4();
extern void prog5();
extern void prog6();

char* prog[] = {"tantantan_LU", "flash_RU", "tantantan_LD", "tantantan_RD", "parallel", "count letters"};


void exeCommand(const char *command);
void help();
void ls();

int shell(void)
{
   
    exeCommand("HELP");

    char command[10];
    while (1)
    {
        puts(">");
        gets(command, 9);
        exeCommand(command);
    }

    return 0;
}

void exeCommand(const char *command)
{
    strip(command);
    if (strlen(command) == 0)
    {
        puts("\n>");
    }
    if (strcmp(command, "HELP") == 0)
    {
        help();
    }
    else if (strcmp(command, "LS") == 0)
    {
        ls();
    }
    else if (strcmp(command, "CLS") == 0)
    {
        cls();
    }
    else
    {
        //two cases
        char EXE[] = "EXE";
        if (strncmp(command, EXE, 3) == 0 && strlen(command) >= 5 && command[4] >= '1' && command[4] <= '9')
        {
            cls();
            switch(command[4] - '0')
            {
                case 1:
                    prog1();
                    break;
                case 2:
                    prog2();
                    break;
                case 3:
                    prog3();
                    break;
                case 4:
                    prog4();
                    break;
                case 5:
                    prog5();
                    break;
                case 6:
                    prog6();
                    break;
            }
        }
        else
        {
            //无效命令
            exeCommand("HELP");
        }
    }
}

void help()
{
    puts("=================================SILVER47OS====================================\n");
    puts("|                Silver47 support the following commands:                     |\n");
    puts("|                     1:HELP                   show this page                 |\n");
    puts("|                     2:LS                     list the user programs         |\n");
    puts("|                     3:EXE programTag         execute the selected programs, |\n");
    puts("|                                              use Q to return                |\n");
    puts("|                     4:CLS                    clear the screen               |\n");
    puts("===============================================================================\n");
    puts("|                                  ATTENTION:                                 |\n");
    puts("|                      EXE 6  to execute the counter program                  |\n");
    puts("|                       EXE programTag only allow one space                   |\n");
    puts("|                              By: 16337092 Huang Junyi                       |\n");
    puts("===============================================================================\n");
}

void ls()
{
    for(int i = 0; i < 6; i++)
    {
        puts(prog[i]);
        puts("\n");
    }
}

