#ifndef _TYPES_H
#define _TYPES_H

#include "raylib.h"

typedef enum _ShapeType
{
    // # # #
    //     #
    g_shaped,
    // # # #
    // #
    g_shaped_reversed,
    // # #
    // # #
    box,
    // # # # #
    line,
    // # #
    //   # #
    z_shaped,
    //   # #
    // # #
    z_shaped_reversed,
    //   #
    // # # #
    triangle,
} ShapeType;

typedef enum _ShapeRotationDirection
{
    /// Counterclockwise rotation.
    left = -1,
    /// Clockwise rotation.
    right = 1,
} ShapeRotationDirection;

/// A number mod 4, as there can be at most 4 distinct rotations.
typedef int ShapeRotation;

typedef struct _Shape
{
    /// Top left corner X coordinate.
    int pos_x;
    /// Top left corner Y coordinate.
    int pos_y;
    ShapeType type;
    ShapeRotation rotation;
} Shape;

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

#endif // _TYPES_H
