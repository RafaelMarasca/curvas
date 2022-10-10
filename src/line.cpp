/**
 * @file line.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe line
 * @version 0.1
 * @date 2022-09
 * 
 */

#include "geometry.h"
#include "line.h"
#include "GL/glew.h"
#include<vector>


/**
 * @brief Construtor da classe line
 * 
 */
line::line(GLenum usage /*= GL_STATIC_DRAW*/):geometry(usage){}

/**
 * @brief Construtor para a classe line
 * 
 * @param vertices : vetor de vértices para a construção da linha
 * @param usage : enum definindo o tipo de desenho - por padrão GL_STATIC_DRAW
 */
line::line(std::vector<GLfloat> &vertices, GLenum usage /*= GL_STATIC_DRAW*/):geometry(vertices, usage){}

/**
 * @brief Método de desenho da classe line - Desenha ua linha definida na tela.
 * 
 */
void line::draw()
{
    geometry::program->use(); //Usa o programa
    glUniform3fv(this->colorLoc, 1, &(this->color[0])); //passa a cor da linha para o shader
    glBindVertexArray(this->VAO); //Seleciona o vertex array
    glDrawArrays(GL_LINES, 0, this->vertices.size()/3); //Desenha as linhas presentes nos buffers
}


bool line::collision(GLfloat x, GLfloat y, GLfloat z){} //Método de colisão para classe linha
bool line::collision(geometry* other){} //Método de colisão para a classe linha


/**
 * @brief Construtor para a classe lineStrip
 * 
 * @param usage Modo de desenho
 */
lineStrip::lineStrip(GLenum usage): line(usage){} 

/**
 * @brief Construtor para a classe lineStrip
 * 
 * @param vertex Vetor de vértices 
 * @param usage Modo de desenho
 */
lineStrip::lineStrip(std::vector<GLfloat> &vertex, GLenum usage ) : line(vertex,usage){}

/**
 * @brief Desenha a lineStrip
 * 
 */
void lineStrip::draw()
{
    geometry::program->use(); //Usa o programa
    glUniform3fv(this->colorLoc, 1, &(this->color[0])); //passa a cor da linha para o shader
    glBindVertexArray(this->VAO); //Seleciona o vertex array
    glDrawArrays(GL_LINE_STRIP, 0, this->vertices.size()/3); //Desenha as linhas presentes nos buffers
    glDrawArrays(GL_POINTS, 0, this->vertices.size()/3);
}
