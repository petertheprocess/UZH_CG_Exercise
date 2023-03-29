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
uniform vec3 lightPos;
uniform vec3 lightColor;
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
        vec3 lightVec=normalize(lightPos-worldPos);
        vec3 viewVec=normalize(camPos-worldPos);
        
        float lightDis=length(lightPos-worldPos);
        vec3 lightSpecular=objectColor*lightColor*pow(max(0,dot(normal,(lightVec+viewVec)/2)),16);
        vec3 lightDiffuse=objectColor*lightColor*max(0,dot(normal,lightVec))/(lightDis*lightDis);
        vec3 lightAmbiant=objectColor*lightColor*.2;
        
        color=lightAmbiant+lightSpecular+lightDiffuse;
        // END TODO
    }
}

