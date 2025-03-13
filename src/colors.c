#include "colors.h"
#include "raylib.h"

Color get_random_color()
{
    int number_of_colors = (int)(sizeof(colors) / sizeof(Color));
    int random_index = GetRandomValue(0, number_of_colors - 1);
    return colors[random_index];
}
