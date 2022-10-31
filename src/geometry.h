/**
 * @file geometry.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Declaração da classe geometry e as constantes utilizados pela classe.
 * @version 0.1
 * @date 2022-09
 * 
 */


#ifndef GEOMETRY_H
#define GEOMETRY_H

#include"shaderProgram.h"
#include<GL/glew.h>
#include<cstddef>
#include<vector>
#include<string>


// Define o valor de RED da cor inicial da forma.
#define GEOMETRY_R 0.329f
// Define o valor de GREEN da cor inicial da forma.
#define GEOMETRY_G 0.341f
// Define o valor de BLUE da cor inicial da forma.
#define GEOMETRY_B 0.486f 

/**
 * @brief Enum que define os planos possíveis do universo
 * 
 */
typedef enum{YZ_PLANE = 0,  XZ_PLANE,  XY_PLANE}enumPlane;

/**
 * @brief Código fonto para o o shader de vértices
 * 
 */
const std::string vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main(){\n"
        "gl_Position = vec4(pos*0.9999f, 1.0f);\n"
        "}\n";

/**
 * @brief Código fonte para o shader de fragmentos.
 * 
 */
const std::string fragmentShaderSource = 
        "#version 330 core\n"
        "uniform vec3 color;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "FragColor = vec4(color, 0.2f);\n"
        "}\n";


/**
 * @brief Classe geometry - Define a abstração de uma forma geométrica.
 * 
 */
class geometry
{
    private:
        static unsigned int geometryCount; //Contador de formas instanciadas
        void setup(); //Função de inicialização

    protected:
        static shaderProgram* program; //POnteiro para o shaderProgram

        std::vector<GLfloat> centralPoint; //Ponto central da forma
        std::vector<GLfloat> vertices; //Vetor de vértices da forma
        GLuint VBO; //Vertex Buffer Object
        GLuint VAO; //Vertex Array Object
        GLenum usage; //Modo de uso da forma
        GLuint colorLoc; //Localização da variável de cor
        std::vector<GLfloat> color; //Vetor que define a cor da forma
        unsigned int vertexNum; //contador de Vértices.

    public:
        geometry(GLenum usage = GL_STATIC_DRAW); //Construtor para geometry
        geometry(std::vector<GLfloat> &vertices, GLenum usage = GL_STATIC_DRAW); //Construtor para geometry
        virtual ~geometry(); //Destrutor para geometry

        void addVertex(std::vector<GLfloat> &vertex); //Insere vértices na forma.
        
        virtual void setColor(GLfloat r, GLfloat g, GLfloat b); //Setter para a cor do objeto.
        virtual void resetColor();


        virtual bool collision(GLfloat x, GLfloat y, GLfloat z) = 0; //Detecta colisão entre a forma e um ponto.
        virtual bool collision(geometry* ) = 0; //Detecta colisão entre duas formas.
        
        void print();


        virtual void draw() = 0; //Método virtual puro para desenho da forma.
};


/**
 * @brief Classe square - Define a abstração de um quadrado
 * 
 */
class square : public geometry
{

    public:

        square(GLenum usage = GL_STATIC_DRAW); //Construtor para a classe square
        square(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos); //Construtor para a classe square

        void draw();

        bool collision(GLfloat x, GLfloat y, GLfloat z); //Detecta colisão entre o quadrado e um ponto.
        bool collision(geometry* other); //Detecta a colisão entre o quadrado e outra geometria.

};


#endif //GEOMETRY_H
