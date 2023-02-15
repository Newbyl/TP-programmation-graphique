#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "../Common/shaders_utilities.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "icosaedre.hpp"

using namespace std;


int win;

GLuint vboid[3];
GLuint programID;
GLuint vaoID;

// Pour les interactions, gestion des events.
GLFWwindow *window;
float stepTrans = 0.1;
int mouseXOld, mouseYOld;
double Xpos, Ypos;
bool leftbutton = false;
bool rightbutton = false;
bool middlebutton = false;

// Un identifiant encore ....
GLuint MatrixID;

// Des matrices ...
// On va utiliser pour les construire et les manipuler glm (OpenGL Mathematics)
glm::mat4 Projection; // une matrice de projection par rapport à l'écran
glm::mat4 View; // une matrice pour se placer par rapport à la caméra
glm::mat4 Model; // une matrice Model pour déplacer dans le repère universel les objets
glm::mat4 MVP; // Et le produit des trois

glm::mat4 translation; // une matrice de translation qui va nous permettre de déplacer le cube
glm::mat4 trans_initial; // une matrice de translation pour centrer le cube
glm::mat4 rotation; // une matrice pour construire une rotation appliquée sur le cube


static GLfloat nvx_coordonnees[];
static GLuint nvx_index[];

void subdivision(int nbtriangles, GLfloat* sommets, GLfloat* newsommets, GLuint* index, GLuint* newindex) {
    for (int i = 0; i < nbtriangles; i+=3) {
        int tri1 = index[i];
        int tri2 = index[i + 1];
        int tri3 = index[i + 2];
        
        int nX1 = (sommet[tri1] + sommet[tri2]) / 2;
        int nX2 = (sommet[tri2] + sommet[tri3]) / 2;
        int nX3 = (sommet[tri1] + sommet[tri3]) / 2;

        int nY1 = (sommet[tri1 + 1] + sommet[tri2 + 1]) / 2;
        int nY2 = (sommet[tri2 + 1] + sommet[tri3 + 1]) / 2;
        int nY3 = (sommet[tri1 + 1] + sommet[tri3 + 1]) / 2;
        
        int nZ1 = (sommet[tri1 + 2] + sommet[tri2 + 2]) / 2;
        int nZ2 = (sommet[tri2 + 2] + sommet[tri3 + 2]) / 2;
        int nZ3 = (sommet[tri1 + 2] + sommet[tri3 + 2]) / 2;

        for (int j = 0; j < 6; j++) {
            newsommets[j] = 
        }
    }
}



// Position de la lumière : elle sera fixe dans la scéne
// cf Clavier pour voir comment la déplacer
GLfloat lumiere[] = {5.0,0.0,0.0};

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

// les identifiants pour envoyer à la carte graphique les matrices et la position de la lumière
GLint MID;


GLint lightID;

// les composantes AMBIANT et DIFFUSE (du blanc avec une certaine intensité
// 0.2 et 0.8 sont des valeurs habituelles par rapport à une lumière classique)
GLfloat ambiant[4] = {0.2,0.2,0.2,1.0};
GLfloat diffuse[4] = {0.8,0.8,0.8,1.0};

GLuint vboID[4];

