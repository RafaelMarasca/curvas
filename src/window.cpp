/**
 * @file window.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Implementação da classe window e dos métodos da classe.
 * @version 0.1
 * @date 2022-09
 * 
 * 
 */


#include"window.h"
#include"scene.h"
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<GL/glew.h>
#include<iostream>
#include<sstream>
#include<thread>
#include<string>
#include<vector>
#include<cmath>
#include"curves.h"

int mouseFlag = 0; //Verifica se o mouse esta sendo pressionado. 


/**
 * @brief Handler para os cliques no elemenstos do menu 0 (Menu de adição de B-Spline)
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void window::menuClick0(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 5: //Botão de Mouse
            {
                std::string orderStr = (w->getMenu())->getTextInput(3); //Obtém o texto escrito na caixa 3
                std::string pointNumStr = (w->getMenu())->getTextInput(1); //Obtém o texto escrito na caixa 1

                if(orderStr.size()!=0 && pointNumStr.size() != 0) //Verifica se a ordem e o numero de pontos são válidos
                {
                    int order=0, pointNum=0;

                    //Converte as strigs para inteiro
                    order = stoi(orderStr);
                    pointNum = stoi(pointNumStr);

                    if(pointNum <= 0 || order <= 0) //Caso a entrada de uma das caixas seja <= 0
                    {
                        w->showPopUp("Entrada deve ser Positiva!"); 
                    }else if(pointNum <= order) //Caso o número de pontos seja menor ou igual a ordem da curva
                    {
                        w->showPopUp("Numero de Pontos <= Ordem!"); 
                    }else //Caso esteja tudo em ordem
                    {
                        w->getMenu()->clear(); //Limpa os dados do menu
                        w->getMenu()->hide(); //Esconde o menu
                        w->addSpline(pointNum, order,MOUSE); //Chama o método de adição de spline
                    }
                }
            }break;
        
            case 6: //Botão de Teclado
            {
                std::string orderStr = (w->getMenu())->getTextInput(3); //Obtém o texto escrito na caixa 3
                std::string pointNumStr = (w->getMenu())->getTextInput(1); //Obtém o texto escrito na caixa 1

                if(orderStr.size() != 0 && pointNumStr.size() != 0) //Verifica se a ordem e o numero de pontos são válidos
                {
                    int order=0, pointNum=0;

                    //Converte as strings para inteiro
                    order = stoi(orderStr);
                    pointNum = stoi(pointNumStr);

                    if(pointNum <= 0 || order <= 0) //Caso a entrada de uma das caixas seja <= 0
                    {
                        w->showPopUp("Entradas devem ser > 0!");
                    }else if(pointNum <= order) //Caso o número de pontos seja menor ou igual a ordem da curva
                    {
                        w->showPopUp("Numero de Pontos <= Ordem!");
                    }else //Caso esteja tudo em ordem
                    {
                        w->getMenu()->clear(); //Limpa os dados do menu
                        w->addSpline(pointNum,order,KEYBOARD); //Chama o método de adição de B-spline;
                        w->setMenu(1); //Seta o menu atual para o menu de adição de pontos
                    } 
                }
            }break;
        }
    }
}

/**
 * @brief Handler para os cliques do menu 1 (menu de adição de pontos).
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void window::menuClick1(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(ID)
        {
            case 5: //Botão Adicionar
            {
                std::string xStr = (w->getMenu())->getTextInput(2); //Obtém o texto da caixa de texto 2.
                std::string yStr = (w->getMenu())->getTextInput(4); //Obtém o texto da caixa de texto 4.

                if(xStr.size()!= 0 && yStr.size()!= 0) //Verifica se os dados são válidos
                {
                    GLfloat x=0, y=0;

                    //Converte as strings para float.
                    x = stof(xStr);
                    y = stof(yStr);

                    if(std::fabs(x)>1  || std::fabs(y)>1) //Verifica se o desenho ficará fora da tela
                    {
                         w->showPopUp("O Desenho Ficara Fora da Tela!");
                    }
                    
                    if(!(w->vision->checkCollision(x,y).first)) //Verifica se o ponto adicionado colidiu com outro ponto
                    {
                        //Caso não tenha colidido

                        bSpline* b = dynamic_cast<bSpline*>(w->selectedShape);

                        std::vector<GLfloat> point = {x,y,0.0f};
                        
                        b->addControlPoint(point); //Adiciona o ponto na spline
                    
                        w->waitingInput --; //Decrementa a variável de espera de entrada.

                        w->getMenu()->clear(); //Limpa os dados do menu
                        
                        if(!w->waitingInput) //Se não há espera por entrada
                        {
                            b->generate(); //Gera a Spline
                            w->inType = NONE; //Seta o tipo de entrada para nenhuma
                            w->setMenu(0,HIDDEN); //Seta o menu como 0.
                        }
                    }else{
                        //Caso tenha havido colisão
                        w->showPopUp("Ponto Repetido!"); //Mostra o popUp de ponto repetido 
                    }
                }
            }break;
        
            case 6: //Botão Cancelar;
                if(w->waitingInput)
                {
                    w->waitingInput = 0; //Zera a flag de espera por entrada
                    w->vision->removeObject(w->selectedShapeID); //Remove o objeto corrente
                    w->setMenu(0);//Seta o menu para 0.
                }
                w->clearSelection(); //Desseleciona os objetos
            break;
        }
    }
}


/**
 * @brief Handler para os cliques do menu 3 (menu de Opção).
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void window::menuClick2(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        if(w->selectedShape) //Verifica se alguma forma está selecionada
        {
            switch(ID)
            {
                case 2: //Botão de Visibilidade
                {
                    bSpline* b = dynamic_cast<bSpline*>(w->selectedShape);
                    b->setControlPointsVisibility((w->getMenu())->getState(ID)); //Seta a visibilidade conforme o estado do botão
                }break;
                
                case 4: //Botão de Clamping/Unclamping
                {
                    bSpline* b = dynamic_cast<bSpline*>(w->selectedShape);

                    if((w->getMenu())->getState(ID)) //Verifica o estado do botão
                        b->makeClamped(); //Se o botão está ON, faz a forma ficar presa
                    else
                        b->makeUnclamped(); //Se o botão está OFF, faz a forma ficar solta
                }break;
            }
        }
    }
}

/**
 * @brief Handler para os cliques do menu 3 (menu de popUp).
 * 
 * @param ID : ID do elemento que gerou o evento de clique.
 */
