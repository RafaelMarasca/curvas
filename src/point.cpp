#include "point.h"
#include<GL/glew.h>
#include<vector>



point::point(GLenum usage /*= GL_DYNAMIC_DRAW*/) : geometry(usage)
{
    this->setColor(1.0f,0.0f, 0.0f);
}


point::point(std::vector<GLfloat>& coordinates, GLenum usage /*= GL_DYNAMIC_DRAW*/) : geometry(coordinates, usage)
{
    this->setColor(1.0f,0.0f, 0.0f);
}



void point::draw()
{
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, this->vertices.size()/3);
}