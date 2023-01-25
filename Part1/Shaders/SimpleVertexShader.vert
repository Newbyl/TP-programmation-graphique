#version 410 core

// Récuperation de l'attribut 0
layout(location = 0) in vec3 vertexPosition;

void main(){
  //gl_Position variable par défaut du vertex shader
  //On y met les coordonnées des points (attribut 0) mais homogènes
  gl_Position.xyz = vertexPosition;
  gl_Position.w = 1.0;
  
}
