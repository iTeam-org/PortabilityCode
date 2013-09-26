#include "portability.h"

#define ERR(msg, ...) fprintf(stderr, "*** %s:%i " msg "\n",\
                              __func__, __LINE__, ## __VA_ARGS__)

static void
_system_call_test(const char *arg)
{
   int val;

   printf("=========== SYSTEM CALL TEST ============\n");
   if (arg == NULL)
     {
        ERR("NULL argument provided!");
        return;
     }
   val = portability_system_call(arg);
   printf("\n%s errno: %i\n", __func__, val);
   printf("=========================================\n\n");
}

static void
_printf_test(void)
{
   printf("\n============ PRINTF LINE BUFFERED TEST ===========\n");
   printf("This is a printf test for line bufferring");
   printf("And another test");
   printf("If everything is on the same line, it's fine");
   printf("\n==================================================\n\n");
}

static void
_color_test_col(Color color, const char *text, int bg)
{
   void (*func)(Color color) =
      (bg) ? portability_background_color_set : portability_text_color_set;

   func(color);
   printf("%s", text);
   func(COLOR_DEFAULT);
   printf("\n");
}

static void
_color_test(void)
{
   printf("\n============ COLOR TEST ===========\n");

   _color_test_col(COLOR_BLACK,   "BLACK---BG", 1);
   _color_test_col(COLOR_RED,     "RED-----BG", 1);
   _color_test_col(COLOR_GREEN,   "GREEN---BG", 1);
   _color_test_col(COLOR_YELLOW,  "YELLOW--BG", 1);
   _color_test_col(COLOR_BLUE,    "BLUE----BG", 1);
   _color_test_col(COLOR_MAGENTA, "MAGENTA-BG", 1);
   _color_test_col(COLOR_CYAN,    "CYAN----BG", 1);
   _color_test_col(COLOR_GRAY,    "GRAY----BG", 1);

   _color_test_col(COLOR_BLACK,   "BLACK---FG", 0);
   _color_test_col(COLOR_RED,     "RED-----FG", 0);
   _color_test_col(COLOR_GREEN,   "GREEN---FG", 0);
   _color_test_col(COLOR_YELLOW,  "YELLOW--FG", 0);
   _color_test_col(COLOR_BLUE,    "BLUE----FG", 0);
   _color_test_col(COLOR_MAGENTA, "MAGENTA-FG", 0);
   _color_test_col(COLOR_CYAN,    "CYAN----FG", 0);
   _color_test_col(COLOR_GRAY,    "GRAY----FG", 0);

   printf("===================================\n\n");
}

static void
_no_init_test(void)
{
   portability_gotoligcol(1, 1);
}

int
main(int argc, char **argv)
{
   /* Check that portability_init() is required */
   _no_init_test();

   /* Init portability */
   portability_init();

   /* Test system() */
   _system_call_test(argc >= 2 ? argv[1] : NULL);

   /* Test printf() */
   _printf_test();

   /* Test colors */
   _color_test();

   /* Shut down portability */
   portability_shutdown();

   return 0;
}

