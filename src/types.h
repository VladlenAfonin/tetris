#ifndef _TYPES_H
#define _TYPES_H

#include "raylib.h"

typedef enum _ShapeType
{
    // # # #
    //     #
    g_shaped = 1,
    // # # #
    // #
    g_shaped_reversed = 2,
    // # #
    // # #
    box = 3,
    // # # # #
    line = 4,
    //   # #
    // # #
    z_shaped = 5,
    // # #
    //   # #
    z_shaped_reversed = 6,
    //   #
    // # # #
    triangle = 7,
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
    Color color;
} Shape;

typedef struct _GlobalParameters
{
    int size_x;
    int size_y;
    float side_size;
    float line_thickness;
    Vector2 offset;
    int score_height;
    int screen_size_x;
    int screen_size_y;
} GlobalParameters;

typedef struct _GlobalState
{
    int score;
    Shape current_shape;
    Shape next_shape;
} GlobalState;

typedef enum _CellType
{
    empty,
    filled,
} CellType;

typedef struct _GridCell
{
    CellType type;
    Color color;
} GridCell;

typedef GridCell GridState[];

#endif // _TYPES_H
