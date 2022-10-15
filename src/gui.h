/**
 * @file gui.h
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Definição das classes necessárias para o sistema de gui.
 * @version 0.1
 * @date 2022-10
 */


#ifndef GUI_H
#define GUI_H

#include"geometry.h"
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<GL/glew.h>
#include<vector>
#include<string>
#include<map>


/**
 * @brief Cores do corpo do menu.
 */
#define FRAME_R 0.2156f
#define FRAME_G 0.1451f
#define FRAME_B 0.3333f

/**
 * @brief Cores Padrão do botão (Amarelo).
 */
#define BUTTON_R 0.9529f
#define BUTTON_G 0.7922f
#define BUTTON_B 0.2510f

/**
 * @brief Cores Padrão do botão (vermelho).
 */
#define BUTTON_RED_R 0.8000f
#define BUTTON_RED_G 0.1568f
#define BUTTON_RED_B 0.2117f

/**
 * @brief Cores Padrão do botão toggle (ON).
 */
#define TOGGLE_ON_R 0.5921f
#define TOGGLE_ON_G 1.0f
#define TOGGLE_ON_B 0.5647f

/**
 * @brief Cores Padrão do botão toggle (OFF).
 */
#define TOGGLE_OFF_R 0.8549f
#define TOGGLE_OFF_G 0.2667f
#define TOGGLE_OFF_B 0.3137f

/**
 * @brief Cores Padrão do texto.
 */
#define TEXT_R 1.0f
#define TEXT_G 1.0f
#define TEXT_B 1.0f

typedef void (*functionPtr) (int); //Definição do tipo ponteiro para função 

/**
 * @brief classe guiElement - Representa um elemento de gui.
 * 
 */
class guiElement : public square
{
    protected:
        int vSize; //Tamanho vertical do elemento em células.
        int hSize; //Tamanho horizontal do elemento em células.
        int ID; //ID do elemento.
        
        GLfloat width; //Tamanho horizontal do elemento em coordenadas normalizadas.
        GLfloat height; //Tamanho Vertical do elemento em coordenadas normalizadas.
        GLfloat xPos; //Posição x do canto superior esquerdo do elemento.
        GLfloat yPos; //Posição y do canto superior esquerdo do elemento.
        
        bool clicked; //Flag de clicado do elemento.

    public:
        guiElement(int verticalSize, int horizontalSize, int ID); //Construtor da classe de guiElement.
        guiElement(GLfloat height, GLfloat width, GLfloat xPos, GLfloat yPos); //Construtor da classe guiElement.

        int getHSize(); //Retorna o tamanho horizontal do elemento em células.
        int getVSize(); //Retorna o tamanho vertical do elemento em células.
        int getID(); //Retorna o ID do elemento.

        virtual void onClick(void (*) (int)); //Função chamada quando o elemento é clicado.
        virtual void onKeyPress(int); //Função chamada quando uma tecla é pressionada.
        virtual void generate(GLfloat height, GLfloat width, GLfloat xpos, GLfloat yPos); //Gera o elemento de gui.
        virtual void setClicked(bool state); //Seta a flag de clicado.
        virtual void draw(); //Desenha o elemento de gui;
        virtual void clear(); //Limpa os dados do elemento de gui.
};


class frame : public guiElement
{
    private:
        int numHorizontalElements; //Número de células na horizontal
        int numVerticalElements; //Número de células na vertical
        
        GLfloat elementHeight; //Altura dos elementos 
        GLfloat elementWidth; //Largura dos elementos
        GLfloat vGap; //Gap vertical entre elementos
        GLfloat hGap; //Gap horizonatl entre elementos
        GLfloat gap; //porcentagem de gap.
        
        functionPtr clickFunction; //Função chamada ao clicar em um elemento do frame
        
        bool isVisible; //Visibilidade do frame

        std::vector<std::vector<guiElement*>> elements; //Matriz de elementos do frame
 
        void addElement(int vpos, int hpos, guiElement* element); //Adiciona um elemento ao frame

    public:
        //Construtor da classe frame
        frame(GLfloat height, GLfloat width, int numVertical, int numHorizontal, 
                GLfloat xPos, GLfloat yPos, int ID = -1, float gap = 0.05f); 
        virtual ~frame(); //Destrutor da classe frame

