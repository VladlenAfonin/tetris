#include "shape.h"
#include "grid.h"
#include <stdio.h>

void Shape_unset(Shape shape, Grid grid, GridState *grid_state)
{
    switch (shape.type)
    {
    case g_shaped:
        if (0 == shape.rotation)
        {
            // # # #
            //     #

            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y + 1, empty);
        }
        else if (1 == shape.rotation)
        {
            //   #
            //   #
            // # #

            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y + 1, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y + 2, empty);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 2, empty);
        }
        else if (2 == shape.rotation)
        {
            // #
            // # # #

            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 1, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y + 1, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y + 1, empty);
        }
        else if (3 == shape.rotation)
        {
            // # #
            // #
            // #

            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, empty);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 1, empty);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 2, empty);
        }
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
        if (0 == shape.rotation)
        {
            // # # #
            //     #

            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y + 1, color_red);
        }
        else if (1 == shape.rotation)
        {
            //   #
            //   #
            // # #

            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y + 1, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y + 2, color_red);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 2, color_red);
        }
        else if (2 == shape.rotation)
        {
            // #
            // # # #

            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 1, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y + 1, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 2, shape.pos_y + 1, color_red);
        }
        else if (3 == shape.rotation)
        {
            // # #
            // #
            // #

            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x + 1, shape.pos_y, color_red);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 1, color_red);
            GridState_set(grid, *grid_state, shape.pos_x, shape.pos_y + 2, color_red);
        }
        break;
    default:
        break;
    }
}

static bool Shape_is_enough_space_g_shaped(ShapeType shape, int rotation, Grid grid, GridState grid_state, int x, int y)
{
    CellType cell;

    if (0 == rotation)
    {
        // # # #
        //     #

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
    }
    else if (1 == rotation)
    {
        //   #
        //   #
        // # #

        if (x < 0 || x + 2 > grid.size_x || y + 3 > grid.size_y)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x + 1, y);
        if (empty != cell)
        {
            printf("ASDF\n");
            return false;
        }

        cell = GridState_get(grid, grid_state, x + 1, y + 1);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x + 1, y + 2);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x, y + 2);
        if (empty != cell)
        {
            return false;
        }
    }
    else if (2 == rotation)
    {
        // #
        // # # #

        if (x < 0 || x + 3 > grid.size_x || y + 2 > grid.size_y)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x, y);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x, y + 1);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x + 1, y + 1);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x + 2, y + 1);
        if (empty != cell)
        {
            return false;
        }
    }
    else if (3 == rotation)
    {
        // # #
        // #
        // #

        if (x < 0 || x + 2 > grid.size_x || y + 3 > grid.size_y)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x, y);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x, y + 1);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x, y + 2);
        if (empty != cell)
        {
            return false;
        }

        cell = GridState_get(grid, grid_state, x + 1, y);
        if (empty != cell)
        {
            return false;
        }
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
        return Shape_is_enough_space_g_shaped(shape, rotation, grid, grid_state, x, y);
        break;
    default:
        break;
    }

    return true;
}
