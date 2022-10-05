/**
 * @file axis.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe axis
 * @version 0.1
 * @date 2022-09
 * 
 */

#include "axis.h"
#include<GL/glew.h>
#include<vector>

/**
 * @brief Construtor da classe line
 * 
 */
axis::axis(): line(GL_STATIC_DRAW)
{
  //Vetor de ticks para o eixo vertical
  GLfloat vertical[6] = {-AXIS_TICK_SIZE, 0.0f, 0.0f,
                          AXIS_TICK_SIZE, 0.0f, 0.0f};
  //Vetor de ticks para o eixo horizontal
  GLfloat horizontal[6] = {0.0f, -AXIS_TICK_SIZE, 0.0f,
                           0.0f,  AXIS_TICK_SIZE, 0.0f};

  std::vector<GLfloat> vertices;

  //Define o desenho dos eixos
  GLfloat lines[] = {-AXIS_TICK_SPACING*10, 0.0f, 0.0f, AXIS_TICK_SPACING*10, 0.0f, 0.0f, 0.0f, -AXIS_TICK_SPACING*10, 0.0f, 0.0f, AXIS_TICK_SPACING*10, 0.0f};

  vertices.insert(vertices.end(), std::begin(lines), std::end(lines));

  /**
   * @brief Desenha os ticks nos eixos.
   * 
   */
  for(int i = 1; i<=10; i++)
  {

    GLfloat pos = i*AXIS_TICK_SPACING;

    vertical[1] = vertical[4] = horizontal[0] = horizontal[3] = pos;
    vertices.insert(vertices.end(), std::begin(horizontal), std::end(horizontal));
    vertices.insert(vertices.end(), std::begin(vertical), std::end(vertical));
    vertical[1] = vertical[4] = horizontal[0] = horizontal[3] = -pos;
    vertices.insert(vertices.end(), std::begin(horizontal), std::end(horizontal));
    vertices.insert(vertices.end(), std::begin(vertical), std::end(vertical));
  }
  this->addVertex(vertices);

}

/**
 * @brief Destrutor da classe axis.
 * 
 */
axis::~axis(){}
