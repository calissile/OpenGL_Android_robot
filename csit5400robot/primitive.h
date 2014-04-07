#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SPHERE_SLICE    40
#define SPHERE_STACK    40

#define CYLINDER_SLICE      40
#define CYLINDER_STACK      20
#define CYLINDER_CAPLOOP    10

void DrawBox(float, float, float);
void DrawCylinder(float, float, float);
void DrawSphere(float);
