#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 colorPosition;

out vec3 fragmentColor;

uniform mat4 MVP; // la matrice transmise par le programme CPU
vec4 Vertex;

void main(){
  
  Vertex.xyz = vertexPosition;
  Vertex.w = 1.0;
  gl_Position = MVP * Vertex;
  
  fragmentColor = colorPosition;
  
  
}




