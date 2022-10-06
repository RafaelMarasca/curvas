/**
 * @file axis.h
 * @author Rafael Marasca Martins
 * @brief Declaração da classe axis e das constantes utilizadas
 * @version 0.1
 * @date 2022-09
 * 
 */

#ifndef AXIS_H
#define AXIS_H


#include "line.h"
#include<GL/glew.h>

#define AXIS_TICK_SIZE 0.025f //Tamanho do tick
#define AXIS_TICK_SPACING 0.1f //Espaçamento entre ticks

/**
 * @brief Classe axis - Define uma abstração para os eixos xy
 * 
 */
class axis : public line
{
  
  public:
    axis(); //Construtor da classe axis
    ~axis(); //Destrutor da classe axis
};



#endif //AXIS_H
