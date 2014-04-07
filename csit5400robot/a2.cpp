/***********************************************************************/
/*                                                                     */
/*                    CSIT5400 Computer Graphics                       */
/*                Assignment 2 - Hierarchical Modeling                 */
/*                        Calvin Chi-kit Chiu                          */
/*                          ID: 20171958                               */
/*                 Android Robot in Mickey Mouse Suit                  */
/*                                                                     */
/*                          Compiler: Xcode 5.1                        */
/*                                                                     */
/*     2,3,Q,R are also used to rotate the robot in x,z direction      */
/*   G,H,J,K are also mapped to rotate the arms away/towards the body  */
/*                                                                     */
/***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "trackball.h"
#include "primitive.h"

// Define the size of the window using these two variables
int window_width = 500, window_height = 500;

// These constants define the motion mode of mouse control
#define MOTION_NONE			0
#define MOTION_TRACKBALL		1
#define MOTION_ZOOM			2
#define MOTION_PAN			3

// These variables are used for controlling the mouse motion
int motion = MOTION_NONE;
int lastx, lasty;
float quat[4];
float eyex = 0, eyey = 0, eyez = 800;

// The display list of the teapot scene
GLuint teapot_list;

// These constants represent the teapot/robot
#define DISPLAY_TEAPOT  0
#define DISPLAY_ROBOT   1

// Display teapot or robot
int current_display = DISPLAY_TEAPOT;

// angle of rotation of limbs, head, etc
float head_angle = 0;

float larm_angle = 0;
float larm_angle_w = 0;

float rarm_angle = 0;
float rarm_angle_w = 0;

float lleg_x_angle = 0;
float rleg_x_angle = 0;

float robot_angle_x = 0;
float robot_angle_y = 0;
float robot_angle_z = 0;

/*
 * Initialize the OpenGL settings and create the display list of the scene
 */