        void addElement(int vPos, int hPos, int vSize, int hSize, int ID); //Adiciona um elemento ao frame
        void show(); //Torna o frame visível
        void draw(); //Desenha o frame
        void hide(); //Torna o frame invisível
        void generate(); //Gera o frame
        void addButton(int vpos, int hpos, int vsize,int hsize, int ID, const char* text); //Adiciona um botão ao frame
        void addButton(int vpos, int hpos, int vsize,int hsize, int ID, const char* text, std::vector<GLfloat>color); //Adiciona um botão ao frame 
        void addToggleButton(int vpos, int hpos, int vsize,int hsize, int ID, bool state); //Adiciona um toggle ao frame
        void addText(int vpos, int hpos, int vsize,int hsize, int ID, const char* text); //Adiciona Texto ao frame
        void addTextInput(int vpos, int hpos, int vsize,int hsize, int ID); //Adiciona uma caixa de entrada de texto ao frame
        void addClickFunction(functionPtr fun); //Adiciona a função de clique ao frame
        void clear(); //Limpa os dados dos elementos do frame
        void keyPress(int key); //Adiciona a função de pressionamento de tecla ao frame
        
        int mouseClick(int button, int state, GLfloat xpos, GLfloat ypos); //Função de handle de cliques de mouse
        
        bool visible(); //Verifica se o frame está visível
        bool getState(int ID); //Retorna o estado dos botões toggle do frame
        void setState(int ID, bool state); //Seta o estado de um botão do frame.
        
        std::string getTextInput(int ID); //Retorna o texto das caixas de texto do frame
       
        guiElement* getElement(int ID); //Retorna um elemento do frame
};

/**
 * @brief Classe botão
 * 
 */
class button : public guiElement
{
    private:
        std::string text; //Texto do botão
        std::vector<GLfloat> color; //Cor do botão

    public:
        button(int vsize,int hsize, int ID, const char* text,std::vector<GLfloat>color); //Construtor da classe botão
        button(int vsize,int hsize, int ID, const char* text); //Construtor da classe botão
        void setText(const char*); //Seta o texto do botão
        void draw(); //Desenha o botão
};

/**
 * @brief Classe textDisp - Representa um texto de exibição.
 * 
 */
class textDisp : public guiElement
{
    private:
        std::string text; //Texto a ser exibido
        std::vector<GLfloat> textColor; //Cor do texto

    public:
        textDisp(int vsize,int hsize, int ID, const char* text, std::vector<GLfloat>color); //Construtor da classe textDisp
        textDisp(int vsize,int hsize, int ID, const char* text); //Construtor da classe textDisp
        void onClick(functionPtr fun); //Função executada ao se clicar no texto
        void setTextColor(GLfloat r, GLfloat g, GLfloat b); //Seta a cor do texto
        void setText(const char* text); //Seta o texto a ser mostrado
        void draw(); //Desenha o texto
};

/**
 * @brief Classe textInput - Representa um campo de entrada de texto.
 * 
 */
class textInput : public guiElement
{
    private:
        std::string text; //Armazena a entrada de texto
        std::vector<GLfloat> textColor; //cor do texto

    public:
        textInput(int vsize,int hsize, int ID); //Construtor da classe textInput

        std::string getText(); //Retorna o texto armazenado 

        void onKeyPress(int); //Função executada ao se pressionar uma tecla
        void onClick(functionPtr fun); //Função executada ao clicar o campo de texto
        void addChar(char character); //Adiciona um caractere à caixa de texto
        void deleteChar(); //Deleta um caractere da caixa de texto
        void draw(); //Desenha a caixa de texto
        void setClicked(bool state); //Seta a flag de clicado  e executa animações
        void clear(); //Limpa o texto
};

/**
 * @brief Classe toggleButton - Representa um interruptor
 * 
 */
class toggleButton : public button
{
    private:
        bool state; //Armazena o estado do interruptor

    public:
        toggleButton(int vsize,int hsize, int ID,bool state); //Construtor para a classe toggleButton
        
        bool getState(); //Retorna o estado do interruptor

        void setState(bool state); //Seta o estado do interruptor
        void onClick(functionPtr fun); //Função executada ao clicar no interruptor
        void toogle(); //Alterna o estado do botão
};

#endif //GUI_H