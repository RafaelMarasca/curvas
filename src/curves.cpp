/**
 * @file curves.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe B-Spline
 * @version 0.1
 * @date 2022-10
 */


#include "curves.h"
#include<iostream>
#include<cmath>
#include<GL/glew.h>


/**
 * @brief Avalia o valor da função base (N(i,j,t)) da curva B-Spline conforme o algoritmo recursivo de de Boor.
 * 
 * @param knots : Vetor de nós da B-Spline.
 * @param i : Índice i - Ponto de controle i.
 * @param j : Índice j - Grau da curva.
 * @param t : Ponto de Avaliação da função
 * @return float - Valor da função base 
 */
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
        A1 = genAlgo(knots, i, j-1, t) * ((t-knots[i])/(knots[i+j] - knots[i]));
    }
    
    if(!(std::abs(knots[i+j+1] - knots[i+1]) < 0.0000001f))
    {
        A2 = genAlgo(knots, i+1, j-1, t) * ((knots[i+j+1]-t)/(knots[i+j+1] - knots[i+1]));
    }

    return  A1+A2; 
}


/**
 * @brief Gera a B-Spline.
 * 
 */
void bSpline::generate()
{
    if(this->controlPoints.size() <= this->order)
        throw std::string("Ordem da spline eh menor que a quantidade de pontos de controle.");

    int size = controlPoints.size()/3;

    //Gera os nós para b-spline presa nos pontos inicial e final
    if(this->isClamped)
    {
        for(int i = 0; i<(size+1-this->order); i++)
        {
            this->knots.push_back(float(i)/float(size-order));
            if(i == 0 || i == (size-this->order))
            {
                for(int j = 0; j< this->order; j++)
                    this->knots.push_back(this->knots[i == 0? 0:(size)]);
            }
        }
    }else //Gera os nós para b-spline não presa nos pontos inicial e final
    {
        for(int i = 0; i<(size+this->order+1); i++)
        {
            this->knots.push_back(float(i)/float(size+this->order));
        }
    }

    //Calcula o valor da curva no ponto t conforme o algoritmo de de Boor
    float t = this->knots[order];

    while(t <= this->knots[size])
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


/**
 * @brief Adiciona um ponto de controle à curva.
 * 
 * @param newControlPoint vetor contendo as coordenadas do ponto de controle a ser inserido.
 */
void bSpline::addControlPoint(std::vector<GLfloat>& newControlPoint)
{
    if((newControlPoint.size()%3)!=0)
    {
        throw std::string("O vetor de pontos deve ter tamanho multiplo de 3");
    }

    this->controlPoints.insert(this->controlPoints.end(), newControlPoint.begin(), newControlPoint.end());

    this->controlLine->addVertex(newControlPoint);
}

/**
 * @brief Adiciona um ponto de controle à curva.
 * 
 * @param x Coordenada x do ponto.
 * @param y Coordenada y do ponto.
 * @param z Coordenada z do ponto.
 */
void bSpline::addControlPoint(GLfloat x, GLfloat y, GLfloat z)
{
    this->controlPoints.push_back(x);
    this->controlPoints.push_back(y);
    this->controlPoints.push_back(z);

    std::vector<GLfloat> newPoint = {x,y,z};
    this->controlLine->addVertex(newPoint);
}


/**
 * @brief Construtor para a b-Spline.
 * 
 * @param order : ordem da b-Spline.
 */
bSpline::bSpline(int order, GLenum usage /*= GL_DYNAMIC_DRAW*/) : geometry(usage)
{
    this->isClamped = true;
    this->controlPointsVisibility = true;
    this->order = order;
    this->geometry::setColor(R_SPLINE, G_SPLINE, B_SPLINE);
    this->controlLine = new lineStrip();
    this->controlLine->setColor(R_CONTROL, G_CONTROL, B_CONTROL);
}

/**
 * @brief Construtor para a b-Spline.
 * 
 * @param controlPoints : vetor dos pontos de controle da curva
 * @param order : ordem da b-Spline.
 */
bSpline::bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage /*= GL_DYNAMIC_DRAW*/): geometry(usage)
{
    this->isClamped = true;
    this->controlPointsVisibility = true;
    this->geometry::setColor(R_SPLINE, G_SPLINE, B_SPLINE);
    this->order = order;
    this->controlLine = new lineStrip();
    this->controlLine->setColor(R_CONTROL, G_CONTROL, B_CONTROL);
}


/**
 * @brief Destrutor da classe bSpline.
 * 
 */
bSpline::~bSpline()
{
    delete this->controlLine;
}

/**
 * @brief Desenha a b-Spline
 * 
 */
void bSpline::draw()
{
    if(this->controlPointsVisibility)
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
bool bSpline::collision(geometry* other)
{
    std::vector<GLfloat>::iterator it;

    //Verifica se há colisão entre um dos pontos de controle da outra curva e a outra curva.        
    for(it = this->controlPoints.begin(); it!= this->controlPoints.end(); it+=3)
    {
        if(other->collision(*it, *(it+1), *(it+2)))
			return true; //colisão detectada.
    }
 
    return false; // Não foi detectada colisão.
}

/**
 * @brief Seta a cor da b-Spline
 * 
 * @param r Valor de vermelho da nova cor 
 * @param g Valor de verde da nova cor
 * @param b Valor de azul da nova cor
 */
void bSpline::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    this->geometry::setColor(r,g,b);
    this->controlLine->setColor(r,g,b);
}

/**
 * @brief Reseta a cor da b-Spline 
 * 
 */
void bSpline::resetColor()
{
    this->geometry::setColor(R_SPLINE,G_SPLINE,B_SPLINE);
    this->controlLine->setColor(R_CONTROL,G_CONTROL,B_CONTROL);
}

/**
 * @brief Torna a spline clamped (Presa aos pontos inicial e final)
 * 
 */
void bSpline::makeClamped()
{
    if(!this->isClamped)
    {
        this->isClamped = true;
        this->vertices.clear();
        this->knots.clear();
        this->generate();
    }    
}

/**
 * @brief Torna a spline unclamped (Não presa aos pontos inicial e final)
 * 
 */
void bSpline::makeUnclamped()
{
    if(this->isClamped)
    {
        this->isClamped = false;
        this->vertices.clear();
        this->knots.clear();
        this->generate();
    }  
}

/**
 * @brief Seta a visibilidade dos pontos de controle da curva
 * 
 * @param isVisible se true: torna os pontos de controle visíveis, se false: torna os pontos de controle invisíveis
 */
void bSpline::setControlPointsVisibility(bool isVisible)
{
    this->controlPointsVisibility = isVisible;
}