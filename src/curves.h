/**
 * @file curves.h
 * @author Rafael Marasca Martins e Lucas Cavalho
 * @brief Declaração da classe B-Spline.
 * @version 0.1
 * @date 2022-10
 */


#ifndef CURVES_H
#define CURVES_H

#include<GL/glew.h>
#include"geometry.h"
#include"line.h"


/**
 * @brief Resolução da curva
 */
#define RESOLUTION 0.01f


/**
 * @brief Cor da Spline.
 */
#define R_SPLINE 0.9529f
#define G_SPLINE 0.0863f
#define B_SPLINE 0.0588f


/**
 * @brief Cor dos Pontos de Controle.
 */
#define R_CONTROL 0.0196f 
#define G_CONTROL 0.7412f
#define B_CONTROL 0.0078f


float genAlgo(std::vector<GLfloat>& knots, int i, int j, float t); //Declaração da função de geração dos nós.


/**
 * @brief Classe b-Spline - Implementa a b-Spline
 * 
 */
class bSpline : public geometry
{
    private:
        int order; //Ordem da curva.
        
        std::vector<GLfloat> knots; //Vetor de nós da curva.
        std::vector<GLfloat> controlPoints; //Vetor de pontos de controle da curva.

        lineStrip* controlLine; //LInha dos pontos de controle.

        bool isClamped; //Variável que armazena o estado da curva (clamped/unclamped).
        bool controlPointsVisibility; //Variável que armazena a visibilidade da curva.

    public:

        bSpline(int order, GLenum usage = GL_STATIC_DRAW); //Construtor para b-Spline.
        bSpline(std::vector<GLfloat>& controlPoints, int order, GLenum usage = GL_STATIC_DRAW); //Construtor para b-Spline.
        ~bSpline(); //Destrutor para b-Spline.

        bool collision(geometry* other); //Verifica colisão com outra geometria
        bool collision(GLfloat x, GLfloat y, GLfloat z); //Verifica colisão com um ponto

        void addControlPoint(std::vector<GLfloat>& newControlPoint); //Adiciona um ponto de controle
        void addControlPoint(GLfloat x, GLfloat y, GLfloat z); //Adiciona um ponto de controle
        void generate(); //Gera a curva
        void setColor(GLfloat r, GLfloat g, GLfloat b); //Seta a cor da curva
        void resetColor(); //Reseta a cor da curva
        void draw(); //Desenha a curva
        void makeClamped(); //Prende a curva aos pontos final e inicial
        void makeUnclamped(); //Desprende a curva dos pontos final e inicial
        void setControlPointsVisibility(bool isVisible); //Seta a visibilidade da curva.
};

#endif //CURVES_H