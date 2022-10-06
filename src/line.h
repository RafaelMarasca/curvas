/**
 * @file line.h
 * @author Rafael Marasca Martins
 * @brief Declaração da classe line
 * @version 0.1
 * @date 2022-09-26
 * 
 */

#ifndef LINE_H
#define LINE_H

#include "geometry.h"
#include <GL/glew.h>
#include <vector>

/**
 * @brief Classe line - Define uma abstração para uma linha
 * 
 */
class line : public geometry
{
    private:
        bool collision(GLfloat x, GLfloat y, GLfloat z); //Detecta colisão entre a forma e um ponto.
        bool collision(geometry* other);

    public:
        line(GLenum usage = GL_STATIC_DRAW); //Construtor para a classe line
        line(std::vector<GLfloat> &vertex, GLenum usage = GL_STATIC_DRAW); //Construtor para a classe line
        void draw(); //Método de desenho da linha
};



/**
 * @brief Classe lineStrip - Define uma abstração para uma connexão entre pontos sucessivos
 * 
 */
class lineStrip: public line
{
    public:
        lineStrip(GLenum usage = GL_DYNAMIC_DRAW); //Construtor para a classe line
        lineStrip(std::vector<GLfloat> &vertex, GLenum usage = GL_DYNAMIC_DRAW); //Construtor para a classe line
        void draw(); //Método de desenho da lineStrip

};


#endif //LINE_H