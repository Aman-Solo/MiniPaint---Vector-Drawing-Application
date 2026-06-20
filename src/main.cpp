#include <GL/freeglut.h>
#include <iostream>

// Window dimensions
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// --- Callback Functions ---

void display() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Set background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // TODO: Loop through and draw all shapes here

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
    // TODO: Handle mouse input for drawing and selecting
    if (state == GLUT_DOWN) {
        std::cout << "Mouse clicked at: (" << x << ", " << y << ")" << std::endl;
    }
}

void keyboard(unsigned char key, int x, int y) {
    // TODO: Handle keyboard shortcuts (changing tools, esc to quit)
    if (key == 27) { // Escape key
        exit(0);
    }
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
    glutKeyboardFunc(keyboard);

    // 5. Start the main event loop
    glutMainLoop();

    return 0;
}
