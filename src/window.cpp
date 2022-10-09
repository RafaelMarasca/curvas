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

int closeFlag = false; //Inicializa a flag de fechamento
int inputFlag = 0; //Inicializa a flag de entrada
int mouseFlag = 0;


void window::menuClick0(int i)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w){
        
        switch(i)
        {
            case 5:
            {
                std::string orderStr = (w->getMenu())->getTextInput(3);
                std::string pointNumStr = (w->getMenu())->getTextInput(1);
                if(orderStr.size()!=0 && pointNumStr.size() != 0)
                {
                    int order=0, pointNum=0;

                    order = stoi(orderStr);
                    pointNum = stoi(pointNumStr);

                    if(pointNum <= order)
                    {
                        throw std::string("IMPOSSIVEL!!!");
                    }

                    std::cout<<order<<std::endl;;
                    std::cout<<pointNum<<std::endl;

                    w->getMenu()->clear();
                    w->getMenu()->hide();
                    w->addSpline(pointNum, order,MOUSE);
                }
            }break;
        
            case 6:
            {
                std::string orderStr = (w->getMenu())->getTextInput(3);
                std::string pointNumStr = (w->getMenu())->getTextInput(1);

                if(orderStr.size() != 0 && pointNumStr.size() != 0)
                {
                    int order=0, pointNum=0;

                    order = stoi(orderStr);
                    pointNum = stoi(pointNumStr);

                    if(pointNum < order)
                    {
                        throw std::string("IMPOSSIVEL!!!");
                    }

                    std::cout<<order<<std::endl;;
                    std::cout<<pointNum<<std::endl;

                    w->getMenu()->clear();
                    w->getMenu()->hide();
                    w->addSpline(pointNum,order,KEYBOARD);
                    w->setMenu(1); 
                }

            }break;
        }
    }
}

void window::menuClick1(int i)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        switch(i)
        {
            case 6:
            {
                std::string xStr = (w->getMenu())->getTextInput(2);
                std::string yStr = (w->getMenu())->getTextInput(4);

                if(xStr.size()!= 0 && yStr.size()!= 0)
                {
                    GLfloat x=0, y=0;

                    x = stof(xStr);
                    y = stof(yStr);

                    std::cout<<x<<","<<y<<std::endl;

                    if(!(w->vision->checkCollision(x,y).first))
                    {
                        bSpline* b = dynamic_cast<bSpline*>(w->selectedShape);
                        std::vector<GLfloat> point = {x,y,0.0f};
                        b->addControlPoint(point);
                    
                        w->waitingInput --;

                        w->getMenu()->clear();
                        if(!w->waitingInput)
                        {
                            b->generate();
                            w->setInputType(NONE);
                            w->setMenu(0,HIDDEN);
                        }
                    }
                }
            }break;
        
            case 5:
                if(w->waitingInput)
                {
                    w->waitingInput = 0;
                    w->vision->removeObject(w->selectedShapeID);
                }
                w->clearSelection(); //Desseleciona os objetos
            break;
        }
    }
}


void window::menuClick2(int ID)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela
    if(w)
    {
        if(w->selectedShape)
        {
            switch(ID)
            {
                case 2:
                {
                    bSpline* b = dynamic_cast<bSpline*>(w->selectedShape);
                    b->setControlPointsVisibility((w->getMenu())->getState(ID));
                }break;
                
                case 4:
                {
                    bSpline* b = dynamic_cast<bSpline*>(w->selectedShape);
                    if((w->getMenu())->getState(ID))
                        b->makeClamped();
                    else
                        b->makeUnclamped();
                }break;
            }
        }
    }
}

void window::addSpline(int pointNum, int order, inputType t)
{
    if(pointNum < order)
        throw std::string("IMPOSSIVEL!!!");
    
    this->inType = t;
    this->waitingInput = pointNum;
    this->selectedShapeID  = this->vision->addObject(new bSpline(order));
    this->selectedShape = this->vision->getObject(this->selectedShapeID).second;
}


