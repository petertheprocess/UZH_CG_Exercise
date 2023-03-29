#version 410 core

/* TODO Task 1 update your vertex shader so that...
*      ...it emits all necessary information to the fragment shader for phong illumination
*      ...it emits the color in case you do gouraud shading
*/

// Input vertex data
layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 vColor;
layout(location=2)in vec3 vNormal;

// Output data will be interpolated for each fragment.
// Tip: Try to use the flat modifier to make color associations of a fragment visible for debugging.
out vec3 objectColor;
out vec3 normal;
out vec3 worldPos;

// matrices that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

uniform bool gouraudShading;

// TODO Task 1 Additional variables
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;
// END TODO

void main()
{
    normal=mat3(transpose(inverse(modelMatrix)))*vNormal;
    
    // Output position of the vertex, in clip space : MVP * vPosition
    gl_Position=mvpMatrix*vec4(vPosition,1);
    
    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    // ... uncomment this for color
    objectColor=vColor;
    
    // ... uncomment this for color according to normals
    // objectColor = vNormal;
    
    worldPos=vec3(modelMatrix*vec4(vPosition,1));
    
    if(gouraudShading)
    {
        // TODO Task 1 add there code for gouraud shading
        vec3 lightVec=normalize(lightPos-worldPos);
        vec3 viewVec=normalize(camPos-worldPos);
        
        float lightDis=length(lightPos-worldPos);
        vec3 lightSpecular=vColor*lightColor*pow(max(0,dot(normal,(lightVec+viewVec)/2)),16);
        vec3 lightDiffuse=vColor*lightColor*max(0,dot(normal,lightVec))/(lightDis*lightDis);
        vec3 lightAmbiant=vColor*lightColor*.2;
        
        objectColor=lightSpecular+lightDiffuse+lightAmbiant;
        // END TODO
    }
}

