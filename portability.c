#include "portability.h"

/* Since this is the implementation of wrappers, we must disable the 'aliases'
 * absolutely. Else, we will call them recursively */
#undef gotoligcol
#undef kbhit
#undef sleep
#undef system
#undef fflush
#ifdef _WIN32
# undef strcasecmp
#endif


static unsigned int _portability_color_bg = COLOR_DEFAULT;
static unsigned int _portability_color_fg = COLOR_DEFAULT;

int
portability_system_call(const char *cmd)
{
   int ret = -1;

   if (!cmd) return -1;

   if (strcasecmp(cmd, "pause") == 0)		// Si on veut faire une pause
     {
        printf("Press any key to continue...");
        ret = getchar();  // Attend la saisie d'un caractère
     }
   else if ((strcasecmp(cmd, "cls") == 0) ||
            (strcmp(cmd, "clear") == 0))
     {
#ifdef _WIN32	    // Si on est sous Windows
        ret = system("cls");
#else		    // Si on est sous Linux/Mac
        ret = system("clear");
#endif
     }
   else
     {
        // Si ce n'est pas une commande gérée, on la transmet tel quel
        ret = system(cmd);
     }
   return ret;
}

void
portability_clear_buffer(FILE* f)
{
    char c;

    if (!f) return;

    // Si on veux vider le buffer d'entrée
    if (f == stdin)
    {
	// Boucle vidant le buffer caractère par caractère
        while( (c=getchar()) != '\n' && c != EOF) ;
    }
    else    // Si on veux vraiment flush un flux de sortie
    {
        fflush(f);
    }
}

#ifndef _WIN32

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

#endif

int portability_kbhit()
{
   int val;
    #ifdef _WIN32
        val = kbhit();
    #else
        portability_change_terminal_mode(1);
        struct timeval tv = {0, 0};
        fd_set rdfs;

        FD_ZERO(&rdfs);
        FD_SET( STDIN_FILENO, &rdfs);

        val = select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv) == 1;
        //return FD_ISSET(STDIN_FILENO, &rdfs);
    #endif

        return val;
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
        printf("%c[%d;%df", 0x1B, poslig, poscol); // How about line buferring here?!
    #endif
}


static void
_portability_color_apply()
{

#ifdef _WIN32
        HANDLE console;
        unsigned int color;

        color = 0;
        console = GetStdHandle(STD_OUTPUT_HANDLE);

        if(_portability_color_fg == COLOR_RED)
                color |= FOREGROUND_RED;
        if(_portability_color_fg == COLOR_BLUE)
                color |= FOREGROUND_BLUE;
        if(_portability_color_fg == COLOR_GREEN)
                color |= FOREGROUND_GREEN;
        if(_portability_color_fg == COLOR_GRAY)
                color |= FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;

        if(_portability_color_bg == COLOR_RED)
                color |= BACKGROUND_RED;
        if(_portability_color_bg == COLOR_BLUE)
                color |= BACKGROUND_BLUE;
        if(_portability_color_bg == COLOR_GREEN)
                color |= BACKGROUND_GREEN;
        if(_portability_color_bg == COLOR_GRAY)
                color |= BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE;
        //if(_portability_color_bg == COLOR_BLACK)
        //        color |= BACKGROUND_BLACK;

        SetConsoleTextAttribute(console, color);
#else

        printf("\033[0m");

        if(_portability_color_bg != COLOR_DEFAULT &&
           _portability_color_fg != COLOR_DEFAULT)
          printf("\033[0;%i;%im", _portability_color_fg,
                 _portability_color_bg + 10);
        else if(_portability_color_bg != COLOR_DEFAULT)
          printf("\033[7;%im", _portability_color_bg);
        else if(_portability_color_fg != COLOR_DEFAULT)
          printf("\033[0;%im", _portability_color_fg);

#endif
}

void
portability_background_color(unsigned int color)
{
        _portability_color_bg = color ;
        _portability_color_apply();
}

void
portability_text_color(unsigned int color)
{
        _portability_color_fg = color ;
        _portability_color_apply();
}

