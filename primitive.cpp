#include "primitive.h"

/*
 * Draw a box centered at the origin
 */
void DrawBox(float width, float length, float height) {
	float halfw = width / 2.0;
	float halfl = length / 2.0;
	float halfh = height / 2.0;

	glBegin(GL_QUADS);

	// Bottom face
	glNormal3f(0, -1, 0);
	glVertex3f(-halfw, -halfh, -halfl);
	glVertex3f( halfw, -halfh, -halfl);
	glVertex3f( halfw, -halfh,  halfl);
	glVertex3f(-halfw, -halfh,  halfl);

	// Left face
	glNormal3f(-1, 0, 0);
	glVertex3f(-halfw, -halfh, -halfl);
	glVertex3f(-halfw, -halfh,  halfl);
	glVertex3f(-halfw,  halfh,  halfl);
	glVertex3f(-halfw,  halfh, -halfl);

	// Right face
	glNormal3f(1, 0, 0);
	glVertex3f( halfw, -halfh, -halfl);
	glVertex3f( halfw,  halfh, -halfl);
	glVertex3f( halfw,  halfh,  halfl);
	glVertex3f( halfw, -halfh,  halfl);

	// Front face
	glNormal3f(0, 0, 1);
	glVertex3f(-halfw, -halfh,  halfl);
	glVertex3f( halfw, -halfh,  halfl);
	glVertex3f( halfw,  halfh,  halfl);
	glVertex3f(-halfw,  halfh,  halfl);

	// Back face
	glNormal3f(0, 0, -1);
	glVertex3f(-halfw, -halfh, -halfl);
	glVertex3f(-halfw,  halfh, -halfl);
	glVertex3f( halfw,  halfh, -halfl);
	glVertex3f( halfw, -halfh, -halfl);

	// Top face
	glNormal3f(0, 1, 0);
	glVertex3f(-halfw,  halfh, -halfl);
	glVertex3f(-halfw,  halfh,  halfl);
	glVertex3f( halfw,  halfh,  halfl);
	glVertex3f( halfw,  halfh, -halfl);

	glEnd();
}

/*
 * Draw a cylinder centered at the origin
 */
void DrawCylinder(float base, float top, float height) {
	// Rotate 90 degree to align the cylinder with the y axis
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -height / 2.0);

	// Draw the cylinder body
	GLUquadricObj *quadric_obj = gluNewQuadric();
	gluCylinder(quadric_obj, base, top, height, CYLINDER_SLICE, CYLINDER_STACK);
	gluDeleteQuadric(quadric_obj);

	glPopMatrix();

	// Draw the bottom cap
	glPushMatrix();
	glTranslatef(0, -height / 2.0, 0);
	glRotatef(90, 1, 0, 0);

	quadric_obj = gluNewQuadric();
	gluDisk(quadric_obj, 0, base, CYLINDER_SLICE, CYLINDER_CAPLOOP);
	gluDeleteQuadric(quadric_obj);

	glPopMatrix();

	// Draw the top cap
	glPushMatrix();
	glTranslatef(0, height / 2.0, 0);
	glRotatef(-90, 1, 0, 0);

	quadric_obj = gluNewQuadric();
	gluDisk(quadric_obj, 0, top, CYLINDER_SLICE, CYLINDER_CAPLOOP);
	gluDeleteQuadric(quadric_obj);

	glPopMatrix();
}

/*
 * Draw a sphere centered at the origin
 */
void DrawSphere(float radius) {
	GLUquadricObj *quadric_obj = gluNewQuadric();
	gluSphere(quadric_obj, radius, SPHERE_SLICE, SPHERE_STACK);
	gluDeleteQuadric(quadric_obj);
}
