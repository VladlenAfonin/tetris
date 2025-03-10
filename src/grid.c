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

static inline void Grid_draw_cell_color(Grid grid, Rectangle rec, Color color)
{
    DrawRectangleRec(rec, color);
    DrawRectangleLinesEx(rec, grid.line_thickness, WHITE);
}

static inline void Grid_draw_cell(Grid grid, Rectangle rec)
{
    DrawRectangleLinesEx(rec, grid.line_thickness, WHITE);
}

static inline void Grid_draw_cell_type(Grid grid, Rectangle rec, GridCell cell)
{
    switch (cell.type)
    {
    case empty:
        Grid_draw_cell(grid, rec);
        break;
    case filled:
        Grid_draw_cell_color(grid, rec, cell.color);
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
            Rectangle rec = Grid_get_cell(grid, i, j);
            GridCell cell = GridState_get(grid, grid_state, i, j);
            Grid_draw_cell_type(grid, rec, cell);
        }
    }
}

static bool row_complete(Grid grid, GridState grid_state, int y)
{
    GridCell cell;
    for (int x = 0; x < grid.size_x; x++)
    {
        cell = GridState_get(grid, grid_state, x, y);
        if (empty == cell.type)
        {
            return false;
        }
    }

    return true;
}

static void row_clean(Grid grid, GridState *grid_state, int y)
{
    GridCell cell = {.type = empty, .color = BLACK};
    for (int x = 0; x < grid.size_x; x++)
    {
        GridState_set(grid, *grid_state, x, y, cell);
    }
}

static void GridState_shift_above(Grid grid, GridState *grid_state, int y_empty)
{
    GridCell cell;
    for (int y = y_empty; y > 0; y--)
    {
        for (int x = 0; x < grid.size_x; x++)
        {
            cell = GridState_get(grid, *grid_state, x, y - 1);
            GridState_set(grid, *grid_state, x, y, cell);
        }
    }

    cell = (GridCell){.type = empty, .color = BLACK};
    for (int x = 0; x < grid.size_x; x++)
    {
        GridState_set(grid, *grid_state, x, 0, cell);
    }
}

static void GridState_cleanup(Grid grid, GridState *grid_state)
{
    for (int y = grid.size_y - 1; y > -1; y--)
    {
        if (!row_complete(grid, *grid_state, y))
        {
            continue;
        }

        row_clean(grid, grid_state, y);
        GridState_shift_above(grid, grid_state, y);
        y++;
    }
}

bool GridState_update_game(Grid grid, GridState *grid_state, Shape *current_shape)
{
    CellType lower_cell;
    CellType current_cell;

    ShapeType type = current_shape->type;
    int rotation = current_shape->rotation;
    int pos_x = current_shape->pos_x;
    int pos_y = current_shape->pos_y;
    int new_pos_y = pos_y + 1;

    Shape_unset(*current_shape, grid, grid_state);
    if (!ShapeType_is_enough_space(type, rotation, grid, *grid_state, pos_x, new_pos_y))
    {
        Shape_set(*current_shape, grid, grid_state);
        GridState_cleanup(grid, grid_state);
        return false;
    }

    current_shape->pos_y = new_pos_y;
    Shape_set(*current_shape, grid, grid_state);

    return true;
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
}
