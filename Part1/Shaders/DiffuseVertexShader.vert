#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 colorPosition;
layout(location = 2) in vec3 vertexNormale;

out vec3 fragmentColor;
out float cosTheta;

vec4 Vertex;
vec3 light_direction;
vec4 normale;

uniform mat4 MVP; // la matrice transmise par le programme CPU
uniform mat4 M; // Matrice Model 
uniform vec3 light_position; // la position de la lumière envoyée par le CPU

void main(){
  
  Vertex.xyz = vertexPosition;
  Vertex.w = 1.0;
  gl_Position = MVP * Vertex;
  
  fragmentColor = colorPosition;
  
  Vertex = M*(vec4(vertexPosition,1.0));
  
  light_direction = normalize(light_position - Vertex.xyz);

  normale = normalize(M*vec4(vertexNormale,1.0));
  
  cosTheta = clamp (dot(normale.xyz,light_direction),0.0,1.0);
  
}