void window::menuClick3(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
       if(ID == 0) //Botão OK
       {
            w->closePopUp(); //Fecha o popUp
       }
    }
}


/**
 * @brief Cria uma B-Spline
 * 
 * @param pointNum Número de pontos
 * @param order Ordem dos polinômios
 * @param t Tipo de entrada
 */
void window::addSpline(int pointNum, int order, inputType t)
{
    this->inType = t;
    this->waitingInput = pointNum;
    this->selectedShapeID  = this->vision->addObject(new bSpline(order));
    this->selectedShape = this->vision->getObject(this->selectedShapeID).second;
}


/**
 * @brief Cria o menu 0 (menu de criação de B-Spline)
 * 
 * @return frame* ponteiro para o menu criado
 */
frame* newSplineMenu()
{
    frame* menu = new frame(1.0f, 1.0f, 6, 4, -0.5, 0.5); //Cria o frame

    //Adiciona os elementos de menu
    menu->addText(0,0,1,4,0,"NOVA B-SPLINE");
    menu->addTextInput(2,0,1,4,1);
    menu->addText(1,0,1,4,2,"Quantidade de Pontos");
    menu->addTextInput(4,0,1,4,3);
    menu->addText(3,0,1,4,4,"Ordem");
    menu->addButton(5,0,1,2,5,"Mouse");
    menu->addButton(5,1,1,2,6,"Teclado");
    menu->addClickFunction(window::menuClick0); //Adiciona a função chamada ao clicar em um elemento do menu 0
    
    menu->generate(); //Gera o menu

    return menu;
}

/**
 * @brief Cria o menu 1 (menu de adição de pontos)
 * 
 * @return frame* ponteiro para o menu criado
 */
