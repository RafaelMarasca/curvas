/**
 * @file gui.cpp
 * @author Rafael Marasca Martins
 * @brief Implementação das classes de gui.
 * @version 0.1
 * @date 2022-10
 */

#include"gui.h"
#include"geometry.h"
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<GL/glew.h>
#include<vector>
#include<string>
#include<iostream>


/****************************** guiElement *********************************/

/**
 * @brief Gera o elemento de gui
 * 
 * @param height Altura do elemento de gui
 * @param width Largura do elemento de gui
 * @param xPos Posição x do canto superior esquerdo do elemento de gui
 * @param yPos Posição y do canto superior esquerdo do elemento de gui
 */
void guiElement::generate(GLfloat height, GLfloat width, GLfloat xPos, GLfloat yPos)
{   
    this->width = width*hSize;
    this->height = height*vSize;
    this->xPos = xPos;
    this->yPos = yPos;
    //Gera o quadrado referente ao elemento
    std::vector<GLfloat> aux (12,0.0f);
    aux[0] = xPos;
    aux[1] = yPos;
    aux[3] = xPos + this->hSize*width;
    aux[4] = yPos;
    aux[6] = xPos + this->hSize*width;
    aux[7] = yPos - this->vSize*height;
    aux[9] = xPos;
    aux[10] = yPos - this->vSize*height;
    this->addVertex(aux);
}


/**
 * @brief Getter para o ID do elemento.
 * 
 * @return int ID do elemento 
 */
int guiElement::getID()
{
    return this->ID;
}

/**
 * @brief Construtor da classe guiElement
 * 
 * @param height altura do elemento de gui em coordenadas normalizadas
 * @param width largura do elemento de gui em coordenadas normalizadas
 * @param xPos posição x do canto superior esquerdo do elemento de gui
 * @param yPos poisição y do canto superior esquerdo do elemento de gui
 */
guiElement::guiElement(GLfloat height, GLfloat width, GLfloat xPos, GLfloat yPos):square(width, height, xPos, yPos)
{
    this->width = width;
    this->height = height;
    this->xPos = xPos;
    this->yPos = yPos;
    this->clicked = false;
}

/**
 * @brief Construtor para a classe guiElement
 * 
 * @param verticalSize Tamanho vertical do elemento em células
 * @param horizontalSize Tamanho horizontal do elemento em células
 * @param ID 
 */
guiElement::guiElement(int verticalSize, int horizontalSize, int ID)
{
    this->hSize = horizontalSize;
    this->vSize = verticalSize;
    this->ID = ID;
    this->clicked = false;
}

/**
 * @brief Getter para o tamanho horizontal do elemento (em células)
 * 
 * @return int Tamanho horizontal do elemento (em células)
 */
int guiElement::getHSize()
{
    return this->hSize;
}

/**
 * @brief Getter para o tamanho vertical do elemento (em células)
 * 
 * @return int Tamanho vertical do elemento (em células)
 */
int guiElement::getVSize()
{
    return this->vSize;
}

/**
 * @brief Limpa os dados do elemento de gui
 * 
 */
void guiElement::clear()
{
    this->setClicked(false);
}


/**
 * @brief Define a função de callback chamada a clicar no elemento
 * 
 * @param fun função de callback
 */
void guiElement::onClick(void (*fun) (int))
{
    fun(this->ID);
}

/**
 * @brief Função chamada ao pressionar uma tecla.
 * 
 */
void guiElement::onKeyPress(int){return;}

/**
 * @brief Seta a flag de clicado do elemento
 * 
 * @param state Novo estado da flag
 */
void guiElement::setClicked(bool state)
{
    this->clicked = state;
}

/**
 * @brief Desenha o elemento
 * 
 */
void guiElement::draw()
{
    this->square::draw();
}


/******************************** frame ************************************/

