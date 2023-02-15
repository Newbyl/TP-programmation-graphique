#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "../Common/shaders_utilities.hpp"
#define GLM_FORCE_RADIANS
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

using namespace std;

// Position de la lumière : elle sera fixe dans la scéne
// cf Clavier pour voir comment la déplacer
GLfloat lumiere[] = {5.0,0.0,0.0};

// Les sommets avec glDrawElements
static GLfloat coordonnees[] = {
  0,0,0, //0
  1,0,0, //1
  1,1,0, //2
  0,1,0, //3

  1,0,0, 
  1,0,-1, 
  1,1,-1, 
  1,1,0, 

  0,0,-1,
  0,1,-1,
  1,1,-1,
  1,0,-1,

  0,0,0,
  0,1,0,
  0,1,-1,
  0,0,-1,

  0,1,0, //3 16
  1,1,0, //7 17
  1,1,-1, //6 18
  0,1,-1, //2 19

  0,0,0, //0 20
  1,0,0, //4 21
  1,0,-1, //5 22
  0,0,-1  //1 23

  };

// Des couleurs ...
static GLfloat couleurs[] = {
  1,1,1,
  1,1,1,
  1,1,1,
  1,1,1,
  

  0,1,1,
  0,1,1,
  0,1,1,
  0,1,1,
  
  0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
  
  1,1,0,
    1,1,0,
    1,1,0,
    1,1,0,

  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  

  1,0,1,
    1,0,1,
    1,0,1,
    1,0,1
};


// Les normales ont été calculées à la main
// comme le sommet est représenté autant de fois que de facettes auxquelles
// il contribue, la normale est calculée par rapport à la facette à laquelle le sommet contribue.
// Si on avait représenté le sommet qu'une seule fois il aurait fallu faire une moyenne de toutes
// les normales des facettes auxquelles le sommet contribue et le résultat est moins bien évidemment.
static GLfloat normales[] = {
  0,0,1,
  0,0,1,
  0,0,1,
  0,0,1,
  
  1,0,0,
  1,0,0,
  1,0,0,
  1,0,0,
  
  0,0,-1,
  0,0,-1,
  0,0,-1,
  0,0,-1,

  -1,0,0,
  -1,0,0,
  -1,0,0,
  -1,0,0,

  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,

  0,-1,0,
  0,-1,0,
  0,-1,0,
  0,-1,0
};


static GLuint indices[] = {
  0,1,2,
  2,3,0,
  
  4,5,6,
  6,7,4,
  
  8,9,10,
  10,11,8,

  12,13,14,
  14,15,12,

  16,17,18,
  18,19,16,

  20,21,22,
  22,23,20
  
};
GLFWwindow* window;
float stepTrans=0.1;
int mouseXOld, mouseYOld;
double Xpos, Ypos;
bool leftbutton = false;
bool rightbutton = false;
bool middlebutton = false;

GLuint vboID[4];
GLuint programID;
GLuint vaoID;

// les identifiants pour envoyer à la carte graphique les matrices et la position de la lumière
GLint MID;
GLint MatrixID;

GLint lightID;

glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

glm::mat4 translation;
glm::mat4 trans_initial;
glm::mat4 rotation;


// les composantes AMBIANT et DIFFUSE (du blanc avec une certaine intensité
// 0.2 et 0.8 sont des valeurs habituelles par rapport à une lumière classique)
GLfloat ambiant[4] = {0.2,0.2,0.2,1.0};
GLfloat diffuse[4] = {0.8,0.8,0.8,1.0};

GLint ambiantID;
GLint diffuseID;

void init() {

  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  
  programID = LoadShaders( "../Shaders/DiffuseVertexShader.vert", "../Shaders/DiffuseFragmentShader.frag" );
  
  glUseProgram(programID);

  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  glGenBuffers(4,vboID);

  glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
  glBufferData(GL_ARRAY_BUFFER,3*6*4*sizeof(float),coordonnees,GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
  glBufferData(GL_ARRAY_BUFFER,3*6*4*sizeof(float),couleurs,GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  glEnableVertexAttribArray(1);
  
  glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
  glBufferData(GL_ARRAY_BUFFER,3*6*4*sizeof(float),normales,GL_STATIC_DRAW);
  glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*12*sizeof(GLuint),indices,GL_STATIC_DRAW);
  
  

  // Génération des identifiants pour les variables Uniform vers le vertex shader
  MatrixID = glGetUniformLocation(programID, "MVP");
  MID = glGetUniformLocation(programID, "M");

  lightID =  glGetUniformLocation(programID, "light_position");
  ambiantID = glGetUniformLocation(programID, "ambiant");
  diffuseID = glGetUniformLocation(programID, "diffuse");
  Projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);

  View = glm::lookAt(glm::vec3(0,0,5.0), glm::vec3(0,0,0), glm::vec3(0,1,0));

  trans_initial = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5,-0.5,0.5));
  
  Model = glm::mat4(1.0f);
  
  rotation = glm::mat4(1.0f);
  
  translation = glm::mat4(1.0f);

}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  Model = translation * rotation * trans_initial;

  MVP =  Projection * View * Model;


  // Envoi des matrices Model et View et de la lumière
  glUniformMatrix4fv(MID, 1, GL_FALSE, &Model[0][0]);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glUniform4fv(ambiantID,1,ambiant);
  glUniform4fv(diffuseID,1,diffuse);
  glUniform3fv (lightID,1,lumiere) ;

  glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, NULL);   
  glfwSwapBuffers(window);

}


