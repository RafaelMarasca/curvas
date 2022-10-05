#ifndef POINT_H
#define POINT_H

#include "geometry.h"
#include<GL/glew.h>
#include<vector>

class point : public geometry
{

    public:
        point(GLenum usage = GL_DYNAMIC_DRAW);
        point(std::vector<GLfloat>& coordinates, GLenum usage = GL_DYNAMIC_DRAW);
        void draw(); //Método para desenho do ponto

};




#endif //POINT_H