#version 130
in vec4 modelCoord;
in vec4 normal;
in vec2 texCoord;
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

		totalColor		= totalColor + (ambient + diffuse + specular);
	}

	// Cartoon shader
	// float foo = dot(halfVect, normal);
	// float scalar;
	// if(foo > 0.95){
	// 	scalar = 1;
	// }
	// else if(foo > .5){
	// 	scalar = .5;
	// }
	// else if(foo > .25){
	// 	scalar = .25;
	// }
	// else{
	// 	scalar = lightIndex;
	// }

	FragColor = totalColor;

	//FragColor = vec4(.15, .5, modelCoord.z, 1);
	//gl_FragColor = clamp(gl_FragCoord, lightIndex.lightIndex, 1.lightIndex);
}
// end shader