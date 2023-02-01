#include <iostream>
#include <math.h>
#include <GL/gl.h>
using namespace std;

void initialisation(const char *nom_file, int *NbSommets, int *NbFacets) {
    FILE *fp = fopen(nom_file, "r");
    *NbFacets = 0;
    *NbSommets = 0;

    char *c = (char *) malloc(sizeof(char));
    float cx, cy, cz;
    while (fscanf(fp, "%c %f %f %f\n", c, &cx, &cy, &cz) != EOF) {
        if (*c == 'v')
            (*NbSommets)++;
        if (*c == 'f')
            (*NbFacets)++;
    }
    fclose(fp);
}

void lecture(const char *nom_file, int NbSommets, int NbFacets, GLfloat *coordonnees, GLuint *indices, float *bbox) {
    FILE *fp = fopen(nom_file, "r");
    char *c = (char *) malloc(sizeof(char));
    float cx, cy, cz;
    fscanf(fp, "%c %f %f %f\n", c, &cx, &cy, &cz);
    coordonnees[0] = cx;
    coordonnees[1] = cy;
    coordonnees[2] = cz;
    bbox[0] = bbox[1] = cx;
    bbox[2] = bbox[3] = cy;
    bbox[4] = bbox[5] = cz;
    for (int i = 1; i < NbSommets; i++) {
        fscanf(fp, "%c %f %f %f\n", c, &cx, &cy, &cz);
        coordonnees[3 * i] = cx;
        coordonnees[3 * i + 1] = cy;
        coordonnees[3 * i + 2] = cz;
        if (cx < bbox[0])
            bbox[0] = cx;
        if (cx > bbox[1])
            bbox[1] = cx;
        if (cy < bbox[2])
            bbox[2] = cy;
        if (cy > bbox[3])
            bbox[3] = cy;
        if (cz < bbox[4])
            bbox[4] = cz;
        if (cz > bbox[5])
            bbox[5] = cz;
    }
    int i1, i2, i3;
    for (int i = 0; i < NbFacets; i++) {
        fscanf(fp, "%c %d %d %d\n", c, &i1, &i2, &i3);
        indices[3 * i] = i1;
        indices[3 * i + 1] = i2;
        indices[3 * i + 2] = i3;
    }
    fclose(fp);
}

void boundingbox(GLfloat* coord, GLfloat* bbox)
{
    coord[0] = bbox[0]; //xmin
    coord[1] = bbox[2]; //ymin
    coord[2] = bbox[5]; //zmax

    coord[3] = bbox[1]; //xmax
    coord[4] = bbox[2]; //ymin
    coord[5] = bbox[5]; //zmax

    coord[6] = bbox[0]; //xmin
    coord[7] = bbox[3]; //ymax
    coord[8] = bbox[5]; //zmax

    coord[9] = bbox[1]; //xmax
    coord[10] = bbox[3]; //ymax
    coord[11] = bbox[5]; //zmax

    coord[12] = bbox[0]; //xmin
    coord[13] = bbox[2]; //ymin
    coord[14] = bbox[4]; //zmin

    coord[15] = bbox[1]; //xmax
    coord[16] = bbox[2]; //ymin
    coord[17] = bbox[4]; //zmin

    coord[18] = bbox[0]; //xmin
    coord[19] = bbox[3]; //ymax
    coord[20] = bbox[4]; //zmin

    coord[21] = bbox[1]; //xmax
    coord[22] = bbox[3]; //ymax
    coord[23] = bbox[4]; //zmin


}