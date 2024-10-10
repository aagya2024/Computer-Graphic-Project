#include <GL/glut.h>
#include <vector>
#include <cmath>

struct Point {
    float x, y, z;
};

// Control points for a 3D heart shape using Bézier surface
std::vector<std::vector<Point>> controlPoints = {
    // Control points for the heart shape
    {
        {  0.0f,  0.5f,  3.0f},  // Top center
        { -6.0f,  6.0f,  3.0f},  // Left top lobe
        { -10.0f,  1.5f,  3.0f},  // Left top lobe
        { -6.0f,  -6.0f,  3.0f},  // Left top lobe
        {  0.0f,  -0.5f,  3.0f}   // Close the loop at the top center
    },
    {
        { 0.0f,  0.5f, 3.0f},  // Top center
        { 6.0f,  6.0f,  3.0f},  // Left top lobe
        { 10.0f,  1.5f,  3.0f},  // Left top lobe
        { 6.0f,  -6.0f,  3.0f},  // Left top lobe
        { 0.0f,  -0.5f,  3.0f}   // Close the loop at the top center
    }
};

// Evaluate a point on a Bézier surface
Point bezierSurface(float u, float v) {
    Point point = {0.0f, 0.0f, 0.0f};
    int n = controlPoints.size() - 1;
    int m = controlPoints[0].size() - 1;

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            float bernsteinU = pow(1 - u, n - i) * pow(u, i);
            float bernsteinV = pow(1 - v, m - j) * pow(v, j);
            point.x += controlPoints[i][j].x * bernsteinU * bernsteinV;
            point.y += controlPoints[i][j].y * bernsteinU * bernsteinV;
            point.z += controlPoints[i][j].z * bernsteinU * bernsteinV;
        }
    }
    return point;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up the camera
    gluLookAt(0.0, 0.0, 5.0,  // Eye position
              0.0, 0.0, 0.0,  // Center position
              0.0, 1.0, 0.0); // Up vector

    // Draw the Bézier surface
    glBegin(GL_POINTS);
    int uDivs = 100; // Number of divisions along u axis
    int vDivs = 100; // Number of divisions along v axis
    for (int i = 0; i <= uDivs; ++i) {
        float u = (float)i / uDivs;
        for (int j = 0; j <= vDivs; ++j) {
            float v = (float)j / vDivs;
            Point p = bezierSurface(u, v);
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Set background color to black
    glColor3f(1.0, 0.0, 0.0);  // Set drawing color to red
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Set the viewport to cover the new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bézier Surface Heart");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}