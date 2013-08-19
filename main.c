#include <stdio.h>
#include <stdlib.h>
#include "portability.h"

int main()
{
    portability_text_color(COLOR_RED);
    printf("Red-------------\n");
    portability_text_color(COLOR_BLUE);
    printf("Blue-------------\n");
    portability_text_color(COLOR_GREEN);
    printf("Green-------------\n");
    portability_text_color(COLOR_GRAY);
    printf("Gray-------------\n");
    return 0;
}