frame* newAddMenu()
{
    frame* menu = new frame(0.5f, 1.0f, 4, 4, -0.5, 0.25); //Cria o frame do menu

    std::vector<GLfloat> red = {BUTTON_RED_R, BUTTON_RED_G,BUTTON_RED_B};
    
    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"ADICIONAR PONTOS");
    menu->addText(1,0,1,1,1,"X");
    menu->addTextInput(1,1,1,1,2);
    menu->addText(1,2,1,1,3,"Y");
    menu->addTextInput(1,3,1,1,4);
    menu->addButton(3,0,1,2,6,"Cancelar",red);
    menu->addButton(3,1,1,2,5,"Adicionar");
    menu->addClickFunction(window::menuClick1); //Adiciona a função chamada ao se clicar no menu
    
    menu->generate(); //Gera o menu

    return menu;
}

/**
 * @brief Cria o menu 2 (menu de Opção)
 * 
 * @return frame* ponteiro para o menu criado
 */
frame* newOptMenu()
{
    frame* menu = new frame(0.5f, 1.0f, 3, 4, -0.5, 0.25); //Cria o frame do menu

    //Adiciona os elementos do menu
    menu->addText(0,0,1,4,0,"MODIFICAR CURVA");
    menu->addText(1,0,1,3,1,"Mostrar PTS de Controle");
    menu->addToggleButton(1,1,1,1,2,true);
    menu->addText(2,0,1,3,3,"Prender B-Spline");
    menu->addToggleButton(2,1,1,1,4,true);
    menu->addClickFunction(window::menuClick2); //Adiciona a função chamada ao clicar em um elemento do menu 2
    
    menu->generate();//Gera o menu

    return menu;
}


/**
 * @brief Cria o menu 3 (menu de popUp)
 * 
 * @return frame* ponteiro para o menu criado
 */
frame* newPopUp()
{
    frame* menu = new frame(0.5f, 1.0f, 2, 4, -0.5, 0.25, -1, 0.1); //Cria o frame do menu
    
    //Adiciona os elementos do menu
    menu->addButton(1,1,1,2,0,"OK");
    menu->addText(0,0,1,4,1,"LOREM IPSUM AMET");
    menu->addClickFunction(window::menuClick3);
    
    menu->generate();//Gera o menu

    return menu;
}


/**
 * @brief Função chamada ao se pressionar uma tecla
 * 
 * @param key : tecla pressionada
 * @param x : posição x do mouse no instante do clique
 * @param y : posição y do mouse no instante do clique
 * @return ** void 
 */
void window::keyp(unsigned char key, int x, int y)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    
    //Verifica qual tecla foi pressionada
    switch(key)
    {
        //Tecla ESC
        case 27: //Desseleciona objeto/cancela operação
            if(w->waitingInput) //Verifica se está sendo construída uma forma
            {
                w->waitingInput = 0; //Seta a flag de espera de entrada para 0
                w->vision->removeObject(w->selectedShapeID); //Remove o objeto que está sendo criado
                w->inType = NONE; //Seta o tipo de entrada pra NONE
                w->setMenu(0); //Seta o menu corrente como 0
            }
            w->clearSelection(); //Desseleciona os objetos
        break;
    
        //Tecla DEL
        case 127: //Deleta a forma selecionada
            try
            {
                w->deleteShape(); //Deleta a forma selecionada
            }catch(std::string s)
            {
                w->showPopUp(s.c_str());
            }
        break;

        //Tecla Espaço
        case 32: //Exibe/Esconde o menu atual
            if(w->menu[w->currentMenu]->visible())
                w->menu[w->currentMenu]->hide();
            else
                w->menu[w->currentMenu]->show();
        break;
    }

    //Passa apenas as teclas de números para as caixas de texto
    if(((key>=48) && (key<=57))|| key == 8 || key == 46 ||key==45)
        w->menu[w->currentMenu]->keyPress(key);

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}

/**
 * @brief Construtor da classe Window
 * 
 * @param width : largura da janela.
 * @param height : altura da janela.
 * @param title : título da janela
 * @param argc : quantidade de argumentos de linha de comando
 * @param argv : argumentos passados por linha de comando
 */
