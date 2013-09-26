#include <stdio.h>
#include <stdlib.h>
#include "portability.h" // Always the last #include

int
main(void)
{
   portability_init();

   /* Your code here */

   portability_shutdown();

   return 0;
}

