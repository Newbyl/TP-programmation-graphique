#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "../Common/shaders_utilities.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

static GLfloat coordonnees[] = {
        -1, 0, 0,
        1, 0, 0,
        0, 1, 0,
        -1, 0, 0,
        1, 0, 0,
        0, -1, 0};


// Le sommet est défini par un attribut supplémentaire : la couleur en RGB
static GLfloat couleurs[] = {
        1, 0, 0, // rouge
        0, 1, 0, // vert
        0, 0, 1, // bleu
        1, 0, 0, // rouge
        0, 1, 0, // vert
        0, 0, 1
};

// 2 attributs donc deux VBO pour les gérer donc deux identifiants
GLuint vboid[2];

GLuint programID;
GLuint vaoID;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Deux nouveaux shaders
    programID = LoadShaders("../Shaders/ColorVertexShader.vert", "../Shaders/ColorFragShader.frag");

    // Création de deux identifiants (un tableau) pour les deux VBO
    glGenBuffers(2, vboid);

    // 1er identifiant pour le VBO des coordonnées + chargement des données etc
    glBindBuffer(GL_ARRAY_BUFFER, vboid[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), coordonnees, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0); // layout(0) dans le shader



    // 2eme identifiant pour le VBO des couleurs +  chargement des données
    glBindBuffer(GL_ARRAY_BUFFER, vboid[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), couleurs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(1); // layout(1) dans le shader

}

void Display(void) {

    // On nettoie l'écran avant de dessiner
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // installation des shaders : programme exécuté par la carte graphique
    glUseProgram(programID);

    // Le dessin des triangles
    // primitive graphique : ici les triangles
    // 0 : car on commence au début des données
    // 6 : car il y a 6 sommets dans deux triangles
    glDrawArrays(GL_TRIANGLES, 0, 6);


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
