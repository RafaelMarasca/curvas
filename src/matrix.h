/**
 * @file matrix.h
 * @author Rafael Marasca Martins
 * @brief Declaração da classe Matrix
 * @version 0.1
 * @date 2022-09
 * 
 */

#ifndef MATRIX_H
#define MATRIX_H

#include<GL/glew.h>
#include<vector>

/**
 * @brief Classe matrix - Define a abstração de uma matriz
 * 
 */
class matrix
{
    protected:

        std::vector<GLfloat> matrixElements; //Vetor definindo os elementos da matriz

        unsigned int colNumber; //Número de colunas da matriz
        unsigned int rowNumber; //Número de linhas da matriz

    public:

        static matrix eye(unsigned int order); //Função estática que retorna uma matriz identidade

        matrix(unsigned rowNumber, unsigned colNumber); //Construtor para a classe matrix
        matrix(const matrix& m); //Construtor de cópia para a classe matrix
        matrix(); //Construtor para a classe matrix
        
        matrix operator*(matrix& m); //Sobrecarga do operador *
        void operator= (const matrix &m1); //Sobrecarga do operador =
		GLfloat& operator() (unsigned int row, unsigned int col); //Sobrecarga do operador função 
        operator GLfloat*(); //Sobrecarga do operador de cast  para GLfloat*
        operator std::vector<GLfloat>(); //Sobrecarga do operador de cast para std::vector<GLfloat>


        unsigned int getColNumber() const; //Getter para o número de colunas da matriz
        unsigned int getRowNumber() const; //Getter para o número de linhas da matriz
};

#endif //MATRIX_H
