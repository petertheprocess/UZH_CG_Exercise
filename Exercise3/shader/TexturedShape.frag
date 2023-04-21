#version 410 core

/* TODO Task 2 update your vertex shader so that...
 *      ...it has texture samplers declared
 *      ...it outputs a diffuse texture as diffuse color
 *      ...it uses a light map to modify the specular highlights
 */

in vec3 objectColor;

/* TODO Task 2 Add the required input data */
in vec2 texCoord;
// in vec3 normal;
in vec3 worldPos;
in mat3 TBN;
// END TODO

/* TODO Task 2 declare texture samplers here */
uniform sampler2D texMap;
uniform sampler2D normalMap;

uniform struct materialBlock{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}material;
uniform struct lightBlock{
    vec3 position;
    vec3 color;
}light;

uniform vec3 camPos;
// END TODO

/* TODO Task 2 fill these structs with values from outside the shader similar
 *      to your matric variables
 */

// TODO END

out vec4 color;

void main()
{
	
	// color = vec4(0,objectColor);
	/* TODO Task 2 add the source code to do phong shading but with the values of the color, normal and specular texture.
	 *      Remember that you can also use the color output for debugging of the
	 *      texture maps and texture coordinates in case that you face troubles.
	 */
	// color = texture(tex, texCoord);
	vec3 normal = texture(normalMap,texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = TBN * normal;
	
	vec3 lightVec=normalize(light.position-worldPos);
    vec3 viewVec=normalize(camPos-worldPos);
        
    float lightDis=length(light.position-worldPos);
    vec3 lightSpecular=light.color*pow(max(0,dot(normal,(lightVec+viewVec)/2)),material.shininess)*material.specular;
    vec3 lightDiffuse=light.color*max(0,dot(normal,lightVec))/(lightDis*lightDis)*material.diffuse;
    vec3 lightAmbiant=light.color*material.ambient;
    color=vec4(texture(texMap,texCoord).rgb*(lightSpecular+lightDiffuse+lightAmbiant),1.0f);
	// color=vec4(objectColor,1.0f);
	// TODO END
}

