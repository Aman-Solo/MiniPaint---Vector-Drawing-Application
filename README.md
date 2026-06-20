# Capstone: Mini Paint / Vector Drawing Application

### Computer Graphics (SITE 3rd Year)

**Class Section:** 2

**Team Members:**
1. Amanuel Solomon - UGR/0540/16
2. Fiker Yohannes - UGR/4617/16
3. Tsedenia Fiseha - UGR/9263/16
4. Yohannes Gumeta - UGR/4852/16
5. Zenebu Melaku - UGR/6058/16

---

## Project Overview
This project is an interactive 2D vector-based drawing program built completely from scratch using C++ and legacy OpenGL (FreeGLUT). Instead of pixel manipulation, the program creates mathematical objects that are drawn dynamically and can be selected, translated, scaled, and rotated in real-time.

## Project Breakdown

### 1: Core Architecture & Window Management
We established the core foundation of the application. This includes initiating the OpenGL context with FreeGLUT, generating a double-buffered display for smooth frame rendering, and setting up the event loop. We also implemented `gluOrtho2D` map projection, which allows us directly to treat our screen resolution (pixels) as 1:1 OpenGL world coordinate geometry.

### 2: Input Handling & Coordinate Mapping
We created the bridging structures to map human input to the render logic. Mouse callbacks (`glutMouseFunc`, `glutMotionFunc`, and `glutPassiveMotionFunc`) capture XY screen pixels directly to OpenGL coordinates without distortion. We also established global application states (`currentTool`, `isDrawing`) controlled through simple keyboard bindings.

### 3: Shape Data Models & Interactive Drawing
We engineered robust mathematical structures (`Point2D`, `ColorRGB`) and consolidated them inside a modular `Shape` class. Shapes store dynamic `std::vector` arrays of points. During this phase, we implemented "Rubber-banding", enabling users to see lines and polygons stretching to their mouse point dynamically *before* they click to finalize vertices onto the canvas.

### 4: Selection System & Application State
We transitioned the app into an interactive editor by building a selection hit-test system. Using an Axis-Aligned Bounding Box (AABB) collision algorithm equipped with mathematical padding, users can click freely drawn thin lines and shapes. Selected shapes provide visual feedback by rendering thicker and highlighting in red. Since shapes render back-to-front, the hit-test loops backward to properly select "top layer" overlapping elements.

### 5: 2D Affine Transformations
We brought the static geometry to life. Instead of mutating raw vertex coordinate data, we utilized OpenGL's native Matrix stack (`glPushMatrix()` / `glPopMatrix()`). We translate the system to the local geometric center of the selected shape, apply our transformations (`glScalef`, `glRotatef`, `glTranslatef`), and translate back. To endure this change mathematically, we apply **Inverse Matrices** to the mouse clicks to continue correctly identifying hit-tests even when a shape is heavily deformed or spun around!

---

## How to Run in VS Code

### Prerequisites
* **VS Code** with the C/C++ extension installed.
* **MinGW-w64** compiler installed (providing `g++`).
* **FreeGLUT libraries** installed and mounted in your MinGW environment.

### Steps to Compile and Run
1. Open this project folder in VS Code.
2. Open a new Terminal in VS Code (`Ctrl + \``).
3. Compile the source code by pasting this precise command into the terminal:
   ```bash
   g++ src/main.cpp -o MiniPaint.exe -lfreeglut -lopengl32 -lglu32
   ```
4. Once compilation finishes (you'll see `MiniPaint.exe` generated in the folder), run the application:
   ```bash
   ./MiniPaint.exe
   ```

---

## Application Controls

### Drawing Modes (Press Keys 1-3)
* **`1`** - Line Tool (Click once to start, second click finishes).
* **`2`** - Polyline Tool (Click to add points, **Right-Click** to finish).
* **`3`** - Polygon Tool (Click to add points, **Right-Click** to finish/close shape).

### Selection & Transformation Mode (Press Key 4)
* **`4`** - Selection Tool (Click a drawn shape to highlight it red).
* **`W` / `A` / `S` / `D`** - Translate/Move the selected shape.
* **`Q` / `E`** - Rotate the selected shape around its own center.
* **`+` (`=`) / `-` (`_`)** - Scale the selected shape larger or smaller.
* **`ESC`** - Exit Application completely.