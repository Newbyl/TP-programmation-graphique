#version 330 core

// Données reçues (interpolées) de la part du vertex shader
in vec3 fragmentColor;

uniform vec4 ambiant;

// Résultats du Fragment shader
out vec3 Color;

// Pour le moment on a que la lumière ambiante!!!
void main(){
  Color = ambiant.xyz*vec3(1.0, 0.0, 0.0);  
}


