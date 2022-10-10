#ifndef CURVES_H
#define CURVES_H

#include<GL/glew.h>
#include"geometry.h"
#include"line.h"


#define RESOLUTION 0.01f

#define R_SPLINE 0.9529f
#define G_SPLINE 0.0863f
#define B_SPLINE 0.0588f

#define R_CONTROL 0.0196f 
#define G_CONTROL 0.7412f
#define B_CONTROL 0.0078f


class bSpline : public geometry
{
    private:
        int order;
        
        std::vector<GLfloat> knots;
        std::vector<GLfloat> controlPoints;

        lineStrip* controlLine;

        bool isClamped;
        bool controlPointsVisibility;

    public:

        bSpline(int order, GLenum usage = GL_STATIC_DRAW);
        bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage = GL_STATIC_DRAW);
        ~bSpline();

        bool collision(geometry* other);
        bool collision(GLfloat x, GLfloat y, GLfloat z);

        void addControlPoint(std::vector<GLfloat>& newControlPoint);
        void addControlPoint(GLfloat x, GLfloat y, GLfloat z);
        void generate();
        void setColor(GLfloat r, GLfloat g, GLfloat b);
        void resetColor();
        void draw();
        void makeClamped();
        void makeUnclamped();
        void setControlPointsVisibility(bool isVisible);
};



#endif //CURVES_H