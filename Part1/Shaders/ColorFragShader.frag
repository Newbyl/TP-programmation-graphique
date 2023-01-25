#version 330 core

// Données reçues (interpolées) de la part du vertex shader
in vec3 fragmentColor;

// Résultats du Fragment shader
//out vec3 Color;

vec4 C;

void main(){
  //Color = fragmentColor;
  C.rgb = fragmentColor;
  C.a = 1;
  gl_FragColor = C;

}
