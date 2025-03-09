#include "grid.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    const int menu_section_size = 300;
    const int target_fps = 60;

    const Grid grid = {
        .size_x = 10,
        .size_y = 15,
        .side_size = 50.0F,
        .line_thickness = 1.0F,
        .offset = (Vector2){.x = 10.0f, .y = 10.0F},
    };

    GridState_create(grid_state, grid);
    GridState_init(grid_state, grid);
    GridState_set(grid, grid_state, 1, 0, color_red);
    GridState_set(grid, grid_state, 2, 0, color_red);
    GridState_set(grid, grid_state, 3, 0, color_red);
    GridState_set(grid, grid_state, 1, 1, color_red);

    const int screen_width = grid.size_x * grid.side_size + 2 * grid.offset.x;
    const int screen_height = grid.size_y * grid.side_size + 2 * grid.offset.y;

    InitWindow(screen_width, screen_height, "RTetris");
    SetTargetFPS(target_fps);

    float dt;
    float update_timer = 0.0F;
    while (!WindowShouldClose())
    {
        dt = GetFrameTime();
        update_timer += dt;

        if (update_timer > 0.2F)
        {
            GridState_update(grid, &grid_state);
            update_timer = 0.0F;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Grid_draw(grid, grid_state);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
