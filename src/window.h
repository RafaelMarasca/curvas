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
#include<map>
#include<utility>
#include"gui.h"


#define SELECTION_R 0.6316f
#define SELECTION_G 0.0549f
#define SELECTION_B 0.7882f

#define MENU_DEFAULT_R 0.1882f
#define MENU_DEFAULT_G 0.0901f
#define MENU_DEFAULT_B 0.3411f

#define MENU1 "[1] NOVA B-SPLINE "
#define MENU2 "[1] MOUSE [2] TECLADO"
#define MENU3 "[1] CLAMPED [2] UNCLAMPED [3] MOSTRAR PONTOS DE CONTROLE [4] ESCONDER PONTOS DE CONTROLE"
#define MENU4 "ORDEM DA B-SPLINE: "
#define MENU5 "QUANTIDADE DE PONTOS DA B-SPLINE: "
#define MENU6 "PONTOS: "

class selectionBox:public square
{
    public:
        void draw();
        void clearVertex();
        void addStart(GLfloat x, GLfloat y);
        void updateLength(GLfloat x, GLfloat y);
};
/*
class menu
{
    private:
        std::string text;
        GLfloat xPos,yPos;
        std::vector<GLfloat> color;
    
    public:
        menu(GLfloat xPos, GLfloat yPos);
        void mouseAction(int button, int xPos, int yPos);
        void keyAction(unsigned char key);
        void setText(std::string str);
        void addChar(char);
        void deleteChar();
        void draw();
        void setColor(GLfloat r, GLfloat g, GLfloat b);
};*/

/**
 * @brief Classe window - Define uma abstração para uma janela de desenho.
 * 
 */
class window
{
    private:
        scene* vision; //Ponteiro para a cena da janela.
        geometry* selectedShape; //Ponteiro para a forma selecionada.
        selectionBox* selBox;

        int waitingMouse; //Flag de espera do mouse.
        int width,height; //Altura e largura da tela.
        
        //menu* console;
        
        std::map<int,frame*> menu;
        int currentMenu;

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
        void clearSelection();

        static void redisplay(); //Método para requerer o redesenho da tela.
        static void draw(); //Método para desenhar os objetos da cena.

        static void mouseClick(int button, int state,int x, int y); //Método chamado ao clicar na tela
        static void keyp(unsigned char, int x, int y); //Método chamado ao pressionar uma tecla
        static void resize(int w, int h); //Método chamado ao redimensionar a tela
        static void mouseMove(int x, int y);
        static void menuClick(int i);
        //static void createMenu();
        //static void splineManagement(int option);
};





#endif //WINDOW_H
