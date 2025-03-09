#include "grid.h"
#include "raylib.h"
#include "shape.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int const menu_section_size = 300;
    int const target_fps = 60;

    Grid const grid = {
        .size_x = 10,
        .size_y = 15,
        .side_size = 50.0F,
        .line_thickness = 1.0F,
        .offset = (Vector2){.x = 10.0f, .y = 10.0F},
    };

    GridState_create(grid_state, grid);
    GridState_init(grid_state, grid);

    Shape current_shape = {.type = g_shaped, .rotation = 0, .pos_x = 4, .pos_y = 0};
    Shape_set(current_shape, grid, &grid_state);

    int const screen_width = grid.size_x * grid.side_size + 2 * grid.offset.x;
    int const screen_height = grid.size_y * grid.side_size + 2 * grid.offset.y;

    InitWindow(screen_width, screen_height, "RTetris");
    SetTargetFPS(target_fps);

    char const *finished_text = "You lost!";
    int const font_size = 30;
    int const text_size = MeasureText(finished_text, font_size);

    float dt;
    float update_timer = 0.0F;
    bool should_rotate = false;
    bool update_result;
    bool is_finished = false;
    int input = 0;
    while (!WindowShouldClose())
    {
        if (is_finished)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("You lost!", (screen_width - text_size) / 2, screen_height / 2, font_size, WHITE);
            EndDrawing();
            continue;
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            input = -1;
        }
        else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            input = 1;
        }
        else
        {
            input = 0;
        }

        if (IsKeyPressed(KEY_R))
        {
            should_rotate = true;
        }

        dt = GetFrameTime();
        update_timer += dt;

        GridState_update_player(grid, &grid_state, &current_shape, input, should_rotate);
        should_rotate = false;

        if (update_timer > 0.2F)
        {
            update_result = GridState_update_game(grid, &grid_state, &current_shape);
            if (!update_result)
            {
                // TODO: Type must be sampled.
                current_shape = (Shape){.type = g_shaped, .rotation = 0, .pos_x = 4, .pos_y = 0};
                if (!Shape_is_enough_space_for_self(current_shape, grid, grid_state))
                {
                    is_finished = true;
                }
            }
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
