/**
 * @file window.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Declaração da classe window
 * @version 0.1
 * @date 2022-09
 * 
 */


#ifndef WINDOW_H
#define WINDOW_H

#include<GL/glew.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include"scene.h"
#include"geometry.h"
#include<atomic>
#include<string>
#include<vector>
#include<utility>

/**
 * @brief Classe window - Define uma abstração para uma janela de desenho.
 * 
 */
class window
{
    private:
        scene* vision; //Ponteiro para a cena da janela.
        geometry* selectedShape; //Ponteiro para a forma selecionada.

        int waitingMouse; //Flag de espera do mouse.
        int width,height; //Altura e largura da tela.
        
        unsigned int selectedShapeID; //ID da forma selecionada

        std::vector<GLfloat> mouseQueue; //Fila de cliques do mouse
        std::vector<std::string> buffer; //Buffer para as entradas do usuário
        
        GLfloat aspectRatio; //Razão de aspecto da tela.

    public:
        //Construtor para a classe window
        window(int width, int height, const char* title, int* argc, char** argv);
         
        ~window(); //Destrutor para a classe window.

        void input(); //Método de entrada de dados
        void init(); //Método de inicialização do tela
        void updateScene(); //Método de atualização da cena exibida na tela.
        void deleteShape(); //Meétodo para deletar uma forma
        void select(std::pair<unsigned int, geometry*>); //Método para selecionar uma forma

        static void redisplay(); //Método para requerer o redesenho da tela.
        static void draw(); //Método para desenhar os objetos da cena.

        static void mouseClick(int button, int state,int x, int y); //Método chamado ao clicar na tela
        static void keyp(unsigned char, int x, int y); //Método chamado ao pressionar uma tecla
        static void resize(int w, int h); //Método chamado ao redimensionar a tela
};


#endif //WINDOW_H
