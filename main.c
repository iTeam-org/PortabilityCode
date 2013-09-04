#include "portability.h"

#define ERR(msg, ...) fprintf(stderr, "*** %s:%i " msg "\n",\
                              __func__, __LINE__, ## __VA_ARGS__)

static void
_system_call_test(const char *arg)
{
   int val;

   if (arg == NULL)
     {
        ERR("NULL argument");
        return;
     }
   val = portability_system_call(arg);
   printf("%s: %i\n", __func__, val);
}

int main(int argc, char **argv)
{
   _system_call_test(argc >= 2 ? argv[1] : NULL);

   portability_background_color(COLOR_RED);
   printf("Red-------------\n");
   portability_background_color(COLOR_BLUE);
   printf("Blue-------------\n");
   portability_background_color(COLOR_GREEN);
   printf("Green-------------\n");
   portability_background_color(COLOR_BLACK);
   printf("BLACK-------------\n");
   portability_background_color(COLOR_GRAY);
   printf("Gray-------------\n");

   portability_text_color(COLOR_RED);
   printf("Red-------------\n");
   portability_text_color(COLOR_BLUE);
   printf("Blue-------------\n");
   portability_background_color(COLOR_DEFAULT);
   portability_text_color(COLOR_GREEN);
   printf("Green-------------\n");
   portability_text_color(COLOR_GRAY);
   printf("Gray-------------\n");
   portability_text_color(COLOR_BLACK);
   printf("BLACK-------------\n");




   return 0;
}

