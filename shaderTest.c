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
// Shader stuff
GLuint vshader, fshader, program;
// Model stuff
GLfloat scalar = .01;
GLfloat ytrans = 0;

void makeShader(){
	GLint compiled, linked;
	const GLchar* vShaderSrc[] = {
		"#version 130\n"
		"out vec4 worldPoint;\n"
		"out vec4 normal;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n"
		"	worldPoint	= gl_ModelViewProjectionMatrix*gl_Vertex;\n"
		"	normal = vec4(normalize(gl_NormalMatrix * gl_Normal), 1);\n"
		//"	gl_BackColor = gl_Vertex;"
		//"	gl_FrontColor = gl_Vertex;"
		"}\n"
		/*"#version 130"
		"uniform float t;" // Time (passed in from the application)
		"attribute vec4 vel; // Particle velocity"
		"const vec4 g = vec4( 0.0, -9.80, 0.0 );"
		"void main()"
		"{"
		"	vec4 position = gl_Vertex;"
		"	position += t*vel + t*t*g;"
		"	gl_Position = gl_ModelViewProjectionMatrix * position;"
		"}"*/
	};
	const GLchar* fShadersrc[] = {
		"#version 130\n"
		"in vec4 worldPoint;\n"
		"in vec4 normal;\n"
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
			"vec4 light = normalize(gl_LightSource[0].position - worldPoint);\n"
			"vec4 foo 	= gl_FrontLightProduct[0].diffuse * max(dot(light,normal), 0);\n"
			"foo = clamp(foo, 0, 1);\n"
			"\n"
			"FragColor = foo;\n"
			//"FragColor = vec4(.15, .5, worldPoint.z, 1);\n"
			//"gl_FragColor = clamp(gl_FragCoord, 0.0, 1.0);"
		"}\n"
	};

	vshader = glCreateShader(GL_VERTEX_SHADER);					// Create shader object
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vshader, 1, vShaderSrc, NULL);					// Attach source code
	glShaderSource(fshader, 1, fShadersrc, NULL);
	glCompileShader(vshader);									// Compile
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled );		// Check if compilation successful
	if(!compiled){
		GLint length;
		GLchar* log;
		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &length );
		log = (GLchar*) malloc(length);
		glGetShaderInfoLog(vshader, length, &length, log);
		fprintf(stderr, "~~ Vertex compile log:\n‘%s’\n", log);
		free(log);
		return;
	}
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &compiled );
	if(!compiled){
		GLint length;
		GLchar* log;
		glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &length );
		log = (GLchar*) malloc(length);
		glGetShaderInfoLog(fshader, length, &length, log);
		fprintf(stderr, "~~ Fragment compile log:\n‘%s’\n", log);
		free(log);
		return;
	}
	program = glCreateProgram();								// Create program object
	glAttachShader(program, vshader);							// Attatch shader
	glAttachShader(program, fshader);
	glLinkProgram(program);										// Link all shaders
	glGetProgramiv(program, GL_LINK_STATUS, &linked );			// Check if linked
	if(linked){
		glUseProgram(program);
	}
	else{
		GLint length;
		GLchar* log;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length );
		log = (GLchar*) malloc(length);
		glGetProgramInfoLog(program, length, &length, log);
		fprintf(stderr, "link log = ‘%s’\n", log);
		free(log);
		return;
	}
}

void init(){
	int maj, min;
	glGetIntegerv(GL_MAJOR_VERSION, &maj);
	glGetIntegerv(GL_MINOR_VERSION, &min);
	//printf("%i, %i\n", maj, min);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.2, .2, .2, 0);

	//makeShader();
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
	
	distance = 15;
	elevation = 0;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(distance*sin(angle), elevation, distance*cos(angle), 0, 1, 0, 0, 1, 0);

	// Lights
	GLfloat lightAmb[] = {.5*255/255, .5*229/255, .5*180/255};
	GLfloat lightDiff[] = {(float)255/255, (float)229/255, (float)180/255};
	GLfloat lightSpec[] = {1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
	GLfloat light0Pos[] = {-100.0, 100.0, 100.0, 1.0};	// Ceiling
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColor3f(1., 0., 0.);

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