GLint ambiantID;
GLint diffuseID;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // programID = LoadShaders("../Shaders/TransformVertexShader.vert", "../Shaders/ColorFragShader.frag");
    programID = LoadShaders( "../Shaders/AmbiantVertexShader.vert", "../Shaders/AmbiantFragmentShader.frag" );

    glGenBuffers(3, vboid);
    glBindBuffer(GL_ARRAY_BUFFER, vboid[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 12 * sizeof(float), coordonnees, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
    glBufferData(GL_ARRAY_BUFFER,3*6*4*sizeof(float),normales,GL_STATIC_DRAW);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(2);


    // glBindBuffer(GL_ARRAY_BUFFER, vboid[1]);
    // //glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), couleurs, GL_STATIC_DRAW);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    // glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 20 * sizeof(GLuint), indices, GL_STATIC_DRAW);


    // Il faut transmettre les informations à la carte graphique et au vertex shader
    // D'où cet identifiant (regarder dans TransformVertexShader et la déclaration d'une variable "uniform" MVP)
    MatrixID = glGetUniformLocation(programID, "MVP");

    MID = glGetUniformLocation(programID, "M");

    lightID =  glGetUniformLocation(programID, "light_position");
    ambiantID = glGetUniformLocation(programID, "ambiant");
    diffuseID = glGetUniformLocation(programID, "diffuse");
    Projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    //Avant on était en projection orthographique
    // Maintenant on est en perspective !
    Projection = glm::perspective(70.0f, 1.0f, 0.1f, 100.0f);
    // En projection orthographique pas besoin de caméra
    // Ici il faut la placer
    View = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    //  std::cout<<glm::to_string(View)<<std::endl;

    // Création d'une matrice de translation pour déplacer le cube de -0.25 en X, Y et Z
    trans_initial = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25, -0.25, -0.25));


    // On initialise Model à l'identité
    Model = glm::mat4(1.0f);

    // Au final l'objet est placé dans le repère universel soit x un point en coordonnées homogènes
    // Model*x : permet d'appliquer des transformations toujours dans le repère universel
    // View*Model*x : on se place dans le repère de la caméra
    // Projection*View*Model*x : on projette sur l'écran 2D
    MVP = Projection * View * Model;

    // Initialisation d'une rotation et d'une translation à l'identité.
    rotation = glm::mat4(1.0f);
    translation = glm::mat4(1.0f);
}

void Display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    // Mise à jour de la matrice Model
    // Attention à l'ordre des transformations
    // une rotation suivie d'une translation n'a pas le même effet qu'une translation suivie d'une rotation
    Model = translation * rotation * trans_initial;
    MVP = Projection * View * Model;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glUniform4fv(ambiantID,1,ambiant);

    // Envoi des matrices Model et View et de la lumière
    glUniformMatrix4fv(MID, 1, GL_FALSE, &Model[0][0]);

    glDrawElements(GL_TRIANGLES, 3 * 20, GL_UNSIGNED_INT, NULL);
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
            leftbutton = true;
            mouseXOld = Xpos;
            mouseYOld = Ypos;
        } else {
            leftbutton = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            middlebutton = true;
            mouseXOld = Xpos;
            mouseYOld = Ypos;
        } else {
            middlebutton = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            mouseXOld = Xpos;
            mouseYOld = Ypos;
            rightbutton = true;
        } else {
            rightbutton = false;
        }
    }
}

void Motion(GLFWwindow *window, double xpos, double ypos) {
    Xpos = xpos;
    Ypos = ypos;
    if (leftbutton == true) {
        rotation = glm::rotate(rotation, (float) (xpos - mouseXOld) * stepTrans / 10, glm::vec3(0.f, 1.f, 0.f));
    }
    if (middlebutton == true) {
        translation = glm::translate(translation, glm::vec3(0.f, 0.f, -(ypos - mouseYOld) * stepTrans / 10));
        mouseXOld = xpos;
        mouseYOld = ypos;
    }
    if (rightbutton == true) {
        translation = glm::translate(translation, glm::vec3((xpos - mouseXOld) * stepTrans / 10,
                                                            -(ypos - mouseYOld) * stepTrans / 10, 0.f));
        mouseXOld = xpos;
        mouseYOld = ypos;
    }
}

int main(int argc, char **argv) {
    // On initialise GLFW : retourne un code d'erreur
    glfwInit();

    // Quelle est la compatibilité OpenGL et contexte créé : ici OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    // CORE_PROFILE on n'admet pas les fonctions OpenGL deprecated dans les versions précédentes  (même si elles sont encore disponibles)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // FORWARD_COMPATIBLE on n'admet pas les fonctions OpenGL qui vont devenir deprecated dans les futures versions ?
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Création de la fenetre
    window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);

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


    // Fonction qui permet d'initialiser "des choses" si nécessaire (souvent nécessaire).
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    init();

    // Lancement de la boucle GLFW
    while (!glfwWindowShouldClose(window)) {
        // Appel de la fonction de dessin
        Display();
        // On récupère les events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;

}
