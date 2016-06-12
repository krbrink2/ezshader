#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <math.h>
#include "ezloader.h"
#include "ezshader.h"

GLfloat angle;
GLfloat elevation;
GLfloat distance;
// Model stuff
GLfloat scalar = .01;
GLfloat ytrans = 0;

void init(){
	int maj, min;
	glGetIntegerv(GL_MAJOR_VERSION, &maj);
	glGetIntegerv(GL_MINOR_VERSION, &min);
	//printf("%i, %i\n", maj, min);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.2, .2, .2, 0);

	FILE *vf, *ff;
	vf = fopen("vertexShader.vert", "r");
	ff = fopen("fragmentShader.frag", "r");
	if(!vf || !ff){
		printf("Can't open file!\n");
		exit(1);
	}
	if(ezMakeShader(vf, ff)){
		printf("Error loading shader!\n");
		exit(1);
	}
	fclose(vf);
	fclose(ff);
	
	distance = 8;
	elevation = 4;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(distance*sin(angle), elevation, distance*cos(angle), 0, 1, 0, 0, 1, 0);

	// Lights
	GLfloat lightAmb[] = {1.0, 1.0, 1.0};//{.5*255/255, .5*229/255, .5*180/255};
	GLfloat lightDiff[] = {1.0, 1.0, 1.0};//{(float)255/255, (float)229/255, (float)180/255};
	GLfloat lightSpec[] = {1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
	GLfloat light0Pos[] = {-100.0, 100.0, 100.0, 1.0};	// Ceiling
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColor3f(1., 0., 0.);

	// Material
	GLfloat matamb[] = {.1, .1, .1, 1.0};
	GLfloat matdiff[] = {.4, .4, .4, 1.0};
	GLfloat matspec[] = {1, 1, 1, 1.0};
	//GLfloat matspec[] = {.01, .01, .01, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, matamb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspec);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	// Teapot
	glPushMatrix();
		glColor3f(.15, .20, .45);
		glutSolidTeapot(2);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
	glDisable(GL_TEXTURE_2D);
}

void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, w/h, .01, 100);
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'w':
			elevation += .5;
			break;
		case 'a':
			angle += -.05;
			break;
		case 's':
			elevation += -.5;
			break;
		case 'd':
			angle += .05;
			break;
		case 'z':
			distance += .5;
			break;
		case 'x':
			distance += -.5;
			break;
		case 27:
			//exit(0);
			glutLeaveMainLoop();
			break;
	}
}

int main(int argc, char* argv[]){
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glewInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}