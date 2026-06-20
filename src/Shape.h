#ifndef SHAPE_H
#define SHAPE_H

#include <GL/freeglut.h>
#include <vector>

// Mathematical structure for 2D Coordinate Mapping
struct Point2D {
    float x, y;
    Point2D(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct ColorRGB {
    float r, g, b;
    ColorRGB(float _r = 0, float _g = 0, float _b = 0) : r(_r), g(_g), b(_b) {}
};

// Represents the "State" or active tool mode the application is in
enum ToolMode {
    TOOL_LINE,
    TOOL_POLYLINE,
    TOOL_POLYGON,
    TOOL_SELECT
};

// Base mathematical object representing a drawn geometry
class Shape {
public:
    ToolMode type;
    std::vector<Point2D> vertices;
    ColorRGB color;

    Shape(ToolMode t, ColorRGB c) : type(t), color(c) {}

    void draw() const {
        if (vertices.empty()) return;

        // Set the color for the shape dynamically
        glColor3f(color.r, color.g, color.b);
        
        if (type == TOOL_LINE) {
            glBegin(GL_LINES);
            for (const auto& v : vertices) glVertex2f(v.x, v.y);
            glEnd();
        } 
        else if (type == TOOL_POLYLINE) {
            glBegin(GL_LINE_STRIP);
            for (const auto& v : vertices) glVertex2f(v.x, v.y);
            glEnd();
        }
        else if (type == TOOL_POLYGON) {
            glBegin(GL_POLYGON);
            for (const auto& v : vertices) glVertex2f(v.x, v.y);
            glEnd();
        }
    }
};

#endif
