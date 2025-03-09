#include "shape.h"
#include "grid.h"

void Shape_unset(Shape shape, Grid grid, GridState *grid_state)
{
    switch (shape.type)
    {
    case g_shaped:
        GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, empty);
        GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, empty);
        GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y, empty);
        GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y + 1, empty);
        break;
    default:
        break;
    }
}

void Shape_set(Shape shape, Grid grid, GridState *grid_state)
{
    switch (shape.type)
    {
    case g_shaped:
        GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, color_red);
        GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, color_red);
        GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y, color_red);
        GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y + 1, color_red);
        break;
    default:
        break;
    }
}

static bool Shape_is_enough_space_g_shaped(Grid grid, GridState grid_state, int x, int y)
{
    CellType cell;

    if (x < 0 || x + 3 > grid.size_x || y + 2 > grid.size_y)
    {
        return false;
    }

    cell = GridState_get(grid, grid_state, x, y);
    if (empty != cell)
    {
        return false;
    }

    cell = GridState_get(grid, grid_state, x + 1, y);
    if (empty != cell)
    {
        return false;
    }

    cell = GridState_get(grid, grid_state, x + 2, y);
    if (empty != cell)
    {
        return false;
    }

    cell = GridState_get(grid, grid_state, x + 2, y + 1);
    if (empty != cell)
    {
        return false;
    }

    return true;
}

inline bool Shape_is_enough_space_for_self(Shape shape, Grid grid, GridState grid_state)
{
    return Shape_is_enough_space(shape.type, grid, grid_state, shape.pos_x, shape.pos_y);
}

bool Shape_is_enough_space(ShapeType shape_type, Grid grid, GridState grid_state, int x, int y)
{
    switch (shape_type)
    {
    case g_shaped:
        return Shape_is_enough_space_g_shaped(grid, grid_state, x, y);
        break;
    default:
        break;
    }

    return true;
}
