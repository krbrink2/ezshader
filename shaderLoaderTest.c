/* 
	Draws a pretty scene.
*/
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <SOIL/SOIL.h>
#include "ezloader.h"
#include "ezshader.h"

// Globals
GLint program;
GLint callListIndices[8];
static GLuint texNames[8];
GLfloat angle;
GLfloat elevation;
GLfloat distance;

/*	Draws tesselated square in the (x,y) plane.
	@subdivs: number of tesselations in each dimension
	@width: width and length of the square
*/
void ezsquare(GLfloat width, GLint subdivs){
	GLfloat unitWidth = width/subdivs;
	GLfloat texUnitWidth = (GLfloat)1/subdivs;
	int i, j;
	for(i = 0; i < subdivs; i++){
		GLfloat y = width - i*unitWidth;
		GLfloat texy = 1 - i*texUnitWidth;
		for(j = 0; j < subdivs; j++){
			GLfloat x = -width/2 + j*unitWidth;
			GLfloat texx = j*texUnitWidth;
			// (x,y) describe top-left corner
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				// Bottom Left
				glNormal3f(0, 0, 1);
				glTexCoord2f(texx, texy-texUnitWidth);
				glVertex3f(x, y-unitWidth, 0);
				// Bottom Right
				glNormal3f(0, 0, 1);
				glTexCoord2f(texx+texUnitWidth, texy-texUnitWidth);
				glVertex3f(x+unitWidth, y-unitWidth, 0);
				// Top right
				glNormal3f(0, 0, 1);
				glTexCoord2f(texx+texUnitWidth, texy);
				glVertex3f(x+unitWidth, y, 0);
				// Top left
				glNormal3f(0, 0, 1);
				//glNormal3f((GLfloat)rand()/RAND_MAX, (GLfloat)rand()/RAND_MAX, ((GLfloat)rand()/RAND_MAX));
				glTexCoord2f(texx, texy);
				glVertex3f(x, y, 0);
			glEnd();
		}
	}

}

