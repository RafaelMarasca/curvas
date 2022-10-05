/**
 * @file matrix.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe matrix
 * @version 0.1
 * @date 2022-09
 * 
 */

#include "matrix.h"
#include<vector>
#include<string>
#include<iostream>

/**
 * @brief Construtor para a classe matrix
 * 
 */
matrix::matrix()
{
   this->rowNumber = 0;
   this->colNumber = 0;
}

/**
 * @brief Constrói uma matriz de rowNumber x colNumber inicializada em 0.
 * 
 * @param rowNumber : número de linhas da matrix
 * @param colNumber : número de colunas da matriz
 */
matrix::matrix(unsigned int rowNumber, unsigned int colNumber) 
{
	this->rowNumber = rowNumber;
	this->colNumber = colNumber;
    this->matrixElements = std::vector<GLfloat>(rowNumber*colNumber, 0);
}

/**
 * @brief Construtor de cópia - Gera uma matriz com os mesmos elementos de m
 * 
 * @param m : matriz a ser copiada
 */
matrix::matrix(const matrix& m) 
{
	this->rowNumber = m.getRowNumber();
	this->colNumber = m.getColNumber();
    this->matrixElements = m.matrixElements;
}


/**
 * @brief Sobrecarga do operador de multiplicação.
 * 
 * @param m : matriz da direita a ser multiplicada
 * @return matrix : matriz resultante da multiplicação.
 */
matrix matrix::operator*(matrix& m)
{
    if(m.getRowNumber()!= this->colNumber)
        throw std::string("Matrizes incompativeis.");


    matrix result(this->rowNumber, m.getColNumber());

    for(unsigned int i = 0; i<this->rowNumber; i++)
    {
        for(unsigned int j = 0; j<m.getColNumber(); j++)
        {
            for(unsigned int k = 0; k < this->colNumber; k++)
            {
                result(i,j) += this->matrixElements[i*this->colNumber+k]*m(k,j);
            }
        }
        
    }
    return result;
}

/**
 * @brief Sobrecarga do operador =
 * 
 * @param m : Uma nova matriz igual a m.
 */
void matrix::operator= (const matrix &m) 
{ 
	this->matrixElements = m.matrixElements; 
	this->rowNumber = m.rowNumber;
	this->colNumber = m.colNumber;
}
        
/**
 * @brief Sobrecarga do operador função - acessa o elemento da linha row e coluna col da matriz
 * 
 * @param row : número da linha a ser acessada
 * @param col : número da coluna a ser acessada
 * @return * GLfloat& 
 */
GLfloat& matrix::operator()(unsigned int row, unsigned int col)
{
    return this->matrixElements[row*this->colNumber + col];
}

/**
 * @brief Sobrecarga do operador de cast para GLfloat*
 * 
 * @return GLfloat* : vetor de floats contendo os elementos da matriz
 */
matrix::operator GLfloat*() 
{
    return &(this->matrixElements[0]);
}

/**
 * @brief Sobrecarga do operador de cast para std::vector<GLfloat>
 * 
 * @return std::vector<GLfloat> : vetor de floats contendo os elementos da matriz 
 */
matrix::operator std::vector<GLfloat>()
{
    return this->matrixElements;
}

/**
 * @brief Getter para o número de colunas da matriz
 * 
 * @return unsigned int : número de colunas da matriz
 */
unsigned int matrix::getColNumber() const
{
    return this->colNumber;
}

/**
 * @brief Getter para o número de linhas da matriz
 * 
 * @return unsigned int : número de linhas da matriz
 */
unsigned int matrix::getRowNumber() const
{
    return this->rowNumber;
}

/**
 * @brief método estático que retorna uma matriz identidade de ordem order
 * 
 * @param order : ordem da matriz
 * @return matrix matriz identidade de ordem order
 */
matrix matrix::eye(unsigned int order)
{
    matrix temp = matrix(order, order);
    for(unsigned int i = 0; i<order; i++)
        temp(i,i) = 1;

    return temp;
}