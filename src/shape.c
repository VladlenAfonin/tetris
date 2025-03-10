#include "shape.h"
#include "grid.h"
#include "types.h"
#include <stdio.h>

// 4 rotations, 4 cells, 2 coordinates for each
// First sort by rotation, then pairs of numbers tell you coordinates of each point.
// Points are not necessarily sorted.
// clang-format off
static int g_shaped_numbers[4 * 2 * 4] = {
    // # # #
    //     #
    0, 0, 1, 0, 2, 0, 2, 1,
    //   #
    //   #
    // # #
    1, 0, 1, 1, 1, 2, 0, 2,
    // #
    // # # #
    0, 0, 0, 1, 1, 1, 2, 1,
    // # #
    // #
    // #
    0, 0, 1, 0, 0, 1, 0, 2,
};
// 4 rotations, borders for X and Y for each (in that order).
static int g_shaped_rotation_borders[4 * 2] = {
    3, 2,
    2, 3,
    3, 2,
    2, 3,
};
// clang-format on

static void Shape_apply(Shape shape, Grid grid, GridState *grid_state, CellType cell_type)
{
    int *row;
    switch (shape.type)
    {
    case g_shaped:
        row = g_shaped_numbers + shape.rotation * 8;
        for (int i = 0; i < 4; i++)
        {
            GridState_set(grid, *grid_state, shape.pos_x + row[2 * i], shape.pos_y + row[2 * i + 1], cell_type);
        }
        break;
    default:
        break;
    }
}

void Shape_unset(Shape shape, Grid grid, GridState *grid_state)
{
    Shape_apply(shape, grid, grid_state, empty);
}

void Shape_set(Shape shape, Grid grid, GridState *grid_state)
{
    // TODO: Make color parameter.
    // TODO: Make color random in the calling function.
    Shape_apply(shape, grid, grid_state, color_red);
}

static bool Shape_check_space(ShapeType shape, int rotation, Grid grid, GridState grid_state, int x, int y)
{
    int *row;
    int *border_row;
    CellType cell;
    switch (shape)
    {
    case g_shaped:
        border_row = g_shaped_rotation_borders + rotation * 2;
        if (x < 0 || x + border_row[0] > grid.size_x || y + border_row[1] > grid.size_y)
        {
            return false;
        }

        row = g_shaped_numbers + rotation * 8;
        for (int i = 0; i < 4; i++)
        {
            cell = GridState_get(grid, grid_state, x + row[2 * i], y + row[2 * i + 1]);
            if (empty != cell)
            {
                return false;
            }
        }

        break;
    default:
        break;
    }

    return true;
}

inline bool Shape_is_enough_space_for_self(Shape shape, Grid grid, GridState grid_state)
{
    return ShapeType_is_enough_space(shape.type, shape.rotation, grid, grid_state, shape.pos_x, shape.pos_y);
}

bool ShapeType_is_enough_space(ShapeType shape, int rotation, Grid grid, GridState grid_state, int x, int y)
{
    switch (shape)
    {
    case g_shaped:
        return Shape_check_space(shape, rotation, grid, grid_state, x, y);
        break;
    default:
        break;
    }

    return true;
}
