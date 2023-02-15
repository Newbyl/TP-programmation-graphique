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

// Les sommets avec glDrawElements
// Attention un sommet est représenté plusieurs fois en prévision de la prochaine version qui nécessitera la définition de la normale
// En commentaire le premier indice c'est le numéro du point si on avait représenté le sommet qu'une fois.
static GLfloat coordonnees[] = {
  0,0,0, //0
  1,0,0, //1
  1,1,0, //2
  0,1,0, //3

  1,0,0, //1 4
  1,1,0, //2 5
  1,1,1, //6 6
  1,0,1, //5 7

  0,0,1, //4 8
  1,0,1, //5 9
  1,1,1, //6 10
  0,1,1, //7 11

  0,0,0, //0 12
  0,0,1, //4 13
  0,1,1, //7 14
  0,1,0, //3 15

  0,1,0, //3 16
  0,1,1, //7 17
  1,1,1, //6 18
  1,1,0, //2 19

  0,0,0, //0 20
  0,0,1, //4 21
  1,0,1, //5 22
  1,0,0  //1 23

};

// Des couleurs ...
static GLfloat couleurs[] = {
  1,1,1,
  0,1,1,
  0,0,1,
  0,1,0,

  1,1,1,
  0,1,1,
  0,0,1,
  0,1,0,

  1,1,1,
  0,1,1,
  0,0,1,
  0,1,0,

  1,1,1,
  0,1,1,
  0,0,1,
  0,1,0,

  1,1,1,
  0,1,1,
  0,0,1,
  0,1,0,

  1,1,1,
  0,1,1,
  0,0,1,
  0,1,0,
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
double mouseXOld, mouseYOld;
double Xpos, Ypos;
bool leftbutton = false;
bool rightbutton = false;
bool middlebutton = false;

GLuint vboID[3];
GLuint programID;
GLuint vaoID;

// les identifiants pour envoyer à la carte graphique les matrices et la position de la lumière
GLint MatrixID;

glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

glm::mat4 translation;
glm::mat4 trans_initial;
glm::mat4 rotation;


// Voilà la définition de la lumière ambiante
// Il s'agit d'une couleur (RGBA)
// là c'est du blanc avec une intensité 0.2
// Dans la fonction Clavier il est prévu de modifier cette intensité (en + ou en -)
GLfloat ambiant[4] = {0.2,0.2,0.2,1.0};

// L'identifiant supplémentaire pour transmettre ce vecteur à la carte graphique
GLint AmbiantID;

void init() {

  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  programID = LoadShaders( "../Shaders/AmbiantVertexShader.vert", "../Shaders/AmbiantFragmentShader.frag" );
  
  glUseProgram(programID);

  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  glGenBuffers(3,vboID);

  glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
  glBufferData(GL_ARRAY_BUFFER,3*6*4*sizeof(float),coordonnees,GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
  glBufferData(GL_ARRAY_BUFFER,3*6*4*sizeof(float),couleurs,GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  glEnableVertexAttribArray(1);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*12*sizeof(GLuint),indices,GL_STATIC_DRAW);
  
  

  // Génération des identifiants pour les variables Uniform vers le vertex shader
  // La matrice et la composante ambiante de la lumière
  MatrixID = glGetUniformLocation(programID, "MVP");
  AmbiantID = glGetUniformLocation(programID, "ambiant");
  
  Projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  View = glm::lookAt(glm::vec3(0,0,-5.0), glm::vec3(0,0,0), glm::vec3(0,1,0));
  
  trans_initial = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5,-0.5,-0.5));
  
  Model = glm::mat4(1.0f);
  
  rotation = glm::mat4(1.0);
  
  translation = glm::mat4(1.0f);

}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  Model = translation * rotation * trans_initial;

  MVP =  Projection * View * Model;

  // Transmission de la matrice
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  // Transmission de la composante ambiante de la lumière
  glUniform4fv(AmbiantID,1,ambiant);
  
  glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, NULL);

  glfwSwapBuffers(window);

}

static void Clavier(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key==GLFW_KEY_D && action==GLFW_PRESS)
        stepTrans+=0.1;

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++)
            ambiant[i] -= 0.1;
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++)
            ambiant[i] += 0.1;
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
                translation = glm::translate(translation, glm::vec3(0.f, 0.f, -(ypos - mouseYOld) * stepTrans / 10));
                mouseXOld = xpos;
                mouseYOld = ypos;
    }
    if (rightbutton==true) {
        translation = glm::translate(translation, glm::vec3(-(xpos-mouseXOld)*stepTrans/10, -(ypos-mouseYOld)*stepTrans/10, 0.f));
        mouseXOld = xpos;
                mouseYOld = ypos;
    }
}

int main(int argc, char **argv) {
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

    return 0;

}
