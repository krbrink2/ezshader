#version 130
out vec4 worldPoint;
out vec4 normal;
void main()
{
	worldPoint	= gl_ModelViewMatrix*gl_Vertex;
	normal = vec4(normalize(gl_NormalMatrix * gl_Normal), 1);
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}