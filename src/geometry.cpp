/**
 * @file geometry.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe Geometry - Define uma forma geométrica.
 * @version 0.1
 * @date 2022-09
 * 
 */


#define _USE_MATH_DEFINES 

#include"geometry.h"
#include"shaderProgram.h"
#include"matrix.h"
#include<vector>
#include<string>
#include<iostream>
#include<cmath>
#include<limits>


//Declaração das strings contendo os códigos fonte dos shaders
extern const std::string vertexShaderSource;
extern const std::string fragmentShaderSource;

//Inicialização da constante estática de classe responsável 
//por contar a quantidade de formas na tela
unsigned int geometry::geometryCount = 0;

//Inicialização do ponteiro estático que aponta para o shader gerado
shaderProgram* geometry::program = nullptr;


/**
 * @brief Construtor para a classe geometry.
 * 
 * @param usage : Enum que determina o tipo de desenho a ser feito na tela.
 * Por padrão o valor é GL_STATIC_DRAW.
 * 
 */
geometry::geometry(GLenum usage /*= GL_STATIC_DRAW*/)
{
    //Inicializa a cor da forma.
    this->color = std::vector<GLfloat>{GEOMETRY_R, GEOMETRY_G, GEOMETRY_B}; 
    this->vertexNum = 0; //Inicializa o número de vértices em 0.
    this->usage = usage; //Inicializa o tipo de uso do desenho.


    //Verifica se já não há um shaderProgram instanciado, caso não haja, instancia.
    if(geometry::program == nullptr)
        geometry::program = new shaderProgram(vertexShaderSource, fragmentShaderSource);
   
    this->setup();  //Chama a Função de inicialização.

    geometry::geometryCount++; //Incrementa o contador de formas.
}



/**
 * @brief Construtor para a classe gometry. 
 * 
 * @param vertices : vetor contendo os vértices da forma geométrica.
 * @param usage : Enum que determina o tipo de desenho a ser feito na tela.
 * Por padrão o valor é GL_STATIC_DRAW.
 * 
 */
geometry::geometry(std::vector<GLfloat> &vertices, GLenum usage /*= GL_STATIC_DRAW*/)
{
    //Inicializa a cor da forma
    this->color = std::vector<GLfloat>{GEOMETRY_R,GEOMETRY_G,GEOMETRY_B};
    this->vertexNum = 0; //Inicializa o número de vértices em 0
    this->usage = usage; //Inicializa o tipo de uso do desenho.
    this->vertices = vertices; //Inicializa o vetor de vértices da forma geométrica.

    //Verifica se já não há um shaderProgram instanciado, caso não haja, instancia.
    if(geometry::program == nullptr)
        geometry::program = new shaderProgram(vertexShaderSource, fragmentShaderSource);

    this->setup();//Chama a Função de inicialização.

    geometry::geometryCount++; //Incrementa o contador de formas.
}

/**
 * @brief Destrutor para a classe geometry.
 * 
 */
geometry::~geometry()
{
    geometry::geometryCount--; //Decrementa o contador de formas.

    //Se não há mais formas no programa, desaloca a memória para o programa shader.
    if(geometry::geometryCount == 0 && geometry::program != nullptr)
    {
        delete geometry::program; //deleta o programa
        geometry::program = nullptr; //Faz o ponteiro para o shaderProgram apontar para null.
    }
    
    //Libera a memória associadas aos buffers criados para a forma corrente.
    glDeleteBuffers(1,&(this->VBO));
    glDeleteVertexArrays(1,&(this->VAO));

}


/**
 * @brief Adiciona vértices ao objeto de geometry.
 * 
 * @param vertex : vetor de vértices a serem adicionados.
 */
void geometry::addVertex(std::vector<GLfloat> &vertex)
{
    //Verifica se há 3 coordenadas em cada ponto
    if(vertex.size()%3 != 0)
        throw std::string("Número inválido de vértices e/ou pontos.");
        
    this->vertexNum += vertex.size()/3; //Incrementa o número de vértices da forma
    (this->vertices).insert((this->vertices).end(), vertex.begin(), vertex.end()); //Insere os vértices
    
    //Tranfere os novos dados aos buffers de desenho.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
    glBufferData(GL_ARRAY_BUFFER, (this->vertices).size()*sizeof(GLfloat),&(this->vertices[0]), this->usage);
}

