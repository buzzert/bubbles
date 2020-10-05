/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <math.h>
#include <bubbles/core/foundation.h>

BUBBLES_NAMESPACE_BEGIN

struct Rect
{
    float x;
    float y;

    int width;
    int height;

    Rect() : x(0), y(0), width(0), height(0) {};
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {};

    bool operator==(const Rect &other) const {
        return (
            x == other.x &&
            y == other.y &&
            width == other.width &&
            height == other.height
        );
    }

    bool operator!=(const Rect &other) const {
        return !(*this == other);
    }

    Rect inset_by(double _x, double _y) const
    {
        return Rect(x + _x, y + _y, width - _x * 2.0, height - _y * 2.0);
    }

    std::string to_string() const {
        return "Rect(" + std::to_string(x) + ", " + std::to_string(y)
                       + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    };

    bool point_inside(float _x, float _y) const
    {
        return (_x > x && _x < (x + width)) && (_y > y && _y < (y + height));
    }
};

static const Rect RECT_ZERO = Rect(0, 0, 0, 0);

struct Size
{
    double width;
    double height;

    Size(double w, double h) : width(w), height(h) {};
};

struct Vec2 {
    double x;
    double y;

    Vec2(double x, double y) : x(x), y(y) {};
};

struct Vec3 {
    double x;
    double y;
    double z;

    double& operator[] (const int index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw;
        }
    }
};

// For lack of a matrix type...
typedef std::vector<std::vector<double>> matrix_t;

Vec3 rotate_x(Vec3 p, double angle);
Vec3 rotate_y(Vec3 p, double angle);
Vec3 rotate_z(Vec3 p, double angle);

Vec3 matrix_vector_product(matrix_t matrix, Vec3 p);

BUBBLES_NAMESPACE_END
