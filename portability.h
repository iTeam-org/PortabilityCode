/* ***************************************************************************
 *	Cette bibliothèque de fonction est distribué librement par l'iTeam !
 *
 *  Son but est de fournir un moyen simple pour compiler les codes enseignés
 *  à l'ECE (qui ne marche correctement que sous Windows).
 *  Nous vous invitons à l'utiliser et l'améliorer, la dernière version
 *  est disponible sur https://github.com/iTeam-Projects/PortabilityCode
 *
 *
 *			iTeam - association de Promotion du Logiciel Libre
 * ***************************************************************************
 *  Fonctions non-portable gérées :
 *	system("PAUSE");
 *	system("CLS");
 *	fflush(stdin);
 *	kbhit();
 *	sleep();
 *  Fonctions non-portable en attente :
 *	getch();
 *	fonction couleurs dans la console
 */

#ifndef PORTABILITY_H_INCLUDED
#define PORTABILITY_H_INCLUDED

/*! Remarques pour les utilisateurs de ce fichier:
 *  0) Téléchargez ce fichier dans votre dossier de projet
 *	(https://github.com/iTeam-Projects/PortabilityCode/downloads)
 *  1) Ajoutez le fichier à votre projet Code::Blocks 
 *	(Project> Add files...> portability.h)
 *  2) Incluez le fichier dans votre code
 *	(Ecrire #include "portability.h" en haut de vos fichiers sources)
 *  3) Ça marche !
 */

/*! Remarques pour les développeurs de ce fichier:
 *  - Les valeurs de retour des fonctions sont conservés pour compatibilités
 *  - Les fonctions usuelles non-portable de Windows sont prises en référence
 *  - Toute nouvelle fonction doit être testé sous les 3 OS 
 *  - Les résultats des tests doivent apparaître dans le wiki GitHub
 */

// Quelques bibliothèques standards utiles
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32	// Si l'on est sous Windows
    #include <conio.h>
    #include <windows.h>
#else		// Si l'on est sous Linux ou Mac
    #include <termios.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/time.h>
#endif

// Couleurs

#define COLOR_BLACK 30
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_BLUE 34
#define COLOR_GRAY 37


// Prototypes des fonctions portables

/*! Remplace les appels systèmes windows courants par leurs correspondances
 *  \param  cmd Chaîne de caractère correspondant à la commande système
 *  \return -1 en cas d'erreur, 0 si cmd est NULL et > 0 sinon
 */
int portability_system_call(const char* cmd);

/*! Efface le buffer d'entrée
 *  \param  f Pointeur sur un descripteur FILE (notamment stdin)
 */
void portability_clear_buffer( FILE* f);

/*! Change le mode de fonctionnement du terminal (utile pour kbhit)
 *  \param dir Correspond à On/Off (1/0) pour le mode du terminal
 */
void portability_change_terminal_mode( int dir);

/*! Détecte qu'une touche a été tapée dans le terminal
 *  \ret 0 si aucune touche frappée, > 0 sinon
 */
int portability_kbhit();

/*! Pause le programme pendant un temps donné
 *  \param time Temps en millisecondes 
 *  \ret -1 si erreur, 0 sinon
 */
unsigned int portability_sleep(unsigned int time);

/*! Change la couleur du texte ou du fond
 * \param color la couleur
 */
void portability_background_color(unsigned int color);
void portability_text_color(unsigned int color);

/*! Déplace le curseur dans la console
 * À noter que le repère est de la forme:
 *  ------------->
 * |(0,0)          X (poscol)
 * |
 * |
 * |
 * v 
 *   Y (poslig)
 * 
 * IMPORTANT: On fait donc un appel avec des coordonnées (Y, X) et non (X, Y)
 *
 *  \param  poslig Correspond au numéro de ligne
 *  \param  poscol Correspond au numéro de colonne
 */
void portability_gotoligcol(int poslig, int poscol);


/*! Macros d'interceptions des appels de fonctions non-portables
 *  À noter que ces macros remplacent les appels non-portables AVANT la 
 *  compilation du code.
 */
#define gotoligcol(x, y) portability_gotoligcol(x, y)
#define kbhit() portability_kbhit()
#define Sleep(time) portability_sleep(time)
#define system(arg) portability_system_call(arg)
#define fflush(arg) portability_clear_buffer(arg)

// Intercepte les appels systèmes et remplace les commandes non-portables
int portability_system_call(const char* cmd)
{

    // ****** DECLARACTION DES VARIABLES ******

    // Enregistre la longueur de la commande
    int cmd_len = strlen(cmd);

    // Déclare une chaîne de l'exacte taille de cmd
    char* command = (char*) malloc(sizeof(char) * (cmd_len + 1));

    // Itérateur
    int i;

    // Valeur de retour de la fonction system
    int ret = 1;

    // ****** PREPARATION DE LA COMMANDE ******

    // Passage de la commande en majuscule 
    //	Car Windows est insensible à la casse contrairement à Linux/Mac
    for (i = 0 ; i < cmd_len ; i++)
        command[i] = toupper(cmd[i]);

    // Ajout du caractère de fin de chaîne
    command[cmd_len] = '\0';

    // ****** TRAITEMENT DE LA COMMANDE  ******

    if (strcmp(command, "PAUSE") == 0)		// Si on veut faire une pause
    {
        printf("Press any key to continue...");
        getchar();  // Attend la saisie d'un caractère
    }
    else if (strcmp(command, "CLS") == 0)
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

    // On libère l'espace mémoire pris par la commande
    free(command);

    // On retourne les éventuelles valeurs de retour de system
    return ret;
}

// Libère le buffer d'entrée notamment (pour éviter les bugs sur les scanf)
void portability_clear_buffer(FILE* f)
{
    // Itérateur
    char c;

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


void
portability_text_color(unsigned int color)
{
#ifdef _WIN32
        HANDLE console;

        console = GetStdHandle(STD_OUTPUT_HANDLE);
        if(color == COLOR_RED)
                color = FOREGROUND_RED;
        if(color == COLOR_BLUE)
                color = FOREGROUND_BLUE;
        if(color == COLOR_GREEN)
                color = FOREGROUND_GREEN;
        if(color == COLOR_GRAY)
                color = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
        SetConsoleTextAttribute(console, color);
#else
        printf("\033[0;#%im", color);
#endif
}

void
portability_background_color(unsigned int color)
{
#ifdef _WIN32
        HANDLE console;

        console = GetStdHandle(STD_OUTPUT_HANDLE);
        if(color == COLOR_RED)
                color = BACKGROUND_RED;
        if(color == COLOR_BLUE)
                color = BACKGROUND_BLUE;
        if(color == COLOR_GREEN)
                color = BACKGROUND_GREEN;
        if(color == COLOR_GRAY)
                color = BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE;
        SetConsoleTextAttribute(console, color);
#else
        printf("\033[7;%im",color);
#endif
}

#endif // PORTABILITY_H_INCLUDED