/**
 * @brief Construtor para a classe frame
 * 
 * @param height Altura do frame em coordenadas normalizadas
 * @param width Largura do frame em coordenadas normalizada
 * @param numVertical Número de elementos na vertical
 * @param numHorizontal Número de elmentos na horizontal
 * @param xPos Posição x do canto superior esquerdo do frame
 * @param yPos Posição y do canto superior esquerdo do frame
 * @param ID ID do frame
 * @param gap gap entre os elementos do frame
 */
frame::frame(GLfloat height, GLfloat width, int numVertical, int numHorizontal, GLfloat xPos, GLfloat yPos, int ID ,float gap):guiElement(height, width, xPos, yPos)
{
    this->numHorizontalElements = numHorizontal;
    this->numVerticalElements = numVertical;
    this->vGap = gap*height;
    this->hGap = gap*width;
    this->gap = gap;
    this->ID = ID;
    this->isVisible = false;
    this->elementHeight = (height-((numVertical+1)*this->vGap))/numVertical;
    this->elementWidth = (width-((numHorizontal+1)*this->hGap))/numHorizontal;

    this->setColor(FRAME_R, FRAME_G, FRAME_B);

    this->elements = std::vector<std::vector<guiElement*>> (numVertical, std::vector<guiElement*>());
}


/**
 * @brief Adiciona um elemento na posição vpos, hpos do grid frame
 * 
 * @param vPos Posição vertical (Célula vertical 0-indexada)
 * @param hPos Posição horizontal (Célula horizontal 0-indexada)
 * @param newElement Ponteiro para o elemento a ser inserido
 */
void frame::addElement(int vPos, int hPos, guiElement* newElement)
{
    if(!(hPos> (this->numHorizontalElements-1) || vPos> (this->numVerticalElements-1)))
    {
        
        while((this->elements[vPos].size())<unsigned(hPos+1))
        {
            this->elements[vPos].push_back(nullptr);
        }

        this->elements[vPos][hPos] = newElement;
    }
}

/**
 * @brief Adiciona um elemento na posição vpos, hpos do grid frame
 * 
 * @param vPos Posição vertical (Célula vertical 0-indexada)
 * @param hPos Posição horizontal (Célula horizontal 0-indexada)
 * @param vSize Tamanho vertical do elemento (em células)
 * @param hSize Tamanho horizontal do elemento (em células)
 */
void frame::addElement(int vPos, int hPos, int vSize, int hSize, int ID)
{
    if(!(hPos> (this->numHorizontalElements-1) || vPos> (this->numVerticalElements-1) || hSize > (this->numHorizontalElements) || vSize> (this->numVerticalElements)))
    {
        guiElement* newElement = new guiElement(vSize, hSize, ID);
        this->addElement(vPos,hPos,newElement);
    }
}

/**
 * @brief Desenha o frame e todos os seus elementos caso ele esteja visível
 * 
 */
void frame::draw()
{
    if(this->isVisible)
    {
        geometry::program->use();
        glUniform3fv(this->colorLoc, 1, &(this->color[0]));
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size()/3);


        std::vector<std::vector<guiElement*>>::iterator it1;
        std::vector<guiElement*>::iterator it2; 
        for(it1 = this->elements.begin(); it1 != this->elements.end(); it1++)
        {
            for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
            {
                if((*it2) != nullptr)
                {
                    (*it2)->draw();
                }
            }
        }
    }
}

/**
 * @brief Setter para o estado de um toggle button.
 * 
 * @param ID ID do botão.
 * @param state Novo estado.
 */
void frame::setState(int ID, bool state)
{
    toggleButton* t = dynamic_cast<toggleButton*>(this->getElement(ID));
    
    if(t)
        t->setState(state);
}


/**
 * @brief Destrutor da classe frame - Destrói o frame e todos os seus elementos
 * 
 */
frame::~frame() 
{
    std::vector<std::vector<guiElement*>>::iterator it1;
    std::vector<guiElement*>::iterator it2;
    for(it1 = this->elements.begin(); it1 != this->elements.end(); it1++)
    {
        for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
        {
            if((*it2) != nullptr)
            {
                delete (*it2);
            }
        }
    }
}

