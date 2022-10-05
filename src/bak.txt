#include "curves.h"
#include<iostream>
#include<GL/glew.h>


float BoorAlgo(std::vector<GLfloat>& knots, int i, int j, float t)
{
    if(j == 1 )
        return (t>= knots[i] && t<knots[i+1]) ? 1 : 0;

    float N1 = BoorAlgo(knots, i,j-1,t);
    float N2 = BoorAlgo(knots, i+1,j-1,t);

    return ((t-knots[i])/(knots[i+j-1] - knots[i]))*N1 + ((knots[i+j]-t)/(knots[i+j]-knots[i+1]))*N2;
}



bSpline::bSpline(GLenum usage /*= GL_DYNAMIC_DRAW*/) : geometry(usage)
{
    this->setColor(1.0f,0.0f, 0.0f);
}

bSpline::bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage /*= GL_DYNAMIC_DRAW*/): geometry(usage)
{
    this->setColor(1.0f,0.0f, 0.0f);

    int size = controlPoints.size()/3;

    

    for(int i = 0; i<(size+1-order); i++)
    {
        this->knots.push_back(float(i)/float(size-order));
        if(i == 0 || i == (size-order))
        {
            for(int j = 0; j< order; j++)
                this->knots.push_back(this->knots[i == 0? 0:(size)]);
        }
    }

    for(int i = 0; i< this->knots.size();i++)
    {
        std::cout<<this->knots[i]<<std::endl;
    }
    
    float t = this->knots[order];

    while(t <= *(this->knots.end()-order-1))
    {
        this->vertices.push_back(0.0f);
        this->vertices.push_back(0.0f);
        this->vertices.push_back(0.0f);

        for(int i = 0; i< size; i++)
        {
            float N = BoorAlgo(this->knots,i,order,t);

            *(this->vertices.end()-3) += N*controlPoints[i*3];
            *(this->vertices.end()-2) += N*controlPoints[(i*3)+1];
            *(this->vertices.end()-1) += N*controlPoints[(i*3)+2];
        }
        
        t += RESOLUTION;
    }

    //Tranfere os novos dados aos buffers de desenho.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
    glBufferData(GL_ARRAY_BUFFER, (this->vertices).size()*sizeof(GLfloat),&(this->vertices[0]), this->usage);
}

void bSpline::draw()
{
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINE_STRIP, 0, this->vertices.size()/3);
}

