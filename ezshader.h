#ifndef EZSHADER_H_
#define EZSHADER_H_
//#include <GL/glut.h>
#include <GL/glew.h>
//#include <GL/gl.h>
#include <stdio.h>

// global declarations
// 	None

// function declarations
int ezUniformf(GLint program, char* name, GLfloat value);
int ezUniformi(GLint program, char* name, GLint value);
int ezMakeShader(FILE *vf, FILE *ff);

#endif // EZSHADER_H_