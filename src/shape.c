#include "shape.h"
#include "colors.h"
#include "grid.h"
#include "raylib.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

static int g_shaped_reversed_numbers[4 * 2 * 4] = {
    // # # #
    // #
    0, 0, 1, 0, 2, 0, 0, 1,
    // # #
    //   #
    //   #
    1, 0, 1, 1, 1, 2, 0, 0,
    //     #
    // # # #
    2, 0, 0, 1, 1, 1, 2, 1,
    // #
    // #
    // # #
    0, 0, 1, 2, 0, 1, 0, 2,
};
// 4 rotations, borders for X and Y for each (in that order).
static int g_shaped_reversed_rotation_borders[4 * 2] = {
    3, 2,
    2, 3,
    3, 2,
    2, 3,
};

static int box_numbers[4 * 8] = {
    0, 0, 0, 1, 1, 0, 1, 1,
    0, 0, 0, 1, 1, 0, 1, 1,
    0, 0, 0, 1, 1, 0, 1, 1,
    0, 0, 0, 1, 1, 0, 1, 1,
};
static int box_rotation_borders[2 * 4] = {
    2, 2,
    2, 2,
    2, 2,
    2, 2,
};

static int line_numbers[4 * 8] = {
    // # # # #
    0, 0, 1, 0, 2, 0, 3, 0,
    // #
    // #
    // #
    // #
    0, 0, 0, 1, 0, 2, 0, 3,

    0, 0, 1, 0, 2, 0, 3, 0,
    0, 0, 0, 1, 0, 2, 0, 3,
};
static int line_rotation_borders[2 * 4] = {
    4, 1,
    1, 4,
    4, 1,
    1, 4,
};

static int z_shaped_numbers[4 * 8] = {
    //   # #
    // # #
    0, 1, 1, 0, 1, 1, 2, 0,
    // #
    // # #
    //   #
    0, 0, 0, 1, 1, 1, 1, 2,

    0, 1, 1, 0, 1, 1, 2, 0,
    0, 0, 0, 1, 1, 1, 1, 2,
};
static int z_shaped_rotation_borders[2 * 4] = {
    3, 2,
    2, 3,
    3, 2,
    2, 3,
};

static int z_shaped_reversed_numbers[4 * 8] = {
    // # #
    //   # #
    0, 0, 1, 0, 1, 1, 2, 1,
    //   #
    // # #
    // #
    1, 0, 0, 1, 1, 1, 0, 2,

    0, 0, 1, 0, 1, 1, 2, 1,
    1, 0, 0, 1, 1, 1, 0, 2,
};
static int z_shaped_reversed_rotation_borders[2 * 4] = {
    3, 2,
    2, 3,
    3, 2,
    2, 3,
};
static int triangle_numbers[4 * 8] = {
    //   #
    // # # #
    0, 1, 1, 0, 1, 1, 2, 1,
    // #
    // # #
    // #
    0, 0, 0, 1, 1, 1, 0, 2,
    // # # #
    //   #
    0, 0, 1, 0, 2, 0, 1, 1,
    //   #
    // # #
    //   #
    0, 1, 1, 0, 1, 1, 1, 2,
};
static int triangle_rotation_borders[2 * 4] = {
    3, 2,
    2, 3,
    3, 2,
    2, 3,
};
// clang-format on

static inline int *get_rotation_borders(ShapeType type, int rotation)
{
    switch (type)
    {
    case g_shaped:
        return g_shaped_rotation_borders + rotation * 2;
        break;
    case g_shaped_reversed:
        return g_shaped_reversed_rotation_borders + rotation * 2;
        break;
    case box:
        return box_rotation_borders + rotation * 2;
        break;
    case line:
        return line_rotation_borders + rotation * 2;
        break;
    case z_shaped:
        return z_shaped_rotation_borders + rotation * 2;
        break;
    case z_shaped_reversed:
        return z_shaped_reversed_rotation_borders + rotation * 2;
        break;
    case triangle:
        return triangle_rotation_borders + rotation * 2;
        break;
    default:
        // Unreachable.
        // TODO: Print error, crash or something.
        return NULL;
        break;
    }
}

static inline int *get_numbers(ShapeType type, int rotation)
{
    switch (type)
    {
    case g_shaped:
        return g_shaped_numbers + rotation * 8;
        break;
    case g_shaped_reversed:
        return g_shaped_reversed_numbers + rotation * 8;
        break;
    case box:
        return box_numbers + rotation * 8;
        break;
    case line:
        return line_numbers + rotation * 8;
        break;
    case z_shaped:
        return z_shaped_numbers + rotation * 8;
        break;
    case z_shaped_reversed:
        return z_shaped_reversed_numbers + rotation * 8;
        break;
    case triangle:
        return triangle_numbers + rotation * 8;
        break;
    default:
        // Unreachable.
        return NULL;
        break;
    }
}

Shape get_random_shape()
{
    ShapeType random_type = (ShapeType)GetRandomValue(1, 7);
    Color random_color = get_random_color();

    return (Shape){
        .rotation = 0,
        .type = random_type,
        .pos_x = 4,
        .pos_y = 0,
        .color = random_color,
    };
}

static void Shape_apply(Shape shape, Grid grid, GridState *grid_state, CellType cell_type)
{
    int *row = get_numbers(shape.type, shape.rotation);
    for (int i = 0; i < 4; i++)
    {
        GridState_set(grid, *grid_state, shape.pos_x + row[2 * i], shape.pos_y + row[2 * i + 1],
                      (GridCell){.type = cell_type, .color = shape.color});
    }
}

void Shape_unset(Shape shape, Grid grid, GridState *grid_state)
{
    Shape_apply(shape, grid, grid_state, empty);
}

void Shape_set(Shape shape, Grid grid, GridState *grid_state)
{
    Shape_apply(shape, grid, grid_state, filled);
}

static bool Shape_check_space(ShapeType shape, int rotation, Grid grid, GridState grid_state, int x, int y)
{
    int *border_row = get_rotation_borders(shape, rotation);
    if (y < 0 || x < 0 || x + border_row[0] > grid.size_x || y + border_row[1] > grid.size_y)
    {
        return false;
    }

    int *row = get_numbers(shape, rotation);
    GridCell cell;
    for (int i = 0; i < 4; i++)
    {
        cell = GridState_get(grid, grid_state, x + row[2 * i], y + row[2 * i + 1]);
        if (empty != cell.type)
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
    // TODO: Remove this indirection.
    return Shape_check_space(shape, rotation, grid, grid_state, x, y);
}
