/* ***************************************************************************
 *	Cette bibliothèque de fonctions est distribuée librement par l'iTeam !
 *
 *  Son but est de fournir un moyen simple pour compiler les codes enseignés
 *  à l'ECE (qui ne marchent correctement que sous Windows).
 *  Nous vous invitons à l'utiliser et l'améliorer. La dernière version
 *  est disponible sur https://github.com/iTeam-Projects/PortabilityCode
 *
 *
 *			iTeam - association de Promotion du Logiciel Libre
 * ***************************************************************************/

#include "portability.h"

/* Since this is the implementation of wrappers, we must disable the 'aliases'
 * absolutely. Else, we will call them recursively */
#undef gotoligcol
#undef kbhit
#undef Sleep
#undef system
#undef fflush

static unsigned int _portability_color_bg = COLOR_DEFAULT;
static unsigned int _portability_color_fg = COLOR_DEFAULT;
static unsigned char _init = 0;

#ifdef ERR
# undef ERR
#endif
#define ERR(msg, ...) do { \
     fprintf(stderr, "*** Error in function %s: " msg "\n", \
             __func__, ## __VA_ARGS__); \
     fprintf(stderr, \
             "*** NAUGHTY PROGRAMMER!!!\n" \
             "*** SPANK SPANK SPANK!!!\n" \
             "*** Now go fix your code. Tut tut tut!\n\n"); } while (0)

/*~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=*
 *                                 Private API                                *
 *=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~*/

static void
_portability_change_terminal_mode(int dir)
{
#ifndef _WIN32
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
#else
   (void) dir; // To avoid warning with -Wunused-parameter
#endif
}

static void
_portability_color_apply(void)
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

   if(_portability_color_bg != COLOR_DEFAULT &&
      _portability_color_fg != COLOR_DEFAULT)
     printf("\033[0;%i;%im", _portability_color_fg,
            _portability_color_bg + 10);
   else if(_portability_color_bg != COLOR_DEFAULT)
     printf("\033[0;%im", _portability_color_bg + 10);
   else if(_portability_color_fg != COLOR_DEFAULT)
     printf("\033[0;%im", _portability_color_fg);
   else
     printf("\033[0;m");

#endif
}


/*~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=*
 *                                 Public API                                 *
 *=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~*/

void
portability_init(void)
{
   int chk;

   /* Disable line buffering - setbuf is deprecated and its usage must be
    * avoided as much as possible (cf. man 3 - BSD) */
   chk = setvbuf(stdout, NULL, _IONBF, 0);
   chk *= setvbuf(stderr, NULL, _IONBF, 0);

   /* Lib has been properly initiated (or not) */
   if (chk == 0)
     _init = 1;
   else
     fprintf(stderr, "*** Error in %s: %s\n", __func__, strerror(errno));
}

void
portability_shutdown(void)
{
   /* Nothing to implement here for now */
}


int
portability_system_call(const char *cmd)
{
   int ret = -1;

   if (!cmd) return -1;

   if (strcasecmp(cmd, "pause") == 0)
     {
        printf("Press any key to continue...");
        ret = getchar();
     }
   else if ((strcasecmp(cmd, "CLS") == 0) ||
            (strcmp(cmd, "clear") == 0))
     {
#ifdef _WIN32
        ret = system("CLS");
#else
        ret = system("clear");
#endif
     }
   else
     {
        ret = system(cmd);
     }

   return ret;
}

void
portability_clear_buffer(FILE *f)
{
   char c;

   // Si on veut vider le buffer d'entrée
   if (f == stdin)
     {
        // Boucle vidant le buffer caractère par caractère
        while( (c=getchar()) != '\n' && c != EOF) ;
     }
   else    // Si on veut vraiment flush un flux de sortie
     {
        fflush(f);
     }
}

int
portability_kbhit(void)
{
   int val;
#ifdef _WIN32
   val = kbhit();
#else
   _portability_change_terminal_mode(1);
   struct timeval tv = {0, 0};
   fd_set rdfs;

   FD_ZERO(&rdfs);
   FD_SET( STDIN_FILENO, &rdfs);

   val = select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv) == 1;
   //return FD_ISSET(STDIN_FILENO, &rdfs);
#endif

   return val;
}

void
portability_sleep(unsigned int time)
{
#ifdef _WIN32
   Sleep(time);
#else
   (void) usleep(time * 1000.0);
#endif
}

void
portability_gotoligcol(int poslig, int poscol)
{
   if (!_init)
     {
        ERR("You didn't initialize portability with portability_init()\n"
            "*** You MUST use portability_init() before using this function!");
        return;
     }

#ifdef _WIN32
   COORD mycoord;
   mycoord.X = poscol;
   mycoord.Y = poslig;
   SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
#else
   printf("%c[%d;%df", 0x1B, poslig, poscol);
#endif
}

void
portability_background_color_set(Color color)
{
   if (!_init)
     {
        ERR("You didn't initialize portability with portability_init()\n"
            "*** You MUST use portability_init() before using this function!");
        return;
     }
   _portability_color_bg = color;
   _portability_color_apply();
}

void
portability_text_color_set(Color color)
{
   if (!_init)
     {
        ERR("You didn't initialize portability with portability_init()\n"
            "*** You MUST use portability_init() before using this function!");
        return;
     }
   _portability_color_fg = color;
   _portability_color_apply();
}

