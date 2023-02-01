#ifndef MODELE_HPP
#define MODELE_HPP

void initialisation(const char *nom_file, int *NbSommets, int *NbFacets);
void lecture(const char *nom_file, int NbSommets, int NbFacets, GLfloat *coordonnees, GLuint *indices, float *bbox);
void boundingbox(GLfloat* coord, GLfloat* bbox);

#endif
