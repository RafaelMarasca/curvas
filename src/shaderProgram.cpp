/**
 * @file shaderProgram.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe shaderProgram
 * @version 0.1
 * @date 2022-09
 * 
 */


#include"shaderProgram.h"
#include<GL/glew.h>
#include<iostream>
#include<string>


/**
 * @brief Construtor para a classe shaderProgram
 * 
 * @param vertexShaderSource : código fonte para o programa de vértices
 * @param fragmentShaderSource : código fonte para o programa de fragmentos
 */
shaderProgram::shaderProgram(const std::string vertexShaderSource, const std::string fragmentShaderSource)
{
    this->shaderProgramId = glCreateProgram(); //Cria um programa GLSL
    this->compile(vertexShaderSource.c_str(), fragmentShaderSource.c_str()); //Compila o programa GLSL
    this->transformLoc = glGetUniformLocation(this->shaderProgramId, "transform"); //Localiza a matriz de transformação no programa gerado
    this->colorLoc = glGetUniformLocation(this->shaderProgramId,"color"); //Localiza o vetor de cor no programa gerado
}

/**
 * @brief Compila o programa shader
 * 
 * @param vertexShaderSource : código fonte para o programa de vértices
 * @param fragmentShaderSource : código fonte para o programa de fragmentos
 */
void shaderProgram::compile(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    int compilationSuccess;
    char compilationLog[512];
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationSuccess);

    if(!compilationSuccess)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, compilationLog);
        std::cerr << "VERTEX SHADER COMPILATION ERROR: " << compilationLog << std::endl;
        throw "VERTEX SHADER COMPILATION ERROR";
    }

    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationSuccess);

    if(!compilationSuccess)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, compilationLog);
        std::cerr << "FRAGMENT SHADER COMPILATION ERROR: " << compilationLog << std::endl;
        throw "FRAGMENT SHADER COMPILATION ERROR";
    }

    glAttachShader(this->shaderProgramId, vertexShader);
    glAttachShader(this->shaderProgramId, fragmentShader);

    glLinkProgram(this->shaderProgramId);

    glGetProgramiv(this->shaderProgramId, GL_LINK_STATUS, &compilationSuccess);

    if(!compilationSuccess)
    {
        glGetProgramInfoLog(this->shaderProgramId, 512, NULL, compilationLog);
        std::cerr << "SHADER PROGRAM LINKING ERROR: " << compilationLog << std::endl;
        throw "SHADER PROGRAM LINKING ERROR";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

/**
 * @brief Destrutor do shaderProgram
 * 
 */
shaderProgram::~shaderProgram()
{
    if(!this->shaderProgramId)
        glDeleteProgram(this->shaderProgramId);
}

/**
 * @brief sinaliza ao opengl para usar o programa compilado 
 * 
 */
void shaderProgram::use()
{
    glUseProgram(this->shaderProgramId);
} 

/**
 * @brief Getter para a localização da matriz de transformação 
 * 
 * @return GLuint : localização da matriz de transformação
 */
GLuint shaderProgram::getTransformLoc()
{
    return this->transformLoc;
}

/**
 * @brief Getter para a localização do vetor de cor
 * 
 * @return GLuint : localização do vetor de cor
 */
GLuint shaderProgram::getColorLoc()
{
    return this->colorLoc;
}