#include "globals.h"
#include "grid.h"
#include "raylib.h"
#include "shape.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    SetRandomSeed(time(NULL));

    int const target_fps = 60;
    int const score_space = 80;

    global_parameters = (GlobalParameters){
        .size_x = 10,
        .size_y = 15,
        .side_size = 50.0F,
        .line_thickness = 1.0F,
        .offset = (Vector2){.x = 10.0F, .y = 10.0F},
        .score_height = score_space,
    };

    global_state = (GlobalState){
        .score = 0,
        .current_scene = gameplay,
    };

    GridState_create(grid_state);
    GridState_init(grid_state);

    global_state.current_shape = get_random_shape();
    global_state.next_shape = get_random_shape();
    Shape_set(global_state.current_shape, &grid_state);

    global_parameters.screen_size_x =
        global_parameters.size_x * global_parameters.side_size + 2 * global_parameters.offset.x;
    global_parameters.screen_size_y =
        global_parameters.size_y * global_parameters.side_size + 2 * global_parameters.offset.y + score_space;

    InitWindow(global_parameters.screen_size_x, global_parameters.screen_size_y, "RTetris");
    SetTargetFPS(target_fps);

    char const *finished_text = "You lost!";
    int const font_size = 30;
    int const text_size = MeasureText(finished_text, font_size);

    float dt;
    float update_timer = 0.0F;
    bool should_rotate = false;
    int input = 0;

    float const default_timer_threshold = 0.5F;
    float const fast_timer_threshold = 0.01F;
    float timer_threshold = default_timer_threshold;

    char score_string[16] = {0};
    int score_font_size = 30;
    int score_string_size_x = 0;

    char const *restart_button_text = "Restart";
    int restart_button_text_size_x = MeasureText(restart_button_text, font_size);
    int restart_button_padding_x = 20;
    int restart_button_padding_y = 20;
    float restart_button_offset_y = (float)(2 * font_size);
    Color restart_button_color = RED;
    Color restart_button_text_color = WHITE;
    Rectangle restart_button_rec = {
        .height = (float)font_size + 2 * restart_button_padding_y,
        .width = (float)restart_button_text_size_x + 2 * restart_button_padding_x,
        .x = global_parameters.screen_size_x / 2.0F,
        .y = global_parameters.screen_size_y / 2.0F + restart_button_offset_y,
    };
    restart_button_rec.x -= restart_button_rec.width / 2.0F;
    bool is_restart_requested = false;

    Vector2 mouse_position;
    while (!WindowShouldClose())
    {
        switch (global_state.current_scene)
        {
        case end:
            mouse_position = GetMousePosition();
            if (CheckCollisionPointRec(mouse_position, restart_button_rec))
            {
                restart_button_color = WHITE;
                restart_button_text_color = BLACK;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    is_restart_requested = true;
                }

                if (is_restart_requested && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    global_state.current_scene = gameplay;
                    global_state.score = 0;
                    global_state.current_shape = get_random_shape();
                    global_state.next_shape = get_random_shape();

                    GridState_init(grid_state);
                }
            }
            else
            {
                is_restart_requested = false;
                restart_button_color = RED;
                restart_button_text_color = WHITE;
            }

            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("You lost!", (global_parameters.screen_size_x - text_size) / 2,
                     global_parameters.screen_size_y / 2, font_size, WHITE);
            DrawText(score_string, (global_parameters.screen_size_x - score_string_size_x) / 2,
                     (global_parameters.score_height - score_font_size) / 2, score_font_size, WHITE);
            DrawRectangleRec(restart_button_rec, restart_button_color);
            DrawText(restart_button_text, (restart_button_rec.x + restart_button_padding_x),
                     (restart_button_rec.y + restart_button_padding_y), font_size, restart_button_text_color);
            EndDrawing();
            continue;
        case gameplay:
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

            if (IsKeyDown(KEY_SPACE))
            {
                timer_threshold = fast_timer_threshold;
            }
            else
            {
                timer_threshold = default_timer_threshold;
            }

            if (IsKeyPressed(KEY_R))
            {
                should_rotate = true;
            }

            dt = GetFrameTime();
            update_timer += dt;

            GridState_update_player(&grid_state, input, should_rotate);
            should_rotate = false;

            if (update_timer > timer_threshold)
            {
                if (!GridState_update_game(&grid_state))
                {
                    global_state.current_shape = get_random_shape();
                    if (!Shape_is_enough_space_for_self(global_state.current_shape, grid_state))
                    {
                        sprintf(score_string, "%d", global_state.score);
                        score_string_size_x = MeasureText(score_string, score_font_size);
                        global_state.current_scene = end;
                    }
                }

                update_timer = 0.0F;
            }

            BeginDrawing();
            ClearBackground(BLACK);
            Grid_draw(grid_state);
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
