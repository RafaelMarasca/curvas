/**
 * @file polygon.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Declaração da classe polygon
 * @version 0.1
 * @date 2022-09
 * 
 */


#ifndef POLYGON_H
#define POLYGON_H

#include"shaderProgram.h"
#include"geometry.h"

#include<GL/glew.h>
#include<cstddef>
#include<vector>
#include<string>

/**
 * @brief Classe polygon - Representa a abstração de um polígono
 * 
 */
class polygon : public geometry
{
    public:
        polygon(GLenum usage = GL_DYNAMIC_DRAW); //construtor para a classe polygon
        polygon(std::vector<GLfloat> &vertex, GLenum usage = GL_DYNAMIC_DRAW); //construtor para a classe polygon
        polygon(std::vector<GLfloat> &vertex, std::vector<GLfloat> &center, GLenum usage = GL_DYNAMIC_DRAW); //construtor para a classe polygon
        ~polygon(); //Destrutor para a classe polygon
        
        void draw(); //Método de desenho para o polígono      
};

#endif //POLYGON_H
