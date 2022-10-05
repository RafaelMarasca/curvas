/**
 * @file polygon.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe polygon
 * @version 0.1
 * @date 2022-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "polygon.h"
#include "shaderProgram.h"
#include "GL/glew.h"
#include<vector>
#include<string>
#include<iostream>

/**
 * @brief Construtor para a classe polygon
 * 
 * @param GLenum usage : Modo de desenho do polígono
 */
polygon::polygon(GLenum usage /*= GL_DYNAMIC_DRAW*/):geometry(usage){}

/**
 * @brief Construtor para a classe polygon
 * 
 * @param vertices : vetor de vértices do polígono sendo definido  
 * @param GLenum usage : Modo de desenho do polígono
 */
polygon::polygon(std::vector<GLfloat> &vertices, GLenum usage/*= GL_DYNAMIC_DRAW*/):geometry(vertices, usage){}


/**
 * @brief Destrutor da classe polígono
 * 
 */
polygon::~polygon(){}

/**
 * @brief Método de desenho da classe polygon - Desenha o polígono definido poligono na tela.
 * 
 */
void polygon::draw()
{
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size()/3);
}

