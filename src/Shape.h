#ifndef SHAPE_H
#define SHAPE_H

#include <GL/freeglut.h>
#include <vector>
#include <algorithm> // For min/max

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
    bool isSelected;

    Shape(ToolMode t, ColorRGB c) : type(t), color(c), isSelected(false) {}

    // Hit Testing (Axis-Aligned Bounding Box)
    bool hitTest(float px, float py) const {
        if (vertices.empty()) return false;
        
        float minX = vertices[0].x, maxX = vertices[0].x;
        float minY = vertices[0].y, maxY = vertices[0].y;
        
        for (const auto& v : vertices) {
            minX = std::min(minX, v.x);
            maxX = std::max(maxX, v.x);
            minY = std::min(minY, v.y);
            maxY = std::max(maxY, v.y);
        }
        
        // Add a 5 pixel mathematical padding so small lines are clickable
        float padding = 5.0f; 
        return (px >= minX - padding && px <= maxX + padding &&
                py >= minY - padding && py <= maxY + padding);
    }

    void draw() const {
        if (vertices.empty()) return;

        // Visual feedback for selection
        if (isSelected) {
            glColor3f(1.0f, 0.0f, 0.0f); // Highlight selected shapes in Red
            glLineWidth(3.0f);           // Make them thicker
        } else {
            glColor3f(color.r, color.g, color.b);
            glLineWidth(1.0f);
        }
        
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
