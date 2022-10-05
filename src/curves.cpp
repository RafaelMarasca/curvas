#include "curves.h"
#include<iostream>
#include<cmath>
#include<GL/glew.h>


float genAlgo(std::vector<GLfloat>& knots, int i, int j, float t)
{
    if(j == 0)
    {
        return (t>= knots[i] && t<knots[i+1]) ? 1 : 0;
    }

    float A1 = 0.0f;
    float A2 = 0.0f;

    if(!(std::abs(knots[i+j] - knots[i]) < 0.0000001f))
    {
        A1 = genAlgo(knots, i,j-1,t)*((t-knots[i])/(knots[i+j] - knots[i]));
    }
    
    if(!(std::abs(knots[i+j+1] - knots[i+1]) < 0.0000001f))
    {
        A2 = genAlgo(knots, i+1,j-1,t)*((knots[i+j+1]-t)/(knots[i+j+1]-knots[i+1]));
    }

    return  A1+A2; 
}


void bSpline::generate()
{
    if(this->controlPoints.size() < this->order+1)
        throw std::string("Ordem da spline eh menor que a quantidade de pontos de controle.");

    std::cout<<"chamado";

    this->controlLine->print();


    int size = controlPoints.size()/3;

    for(int i = 0; i<(size+1-this->order); i++)
    {
        this->knots.push_back(float(i)/float(size-order));
        if(i == 0 || i == (size-this->order))
        {
            for(int j = 0; j< this->order; j++)
                this->knots.push_back(this->knots[i == 0? 0:(size)]);
        }
    }

    /**for(int i = 0; i<knots.size(); i++)
    {
        std::cout<<knots[i]<<std::endl;
    }*/
   

    float t = 0.0f;//this->knots[order] + RESOLUTION;

    while(t <= 1.0f)//*(this->knots.end()-order-1))
    {
        this->vertices.push_back(0.0f);
        this->vertices.push_back(0.0f);
        this->vertices.push_back(0.0f);

        for(int i = 0; i< size; i++)
        {
            float N = genAlgo(this->knots,i,order,t);

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

void bSpline::addControlPoint(std::vector<GLfloat>& newControlPoint)
{
    if((newControlPoint.size()%3)!=0)
    {
        throw std::string("O vetor de pontos deve ter tamanho multiplo de 3");
    }

    this->controlPoints.insert(this->controlPoints.end(), newControlPoint.begin(), newControlPoint.end());

    this->controlLine->addVertex(newControlPoint);
}



bSpline::bSpline(int order, GLenum usage /*= GL_DYNAMIC_DRAW*/) : geometry(usage)
{
    this->order = order;
    this->setColor(R_SPLINE, G_SPLINE, B_SPLINE);
    this->controlLine = new lineStrip();
    this->controlLine->setColor(R_CONTROL, G_CONTROL, B_CONTROL);
}

bSpline::bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage /*= GL_DYNAMIC_DRAW*/): geometry(usage)
{
    this->setColor(R_SPLINE, G_SPLINE, B_SPLINE);
    this->order = order;
    this->controlLine = new lineStrip();
    this->controlLine->setColor(R_CONTROL, G_CONTROL, B_CONTROL);
}


bSpline::~bSpline()
{
    delete this->controlLine;
}

void bSpline::draw()
{
    this->controlLine->draw();
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINE_STRIP, 0, this->vertices.size()/3);
}




/**
 * @brief Recebe um ponto (x,y,z) e verifica se há colisão entre um dos pontos de 
 * controle da curva e o ponto fornecido.
 * 
 * @param x : coordenada x do ponto.
 * @param y : coordenada y do ponto.
 * @param z : coordenada z do ponto.
 * @return true : Se há uma colisão entre um dos pontos de controle e o ponto x,y,z.
 * @return false : Caso não haja colisão entre o ponto x,y,z e algum ponto de controle.
 */
bool bSpline::collision(GLfloat x, GLfloat y, GLfloat z)
{
    std::vector<GLfloat>::iterator it; //Iterator para um vector de GLfloat
                                 
    //Verifica se há colisão entre o ponto e os vértices                                 
    for(it = this->controlPoints.begin(); it!= this->controlPoints.end(); it+=3)
    {
       if((std::abs(*it - x) < 0.01f ) && (std::abs(*(it+1) - y) < 0.01f ))
        return true;
    }
}

/**
 * @brief Verifica colisão entre duas curvas
 * 
 * @param other : A outra curva que se quer verificar se houve colisão.
 * @return true : Caso haja colisão entre as curvas.
 * @return false : Caso não haja colisão entre as curvas.
 */
bool bSpline::collision(bSpline* other)
{
    std::vector<GLfloat>::iterator it;

    //Verifica se há colisão entre um dos pontos de controle da outra curva e a outra curva.        
    for(it = other->controlPoints.begin(); it!= other->controlPoints.end(); it+=3)
    {
        if(this->collision(*it, *(it+1), *(it+2)))
			return true; //colisão detectada.
    }
 
    return false; // Não foi detectada colisão.
}