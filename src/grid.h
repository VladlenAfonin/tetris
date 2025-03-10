#ifndef _GRID_H
#define _GRID_H

#include "types.h"

#define GridState_create(var, grid) GridCell var[grid.size_x * grid.size_y];
#define GridState_init(grid_state, grid)                                                                               \
    for (int i = 0; i < grid.size_y; i++)                                                                              \
    {                                                                                                                  \
        for (int j = 0; j < grid.size_x; j++)                                                                          \
        {                                                                                                              \
            grid_state[j + i * grid.size_x] = (GridCell){.type = empty, .color = BLACK};                               \
        }                                                                                                              \
    }

static inline GridCell GridState_get(Grid grid, GridState grid_state, int x, int y)
{
    return grid_state[x + y * grid.size_x];
}

static inline void GridState_set(Grid grid, GridState grid_state, int x, int y, GridCell cell)
{
    grid_state[x + y * grid.size_x] = cell;
}

void Grid_draw(Grid grid, GridState grid_state);
/// Non-player related update.
bool GridState_update_game(Grid grid, GridState *grid_state, Shape *current_shape);
/// Player related update.
bool GridState_update_player(Grid grid, GridState *grid_state, Shape *current_shape, int input, bool should_rotate);

#endif // _GRID_H
