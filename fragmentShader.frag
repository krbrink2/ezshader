#version 130
in vec4 modelCoord;
in vec4 normal;
in vec2 texCoord;
in vec3 u;
in vec3 v;
in vec3 hu;
in vec3 hv;
out vec4 FragColor;
uniform sampler2D texture;
void main(void)
{
	vec4 fragColor = texture2D(texture, texCoord);	
	vec4 totalColor = vec4(0, 0, 0, 1);

	for(int i = 0; i < 2; i++){
		vec4 lightVect	= vec4(normalize(gl_LightSource[i].position.xyz - modelCoord.xyz), 1);
		vec4 eyeVect	= vec4(normalize(-modelCoord.xyz), 1);
		vec4 halfVect	= vec4(normalize(lightVect.xyz + eyeVect.xyz), 1);

		vec4 ambient	= gl_FrontLightProduct[i].ambient*fragColor;

		vec4 diffuse 	= gl_FrontLightProduct[i].diffuse * max(dot(lightVect.xyz, normal.xyz), 0) * fragColor;

		vec4 specular	= gl_FrontLightProduct[i].specular * pow( max( dot(vec3(halfVect), vec3(normal)), 0), gl_FrontMaterial.shininess);
		// Ashikhmin
		int nu			= 100;
		int nv			= 100;
		float phi		= 3.14159/4;//atan(sqrt((nu+1)/(nv+1))*tan())
		//vec4 ashPecular = sqrt((nu+1)*(nv+1))/(8*3.14159) * pow(dot(normal, halfVect), 

		totalColor		= totalColor + (ambient + diffuse + specular);
	}

	FragColor = totalColor;
}
// end shader