/**
 * @brief Método de handle para os cliques de mouse
 * 
 * @param button Botão do mouse que gerou o evento
 * @param state Estado do botão
 * @param xpos Posição x do mouse em coordenadas normalizadas
 * @param ypos Posição y do mouse em coordenadas normalizadas
 * @return int 
 */
int frame::mouseClick(int button, int state, GLfloat xpos, GLfloat ypos)
{
    if(isVisible) //Verifica se o frame está visível
    {
        if(this->collision(xpos,ypos,0.0f)) //Verifica se houve colisão com o frame
        {
            std::vector<std::vector<guiElement*>>::iterator it1;
            std::vector<guiElement*>::iterator it2; 

            for(it1 = this->elements.begin(); it1!= this->elements.end(); it1++)
            {
                for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
                {
                    if((*it2)!= nullptr)
                    {
                        if((*it2)->collision(xpos,ypos,0.0f)) //Verifica se houve colisão com algum elemento
                        {
                            if(this->clickFunction)
                                (*it2)->onClick(clickFunction); //Caso houve colisão, aciona a função de clique do elemento.
                        }else
                        {
                            (*it2)->setClicked(false); //Caso não tenha havido colisão, seta a flag de clicado como falso
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Adiciona uma função de callback de clique ao frame
 * 
 * @param fun Função de callback.
 */
void frame::addClickFunction(functionPtr fun)
{
    this->clickFunction = fun;
}

/**
 * @brief Gera o frame e os elementos associados.
 * 
 */
void frame::generate()
{

    std::vector<std::vector<guiElement*>>::iterator it1;
    std::vector<guiElement*>::iterator it2;
    int i=0, j =0;
    float element_xPos = this->xPos + this->hGap;
    float element_yPos = this->yPos - this->vGap;
    
    for(it1 = this->elements.begin(); it1 != this->elements.end(); it1++)
    {
        for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
        {

            if((*it2) != nullptr)
            {
                GLfloat width = this->elementWidth + (((*it2)->getHSize()-1)*hGap/(*it2)->getHSize());
                (*it2)->generate(this->elementHeight, width, element_xPos, element_yPos);
                element_xPos += (elementWidth + hGap)*(*it2)->getHSize();
            }else
            {
                element_xPos += this->elementWidth + this->hGap;
            }
        }
        element_xPos = this->xPos + this->hGap;
        element_yPos -= (this->elementHeight + this->vGap);
        j++;
    }
}


/**
 * @brief Getter para um elemento do frame
 * 
 * @param ID ID do elemento a ser recuperado.
 * @return guiElement* ponteiro para o elemento
 */
guiElement* frame::getElement(int ID)
{
    std::vector<std::vector<guiElement*>>::iterator it1;
    std::vector<guiElement*>::iterator it2;

    for(it1 = this->elements.begin(); it1!= this->elements.end(); it1++)
    {
        for(it2 = (*it1).begin(); it2!= (*it1).end(); it2++)
        {
            if((*it2)!= nullptr)
            {
                if((*it2)->getID() == ID)
                {
                    return (*it2);
                }
            }
        }
    }
    return nullptr;
}


/**
 * @brief Getter para o estado de um interruptor do frame
 * 
 * @param ID ID do interruptor
 * @return true caso o estado do interruptor seja ON
 * @return false caso o estado do interruptor seja OFF
 */
bool frame::getState(int ID)
{
    toggleButton* t = dynamic_cast<toggleButton*>(this->getElement(ID));
    
    if(t)
    {
        return t->getState();
    }else
    {
        return false;
    }
}

/**
 * @brief Limpa os dados do frame e seus elementos associados
 * 
 */
void frame::clear()
{
    std::vector<std::vector<guiElement*>>::iterator it1;
    std::vector<guiElement*>::iterator it2; 

    for(it1 = this->elements.begin(); it1!= this->elements.end(); it1++)
    {
        for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
        {
            if((*it2)!= nullptr)
            {
                (*it2)->clear(); 
            }
        }
    }
}

/**
 * @brief Torna o frame invisível
 * 
 */
void frame::hide()
{
    this->isVisible = false;
}

/**
 * @brief Torna o frame visível
 * 
 */
void frame::show()
{
    this->isVisible = true;
}

/**
 * @brief Getter para a visibilidade do frame
 * 
 * @return true caso o frame esteja sendo exibido
 * @return false caso o frame não esteja sendo exibido
 */
bool frame::visible()
{
    return this->isVisible;
}


/**
 * @brief Adiciona um botão ao frame
 * 
 * @param vpos Posição vertical do botão (Posição da célula 0-indexada)
 * @param hpos Posição horizondal do botão (Posição da célula 0-indexada)
 * @param vsize Tamaho Vertical do botão (Em células)
 * @param hsize Tamanho horizontal do botão (Em Células)
 * @param ID ID do botão
 * @param text Texto do botão
 */
void frame::addButton(int vpos, int hpos, int vsize,int hsize, int ID, const char* text)
{
    button* b = new button(vsize, hsize, ID, text);
    this->addElement(vpos,hpos,b);
}

/**
 * @brief Adiciona um botão ao frame
 * 
 * @param vpos Posição vertical do botão (Posição da célula 0-indexada)
 * @param hpos Posição horizondal do botão (Posição da célula 0-indexada)
 * @param vsize Tamaho Vertical do botão (Em células)
 * @param hsize Tamanho horizontal do botão (Em Células)
 * @param ID ID do botão
 * @param text Texto do botão
 * @param color Cor do botão
 */
void frame::addButton(int vpos, int hpos, int vsize,int hsize, int ID, const char* text, std::vector<GLfloat> color)
{
    button* b = new button(vsize, hsize, ID, text);
    b->setColor(color[0],color[1],color[2]);
    this->addElement(vpos,hpos,b);
}

/**
 * @brief Adiciona um interruptor ao frame
 * 
 * @param vpos Posição vertical do interruptor (Posição da célula 0-indexada)
 * @param hpos Posição horizondal do interruptor (Posição da célula 0-indexada)
 * @param vsize Tamaho Vertical do interruptor (Em células)
 * @param hsize Tamanho horizontal do interruptor (Em Células)
 * @param ID ID do interruptor
 */
void frame::addToggleButton(int vpos, int hpos, int vsize,int hsize, int ID, bool initialState)
{   
    toggleButton* t = new toggleButton(vsize, hsize, ID, initialState);
    this->addElement(vpos,hpos,t);
}


/**
 * @brief Getter para o texto de uma caixa de texto do frame
 * 
 * @param ID ID da caixa de texto
 * @return std::string - texto presente na caixa no instante da chamada.
 */
std::string frame::getTextInput(int ID)
{
    textInput* t = dynamic_cast<textInput*>(this->getElement(ID));
    if(t)
        return t->getText();
    else
        return std::string();
}


/**
 * @brief Funçaõ de handle de teclas pressionadas
 * 
 * @param key Tecla pressionada
 */
void frame::keyPress(int key)
{
    if(isVisible)
    {
        std::vector<std::vector<guiElement*>>::iterator it1;
        std::vector<guiElement*>::iterator it2; 

        for(it1 = this->elements.begin(); it1!= this->elements.end(); it1++)
        {
            for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
            {
                if((*it2)!= nullptr)
                {
                    (*it2)->onKeyPress(key);   
                }
            }
        }
    }
}

/**
 * @brief Adiciona um texto ao frame
 * 
 * @param vpos Posição vertical do texto (Posição da célula 0-indexada)
 * @param hpos Posição horizondal do texto (Posição da célula 0-indexada)
 * @param vsize Tamaho Vertical do texto (Em células)
 * @param hsize Tamanho horizontal do texto (Em Células)
 * @param ID ID do texto
 * @param text Texto do texto
 */
void frame::addText(int vpos, int hpos, int vsize,int hsize, int ID, const char* text)
{
    textDisp* t = new textDisp(vsize, hsize, ID, text);
    this->addElement(vpos,hpos,t);
}

/**
 * @brief Adiciona uma caixa de texto ao frame
 * 
 * @param vpos Posição vertical da caixa de texto (Posição da célula 0-indexada)
 * @param hpos Posição horizondal da caixa de texto (Posição da célula 0-indexada)
 * @param vsize Tamaho Vertical da caixa de texto (Em células)
 * @param hsize Tamanho horizontal da caixa de texto (Em Células)
 * @param ID ID da caixa de texto
 */
void frame::addTextInput(int vpos, int hpos, int vsize,int hsize, int ID)
{
    textInput* t = new textInput(vsize, hsize, ID);
    this->addElement(vpos,hpos,t);
}


/************************************** button *****************************************/

/**
 * @brief Desenha o botão 
 * 
 */
void button::draw()
{
    this->guiElement::draw(); //Desenha o corpo do botão 

    //Desenha o texto do botão
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f,1.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(0.0f,0.0f,0.0f, 1.0f);

    int length = glutBitmapLength(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text.c_str()));
    
    glRasterPos2f(this->xPos + ((this->width)-(length*0.00333f))/2.0, this->yPos-this->height/2);

    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text.c_str()));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


/**
 * @brief Construtor da classe button
 * 
 * @param vsize Tamanho vertical do botão (em células)
 * @param hsize Tamanho horizontal do botão (em células)
 * @param ID ID do botão
 * @param text Texto do botão
 */
button::button(int vsize,int hsize, int ID, const char* text):guiElement(vsize, hsize, ID)
{
    this->text = text;
    this->setColor(BUTTON_R, BUTTON_G, BUTTON_B);
}

/**
 * @brief Construtor da classe button
 * 
 * @param vsize Tamanho vertical do botão (em células)
 * @param hsize Tamanho horizontal do botão (em células)
 * @param ID ID do botão
 * @param text Texto do botão
 * @param color Cor do botão
 */
button::button(int vsize,int hsize, int ID, const char* text, std::vector<GLfloat>color):guiElement(vsize, hsize, ID)
{
    this->text = text;
    this->setColor(color[0],color[1],color[2]);
}

/**
 * @brief Setter do texto do botão
 * 
 * @param text Novo texto do botão
 */
void button::setText(const char* text)
{
    this->text = text;
}



/**************************** textDisp ***********************************/

/**
 * @brief Construtor da classe textDisp
 * 
 * @param vsize Tamanho vertical (em células)
 * @param hsize Tamanho horizontal (em células)
 * @param ID ID do texto
 * @param text Texto a ser adicionado
 */
textDisp::textDisp(int vsize,int hsize, int ID, const char* text):guiElement(vsize, hsize, ID)
{
    this->text = text;
    this->setColor(FRAME_R, FRAME_G, FRAME_B);
    this->textColor = {TEXT_R, TEXT_G, TEXT_B};
}

/**
 * @brief Setter para a cor do texto
 * 
 * @param r Quantidade de vermelho da cor
 * @param g Quantidade de verde da cor
 * @param b Quantidade de azul da cor
 */
void textDisp::setTextColor(GLfloat r, GLfloat g, GLfloat b)
{
    this->textColor[0] = r;
    this->textColor[1] = g;
    this->textColor[2] = b;
}

/**
 * @brief Desenha o texto na tela
 * 
 */
void textDisp::draw()
{
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f,1.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(this->textColor[0],this->textColor[1],this->textColor[2], 1.0f);

    int length = glutBitmapLength(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text.c_str()));

    glRasterPos2f(this->xPos + ((this->width)-(length*0.00333f))/2.0, this->yPos-this->height/2);

    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text.c_str()));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

}

/**
 * @brief Adiciona uma função de callback para o textDisp
 * 
 * @param fun Função de callback
 */
void textDisp::onClick(functionPtr fun)
{
    this->setClicked(true);
    fun(this->ID);
}

/**
 * @brief Setter para o texto do elemento
 * 
 * @param text Novo texto a ser exibido
 */
void textDisp::setText(const char* text)
{
    this->text = text;
}

/*********************************** textInput **************************************/

/**
 * @brief Construtor para a classe textInput
 * 
 * @param vsize Tamanho vertical (em células)
 * @param hsize Tamanho horizontal (em células)
 * @param ID ID da caixa de texto
 */
textInput::textInput(int vsize,int hsize, int ID):guiElement(vsize, hsize, ID)
{
    this->setColor(1.0f,1.0f,1.0f);
}

/**
 * @brief Adiciona um caractere ao texto da caixa
 * 
 * @param character Caractere a ser adicionado
 */
void textInput::addChar(char character)
{
    this->text.push_back(character);
}

/**
 * @brief Deleta um caractere da caixa de texto
 * 
 */
void textInput::deleteChar()
{
    if(this->text.size())
        this->text.pop_back();
}

/**
 * @brief Desenha a caixa de texto
 * 
 */
void textInput::draw()
{
    this->guiElement::draw(); //Desenha o corpo da caixa de texto

    //Desenha o texto da caixa de texto
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f,1.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glRasterPos2f(this->xPos, this->yPos-this->height/2);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text.c_str()));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

/**
 * @brief Função de pressionamento de tecla
 * 
 * @param key Tecla pressionada
 */
void textInput::onKeyPress(int key)
{
    if(this->clicked)
    {
        if(key == 8)
        {
            this->deleteChar();
        }else
        {
            this->addChar(char(key));
        }
    }
}

/**
 * @brief Adiciona uma função de callback para o clique do mouse
 * 
 * @param fun Função de callback a ser adicionada
 */
void textInput::onClick(functionPtr fun)
{
    this->setClicked(true);
    fun(this->ID);
}

/**
 * @brief Setter para a flag de clicado - Colore a caixa e ativa a inserção de caracteres
 * 
 * @param state Novo valor da flag
 */
void textInput::setClicked(bool state)
{
    this->clicked = state;
    if(state == true)
    {
        this->setColor(0.4196f,0.6039f,0.7686f);
    }else
    {
        this->setColor(TEXT_R,TEXT_G,TEXT_B);
    }
}

/**
 * @brief Getter para o texto inserido na caixa
 * 
 * @return std::string String atual inserida na caixa de texto
 */
std::string textInput::getText()
{
    return this->text;
}

/**
 * @brief Limpa o texto armazenado em buffer
 * 
 */
void textInput::clear()
{
    this->guiElement::clear();
    this->text.clear();
}

/******************************* toggleButton ***********************************/

/**
 * @brief Construtor para a classe toggleButton
 * 
 * @param vsize Tamanho vertical (em células)
 * @param hsize Tamanho horizontal (em células)
 * @param ID ID do interruptor
 * @param state Estado inicial do interruptor
 */
toggleButton::toggleButton(int vsize,int hsize, int ID,bool state):button(vsize, hsize, ID, "")
{
    this->setState(state);
}

/**
 * @brief Getter para o estado do Interruptor
 * 
 * @return true Caso o interruptor eteja ON
 * @return false Caso o interruptor esteja OFF
 */
bool toggleButton::getState()
{
    return this->state;
}

/**
 * @brief Setter para o estado do interruptor
 * 
 * @param state Novo estado do interruptor
 */
void toggleButton::setState(bool state)
{
    if(state)
    {
        this->setText("ON");
        this->setColor(TOGGLE_ON_R,TOGGLE_ON_G,TOGGLE_ON_B);
    }else
    {
        this->setText("OFF");
        this->setColor(TOGGLE_OFF_R,TOGGLE_OFF_G,TOGGLE_OFF_B);
    }
    this->state = state;
}

/**
 * @brief Alterna o estado do interruptor
 * 
 */
void toggleButton::toogle()
{
    this->setState(!this->state);
}

/**
 * @brief Adiciona uma função de callback para o clique do interruptor
 * 
 * @param fun Função de callback a ser adicionada
 */
void toggleButton::onClick(functionPtr fun)
{
    this->toogle();
    fun(this->ID);
}