#version 330 core

// Récuperation de l'attribut 0 et 1
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 colorPosition;

out vec3 fragmentColor;

void main(){
  gl_Position.xyz = vertexPosition;
  gl_Position.w = 1.0;	
  // fragmentColor : données transmises au fragment shader
  fragmentColor = colorPosition;
}
