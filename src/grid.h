#ifndef _GRID_H
#define _GRID_H

#include "raylib.h"
typedef struct _Grid
{
    const int size_x;
    const int size_y;
    const float side_size;
    const float line_thickness;
    const Vector2 offset;
} Grid;

typedef enum _CellType
{
    empty,
    color_red,
} CellType;

typedef CellType GridState[];

#define GridState_create(var, grid) CellType var[grid.size_x * grid.size_y];
#define GridState_init(grid_state, grid)                                                                               \
    for (int i = 0; i < grid.size_y; i++)                                                                              \
    {                                                                                                                  \
        for (int j = 0; j < grid.size_x; j++)                                                                          \
        {                                                                                                              \
            grid_state[j + i * grid.size_x] = empty;                                                                   \
        }                                                                                                              \
    }

static inline CellType GridState_get(Grid grid, GridState grid_state, int x, int y)
{
    return grid_state[x + y * grid.size_x];
}

static inline void GridState_set(Grid grid, GridState grid_state, int x, int y, CellType cell_type)
{
    grid_state[x + y * grid.size_x] = cell_type;
}

void Grid_draw(Grid grid, GridState grid_state);
void GridState_update(Grid grid, GridState *grid_state);

#endif // _GRID_H
