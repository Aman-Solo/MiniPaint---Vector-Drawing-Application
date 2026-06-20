#ifndef SHAPE_H
#define SHAPE_H

#include <GL/freeglut.h>
#include <vector>
#include <algorithm> // For min/max
#include <cmath>

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
    float tx, ty; // translation
    float sx, sy; // scaling
    float angle;  // rotation in degrees

    Shape(ToolMode t, ColorRGB c) : type(t), color(c), isSelected(false), tx(0), ty(0), sx(1), sy(1), angle(0) {}

    // Computes the geometric center for pivot-based transformations
    Point2D getCenter() const {
        if (vertices.empty()) return Point2D(0,0);
        float cx = 0, cy = 0;
        for (const auto& v : vertices) {
            cx += v.x;
            cy += v.y;
        }
        return Point2D(cx / vertices.size(), cy / vertices.size());
    }

    // Hit Testing (Axis-Aligned Bounding Box)
    bool hitTest(float px, float py) const {
        if (vertices.empty()) return false;
        
        Point2D center = getCenter();
        
        // Because OpenGL handles the visual transformations independently, 
        // we must apply INVERSE transformations to the mouse coordinates to test it mathematically!
        px -= tx; py -= ty;                 // Undo Translation
        px -= center.x; py -= center.y;     // Move to origin
        
        float theta = -angle * 3.14159f / 180.0f; // Undo Rotation
        float rotX = px * cos(theta) - py * sin(theta);
        float rotY = px * sin(theta) + py * cos(theta);
        
        rotX /= sx; rotY /= sy;             // Undo Scale
        
        px = rotX + center.x;               // Move back from origin
        py = rotY + center.y;
        
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

        // Save current OpenGL matrix state
        glPushMatrix();
        
        // Apply sequence of affine matrices 
        // 1. Move to the overall translated position
        glTranslatef(tx, ty, 0.0f);

        // 2. Translate to center, apply rotation/scale, and translate back
        // This ensures the shape scales and rotates around its OWN center, not coordinates (0,0)
        Point2D center = getCenter();
        glTranslatef(center.x, center.y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(sx, sy, 1.0f);
        glTranslatef(-center.x, -center.y, 0.0f);

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
        
        // Restore previous matrix state so other shapes aren't affected
        glPopMatrix();
    }
};

#endif