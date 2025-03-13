#ifndef _GRID_H
#define _GRID_H

#include "globals.h"
#include "types.h"

#define GridState_create(var) GridCell var[global_parameters.size_x * global_parameters.size_y];
#define GridState_init(grid_state)                                                                                     \
    for (int i = 0; i < global_parameters.size_y; i++)                                                                 \
    {                                                                                                                  \
        for (int j = 0; j < global_parameters.size_x; j++)                                                             \
        {                                                                                                              \
            grid_state[j + i * global_parameters.size_x] = (GridCell){.type = empty, .color = BLACK};                  \
        }                                                                                                              \
    }

static inline GridCell GridState_get(GridState grid_state, int x, int y)
{
    return grid_state[x + y * global_parameters.size_x];
}

static inline void GridState_set(GridState grid_state, int x, int y, GridCell cell)
{
    grid_state[x + y * global_parameters.size_x] = cell;
}

void Grid_draw(GridState grid_state);
/// Non-player related update.
bool GridState_update_game(GridState *grid_state);
/// Player related update.
bool GridState_update_player(GridState *grid_state, int input, bool should_rotate);

#endif // _GRID_H
