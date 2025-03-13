#ifndef _SHAPE_H
#define _SHAPE_H

#include "types.h"
#include <stdbool.h>

/// Used for random sampling.
static ShapeType available_shape_types[7] = {g_shaped, g_shaped_reversed, box, line, z_shaped, z_shaped_reversed};

static inline void Shape_rotate(Shape shape, ShapeRotationDirection direction)
{
    shape.rotation = (shape.rotation + (int)direction) % 4;
}

void Shape_set(Shape shape, GridState *grid_state);
void Shape_unset(Shape shape, GridState *grid_state);
bool ShapeType_is_enough_space(ShapeType type, int rotation, GridState grid_state, int x, int y);
bool Shape_is_enough_space_for_self(Shape shape, GridState grid_state);
Shape get_random_shape();

#endif // _SHAPE_H
