#version 130
in vec4 worldPoint;
in vec4 normal;
out vec4 FragColor;
void main(void)
{
	vec4 light = normalize(gl_LightSource[0].position - worldPoint);
	vec4 foo 	= gl_FrontLightProduct[0].diffuse * max(dot(light,normal), 0);
	foo = clamp(foo, 0, 1);
	FragColor = foo;
	//"FragColor = vec4(.15, .5, worldPoint.z, 1);
	//"gl_FragColor = clamp(gl_FragCoord, 0.0, 1.0);
}
