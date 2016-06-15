#version 130
out vec4 worldPoint;
out vec4 normal;
out vec2 texCoord;
void main()
{
	worldPoint	= gl_ModelViewMatrix*gl_Vertex;
	normal = vec4(normalize(gl_NormalMatrix * gl_Normal), 1);
	texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
// End