frame* newSplineMenu()
{
    frame* menu = new frame(1.0f,1.0f, 6, 4,-0.5,0.5);

    menu->addText(0,0,1,4,0,"NOVA B-SPLINE");
    menu->addTextInput(2,0,1,4,1);
    menu->addText(1,0,1,4,2,"Quantidade de Pontos");
    menu->addTextInput(4,0,1,4,3);
    menu->addText(3,0,1,4,4,"Ordem");
    menu->addButton(5,0,1,2,5,"Mouse");
    menu->addButton(5,1,1,2,6,"Teclado");
    menu->addClickFunction(window::menuClick0);
    menu->generate();

    return menu;
}

frame* newAddMenu()
{
    frame* menu = new frame(0.5f,1.0f, 4, 4,-0.5,0.5);

    menu->addText(0,0,1,4,0,"ADICIONAR PONTOS");
    menu->addText(1,0,1,1,1,"X");
    menu->addTextInput(1,1,1,1,2);
    menu->addText(1,2,1,1,3,"Y");
    menu->addTextInput(1,3,1,1,4);
    menu->addButton(3,0,1,2,6,"Cancelar");
    menu->addButton(3,1,1,2,5,"Adicionar");
    menu->addClickFunction(window::menuClick1);
    menu->generate();

    return menu;
}

frame* newOptMenu()
{
    frame* menu = new frame(0.5f,1.0f, 3, 4,-0.5,0.5);

    menu->addText(0,0,1,4,0,"MODIFICAR CURVA");
    menu->addText(1,0,1,3,1,"Mostrar PTS de Controle");
    menu->addToggleButton(1,1,1,1,2,true);
    menu->addText(2,0,1,3,3,"Prender B-Spline");
    menu->addToggleButton(2,1,1,1,4,true);
    menu->addClickFunction(window::menuClick2);
    menu->generate();

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
        case 27:
            if(w->waitingInput)
            {
                w->waitingInput = 0;
                w->vision->removeObject(w->selectedShapeID);
            }
            
            w->clearSelection(); //Desseleciona os objetos
        break;
    
        //Tecla DEL
        case 127:
            try
            {
                w->deleteShape(); //Deleta a forma selecionada
            }catch(std::string s)
            {
                std::cout<<s<<std::endl;
            }
        break;

        //Tecla Espaço
        case 32:
            if(w->menu[w->currentMenu]->visible())
                w->menu[w->currentMenu]->hide();
            else
                w->menu[w->currentMenu]->show();
        break;

        //Tecla ,
        case 44:
            if(w->selectedShape && !w->waitingInput)
                w->setMenu(2);

        break;

    }

    if((key>=65)&& (key<=90) || ((key>=48)&& (key<=57))||((key>=97)&& (key<=122)) || key == 8 || key ==46 ||key==45)
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
    glutIdleFunc(window::redisplay);
    glutReshapeFunc(window::resize);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutSetWindowData(this);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(7);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2);
    
    //this->createMenu();

    //Inicializa o GLEW
    if(glewInit() != GLEW_OK)
    {
        //Falha na inicialização
        throw std::string("Falha na inicialização do GLEW.");
    }

    this->vision= new scene; //Cria uma cena para exibição
    //this->waitingMouse = 0; //Inicializa a flag de espera do mouse em 0
    this->waitingInput = 0;
    this->inType = NONE; 
    this->width = width; //Inicializa a largura da tela
    this->height = height; //Inicializa a altura da tela
    this->selectedShape = nullptr; //Inicializa a forma selecionada como NULL (Nenhuma forma selecionada)
    this->selectedShapeID = 0; //Inicializa o ID de forma selecionada como 0 (Nenhuma forma selecionada)
    this->selBox = new selectionBox();
    this->selBox->setColor(SELECTION_R,SELECTION_G,SELECTION_B);
    this->menu[0] = newSplineMenu();
    this->menu[1] = newAddMenu();
    this->menu[2] = newOptMenu();
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

    for(itMenu = this->menu.begin(); itMenu!= this->menu.end(); itMenu++)
        delete (*itMenu).second; 

    delete this->vision; //Deleta a cena.

    delete this->selBox;
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
    w->updateScene(); //Atualiza a cena
    w->vision->draw(); //Desenha a cena
    w->selBox->draw();
    w->menu[w->currentMenu]->draw();

    glutSwapBuffers(); //Troca os buffers (Exibe na tela)
}

