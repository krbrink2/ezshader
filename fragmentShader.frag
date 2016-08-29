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
	float pi = 3.14159;
	vec4 fragColor = texture2D(texture, texCoord);	
	vec4 totalColor = vec4(0, 0, 0, 1);

	for(int i = 0; i < 1; i++){
		i = 1;
		// Unit vectors
		vec3 l	= normalize(gl_LightSource[i].position.xyz - modelCoord.xyz);
		vec3 e	= normalize(-modelCoord.xyz);
		vec3 h	= normalize(l.xyz + e.xyz);

		// Blinn-Phong
		vec4 ambient	= gl_FrontLightProduct[i].ambient*fragColor;
		vec4 diffuse 	= gl_FrontLightProduct[i].diffuse * max(dot(l, n), 0) * fragColor;
		vec4 specular	= gl_FrontLightProduct[i].specular * pow( max( dot(vec3(h), n), 0), gl_FrontMaterial.shininess);

		// Ashikhmin diffuse
		vec4 ashDiffuse	= gl_FrontMaterial.diffuse * vec4((vec3(1) - gl_FrontMaterial.specular.rgb), 1);
		ashDiffuse		*= 28.0/(23.0*pi) * (1 - pow((1 - dot(n, e)/2.0), 5)) * (1 - pow(1 - dot(n, l)/2.0, 5));

		// Ashikhmin specular
		int nu				= 100;
		int nv				= 100;
		vec3 hu 			= normalize(dot(h, u)*u);
		vec3 hv				= normalize(dot(h, v)*v);
		float ashSpecNorm 	= sqrt((nu+1)*(nv+1))/(8*pi);
		float ashSpecExp 	= (nu*pow(dot(h,u), 2) + nv*pow(dot(h,v), 2))/(1 - pow(dot(h,n), 2));
		float ashSpecDenom 	= dot(h,l)*max(dot(n,l), dot(n,e));
		vec4 ashSpecFres	= vec4(gl_FrontMaterial.specular.rgb + (vec3(1.0) - gl_FrontMaterial.specular.rgb)*pow(1 - dot(l, h), 5), 1);
		vec4 ashSpecular	= ashSpecNorm * pow(max(dot(n, h), 0), ashSpecExp) * ashSpecFres / ashSpecDenom;

		totalColor		+= clamp(ambient*0 + ashDiffuse + ashSpecular, 0, 1) * fragColor;
	}

	//totalColor = vec4(v, 1);
	FragColor = totalColor;
}
// end shader