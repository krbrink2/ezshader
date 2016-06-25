#version 130
in vec4 modelCoord;
in vec3 n;
in vec2 texCoord;
in vec3 u;
in vec3 v;
out vec4 FragColor;
uniform sampler2D texture;
void main(void)
{
	vec4 fragColor = texture2D(texture, texCoord);	
	vec4 totalColor = vec4(0, 0, 0, 1);

	for(int i = 0; i < 2; i++){
		// Unit vectors
		vec3 l	= normalize(gl_LightSource[i].position.xyz - modelCoord.xyz);
		vec3 e	= normalize(-modelCoord.xyz);
		vec3 h	= normalize(l.xyz + e.xyz);

		vec4 ambient	= gl_FrontLightProduct[i].ambient*fragColor;

		vec4 diffuse 	= gl_FrontLightProduct[i].diffuse * max(dot(l, n), 0) * fragColor;

		vec4 specular	= gl_FrontLightProduct[i].specular * pow( max( dot(vec3(h), n), 0), gl_FrontMaterial.shininess);
		// Ashikhmin
		int nu			= 100;
		int nv			= 100;
		vec3 hu 		= normalize(dot(h, u)*u);
		vec3 hv			= normalize(dot(h, v)*v);
		float exponent	= (nu*pow(dot(h,u), 2) + nv*pow(dot(h,v), 2))/(1 - pow(dot(h,n), 2));
		exponent = 100;
		vec4 ashSpecular = sqrt((nu+1)*(nv+1))/(8*3.14159) * pow(dot(n, h), exponent) * vec4(1);
		//ashSpecular		/= dot(h,l)*max(dot(n,v), dot(n,e));
		//ashSpecular		*= gl_FrontLightProduct[i].specular + (vec4(1) - gl_FrontLightProduct[i].specular)*pow((1 - dot(l, h)), 5);

		totalColor		= totalColor + vec4(dot(n,u));//(ambient + diffuse + ashSpecular);
	}

	FragColor = totalColor;
}
// end shader