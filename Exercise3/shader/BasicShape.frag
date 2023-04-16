#version 410 core

/* TODO Task 1 update your vertex shader so that...
*      ...it emits the color for phong illumination
*      ...it emits the color from the vertex shader in case you do gouraud shading there
*/

in vec3 objectColor;
in vec3 normal;
in vec3 worldPos;

/* TODO Task 1 fill these structs with values from outside the shader similar
*      to your matric variables
*/
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

uniform bool gouraudShading;

// Ouput data
out vec3 color;

void main()
{
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color=objectColor;
    
    if(!gouraudShading)
    {
        // TODO Task 1 add there code for phong lighting
        vec3 lightVec=normalize(light.position-worldPos);
        vec3 viewVec=normalize(camPos-worldPos);
        
        float lightDis=length(light.position-worldPos);
        vec3 lightSpecular=light.color*pow(max(0,dot(normal,(lightVec+viewVec)/2)),material.shininess)*material.specular;
        vec3 lightDiffuse=light.color*max(0,dot(normal,lightVec))/(lightDis*lightDis)*material.diffuse;
        vec3 lightAmbiant=light.color*material.ambient;
        
        color=objectColor*(lightAmbiant+lightSpecular+lightDiffuse);
        // END TODO
    }
}