window::window(int width, int height, const char* title, int* argc, char** argv)
{
    //Funções de inicialização do GLUT
    glutInit(argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width,height);
    glutCreateWindow(title);
    glutDisplayFunc(window::draw);
    glutMouseFunc(window::mouseClick);
    glutMotionFunc(window::mouseMove);
    glutKeyboardFunc(keyp);
    glutReshapeFunc(window::resize);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutSetWindowData(this);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(7);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2);

    //Inicializa o GLEW
    if(glewInit() != GLEW_OK)
    {
        //Falha na inicialização
        throw std::string("Falha na inicialização do GLEW.");
    }

    this->vision= new scene; //Cria uma cena para exibição
    this->waitingInput = 0; //Inicializa a flag de espera de entrada em 0
    this->inType = NONE;  //Tipo de entrada
    this->width = width; //Inicializa a largura da tela
    this->height = height; //Inicializa a altura da tela
    this->selectedShape = nullptr; //Inicializa a forma selecionada como NULL (Nenhuma forma selecionada)
    this->selectedShapeID = 0; //Inicializa o ID de forma selecionada como 0 (Nenhuma forma selecionada)
   
    //Inicializa a caixa de seleção
    this->selBox = new selectionBox();
    this->selBox->setColor(SELECTION_R,SELECTION_G,SELECTION_B);
   
    //Inicializa os menus
    this->menu[0] = newSplineMenu();
    this->menu[1] = newAddMenu();
    this->menu[2] = newOptMenu();
    this->menu[3] = newPopUp();
    this->currentMenu = 0;
}


/**
 * @brief Destrutor para a classe Window.
 * 
 */
window::~window()
{
    sceneIterator it;
    std::pair<unsigned int, geometry*> object;
    std::map<int, frame*>::iterator itMenu;

    //Itera por todos os objetos da cena atual e os deleta.
    for(it = this->vision->begin(); it!= this->vision->end(); it++)
    {
        object = this->vision->getObject((*it).first);
        delete object.second;
        object.second = nullptr;   
    }

    //Deleta os menus
    for(itMenu = this->menu.begin(); itMenu!= this->menu.end(); itMenu++)
        delete (*itMenu).second; 

    delete this->vision; //Deleta a cena.

    delete this->selBox; //Deleta a caixa de seleção
}


/**
 * @brief Método de desenho para a classe Window.
 * 
 */
void window::draw()
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    glClearColor(0.784f,0.784f,0.784f, 1.0f); //Determina a cor de limpeza do buffer
    glClear(GL_COLOR_BUFFER_BIT); //Limpa o buffer com a corr de background
    w->vision->draw(); //Desenha a cena
    w->selBox->draw(); //Desenha a caixa de seleção
    w->menu[w->currentMenu]->draw(); //Desenha o menu corrente.

    glutSwapBuffers(); //Troca os buffers (Exibe na tela)
}

/**
 * @brief Função que executa a janela.
 * 
 */
void window::init()
{
    glutMainLoop(); //Roda o loop principal da janela
}

/**
 * @brief Método executado ao clicar com o mouse na tela
 * 
 * @param button : botão do mouse clicado
 * @param state : estado do botão no momento do clique
 * @param x : posição x do mouse no instante do clique
 * @param y : posição y do mouse no instante do clique
 */
