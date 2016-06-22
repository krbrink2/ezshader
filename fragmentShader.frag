#version 130
in vec4 modelCoord;
in vec4 normal;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texture;
void main(void)
{
	//vec4 lightVect	= normalize(gl_LightSource[0].position - modelCoord);
	vec4 lightVect	= vec4(normalize(gl_LightSource[0].position.xyz - modelCoord.xyz), 1);
	vec4 eyeVect	= vec4(normalize(-modelCoord.xyz), 1);
	vec4 halfVect	= vec4(normalize(lightVect.xyz + eyeVect.xyz), 1);

	vec4 fragColor = texture2D(texture, texCoord);

	vec4 ambient	= gl_FrontLightProduct[0].ambient*fragColor;

	vec4 diffuse 	= gl_FrontLightProduct[0].diffuse * max(dot(lightVect.xyz, normal.xyz), 0) * fragColor;
	//diffuse = max(dot(lightVect,normal), 0)*vec4(0, 1, 1, 1);
	//diffuse = lightVect;

	vec4 specular	= gl_FrontLightProduct[0].specular * pow( max( dot(vec3(halfVect), vec3(normal)), 0), gl_FrontMaterial.shininess);

	vec4 totalColor	= min(ambient*0 + diffuse + specular*0, 1);

	// Cartoon shader
	float foo = dot(halfVect, normal);
	float scalar;
	if(foo > 0.95){
		scalar = 1;
	}
	else if(foo > .5){
		scalar = .5;
	}
	else if(foo > .25){
		scalar = .25;
	}
	else{
		scalar = 0;
	}
	FragColor = scalar*vec4(1, 0, 0, 1);

	FragColor = totalColor;

	//FragColor = vec4(.15, .5, modelCoord.z, 1);
	//gl_FragColor = clamp(gl_FragCoord, 0.0, 1.0);
}
// end shader