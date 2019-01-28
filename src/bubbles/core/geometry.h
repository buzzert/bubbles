/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <math.h>
#include <bubbles/core/foundation.h>

#define RECT_ZERO Rect()

BUBBLES_NAMESPACE_BEGIN

struct Rect
{
    float x;
    float y;

    int width;
    int height;

    Rect() : x(0), y(0), width(0), height(0) {};
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {};

    bool operator==(Rect &other) {
        return (
            x == other.x &&
            y == other.y &&
            width == other.width &&
            height == other.height
        );
    }

    bool operator!=(Rect &other) {
        return !(*this == other);
    }

    std::string to_string() const {
        return "Rect(" + std::to_string(x) + ", " + std::to_string(y)
                       + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    };
};

struct Vec2 {
    double x;
    double y;
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
