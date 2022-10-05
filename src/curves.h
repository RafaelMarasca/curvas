#ifndef CURVES_H
#define CURVES_H

#include<GL/glew.h>
#include"geometry.h"


#define RESOLUTION 0.01f

class bSpline : public geometry
{
    private:
        std::vector<GLfloat> knots;

    public:

        bSpline(GLenum usage = GL_STATIC_DRAW);
        bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage = GL_STATIC_DRAW);
        
        void draw();
};



#endif //CURVES_H