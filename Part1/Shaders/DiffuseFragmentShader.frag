
#version 330 core

// Données reçues (interpolées) de la part du vertex shader
in vec3 fragmentColor;
in float cosTheta;

uniform vec4 ambiant;
uniform vec4 diffuse;

// Résultats du Fragment shader
out vec3 Color;

void main(){
  
  Color = ambiant.xyz*fragmentColor + diffuse.xyz*cosTheta;
  
}


