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

#define  _XOPEN_SOURCE_EXTENDED 1
#include <strings.h>

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
typedef enum
{
   COLOR_DEFAULT = 0,
   COLOR_BLACK   = 30,
   COLOR_RED     = 31,
   COLOR_GREEN   = 32,
   COLOR_YELLOW  = 33,
   COLOR_BLUE    = 34,
   COLOR_MAGENTA = 35,
   COLOR_CYAN    = 36,
   COLOR_GRAY    = 37
} Color;


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

/* FIXME This is madness! Before code means when you call system, it will loop */
/*#define gotoligcol(x, y) portability_gotoligcol(x, y)*/
//#define kbhit() portability_kbhit()
//#define Sleep(time) portability_sleep(time)
//#define system(arg) portability_system_call(arg)
//#define fflush(arg) portability_clear_buffer(arg)



#endif // PORTABILITY_H_INCLUDED