/**
 * @brief Função que executa a janela.
 * 
 */
void window::init()
{
    //std::thread inputThread = std::thread(&window::input,this); //Cria a thread de entrada de dados
    glutMainLoop(); //Roda o loop principal da janela
    closeFlag = true; //Após o fechamento da janela, seta a flag de fechamento como true.
    //std::cout<<"Aperte Enter para Sair"<<std::endl;
    //inputThread.join(); //Espera a thread de entrada de dados terminar.
}

/**
 * @brief Método de entrada de dados
 * 
 */
void window::input()
{
    //Roda enquanto janela de desenho não foi fechada
    while(!(closeFlag))
    {
        std::string in;
        std::getline(std::cin, in);

        std::string temp;
        std::stringstream ss(in);

        //Adiciona a entrada de usuário ao buffer da janela
        while(ss >> temp)
            this->buffer.push_back(temp);
        inputFlag = true; //Seta a flag de dados disponíveis como true
    } 
}


/**
 * @brief Requere o redesenho da tela
 * 
 */
void window::redisplay()
{
    //Se há dados no buffer de entrada, requere que a função de update seja executada.
    if(inputFlag)
    {
        glutPostRedisplay();
        inputFlag = false;
    }
}


/**
 * @brief Função de atualização da cena exibida na tela.
 * 
 */
