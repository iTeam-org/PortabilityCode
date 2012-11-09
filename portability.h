#ifndef PORTABILITY_H_INCLUDED
#define PORTABILITY_H_INCLUDED

/*! Fonctions de portabilités transparente pour porter un code Windows tel quel sous Linux et Mac
 *  Fonctions non-portable gérées :
 *  system("PAUSE");
 *  system("CLS");
 *  fflush(stdin);
 *  kbhit();
 *  getch(); ?
 *  sleep();
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/time.h>
#endif

int portability_system_call(const char* cmd);
void portability_clear_buffer( FILE* f);
void portability_change_terminal_mode( int dir);
int portability_kbhit();
unsigned int portability_sleep(unsigned int time);
void portability_gotoligcol(int poslig, int poscol);

int portability_system_call(const char* cmd)
{
    char* command = (char*) malloc(sizeof(char) * (strlen(cmd) + 1));
    int i;

    for (i = 0 ; i < strlen(cmd) ; i++)
        command[i] = toupper(cmd[i]);

    command[strlen(cmd)] = '\0';

    if (strcmp(command, "PAUSE") == 0)
    {
        printf("Press any key to continue...");
        getchar();
    }
    else if (strcmp(command, "CLS") == 0)
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    else
    {
        system(cmd);
    }

    free(command);

    return 0;
}

void portability_clear_buffer(FILE* f)
{
    if (f == stdin)
    {
        char c;
        while( (c=getchar()) != '\n' && c != EOF);
    }
    else
    {
        fflush(f);
    }
}

void portability_change_terminal_mode(int dir)
{
    static struct termios old_term, new_term;

    if (dir == 1)
    {
        tcgetattr( STDIN_FILENO, &old_term);
        new_term = old_term;
        new_term.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &new_term );
    }
    else
    {
        tcsetattr( STDIN_FILENO, TCSANOW, &old_term);
    }
}

int portability_kbhit()
{

    #ifdef _WIN32
        kbhit();
    #else
        portability_change_terminal_mode(1);
        struct timeval tv = {0, 0};
        fd_set rdfs;

        FD_ZERO(&rdfs);
        FD_SET( STDIN_FILENO, &rdfs);

        return select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv) == 1;
        //return FD_ISSET(STDIN_FILENO, &rdfs);
    #endif
}

unsigned int portability_sleep(unsigned int time)
{
    #ifdef _WIN32
        Sleep(time);
        return 0;
    #else
        return usleep(time * 1000);
    #endif
}

void portability_gotoligcol(int poslig, int poscol)
{
    #ifdef _WIN32
        COORD mycoord;
        mycoord.X = poscol;
        mycoord.Y = poslig;
        SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
    #else
        printf("%c[%d;%df", 0x1B, poslig, poscol);
    #endif
}

#define gotoligcol(x, y) portability_gotoligcol(x, y)

#define kbhit() portability_kbhit()

#define Sleep(time) portability_sleep(time)

#define system(arg) portability_system_call(arg)

#define fflush(arg) portability_clear_buffer(arg)

#endif // PORTABILITY_H_INCLUDED
