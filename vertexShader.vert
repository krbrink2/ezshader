#version 130
out vec4 modelCoord;
out vec3 n;
out vec2 texCoord;
out vec3 u;
out vec3 v;
void main()
{
	modelCoord	= gl_ModelViewMatrix*gl_Vertex;
	n = normalize(gl_NormalMatrix * gl_Normal);
	texCoord = gl_MultiTexCoord0.xy;

	// vec3 a 		= normalize(vec3(1, 1, 1));
	// vec3 u 		= normalize(a - dot(a, n)*n);
	// vec3 b 		= normalize(vec3(1, 1, -1));
	// vec3 v 		= normalize(b - dot(b, n)*n);
	v			= normalize(cross(gl_Normal, vec3(0, 1, 0)));
	u			= normalize(cross(v, gl_Normal));
	v 			= mat3(gl_ModelViewMatrix) * v;
	u 			= mat3(gl_ModelViewMatrix) * u;

	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
// End