void window::mouseClick(int button, int state,int x, int y)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    if(w)
    {
        //Se o botão esquerdo foi pressionado
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            float xpos, ypos;

            //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
            if(w->width > w->height){
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
                ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
            }else{
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
                ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
            }

            if(w->menu[w->currentMenu]->visible()) //Verifica se há um menu sendo exibido
            {   
                w->menu[w->currentMenu]->mouseClick(button,state,xpos,ypos); //Função de clique de mouse para os menus
            }else{
                std::vector<GLfloat> coordinates = {xpos, ypos, 0.0f};

                //Cria o início caixa de seleção caso não esteja sendo inserido um ponto
                if(mouseFlag == 0 && !w->waitingInput)
                {
                    mouseFlag = 1; 
                    w->selBox->addStart(xpos,ypos);
                }

                //Caso esteja sendo inserida uma forma via mouse
                if(w->waitingInput && w->inType == MOUSE)
                { 
                    if(w->selectedShape)
                    {
                        bSpline* B = dynamic_cast <bSpline*> (w->selectedShape);
                        
                        if((w->vision->checkCollision(xpos, ypos)).first) //Verifica se o ponto inserido é repetido
                        {
                            w->showPopUp("Ponto Repetido!");      

                        }else{
                            //Caso não seja repetido adiciona o ponto repetido
                            B->addControlPoint(xpos,ypos,0.0f); 
                            w->waitingInput--;
                            
                            if(!(w->waitingInput))//Verifica se acabou a inserção de pontos
                            {
                                //Gera a B-Spline
                                bSpline* B = dynamic_cast<bSpline*> (w->selectedShape);
                                B->generate();
                                w->inType = NONE;
                            }
                        }   
                    }
                }
            }
        }

        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) //Bloco executado ao soltar o mouse e não estar sendo inserida nenhuma forma
        {
            float xpos, ypos;

            //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
            if(w->width > w->height){
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
                ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
            }else{
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
                ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
            }

            if(mouseFlag == 1 && !w->waitingInput) //Altera o tamanho da caixa de seleção
            {
                mouseFlag = 0;
                std::pair<unsigned int, geometry*> temp = w->vision->checkCollision(w->selBox);
                w->select(temp); //Seleciona a forma englobada pela caixa de seleção (se houver);
                w->selBox->clearVertex(); //Apaga a caixa de seleção
            }
        }
        glutPostRedisplay(); //Requere que a tela seja redesenhada    
    }
}


/**
 * @brief Deleta a forma selecionada atualmente
 * 
 */
void window::deleteShape()
{
    if(this->selectedShape) //Caso alguma forma esteja selecionada
    {
	    geometry* temp = this->vision->removeObject(this->selectedShapeID);//Remove o objeto da cena
        if(!temp)
        {
            throw std::string("Objeto inválido");
		}else
		{
            //Limpa a seleção
		    delete temp;
            this->clearSelection();
		}
    }else
    {
        throw std::string("Nenhum objeto Selecionado.");
    }
}

/**
 * @brief Seleciona uma forma da tela.
 * 
 * @param obj Par ID, ponteiro para gometry, definindo o ID da forma e a própria forma.
 */
void window::select(std::pair<unsigned int, geometry*> obj)
{
    if(this->selectedShape)
        this->selectedShape->resetColor(); //Muda a cor da forma desselecionada
    if((this->selectedShape = obj.second))
        this->selectedShape->setColor(SELECTION_R, SELECTION_G, SELECTION_B); //Muda a cora da nova forma selecionada
    this->selectedShapeID = obj.first;
    
    if(obj.second == nullptr)
        this->setMenu(0,HIDDEN);
    else
        this->setMenu(2,HIDDEN);

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}


/**
 * @brief Limpa a forma selecionada
 * 
 */
void window::clearSelection()
{
    this->select(std::pair<unsigned int, geometry*>{0,nullptr});

    this->setMenu(0,HIDDEN); //Seta o menu corrente para 0.

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}

/**
 * @brief Método chamado quando a tela é redimensionada - Mantém a razão de aspecto.
 * 
 * @param newWidth : nova largura da janela
 * @param newHeight : nova altura da janela
 */
void window::resize(int newWidth, int newHeight)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    w->width = newWidth; //Define a nova largura da janela
    w->height = newHeight;  //Define a nova altura da janela

    GLfloat aspectRatio = (GLfloat)newWidth / (GLfloat)newHeight; //Calcula a razão de aspecto da janela atual
    w->aspectRatio = aspectRatio; //Atualiza o valor da razão de aspecto da janela atual

    //Redimensiona a viewport para um quadrado limitado pela menor dimensão da janela
    if(newWidth>newHeight)
    {
        GLfloat aux = GLfloat(newWidth-newHeight)/2.0f;
        glViewport(aux, 0, newHeight, newHeight);
    }else
    {
        GLfloat aux = GLfloat(newHeight-newWidth)/2.0f;
        glViewport(0, aux, newWidth, newWidth);
    }
   
    //Inicializa o sistema de coordenadas para a projeção ortogonal
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //Mantém a razão de aspecto na projeção.
    if (newWidth <= newHeight)
    {
        glOrtho (-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, 1.0f, -1.0f);
    }else
    {
        glOrtho (-1.0f * aspectRatio, 1.0f * aspectRatio,-1.0f, 1.0f, 1.0f, -1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}


/**
 * @brief Função para lidar com o movimento do mouse enquanto está pressionado.
 * 
 * @param x : Posição x do mouse.
 * @param y : Posição y do mouse.
 */
void window::mouseMove(int x, int y)
{
    //Se o mouse está pressionado
    if(mouseFlag)
    {
        window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

        float xpos, ypos;

        //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
        if(w->width > w->height){
            xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
            ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
        }else{
            xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
            ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
        }

        w->selBox->updateLength(xpos,ypos); //Altera o tamanho da caixa de seleção

        glutPostRedisplay();
    }
}

/**
 * @brief Método de desenho da caixa de seleção.
 */
void selectionBox::draw()
{
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINE_LOOP, 0, this->vertices.size()/3);
}


