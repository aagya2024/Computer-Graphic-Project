#include <GL/glut.h>  
#include <vector>  
#include <cmath>  

struct Point {  
    float x, y, z;  
};  

Point bezierSurfacePoint(const std::vector<std::vector<Point>>& controlPoints, float u, float v) {  
    Point result = {0.0, 0.0, 0.0};  
    size_t m = controlPoints.size() - 1;  
    size_t n = controlPoints[0].size() - 1;  

    for (size_t i = 0; i <= m; ++i) {  
        for (size_t j = 0; j <= n; ++j) {  
            float binomialCoeffU = tgamma(m + 1) / (tgamma(i + 1) * tgamma(m - i + 1));  
            float binomialCoeffV = tgamma(n + 1) / (tgamma(j + 1) * tgamma(n - j + 1));  
            float term = binomialCoeffU * pow(u, i) * pow(1 - u, m - i) * binomialCoeffV * pow(v, j) * pow(1 - v, n - j);  
            result.x += term * controlPoints[i][j].x;  
            result.y += term * controlPoints[i][j].y;  
            result.z += term * controlPoints[i][j].z;  
        }  
    }  
    return result;  
}  

// Updated control points to create a more pronounced mountain shape  
std::vector<std::vector<Point>> controlPoints = {  
    {{-1.0, 0.0, 0.0}, {-0.5, 0.5, 0.5}, {0.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, {1.0, 0.0, 0.0}},  
    {{-1.0, 0.0, -0.5}, {-0.5, 0.3, 0.3}, {0.0, 0.5, 0.8}, {0.5, 0.3, 0.3}, {1.0, 0.0, -0.5}},  
    {{-1.0, 0.0, -0.7}, {-0.5, 0.3, 0.0}, {0.0, 0.0, 0.5}, {0.5, 0.3, 0.0}, {1.0, 0.0, -0.7}},  
    {{-1.0, 0.0, -1.0}, {-0.5, 0.0, -0.2}, {0.0, 0.2, 0.0}, {0.5, 0.0, -0.2}, {1.0, 0.0, -1.0}}  
};  

void display() {  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Set the color to green  
    glColor3f(0.0, 1.0, 0.0);  

    // Draw the Bézier surface  
    for (float u = 0; u < 1; u += 0.05) {  
        glBegin(GL_TRIANGLE_STRIP);  
        for (float v = 0; v <= 1; v += 0.05) {  
            Point p1 = bezierSurfacePoint(controlPoints, u, v);  
            Point p2 = bezierSurfacePoint(controlPoints, u + 0.05, v);  
            glVertex3f(p1.x, p1.y, p1.z);  
            glVertex3f(p2.x, p2.y, p2.z);  
        }  
        glEnd();  
    }  

    glutSwapBuffers();  
}  

void init() {  
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set the background color to white  
    glEnable(GL_DEPTH_TEST);         // Enable depth testing  
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();  
    gluPerspective(45.0, 1.0, 1.0, 10.0); // Set the perspective projection  
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();  
    gluLookAt(0.0, 2.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Set the camera position and orientation  
}  

int main(int argc, char** argv) {  
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  
    glutInitWindowSize(500, 500);  
    glutInitWindowPosition(100, 100);  
    glutCreateWindow("Mountains using Bézier Surfaces");  
    init();  
    glutDisplayFunc(display);  
    glutMainLoop();  
    return 0;  
}