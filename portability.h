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
 * ***************************************************************************
 *  Fonctions non-portables gérées :
 *	system("PAUSE");
 *	system("CLS");
 *	fflush(stdin);
 *	kbhit();
 *	sleep();
 *	fonction couleurs dans la console
 *  Fonctions non-portables en attente :
 *	getch();
 */

#ifndef PORTABILITY_H_INCLUDED
#define PORTABILITY_H_INCLUDED


/**
 * @file portability.h
 * @brief Fonctions portables à destination des ING1 de l'ECE. Il s'agit
 * de substituts à certaines fonctions qui sont spécifiques à Windows.
 * @author iTeam
 * @version 1.1
 * @date 2013-09-26
 *
 * @mainpage
 * Fonctions portables à destination des ING1 de l'ECE. Il s'agit
 * de substituts à certaines fonctions qui sont spécifiques à Windows.
 * @section howto Comment utiliser ce code?
 *
 * 1) Télécharger les fichiers portability.c ET portability.h à l'adresse
 * https://github.com/iTeam-Projects/PortabilityCode/downloads
 *
 * 2) Ajouter les fichiers au projet. Pour Code::Blocks :
 *
 * @verbatim
   Project > Add files... > portability.c
   Project > Add files... > portability.h
   @endverbatim
 *
 * 3) Pour chaue fichier .c, inclure portability.h
 *    @verbatim  #include "portability.h" @endverbatim
 * ATTENTION: IL DOIT ETRE INCLU EN DERNIER
 *
 * @defgroup portability
 * @brief Fonctions compatibles
 * @{
 */


/* Remarques pour les développeurs de ce fichier:
 *  - Les valeurs de retour des fonctions sont conservés pour compatibilités
 *  - Les fonctions usuelles non-portable de Windows sont prises en référence
 *  - Toute nouvelle fonction doit être testée sous les 3 OS
 *  - Les résultats des tests doivent apparaître dans le wiki GitHub
 */

// Quelques bibliothèques standards utiles
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#define _XOPEN_SOURCE_EXTENDED 1

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

/**
 * @enum Color Colors that are available in the terminal. They can be used
 * to change the background and the text colors.
 */
typedef enum
{
   COLOR_DEFAULT = 0,  /**< Couleur par défaut */
   COLOR_BLACK   = 30, /**< Couleur noire */
   COLOR_RED     = 31, /**< Couleur rouge */
   COLOR_GREEN   = 32, /**< Couleur verte */
   COLOR_YELLOW  = 33, /**< Couleur jaune */
   COLOR_BLUE    = 34, /**< Couleur bleue */
   COLOR_MAGENTA = 35, /**< Couleur magenta */
   COLOR_CYAN    = 36, /**< Couleur cyan */
   COLOR_GRAY    = 37  /**< Couleur grise */
} Color;



/**
 * Change la couleur de l'arrière plan
 * @param color Couleur à appliquer
 */
void portability_background_color_set(Color color);

/**
 * Change la couleur du texte
 * @param color Couleur à appliquer
 */
void portability_text_color_set(Color color);


/**
 * Initialize the portability library
 * - Disables the line bufferrng of defaults fds
 */
void portability_init(void);

/**
 * Shuts down the portability resources.
 */
void portability_shutdown(void);


/* Remove existing aliases */
#ifdef gotoligcol
# undef gotoligcol
#endif
#ifdef kbhit
# undef kbhit
#endif
#ifdef Sleep
# undef Sleep
#endif
#ifdef system
# undef system
#endif
#ifdef fflush
# undef fflush
#endif

/**
 * @def gotoligcol(x, y)
 * Déplace le curseur dans la console
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
 * @verbatim
   // Put the cursor at line 0, column 0:
   gotoligcol(0, 0);
   @endverbatim
 *
 * @param x Correspond à l'index de ligne
 * @param y Correspond à l'index de colonne
 */
#define gotoligcol(x, y) portability_gotoligcol(x, y)

/**
 * @def kbhit()
 * Détecte qu'une touche a été tapée dans le terminal
 *
 * @verbatim
   // Waits for a key input
   kbhit();
   @endverbatim
 * @return 0 si aucune touche frappée, > 0 sinon
 */
#define kbhit() portability_kbhit()

/**
 * Pause le programme pendant un temps donné
 * @verbatim
   // Waits for two seconds (2000 milliseconds)
   Sleep(2000);
   @endverbatim
 * @param time Temps en millisecondes
 */
#define Sleep(time) portability_sleep(time)

/**
 * @def system(arg)
 *
 * Remplace les appels systèmes Windows courants par leurs correspondances
 *
 * Example:
 * @verbatim
   // Clear the console
   system("cls");

   // Pause the console
   system("pause");
   @endverbatim
 * @param cmd Chaîne de caractère correspondant à la commande système
 * @return 0 si la commande a bien été exécutée, -1 si @e cmd est NULL,
 * > 0 si une erreur interne est survenue.
 */
#define system(arg) portability_system_call(arg)

/**
 * @def fflush(arg)
 *
 * Efface le buffer d'entrée
 * @verbatim
   // Nettoyer l'entrée clavier:
   fflush(stdin);
   @endverbatim
 * @param f Pointeur sur un descripteur FILE
 */
#define fflush(arg) portability_clear_buffer(arg)

// http://msdn.microsoft.com/en-us/library/e0z9k731(v=vs.90).aspx
/**
 * @def strcasecmp(str1, str2)
 *
 * Compare deux chaînes de caractère sans tenir compte de la casse
 *
 * @verbatim
   strcasecmp("A string", "a StRinG"); // Identical strings
   strcasecmp("A string", "Q string"); // Different strings
   strcasecmp("A string", "A string"); // Identical strings
   @endverbatim
 *
 * @param str1 Une chaîne de caractères
 * @param str2 Une autre chaîne de caractères
 *
 * @return 0 if the strings are identical
 */
#define strcasecmp(str1, str2) _stricmp(str1, str2)

#ifndef _WIN32
# undef strcasecmp
#endif

#ifdef ERROR
# undef ERROR
#endif

/**
 * @def ERROR(message, ...)
 * Affiche une message d'erreur dans la console qui indique le nom de la
 * fonction ainsi que la ligne à laquelle ce message a été appelé.
 *
 * @verbatim
   void
   my_gorgeous_function(void)
   {
      int i = 4;
      ERROR("Here, the value of i is %i. This is not normal!", i);
      // Displays:
      // *** [my_gorgeous_function:5] Here, the value of i is 4. This is not normal!
   }
   @endverbatim
 *
 * @param message Format du message (comme printf)
 * @param ... Arguments du message (comme printf)
 */
#define ERROR(message, ...) \
   fprintf(stderr, "*** [%s:%i] "msg"\n", __func__, __LINE__, ## __VA_ARGS__)

/** @} */

/* Compatibility functions */
void portability_gotoligcol(int poslig, int poscol);
int portability_kbhit(void);
void portability_clear_buffer(FILE *f);
void portability_sleep(unsigned int time);
int portability_system_call(const char *cmd);

#endif // PORTABILITY_H_INCLUDED

/**
 * @example base.c
 * @example test.c
 */
