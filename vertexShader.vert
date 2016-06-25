#version 130
out vec4 modelCoord;
out vec4 normal;
out vec2 texCoord;
out vec3 u;
out vec3 v;
out vec3 hu;
out vec3 hv;
void main()
{
	modelCoord	= gl_ModelViewMatrix*gl_Vertex;
	normal = vec4(normalize(gl_NormalMatrix * gl_Normal), 1);
	texCoord = gl_MultiTexCoord0.xy;

	vec3 a 		= normalize(vec3(1, 1, 1));
	vec3 u 		= normalize(a - dot(a, normal.xyz)*normal.xyz);
	vec3 b 		= normalize(vec3(1, 1, -1));
	vec3 v 		= normalize(b - dot(b, normal.xyz)*normal.xyz);
	vec3 hu 	= normalize(dot(h, u)*u);
	vec3 hv		= normalize(dot(h, v)*v);

	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
// End