void window::updateScene()
{

    if(this->mouseQueue.size())
    {
        if(this->selectedShape)
        {

            bSpline* B = dynamic_cast <bSpline*> (this->selectedShape);
            
            if((this->vision->checkCollision(this->mouseQueue[0], this->mouseQueue[1])).first)
            {
                this->mouseQueue.clear(); // Limpa a fila do mouse.
                this->waitingInput = 0;
                
            }else{
                B->addControlPoint(this->mouseQueue);
                //std::cout<<this->mouseQueue.size()<<std::endl;
                this->mouseQueue.clear(); // Limpa a fila do mouse.

                if(!(this->waitingInput))
                {
                    bSpline* B = dynamic_cast<bSpline*> (this->selectedShape);
                    B->generate();
                }
            }   
        }
    }

    /*else if(buffer.size()) //Caso haja entrada do usuário disponível para ser consumida, parsea o buffer
    {  
        
        this->buffer.clear();
        std::cout<<s<<std::endl;
    } */
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

            if(w->menu[w->currentMenu]->visible())
            {   
                w->menu[w->currentMenu]->mouseClick(button,state,xpos,ypos);
            }else{
                std::vector<GLfloat> coordinates = {xpos, ypos, 0.0f};

                if(mouseFlag == 0 && !w->waitingInput)
                {
                    std::cout<<xpos<<" "<<ypos<<std::endl;
                    mouseFlag = 1;
                    w->selBox->addStart(xpos,ypos);
                }

                
        
                //Verifica se houve colisão entre o clique do mouse e algum objeto da cena
                //std::pair<unsigned int, geometry*> temp = w->vision->checkCollision(xpos,ypos);

                /*if(temp.second) //Se houve colisão com uma forma
                {
                    if(w->waitingMouse) //Caso estejam sendo selecionados os vértices de uma forma, invalida a entrada
                    {
                        w->waitingMouse = 0;
                        w->selectedShape = nullptr;
                        w->selectedShapeID = 0;

                        std::cout<< "Ponto invalido (Colisao detectada)." << std::endl;
                        return;
                    }else //Caso não estejam sendo selecionados os vértices de uma forma
                    {
                        std::cout<<"Selecionado"<<std::endl;
                        w->select(temp); //Seleciona a forma.
                    }
                }else if(w->waitingMouse){ //Se não houve colisão, adiciona o clique à fila de cliques
                    
                    w->mouseQueue.push_back(GLfloat(xpos));
                    w->mouseQueue.push_back(GLfloat(ypos));
                    w->mouseQueue.push_back(0.0f);
                    w->waitingMouse--;
                }else{
                    w->select(temp); //Caso não haja colisão e não estejam sendo selecionados
                                    //os vértices de uma forma, limpa a seleção atual
                }*/

                if(w->waitingInput && w->inType == MOUSE)
                { 
                    w->mouseQueue.push_back(GLfloat(xpos));
                    w->mouseQueue.push_back(GLfloat(ypos));
                    w->mouseQueue.push_back(0.0f);
                    w->waitingInput--;
                }
            }
        }

        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        {
            std::cout<<"cheguei pra ficar antes"<<std::endl;
            float xpos, ypos;

            //Mapeia os cliques das coordenadas da janela para as coordenadas normalizadas
            if(w->width > w->height){
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f)*w->aspectRatio;
                ypos =  (-2.0*float(y)/float(w->height)) + 1.0f;
            }else{
                xpos =  ((2.0*float(x)/float(w->width)) - 1.0f);
                ypos =  ((-2.0*float(y)/float(w->height)) + 1.0f)/w->aspectRatio;
            }

            if(mouseFlag == 1 && !w->waitingInput)
            {
                mouseFlag = 0;
                std::pair<unsigned int, geometry*> temp = w->vision->checkCollision(w->selBox);
                w->select(temp);
                std::cout<<"cheguei pra ficar"<<std::endl;
                w->selBox->clearVertex();
            }
        }

        /* if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
            {
                if(w->selectedShape){
                //glutAttachMenu(GLUT_RIGHT_BUTTON);  //createMenu();
                    //createMenu();
                    std::cout<<"teste"<<std::endl;
                }
            }*/

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

void window::clearSelection()
{
    this->selectedShape = nullptr;
    this->selectedShapeID = 0;

    this->setMenu(0,HIDDEN);

    //glutDetachMenu(GLUT_RIGHT_BUTTON);

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


void window::mouseMove(int x, int y)
{
    window* w = (window*)glutGetWindowData(); //Obtém os dados da janela

    if(mouseFlag)
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

        w->selBox->updateLength(xpos,ypos);

        glutPostRedisplay();
    }
}

void selectionBox::draw()
{
    geometry::program->use();
    glUniform3fv(this->colorLoc, 1, &(this->color[0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINE_LOOP, 0, this->vertices.size()/3);
}



void selectionBox::updateLength(GLfloat x, GLfloat y)
{
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


void selectionBox::addStart(GLfloat x, GLfloat y)
{
    std::cout<<"Adicionado"<<std::endl;
    std::vector<GLfloat> coordinates = {x,y,0.0f};
    this->clearVertex();
    this->addVertex(coordinates);
}

void selectionBox::clearVertex()
{
    this->vertices.clear();
}


frame* window::getMenu()
{
    return this->menu[this->currentMenu];
}

void window::setMenu(int ID, menuState state)
{
    this->menu[this->currentMenu]->clear();
    this->menu[this->currentMenu]->hide();
    if(state == VISIBLE)
        this->menu[ID]->show();
    else
        this->menu[ID]->hide();
    this->currentMenu = ID;
}

int window::getWaiting()
{
    return this->waitingInput;
}

void window::setWaiting(int wait)
{
    this->waitingInput = wait;
}

void window::setInputType(inputType t)
{
    this->inType = t;
}