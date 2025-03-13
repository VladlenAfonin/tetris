#include "grid.h"
#include "globals.h"
#include "raylib.h"
#include "shape.h"
#include "types.h"
#include <stdio.h>

static inline Rectangle Grid_get_cell(int grid_x, int grid_y)
{
    return (Rectangle){
        .x = global_parameters.offset.x + (float)grid_x * global_parameters.side_size,
        .y = global_parameters.offset.y + (float)grid_y * global_parameters.side_size +
             (float)global_parameters.score_height,
        .height = global_parameters.side_size,
        .width = global_parameters.side_size,
    };
}

static inline void Grid_draw_score()
{
    int score_font_size = 30;
    char score_string[16] = {0};
    sprintf(score_string, "%d", global_state.score);
    int score_string_size_x = MeasureText(score_string, score_font_size);
    DrawText(score_string, (global_parameters.screen_size_x - score_string_size_x) / 2,
             (global_parameters.score_height - score_font_size) / 2, score_font_size, WHITE);
}

static inline void Grid_draw_cell_color(Rectangle rec, Color color)
{
    DrawRectangleRec(rec, color);
    DrawRectangleLinesEx(rec, global_parameters.line_thickness, WHITE);
}

static inline void Grid_draw_cell(Rectangle rec)
{
    DrawRectangleLinesEx(rec, global_parameters.line_thickness, WHITE);
}

static inline void Grid_draw_cell_type(Rectangle rec, GridCell cell)
{
    switch (cell.type)
    {
    case empty:
        Grid_draw_cell(rec);
        break;
    case filled:
        Grid_draw_cell_color(rec, cell.color);
        break;
    default:
        break;
    }
}

void Grid_draw(GridState grid_state)
{
    for (int i = 0; i < global_parameters.size_x; i++)
    {
        for (int j = 0; j < global_parameters.size_y; j++)
        {
            Rectangle rec = Grid_get_cell(i, j);
            GridCell cell = GridState_get(grid_state, i, j);
            Grid_draw_cell_type(rec, cell);
        }
    }

    Grid_draw_score();
}

static bool row_complete(GridState grid_state, int y)
{
    GridCell cell;
    for (int x = 0; x < global_parameters.size_x; x++)
    {
        cell = GridState_get(grid_state, x, y);
        if (empty == cell.type)
        {
            return false;
        }
    }

    return true;
}

static void row_clean(GridState *grid_state, int y)
{
    GridCell cell = {.type = empty, .color = BLACK};
    for (int x = 0; x < global_parameters.size_x; x++)
    {
        GridState_set(*grid_state, x, y, cell);
    }
}

static void GridState_shift_above(GridState *grid_state, int y_empty)
{
    GridCell cell;
    for (int y = y_empty; y > 0; y--)
    {
        for (int x = 0; x < global_parameters.size_x; x++)
        {
            cell = GridState_get(*grid_state, x, y - 1);
            GridState_set(*grid_state, x, y, cell);
        }
    }

    cell = (GridCell){.type = empty, .color = BLACK};
    for (int x = 0; x < global_parameters.size_x; x++)
    {
        GridState_set(*grid_state, x, 0, cell);
    }
}

static int GridState_cleanup(GridState *grid_state)
{
    int cleaned_row_count = 0;

    for (int y = global_parameters.size_y - 1; y > -1; y--)
    {
        if (!row_complete(*grid_state, y))
        {
            continue;
        }

        row_clean(grid_state, y);
        GridState_shift_above(grid_state, y);
        y++;
        cleaned_row_count++;
    }

    if (cleaned_row_count == 1)
    {
        return 40;
    }
    else if (cleaned_row_count == 2)
    {
        return 100;
    }
    else if (cleaned_row_count == 3)
    {
        return 300;
    }
    else if (cleaned_row_count == 4)
    {
        return 1200;
    }

    return 0;
}

bool GridState_update_game(GridState *grid_state)
{
    ShapeType type = global_state.current_shape.type;
    int rotation = global_state.current_shape.rotation;
    int pos_x = global_state.current_shape.pos_x;
    int pos_y = global_state.current_shape.pos_y;
    int new_pos_y = pos_y + 1;

    Shape_unset(global_state.current_shape, grid_state);
    if (!ShapeType_is_enough_space(type, rotation, *grid_state, pos_x, new_pos_y))
    {
        Shape_set(global_state.current_shape, grid_state);
        global_state.score += GridState_cleanup(grid_state);
        return false;
    }

    // This may be erroneous.
    global_state.current_shape.pos_y = new_pos_y;
    Shape_set(global_state.current_shape, grid_state);

    return true;
}

bool GridState_update_player(GridState *grid_state, int input, bool should_rotate)
{
    ShapeType type = global_state.current_shape.type;

    int rotation = global_state.current_shape.rotation;
    int new_rotation = should_rotate ? (rotation + 1) % 4 : rotation;

    int pos_x = global_state.current_shape.pos_x;
    int pos_y = global_state.current_shape.pos_y;
    int new_pos_x = pos_x + input;

    Shape_unset(global_state.current_shape, grid_state);
    if (!ShapeType_is_enough_space(type, new_rotation, *grid_state, new_pos_x, pos_y))
    {
        Shape_set(global_state.current_shape, grid_state);
        return false;
    }

    global_state.current_shape.rotation = new_rotation;
    global_state.current_shape.pos_x = new_pos_x;
    Shape_set(global_state.current_shape, grid_state);

    return true;
}
