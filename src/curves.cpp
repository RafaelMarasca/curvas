#include "curves.h"
#include<iostream>
#include<cmath>
#include<GL/glew.h>


float BoorAlgo(std::vector<GLfloat>& knots, int i, int j, float t)
{
    if(j == 0)
    {
        return (t>= knots[i] && t<knots[i+1]) ? 1 : 0;
    }

    float A1 = 0.0f;
    float A2 = 0.0f;
    if(!(std::abs(knots[i+j] - knots[i]) < 0.0000001f))
    {
        A1 = BoorAlgo(knots, i,j-1,t)*((t-knots[i])/(knots[i+j] - knots[i]));
    }
    
    if(!(std::abs(knots[i+j+1] - knots[i+1]) < 0.0000001f))
    {
        A2 = BoorAlgo(knots, i+1,j-1,t)*((knots[i+j+1]-t)/(knots[i+j+1]-knots[i+1]));
    }

    return  A1+A2; 
}



bSpline::bSpline(GLenum usage /*= GL_DYNAMIC_DRAW*/) : geometry(usage)
{
    this->setColor(1.0f,0.0f, 0.0f);
}

bSpline::bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage /*= GL_DYNAMIC_DRAW*/): geometry(usage)
{
    this->setColor(1.0f,0.0f, 0.0f);

    int size = controlPoints.size()/3;

    /*for(int i = 0; i<(size+order+1); i++)
    {
        this->knots.push_back(float(i)/float(size+order));
        std::cout<<knots[i]<<std::endl;
    }*/

    for(int i = 0; i<(size+1-order); i++)
    {
        this->knots.push_back(float(i)/float(size-order));
        if(i == 0 || i == (size-order))
        {
            for(int j = 0; j< order; j++)
                this->knots.push_back(this->knots[i == 0? 0:(size)]);
        }
    }


    for(int i = 0; i<knots.size(); i++)
    {
        std::cout<<knots[i]<<std::endl;
    }
   

    float t = 0.0f;//this->knots[order] + RESOLUTION;

    while(t <= 1.0f)//*(this->knots.end()-order-1))
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