void InitSettings()
{
	// Reset the parameter for trackball motion
	trackball(quat, 0.0, 0.0, 0.0, 0.0);

	// Set up OpenGL parameters
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glClearColor(0.6, 0.6, 0.6, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Set up the light
	GLfloat position[] = {500, 500, 500, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);

	// Set up the material values for specular reflection
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shininess[] = {50.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	// Create a display list for the teapot scene
	teapot_list = glGenLists(1);
	glNewList(teapot_list, GL_COMPILE);

		// Create the objects: a teapot and a floor

		// The vertex ordering of the GLUT teapot is clockwise, therefore
		// glFrontFace(GL_CW) is used before drawing
		glFrontFace(GL_CW);
		glColor3f(1, 0.5, 0.5);
		glutSolidTeapot(250);
		glFrontFace(GL_CCW);

		// Draw the floor, note that the vertex ordering is counter-clockwise
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glNormal3f(0, 1, 0);
			glVertex3f(500, -190, 500);
			glVertex3f(500, -190, -500);
			glVertex3f(-500, -190, -500);
			glVertex3f(-500, -190, 500);
		glEnd();

	glEndList();
}


/*
 * The callback function when a redraw has been requested
 */
void DisplayFunc()
{
	// Create the screen for both colour and depth values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// The modelview matrix is pushed before drawing the scene
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set up the trackball transformation
	float rotmatrix[4][4];
	build_rotmatrix(rotmatrix, quat);
	glMultMatrixf(&rotmatrix[0][0]);

	// Draw the scene from the display list
	if (current_display == DISPLAY_TEAPOT)
		glCallList(teapot_list);
	else {
        
        // torso
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(robot_angle_y, 0, 1, 0);
        glRotatef(robot_angle_x, 1, 0, 0);
        glRotatef(robot_angle_z, 0, 0, 1);
        glTranslatef(0.0, -50.0, 0.0);
        DrawCylinder(160, 160, 200);
        glPushMatrix();
        
        // left shoulder
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(-175, 50, 0);
        glRotatef(90, 0, 0, 1);
        glRotatef(larm_angle, 0, 1, 0);
        glRotatef(larm_angle_w, 0, 0, 1);
        DrawSphere(35);
        
        // left arm
        glTranslatef(-60, 0, 0);
        glRotatef(90, 0, 0, 1);
        DrawCylinder(35, 35, 120);
        
        // left "palm"
        glColor3f(1.0,1.0,1.0);
        glTranslatef(0, 90, 0);
        DrawCylinder(35, 35, 60);
        
        glTranslatef(0, 25, 0);
        DrawSphere(35);
        
        
        glRotatef(-145, 0, 0, 1);
        glPopMatrix();
        glPushMatrix();
        
        // right shoulder
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(175, 50, 0);
        glRotatef(90, 0, 0, 1);
        glRotatef(rarm_angle, 0, 1, 0);
        glRotatef(rarm_angle_w, 0, 0, 1);
        DrawSphere(35);
        
        // right arm
        glTranslatef(-60, 0, 0);
        glRotatef(-90, 0, 0, 1);
        DrawCylinder(35, 35, 120);
        
        // right "palm"
        glColor3f(1.0,1.0,1.0);
        glTranslatef(0, -90, 0);
        DrawCylinder(35, 35, 60);
        
        glTranslatef(0, -25, 0);
        DrawSphere(35);

        
        glRotatef(145, 0, 0, 1);
        glPopMatrix();
        glPushMatrix();


        // mickey mouse's head
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(head_angle, 0, 1, 0);
        glTranslatef(0.0, 100.0, 0.0);
        DrawSphere(150);
        
        glPushMatrix();
        // dem ears
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(150.0, 150.0, 0.0);
        glRotatef(90, 1, 0, 0);
        DrawCylinder(80, 80, 20);
        
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(-300, 0, 0);
        DrawCylinder(80, 80, 20);
        
        glPopMatrix();
        glPushMatrix();
        
        // android's head
        glColor3f(0,1,0);
        glTranslatef(0, 0, 5);
        DrawSphere(150);
        
        // dem eyes
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(-50, 65, 116);
        glRotatef(90, 1, 0, 0);
        DrawCylinder(10, 10, 30);
        
        glTranslatef(100, 0, 0);
        DrawCylinder(10, 10, 30);
        
        glRotatef(-90, 1, 0, 0);
        
        
        // dem mouth... android robot DOES NOT have a mouth!
        glTranslatef(-50, -61, -120);
        DrawCylinder(155, 155, 5);
        
        // dem clear mutated mouth... just to fulfill the requirement!
        // remove if you feel uncomfortable looking at it
        //glTranslatef(0, 20, 130);
        //DrawBox(100,50,5);
        
        glPopMatrix();
        
        glRotatef(-90, 1, 0, 0);
        glPopMatrix();

        
        // pants
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(0.0, -100, 0.0);
        DrawCylinder(161, 161, 165);

        glPushMatrix();
        
        // buttons
        glColor3f(1.0,1.0,1.0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(50, 145, -20);
        DrawCylinder(40, 40, 35);
        glTranslatef(-100, 0, 0);
        DrawCylinder(40, 40, 35);
        
        // Tail
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(50, -350, 0);
        DrawCylinder(10, 10, 200);
        
        glTranslatef(0,-100,0);
        DrawSphere(10);
        
        glRotatef(-90, 1, 0, 0);
        glPopMatrix();
        glPushMatrix();

        
        
        // left ass + leg + shoe
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(lleg_x_angle, 1, 0, 0);
        glTranslatef(-70, -50, 0.0);
        DrawSphere(60);
        
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(0, -80, 0);
        DrawCylinder(35, 35, 60);
        
        glColor3f(1.0,1.0,0.0);
        glTranslatef(0, -50, 0);
        DrawCylinder(65, 65, 40);
        
        glPopMatrix();
        glPushMatrix();
        
        // right ass + leg + shoe
        
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(rleg_x_angle, 1, 0, 0);
        glTranslatef(70, -50.0, 0.0);
        DrawSphere(60);
        
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(0, -80, 0);
        DrawCylinder(35, 35, 60);
        
        glColor3f(1.0,1.0,0.0);
        glTranslatef(0, -50, 0);
        DrawCylinder(65, 65, 40);
        
        glPopMatrix();
        

	}

	glPopMatrix();

	// Redraw the screen by swapping the buffer
	glutSwapBuffers();
}




/*
 * The callback function when the window is resized
 */
void ReshapeFunc(int w, int h)
{
	// Set the viewport to be the same as the window
	glViewport(0, 0, w, h);

	// Set up the eye view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, eyez, 0, 0, 0, 0, 1, 0);

	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 2000.0);

	window_width = w;
	window_height = h;
}

/*
 * The callback function when a key is pressed
 */


void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
    case ',':
        robot_angle_y += 5;
        glutPostRedisplay();
        break;
            
    case '.':
        robot_angle_y -= 5;
        glutPostRedisplay();
        break;
            
    case 'q':
        robot_angle_x += 5;
        glutPostRedisplay();
        break;
            
    case 'r':
        robot_angle_x -= 5;
        glutPostRedisplay();
        break;
            
    case '2':
        robot_angle_z += 5;
        glutPostRedisplay();
        break;
    
    case '3':
        robot_angle_z -= 5;
        glutPostRedisplay();
        break;
            
    case 'w':
        head_angle += 5;
        glutPostRedisplay();
        break;
    
    case 'e':
        head_angle -= 5;
        glutPostRedisplay();
        break;
    
    case 'a':
        larm_angle += 5;
        glutPostRedisplay();
        break;
            
    case 's':
        larm_angle -= 5;
        glutPostRedisplay();
        break;
            
    case 'g':
        if(larm_angle_w!=-175)
            larm_angle_w -= 5;
        glutPostRedisplay();
        break;
            
    case 'h':
        if(larm_angle_w!=0)
            larm_angle_w +=5;
        glutPostRedisplay();
        break;
        
    case 'j':
        if(rarm_angle_w!=0)
            rarm_angle_w -= 5;
        glutPostRedisplay();
        break;
        
    case 'k':
        if(rarm_angle_w!=175)
            rarm_angle_w +=5;
        glutPostRedisplay();
        break;

    case 'd':
        rarm_angle += 5;
        glutPostRedisplay();
        break;
        
    case 'f':
        rarm_angle -= 5;
        glutPostRedisplay();
        break;
            
    case 'x':
        if(lleg_x_angle!=45)
            lleg_x_angle += 5;
        glutPostRedisplay();
        break;
            
    case 'z':
        if(lleg_x_angle!=-45)
            lleg_x_angle -= 5;
        glutPostRedisplay();
        break;
            
    case 'v':
        if(rleg_x_angle!=45)
            rleg_x_angle += 5;
        glutPostRedisplay();
        break;

    case 'c':
        if(rleg_x_angle!=-45)
            rleg_x_angle -= 5;
        glutPostRedisplay();
        break;
            
	case ' ':
		// Switch the scene between teapot/tin robot
		if (current_display == DISPLAY_TEAPOT)
			current_display = DISPLAY_ROBOT;
		else
			current_display = DISPLAY_TEAPOT;
		glutPostRedisplay();
		break;
	case 27:
		// Exit the program using the escape key
		exit(0);
		break;
	}
}

