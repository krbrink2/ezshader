#version 130
in vec4 worldPoint;
in vec4 normal;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texture;
void main(void)
{
	vec4 lightVect	= normalize(gl_LightSource[0].position - worldPoint);
	vec4 eyeVect	= vec4(normalize(-worldPoint.xyz), 1);
	vec4 halfVect	= vec4(normalize(lightVect.xyz + eyeVect.xyz), 1);

	vec4 ambient	= gl_FrontLightProduct[0].ambient;
	ambient			= clamp(ambient, 0, 1);

	vec4 diffuse 	= gl_FrontLightProduct[0].diffuse * max(dot(lightVect,normal), 0);
	diffuse 		= clamp(diffuse, 0, 1);

	vec4 specular	= gl_FrontLightProduct[0].specular * clamp( pow( max( dot(vec3(halfVect), vec3(normal)), 0), gl_FrontMaterial.shininess), 0, 1);
	//specular	= gl_LightSource[0].specular * max(pow(dot(halfVect, normal), 9), 0);

	vec4 totalColor	= clamp(diffuse + specular + ambient, 0, 1);
	FragColor 		= texture2D(texture, texCoord);
	//FragColor = vec4(.15, .5, worldPoint.z, 1);
	//gl_FragColor = clamp(gl_FragCoord, 0.0, 1.0);
}
// end shader