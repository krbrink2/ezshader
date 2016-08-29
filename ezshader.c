#include "ezshader.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>

GLuint vshader, fshader, program;

// Returns program id on success, or -1 on failure
int ezMakeShader(FILE *vf, FILE *ff){
	GLint compiled, linked;
	// Load vertex shader
	GLchar* vShaderSrc;
	long length;
	if(vf)
	{
	  fseek(vf, 0, SEEK_END);
	  length = ftell(vf);
	  fseek(vf, 0, SEEK_SET);
	  vShaderSrc = malloc(length);
	  if (vShaderSrc){
	  	fread(vShaderSrc, 1, length, vf);
	  }
	}
	const GLchar* vShaderSrc_const = vShaderSrc;
	// Load fragment shader
	GLchar* fShaderSrc;
	if(ff)
	{
	  fseek(ff, 0, SEEK_END);
	  length = ftell(ff);
	  fseek(ff, 0, SEEK_SET);
	  fShaderSrc = malloc(length);
	  if (fShaderSrc){
	  	fread(fShaderSrc, 1, length, ff);
	  }
	}
	const GLchar* fShaderSrc_const = fShaderSrc;

	vshader = glCreateShader(GL_VERTEX_SHADER);					// Create shader object
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vshader, 1, &vShaderSrc_const, NULL);		// Attach source code
	glShaderSource(fshader, 1, &fShaderSrc_const, NULL);
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
		return -1;
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
		return -1;
	}
	program = glCreateProgram();								// Create program object
	if(!program){
		printf("~~ Failed to create program!\n");
		return -1;
	}
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
		return -1;
	}

	return program;
}