/**
 * @brief Inicializa so buffers e atributos da forma.
 * 
 */
void geometry::setup()
{
    //Gera os buffers de objeto e array de vértices.
    glGenBuffers(1, &(this->VBO));
    glGenVertexArrays(1,&(this->VAO));

    //Seleciona o array de vértices da forma corrente.
    glBindVertexArray(this->VAO);

    //Transfere os dados para o buffer de objetos.
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices).size()*sizeof(GLfloat),&(this->vertices[0]), usage);

    //Aponta os atributos de vértice.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);

    //Inicia o uso do programa de shader e seta a cor inicial da forma.
    this->program->use();
    this->colorLoc = this->program->getColorLoc();
    glUniformMatrix4fv(this->colorLoc, 1, GL_TRUE, &(this->color[0]));
}


/**
 * @brief Recebe um ponto (x,y,z) e verifica se há colisão entre a forma e o ponto.
 * 
 * @param x : coordenada x do ponto.
 * @param y : coordenada y do ponto.
 * @param z : coordenada z do ponto.
 * @return true : Se há uma colisão entre o ponto x,y,z e a forma.
 * @return false : Caso não haja colisão entre o ponto x,y,z e a forma.
 */
bool geometry::collision(GLfloat x, GLfloat y, GLfloat z)
{
    std::vector<GLfloat>::iterator it; //Iterator para um vector de GLfloat
    int i = 1; //Variável auxiliar

    //Inicializa o vetor que representa os valores mínimos e máximos das coordenadas x e y da forma
    std::vector<GLfloat> min_max={std::numeric_limits<float>::max(),std::numeric_limits<float>::lowest(),
                                 std::numeric_limits<float>::max(),std::numeric_limits<float>::lowest()};
                                 
    //Verifica se há colisão entre o ponto e os vértices                                 
    for(it = this->vertices.begin(); it!= this->vertices.end(); it+=3)
    {
       if((std::abs(*it - x) < 0.0000001f ) && (std::abs(*(it+1) - y) < 0.000001f ))
        return true;
    }
                             
    //Obtém os pontos mínimos e máximos das coordenadas x e y e armazena no vetor min_max
    for(it = this->vertices.begin(); it!= this->vertices.end(); it++)
    {
        if(i%3 == 1)
        {
            if(*it<min_max[0])
                min_max[0] = *it;
            if(*it>min_max[1])
                min_max[1] = *it;
        }else if(i%3 == 2)
        {
            if(*it<min_max[2])
                min_max[2] = *it;
            if(*it>min_max[3])
                min_max[3] = *it;
        }   
        i++;
    }

    //verifica se o ponto se encontra dentro dos limites do vetor min_max
    if(x>min_max[0] && x<min_max[1] && y>min_max[2] && y<min_max[3])
    {
        //Sim, o ponto se encontra dentro dos limites(colisão).
        return true;
    }else{
        //Não o ponto não se encontra dentro dos limites (i.e. não há colisão).
        return false;
    }   
}

/**
 * @brief Verifica colisão entre duas formas
 * 
 * @param other : a forma a se verificar colisão.
 * @return true : Caso haja colisão entre as formas.
 * @return false : Caso não haja colisão entre as formas.
 */
bool geometry::collision(geometry* other)
{
    std::vector<GLfloat>::iterator it;

    //Verifica se há colisão entre um dos vértices do outro objeto e a forma atual.        
    for(it = other->vertices.begin(); it!= other->vertices.end(); it+=3)
    {

        if(this->collision(*it, *(it+1), *(it+2)))
			return true; //colisão detectada.
    }
 
    return false; // Não foi detectada colisão.
}


/**
 * @brief Setter para a cor do objeto
 * 
 * @param r : valor de RED da nova cor
 * @param g : valor de GREEN da nova cor
 * @param b : valor de BLUE da nova cor
 */
void geometry::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
}