// Les touches F-H-T-V déplacent la lampe en x et y
// J et L déplacent la lampe sur l'axe des z
// C et B pour l'intensité de la lumière ambiante
// D et G pour l'intensité de la lumière diffuse
static void Clavier(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        lumiere[0] -= 0.5;
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
        lumiere[0] += 0.5;
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        lumiere[1] -= 0.5;
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        lumiere[1] += 0.5;
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
        lumiere[2] -= 0.5;
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        lumiere[2] += 0.5;
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++)
            ambiant[i] -= 0.1;
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++)
            ambiant[i] += 0.1;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++)
            diffuse[i] -= 0.1;
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++)
            diffuse[i] += 0.1;
    }

    if (key ==  GLFW_KEY_UP)
        translation = glm::translate(translation, glm::vec3(0.f, stepTrans, 0.f));
    if (key ==  GLFW_KEY_DOWN)
        translation = glm::translate(translation, glm::vec3(0.f, -stepTrans, 0.f));
    if (key ==  GLFW_KEY_RIGHT)
        translation = glm::translate(translation, glm::vec3(-stepTrans, 0.f, 0.f));
    if (key ==  GLFW_KEY_LEFT)
        translation = glm::translate(translation, glm::vec3(stepTrans, 0.f, 0.f));
    if (key ==  GLFW_KEY_PAGE_UP)
        translation = glm::translate(translation, glm::vec3(0.f, 0.f, stepTrans));
    if (key ==  GLFW_KEY_PAGE_DOWN)
        translation = glm::translate(translation, glm::vec3(0.f, 0.f, -stepTrans));

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    glfwSwapBuffers(window);
}

void Souris(GLFWwindow *window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftbutton=true;
            mouseXOld = Xpos;
            mouseYOld = Ypos;
        } else {
            leftbutton=false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            middlebutton=true;
            mouseXOld = Xpos;
            mouseYOld = Ypos;
        } else {
            middlebutton=false;
        }
    }

    if (button== GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            mouseXOld = Xpos;
            mouseYOld = Ypos;
            rightbutton=true;
        }
        else {
            rightbutton=false;
        }
    }
}

void Motion(GLFWwindow *window, double xpos, double ypos)
{
    Xpos = xpos;
    Ypos = ypos;
    if (leftbutton==true) {
        rotation = glm::rotate(rotation, (float)(xpos-mouseXOld)*stepTrans/10, glm::vec3(0.f, 1.f, 0.f));
    }
    if (middlebutton==true) {
        translation = glm::translate(translation, glm::vec3(0.f, 0.f, (ypos - mouseYOld) * stepTrans / 10));
        mouseXOld = xpos;
        mouseYOld = ypos;
    }
    if (rightbutton==true) {
        translation = glm::translate(translation, glm::vec3((xpos-mouseXOld)*stepTrans/10, -(ypos-mouseYOld)*stepTrans/10, 0.f));
        mouseXOld = xpos;
        mouseYOld = ypos;
    }
}

int main(int argc, char** argv)
{

    glfwInit();

    // Quelle est la compatibilité OpenGL et contexte créé : ici OpenGL 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    // CORE_PROFILE on n'admet pas les fonctions OpenGL deprecated dans les versions précédentes  (même si elles sont encore disponibles)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // FORWARD_COMPATIBLE on n'admet pas les fonctions OpenGL qui vont devenir deprecated dans les futures versions ?
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Création de la fenetre
    window = glfwCreateWindow(500, 500, "Test Lumiere 1", NULL, NULL);

    glfwSetKeyCallback(window, Clavier);
    glfwSetMouseButtonCallback(window, Souris);
    glfwSetCursorPosCallback(window, Motion);

    if (!window) {
        std::cout << "Impossible de créer la fenêtre GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Taille et position à l'écran de la fenêtre créée
    glfwSetWindowPos(window, 100, 100);
    glfwSetWindowSize(window, 500, 500);

    // On attache le contexte actuel OpenGL à la fenêtre
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    init();
    while (!glfwWindowShouldClose(window)) {
        Display();
        glfwPollEvents();
    }

    glfwTerminate();
  return 0 ;
  
}