void init(){
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.2, .2, .2, 0);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Try messing with this
	texNames[0] = SOIL_load_OGL_texture("..//textures//wood.jpg", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	texNames[1] = SOIL_load_OGL_texture("..//textures//metal.jpg", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	texNames[2] = SOIL_load_OGL_texture("..//textures//paper.jpg", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	texNames[3] = SOIL_load_OGL_texture("..//textures//tuscany.jpg", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	texNames[4] = SOIL_load_OGL_texture("..//textures//woodFloor.jpg", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Generate mesh call lists
	char teapotfname[] = "..//meshes//teapot.obj";
	char tablefname[] = "..//meshes//dtable.obj";
	char lampfname[] = "..//meshes//dlamp.obj";
	char toyplanefname[] = "..//meshes//toyplane.obj";
	callListIndices[0] = glGenLists(4);
	callListIndices[1] = callListIndices[0] + 1;
	callListIndices[2] = callListIndices[0] + 2;
	callListIndices[3] = callListIndices[0] + 3;
	// teapot
	FILE *fp = fopen(teapotfname, "r");
	if(!fp){
		printf("Failed to open file %s!\n", teapotfname);
		exit(1);
	}
	ezloadCallList(callListIndices[0], fp);
	fclose(fp);
	// table
	fp = fopen(tablefname, "r");
	if(!fp){
		printf("Failed to open file %s!\n", tablefname);
		exit(1);
	}
	ezloadCallList(callListIndices[1], fp);
	fclose(fp);
	// lamp
	fp = fopen(lampfname, "r");
	if(!fp){
		printf("Failed to open file %s!\n", lampfname);
		exit(1);
	}
	ezloadCallList(callListIndices[2], fp);
	fclose(fp);
	// toyplane
	fp = fopen(toyplanefname, "r");
	if(!fp){
		printf("Failed to open file %s!\n", toyplanefname);
		exit(1);
	}
	ezloadCallList(callListIndices[3], fp);
	fclose(fp);

	// Create shader
	FILE *vf, *ff;
	vf = fopen("vertexShader.vert", "r");
	ff = fopen("fragmentShader.frag", "r");
	if(!vf || !ff){
		printf("Can't open file!\n");
		exit(1);
	}
	program = ezMakeShader(vf, ff);
	if(-1 == program){
		printf("Error loading shader!\n");
		exit(1);
	}
	fclose(vf);
	fclose(ff);

	distance = 20;
	elevation = 9;
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(distance*sin(angle), elevation, distance*cos(angle), 0, 7, 0, 0, 1, 0);
	glEnable(GL_LIGHTING);

	// Lights
	// Light 0: distance
	GLfloat light0Intensity = .5;
	GLfloat light0Amb[] = {light0Intensity, light0Intensity, light0Intensity};
	GLfloat light0Diff[] = {light0Intensity, light0Intensity, light0Intensity};
	GLfloat light0Spec[] = {light0Intensity, light0Intensity, light0Intensity};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);
	GLfloat light0Pos[] = {0, 100.0, 100.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glDisable(GL_LIGHT0);
	// Light 1: lamp
	GLfloat light1Amb[] = {0, 0, 0};
	GLfloat light1Diff[] = {1, .22, .05};
	GLfloat light1Spec[] = {1, .22, .05};
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1Amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1Spec);
	GLfloat light1Pos[] = {0.0, 12.0, 1.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
	glEnable(GL_LIGHT1);

	GLint numLights = 2;

	// Uniforms
	GLint fooLoc = glGetUniformLocation(program, "foo");
	if (fooLoc != -1)
   		glUniform1f(fooLoc, 1.0);
   	GLint barLoc = glGetUniformLocation(program, "bar");
	if (barLoc != -1)
   		glUniform1f(barLoc, 1.0);
   	GLint numLightsLoc = glGetUniformLocation(program, "numLights");
   	if (numLightsLoc != -1)
   		glUniform1i(numLightsLoc, numLights);


	// Draw objects
	// Teapot
	// @TODO: disable texturing
	// Set material
	GLfloat matAmb[] = {.2, .2, .2, 1.f};
	GLfloat matDiff[] = {1, 1, 1, 1.f};
	GLfloat matSpec[] = {.1, .1, .1, 1.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	// Texturing
	//glActiveTexture(GL_TEXTURE2);
	// Note: ActiveTexture only breaks stuff.
	//glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texNames[2]);
	// DO NOT let OpenGL generate texture vertices
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);*/
	glPushMatrix();
		glTranslatef(1, 7.55, 2.5);
		glRotatef(-45, 0, 1, 0);
		glScalef(.02, .02, .02);
		glCallList(callListIndices[0]);
	glPopMatrix();

	// Lamp
	// Set material
	// GLfloat matAmb[] = {.2, .2, .2, 1.f};
	// GLfloat matDiff[] = {.6, .6, .6, 1.f};
	// GLfloat matSpec[] = {1.f, 1.f, 1.f, 1.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	// Texturing
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texNames[1]);
	// DO NOT let OpenGL generate texture vertices
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);*/
	glPushMatrix();
		glTranslatef(0, 7.48, 1);
		glRotatef(-20, 0, 1, 0);
		glScalef(.07, .07, .07);
		glCallList(callListIndices[2]);
	glPopMatrix();

	// Toyplane
	// Set material
	// GLfloat matAmb[] = {.2, .2, .2, 1.f};
	// GLfloat matDiff[] = {.6, .6, .6, 1.f};
	// GLfloat matSpec[] = {1.f, 1.f, 1.f, 1.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	// Texturing
	//glActiveTexture(GL_TEXTURE2);
	// Note: ActiveTexture only breaks stuff.
	//glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	// DO NOT let OpenGL generate texture vertices
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);*/
	glPushMatrix();
		glTranslatef(-2, 8.05, 2.5);
		glRotatef(-45, 0, 1, 0);
		glScalef(.07, .07, .097);
		glRotatef(20, 0, 0, 1);
		glCallList(callListIndices[3]);
	glPopMatrix();

	// Table
	// Set material
	// matAmb[] = {.2, .2, .2, 1.f};
	// matDiff[] = {.6, .6, .6, 1.f};
	// matSpec[] = {1.f, 1.f, 1.f, 1.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	// Texturing
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	// DO NOT let OpenGL generate texture vertices
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);*/
	glPushMatrix();
		glScalef(.1, .1, .1);
		glTranslatef(0, 1, 0);
		glCallList(callListIndices[1]);
	glPopMatrix();


	// Wall
	GLfloat wallMatAmb[] = {.05, .05, .05, 1.f};
	GLfloat wallMatDiff[] = {1, 1, 1, 1.f};
	GLfloat wallMatSpec[] = {.01, .01, .01, 1.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, wallMatAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, wallMatDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wallMatSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texNames[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glPushMatrix();
		glTranslatef(0., -.5, -1.26);
		ezsquare(20, 10);
	glPopMatrix();

	// Floor
	GLfloat floorMatAmb[] = {.0, .0, .0, 1.f};
	GLfloat floorMatDiff[] = {.9, .9, .9, 1.f};
	GLfloat floorMatSpec[] = {.1, .1, .1, 1.f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, floorMatAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, floorMatDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, floorMatSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texNames[4]);
	glPushMatrix();
		glTranslatef(0, 0, 10);
		glRotatef(-90, 1, 0, 0);
		ezsquare(20, 20);
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
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
			glutLeaveMainLoop();
			break;
	}
}

void mouse(int button, int state, int x, int y){

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
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}