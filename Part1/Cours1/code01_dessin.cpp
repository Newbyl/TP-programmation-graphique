#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "../Common/shaders_utilities.hpp"


using namespace std;

// Les coordonnées de 3 sommets dans le repère universel

static GLfloat coordonnees[] = {
        -1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0};


// Des identifiants
GLuint vboID;
GLuint programID;
GLuint vaoID;

// Afin de réunir les initialisations
void init() {
    // Définition de la couleur de l'écran (quand on efface le dessin on remplace par cette couleur)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // La valeur pour effacer le buffer de profondeur
    glClearDepth(1.0);
    // Activation du test de profondeur
    glEnable(GL_DEPTH_TEST);

    // Création d'un identifiant de VAO Vertex Array Object
    glGenVertexArrays(1, &vaoID);
    // Dorénavant le VAO actif est celui lié à VertexArrayID : vaoID
    glBindVertexArray(vaoID);

    // Lecture etc de shaders (le programme utilisé n'a pas d'importance pour le moment)
    // Par contre il n'est pas inutile d'aller voir les 2 shaders ...
    programID = LoadShaders("../Shaders/SimpleVertexShader.vert", "../Shaders/SimpleFragmentShader.frag");

    // Création d'un identifiant de VBO Vertex Buffer Object
    glGenBuffers(1, &vboID);

    // On travaille désormais sur ce VBO identifié par vboID
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    // On charge les données à partir du tableau sommets qui contient bien les coordonnées 3D de 3 sommets
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), coordonnees, GL_STATIC_DRAW);

    // On décrit le contenu du VBO :
    // 0 : l'index de l'attribut (en lien avec le vertex shader)
    // 3 : car l'attribut est composé de 3 valeurs (les coordonnées)
    // Le type : GL_FLOAT
    // GL_FALSE car les coordonnées ne sont pas normalisées
    // 0 : car on commence en début du tableau (pas de décalage)
    // (void*)0 car les données sont dans le VBO associé et non pas sur le CPU
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    // Activation de cet attribut (cf layout(0) dans le vertex shader)
    glEnableVertexAttribArray(0);
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
        // On nettoie l'écran avant de dessiner
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // installation des shaders : programme exécuté par la carte graphique
        glUseProgram(programID);

        // Le dessin des triangles
        // primitive graphique : ici les triangles
        // 0 : car on commence au début des données
        // 3 : car il y a 3 sommets dans un triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Echange des buffers écriture de l'image et lecture (si double buffering)
        glfwSwapBuffers(window);

        // On récupère les events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