/**
 * @brief Atualiza o tamanho da caixa de seleção.
 * 
 * @param x : Coordenada normalizada x do canto inferior direito da caixa.
 * @param y : Coordenada normalizada y do canto inferior direito da caixa.
 */
void selectionBox::updateLength(GLfloat x, GLfloat y)
{
    //Verifica se há vertices no quadrado
    if(this->vertices.size())
    {
        this->vertices.erase(this->vertices.begin()+3, this->vertices.end());
        std::vector<GLfloat> coordinates = {x, this->vertices[1], this->vertices[2]};
        this->addVertex(coordinates);
        coordinates[1] = y;
        this->addVertex(coordinates);
        coordinates[0] = this->vertices[0];
        this->addVertex(coordinates);

    }   
}

/**
 * @brief Adiciona o ponto do canto superior esquerdo da caixa de seleção.
 * 
 * @param x : Coordenada normalizada x do ponto superior esquerdo da caixa.
 * @param y : Coordenada normalizada y do ponto superior esquerdo da caixa.
 */
void selectionBox::addStart(GLfloat x, GLfloat y)
{
    std::vector<GLfloat> coordinates = {x,y,0.0f};
    this->clearVertex(); //Limpa os vértices
    this->addVertex(coordinates); //Adiciona o vértice de início
}

/**
 * @brief Limpa (exclui) os vértices da caixa de seleção.
 */
void selectionBox::clearVertex()
{
    this->vertices.clear();
}

/**
 * @brief Retorna o menu corrente.
 * 
 * @return frame* Ponteiro para o menu corrente
 */
frame* window::getMenu()
{
    return this->menu[this->currentMenu];
}

/**
 * @brief Seta o menu corrente.
 * 
 * @param ID : ID do menu que será corrente 
 * @param state : Estado inicial do menu (HIDDEN/ VISIBLE) - DEFAULT: VISIBLE
 */
void window::setMenu(int ID, menuState state)
{
    this->menu[this->currentMenu]->clear(); //Limpa os dados do menu corrente.
    this->menu[this->currentMenu]->hide(); //Esconde o menu corrente.
    if(state == VISIBLE) //Verifica se o estado inicial é VISIBLE.
        this->menu[ID]->show(); //Mostra o novo menu.
    else
        this->menu[ID]->hide(); //Esconde o novo menu.
    this->currentMenu = ID; //Seta o menu ID como corrente.
}

/**
 * @brief Mostra o popup
 * 
 * @param text Texto a ser exibido pelo popup
 */
void window::showPopUp(const char* text)
{
    //Armazena os dados do menu corrente
    this->tempMenu.first = currentMenu;
    this->tempMenu.second = this->menu[this->currentMenu]->visible();  
    
    //Seta o menu atual como popup
    this->setMenu(3);
    textDisp* t = dynamic_cast<textDisp*>(this->menu[3]->getElement(1));
    //Altera o texto do popup
    if(t)
        t->setText(text);
}

/**
 * @brief Fecha o popUp corrente
 * 
 */
void window::closePopUp()
{
    //Resgata o menu passado conforme salvo pela função showPopUp
    if(this->tempMenu.second)
        this->setMenu(this->tempMenu.first, VISIBLE);
    else
        this->setMenu(this->tempMenu.first, HIDDEN);
}