/*
 * The callback function when a mouse button is pressed
 */
void MouseFunc(int button, int state, int x, int y)
{
	// Control the current motion of the mouse control
	if (state == GLUT_DOWN) {
		lastx = x;
		lasty = y;

		motion = MOTION_NONE;
		if (button == GLUT_LEFT_BUTTON)
			motion = MOTION_TRACKBALL;
		else if (button == GLUT_RIGHT_BUTTON) {
			if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
				motion = MOTION_PAN;
			else
				motion = MOTION_ZOOM;
		}
	}
}

/*
 * The callback function when the mouse is moved
 */
void MotionFunc(int x, int y)
{
	// Calculate the transformation for trackball control
	if (motion == MOTION_TRACKBALL) {
		float dq[4];
		trackball(dq,
				  (2.0 * lastx - window_width) / window_width,
				  (window_height - 2.0 * lasty) / window_height,
				  (2.0 * x - window_width) / window_width,
				  (window_height - 2.0 * y) / window_height);
		add_quats(dq, quat, quat);

		lastx = x;
		lasty = y;

		glutPostRedisplay();
	}
	// Adjust the eye position for zooming
	else if (motion == MOTION_ZOOM) {
		eyez = eyez * (1.0 - (float) (lasty - y) / window_height);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyex, eyey, eyez, eyex, eyey, 0, 0, 1, 0);

		lastx = x;
		lasty = y;

		glutPostRedisplay();
	}
	// Adjust the eye position for panning
	else if (motion == MOTION_PAN) {
		eyex -= 2.0 * (x - lastx);
		eyey += 2.0 * (y - lasty);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyex, eyey, eyez, eyex, eyey, 0, 0, 1, 0);

		lastx = x;
		lasty = y;

		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	// Initialize the window with double and depth buffering
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Assignment 2 - Hierarchical Modeling");

	// Initialize the OpenGL settings
	InitSettings();

	// Set up the callback functions
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutReshapeFunc(ReshapeFunc);
	glutDisplayFunc(DisplayFunc);

	// Start the main loop
	glutMainLoop();
}
