#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "../Common/shaders_utilities.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;


// Nouveau tableau de coordonnées
static GLfloat coordonnees[] = {
        0, 0, 0,       // sommet 0
        0.5, 0, 0,     // 1
        0.5, 0.5, 0,   //2
        0, 0.5, 0,     //3
        0, 0, 0.5,     //4
        0.5, 0, 0.5,   //5
        0.5, 0.5, 0.5, //6
        0, 0.5, 0.5    //7
};

// Nouveau tableau de couleurs
static GLfloat couleurs[] = {
        1, 1, 1,
        0, 1, 1,
        0, 0, 1,
        0, 1, 0,
        1, 1, 1,
        0, 1, 1,
        0, 0, 1,
        0, 1, 0
};

// Un tableau d'indices pour dessiner un cube à partir des numéros des sommets
GLuint indices[] = {
        3, 0, 2,
        0, 2, 1,
        1, 2, 6,
        1, 6, 5,
        6, 5, 7,
        5, 7, 4,
        7, 4, 3,
        4, 3, 0,
        3, 2, 7,
        2, 7, 6,
        0, 1, 4,
        1, 4, 5
};

// Un tableau de 3 identifiants ... coordonnées, couleurs et indices
GLuint vboid[3];
GLuint programID;
GLuint vaoID;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Les mêmes shaders que le code02
    programID = LoadShaders("../Shaders/ColorVertexShader.vert", "../Shaders/ColorFragShader.frag");


    glGenBuffers(3, vboid);

    glBindBuffer(GL_ARRAY_BUFFER, vboid[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), coordonnees, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboid[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), couleurs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(1);

    // Pour les indices (ce n'est pas un attribut) GL_ELEMENT_ARRAY_BUFFER et on active rien !
    // Mais ATTENTION il faut toujours que ce buffer soit activé avant le dessin
    // Ici c'est le dernier VBO "bindé".
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 12 * sizeof(GLuint), indices, GL_STATIC_DRAW);


}

void Display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    // La primitive triangle est utilisée et on dessine un cube à l'aide de 2 triangles par face
    // donc 12 triangles de 3 points chacun
    // Le dernier argument est NULL car on utilise des VBO.
    glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, NULL);


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
    GLFWwindow *window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
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
        // Echange des buffers écriture de l'image et lecture (si double buffering)
        glfwSwapBuffers(window);

    }

    glfwTerminate();

    return 0;

}
