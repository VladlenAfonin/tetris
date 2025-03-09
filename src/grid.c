#include "grid.h"
#include "raylib.h"

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

void GridState_update(Grid grid, GridState *grid_state)
{
    CellType lower_cell;
    CellType current_cell;

    for (int i = 0; i < grid.size_x; i++)
    {
        for (int j = grid.size_y - 2; j > -1; j--)
        {
            current_cell = GridState_get(grid, *grid_state, i, j);
            if (empty == current_cell)
            {
                continue;
            }

            lower_cell = GridState_get(grid, *grid_state, i, j + 1);
            if (empty != lower_cell)
            {
                continue;
            }

            GridState_set(grid, *grid_state, i, j + 1, current_cell);
            GridState_set(grid, *grid_state, i, j, empty);
        }
    }
}
