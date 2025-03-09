#include "grid.h"
#include "raylib.h"
#include "shape.h"
#include "types.h"
#include <stdio.h>

static inline Rectangle Grid_get_cell(Grid grid, int grid_x, int grid_y)
{
    return (Rectangle){
        .x = grid.offset.x + (float)grid_x * grid.side_size,
        .y = grid.offset.y + (float)grid_y * grid.side_size,
        .height = grid.side_size,
        .width = grid.side_size,
    };
}

static inline void Grid_draw_cell_color(Grid grid, Rectangle cell, Color color)
{
    DrawRectangleRec(cell, color);
    DrawRectangleLinesEx(cell, grid.line_thickness, WHITE);
}

static inline void Grid_draw_cell(Grid grid, Rectangle cell)
{
    DrawRectangleLinesEx(cell, grid.line_thickness, WHITE);
}

static inline void Grid_draw_cell_type(Grid grid, Rectangle cell, CellType cell_type)
{
    switch (cell_type)
    {
    case empty:
        Grid_draw_cell(grid, cell);
        break;
    case color_red:
        Grid_draw_cell_color(grid, cell, RED);
        break;
    default:
        break;
    }
}

void Grid_draw(Grid grid, GridState grid_state)
{
    for (int i = 0; i < grid.size_x; i++)
    {
        for (int j = 0; j < grid.size_y; j++)
        {
            Rectangle cell = Grid_get_cell(grid, i, j);
            CellType cell_type = GridState_get(grid, grid_state, i, j);
            Grid_draw_cell_type(grid, cell, cell_type);
        }
    }
}

bool GridState_update_game(Grid grid, GridState *grid_state, Shape *current_shape)
{
    CellType lower_cell;
    CellType current_cell;

    ShapeType type = current_shape->type;
    int pos_x = current_shape->pos_x;
    int pos_y = current_shape->pos_y;
    int new_pos_y = pos_y + 1;

    switch (type)
    {
    case g_shaped:
        Shape_unset(*current_shape, grid, grid_state);
        if (!ShapeType_is_enough_space(current_shape->type, current_shape->rotation, grid, *grid_state, pos_x,
                                       new_pos_y))
        {
            Shape_set(*current_shape, grid, grid_state);
            return false;
        }

        current_shape->pos_y = new_pos_y;
        Shape_set(*current_shape, grid, grid_state);

        return true;
    default:
        return false;
        break;
    }
}

bool GridState_update_player(Grid grid, GridState *grid_state, Shape *current_shape, int input, bool should_rotate)
{
    CellType lower_cell;
    CellType current_cell;

    ShapeType type = current_shape->type;

    int rotation = current_shape->rotation;
    int new_rotation = should_rotate ? (rotation + 1) % 4 : rotation;

    int pos_x = current_shape->pos_x;
    int pos_y = current_shape->pos_y;
    int new_pos_x = pos_x + input;

    switch (type)
    {
    case g_shaped:
        Shape_unset(*current_shape, grid, grid_state);
        if (!ShapeType_is_enough_space(type, new_rotation, grid, *grid_state, new_pos_x, pos_y))
        {
            Shape_set(*current_shape, grid, grid_state);
            return false;
        }

        current_shape->rotation = new_rotation;
        current_shape->pos_x = new_pos_x;
        Shape_set(*current_shape, grid, grid_state);

        return true;
    default:
        return false;
        break;
    }
}
