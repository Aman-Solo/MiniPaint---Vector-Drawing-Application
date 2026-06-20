#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include "Shape.h"

// Window dimensions
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// --- Application State & Data Models ---
std::vector<Shape> shapes;
ToolMode currentTool = TOOL_LINE;
ColorRGB currentColor(0.0f, 0.0f, 0.0f); // Default draw color is black
bool isDrawing = false;
Shape currentShape(TOOL_LINE, currentColor);
Point2D mousePos(0, 0);

// --- Callback Functions ---

void display() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Set background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Render all finalized shapes on our canvas
    for (const auto& shape : shapes) {
        shape.draw();
    }

    // Render the interactive shape currently being created ("rubber-banding")
    if (isDrawing) {
        Shape previewShape = currentShape;
        previewShape.vertices.push_back(mousePos); // dynamically lock to moving mouse
        
        // Polygons less than 3 vertices shouldn't be filled, draw them as polylines
        if (previewShape.type == TOOL_POLYGON && previewShape.vertices.size() < 3) {
            previewShape.type = TOOL_POLYLINE;
        }
        previewShape.draw();
    }

    // Swap buffers for smooth drawing (double buffering)
    glutSwapBuffers();
}

void reshape(int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // 2D Orthographic Projection: map screen directly to 2D coordinates
    // left, right, bottom, top, near, far
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!isDrawing) {
            // Initiate a new shape
            isDrawing = true;
            currentShape = Shape(currentTool, currentColor);
            currentShape.vertices.push_back(Point2D(x, y));
        } else {
            // Anchor a point to the progressing shape
            currentShape.vertices.push_back(Point2D(x, y));
            
            // Lines are simple: commit immediately on the second click
            if (currentTool == TOOL_LINE && currentShape.vertices.size() == 2) {
                shapes.push_back(currentShape);
                isDrawing = false;
            }
        }
    } 
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Right-click commits Polylines and Polygons
        if (isDrawing && (currentTool == TOOL_POLYLINE || currentTool == TOOL_POLYGON)) {
            if (currentShape.vertices.size() >= 2) {
                shapes.push_back(currentShape); // Save to the mathematical canvas list
            }
            isDrawing = false;
        }
    }
    glutPostRedisplay(); // Request a redraw to update the immediate frame
}

// Triggers when dragging while holding a mouse button down
void mouseMotion(int x, int y) {
    mousePos.x = x;
    mousePos.y = y;
    if (isDrawing) glutPostRedisplay();
}

// Triggers when moving the mouse freely (no buttons held)
void mousePassiveMotion(int x, int y) {
    mousePos.x = x;
    mousePos.y = y;
    if (isDrawing) glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // Escape key allows graceful exit
        exit(0);
    } 
    else if (key == '1') {
        currentTool = TOOL_LINE;
        isDrawing = false; // Reset state when switching tools
        std::cout << "Switched Tool Mode --> LINE" << std::endl;
    } 
    else if (key == '2') {
        currentTool = TOOL_POLYLINE;
        isDrawing = false;
        std::cout << "Switched Tool Mode --> POLYLINE" << std::endl;
    } 
    else if (key == '3') {
        currentTool = TOOL_POLYGON;
        isDrawing = false;
        std::cout << "Switched Tool Mode --> POLYGON" << std::endl;
    }
    glutPostRedisplay();
}

// --- Initialization ---

void initOpenGL() {
    // Basic OpenGL state setup
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White canvas
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Smooth lines (anti-aliasing)
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
    // 1. Initialize GLUT
    glutInit(&argc, argv);
    
    // 2. Configure window properties
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // Double buffering, RGBA colors
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Capstone: Vector Drawing Application");

    // 3. Initialize OpenGL states
    initOpenGL();

    // 4. Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);         // Added motion triggers
    glutPassiveMotionFunc(mousePassiveMotion);
    glutKeyboardFunc(keyboard);

    // 5. Start the main event loop
    glutMainLoop();

    return 0;
}
