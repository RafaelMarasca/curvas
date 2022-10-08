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
#include<cmath>
#include"curves.h"

int closeFlag = false; //Inicializa a flag de fechamento
int inputFlag = 0; //Inicializa a flag de entrada
int mouseFlag = 0;
int currentMenu = 1;
float pixels = 0;



void teste(int i)
{
    std::cout<<"Clicado: "<<i<<std::endl;
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
            w->clearSelection(); //Desseleciona os objetos
        break;
    
        //Tecla DEL
        case 127:
            try
            {
                //Deleta a forma selecionada
                w->deleteShape();
            }catch(std::string s)
            {
                std::cout<<s<<std::endl;
            }
        break;



        case 32:
            /*w->waitingMouse = 4;
            w->selectedShapeID  = w->vision->addObject(new bSpline(3));
            if(w->selectedShapeID == 0)
                std::cout<<"erro 404";
            w->selectedShape = w->vision->getObject(w->selectedShapeID).second;
            std::cout<<"isso eu fiz";*/
            if(w->menu->visible())
                w->menu->hide();
            else
                w->menu->show();
            
        break;
    }

    if((key>=65)&& (key<=90) || ((key>=48)&& (key<=57))||((key>=97)&& (key<=122)) || key == 8)
        w->menu->keyPress(key);

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
    this->waitingMouse = 0; //Inicializa a flag de espera do mouse em 0
    this->width = width; //Inicializa a largura da tela
    this->height = height; //Inicializa a altura da tela
    this->selectedShape = nullptr; //Inicializa a forma selecionada como NULL (Nenhuma forma selecionada)
    this->selectedShapeID = 0; //Inicializa o ID de forma selecionada como 0 (Nenhuma forma selecionada)
    this->selBox = new selectionBox();
    this->selBox->setColor(SELECTION_R,SELECTION_G,SELECTION_B);
    //this->console = new menu(-0.9f,-0.9f);
    //this->console->setText(MENU1);
    //pixels = 2.0f/width;
    //std::cout<<pixels;
    
    //this->menu = new frame(1.0f,1.0f, 1, 4,-0.5,0.5);

    this->menu = new frame(1.0f,1.0f, 6, 4,-0.5,0.5);
   
    this->menu->addSubFrame(0,0,1,4,1,1,0);


    //this->menu->setHGap(0.0f);
    frame* f = menu->getSubFrame(0);
    this->menu->addText(0,0,1,4,0,"NOVA B-SPLINE");
    this->menu->addTextInput(2,0,1,4,1);
    this->menu->addText(1,0,1,4,2,"Quantidade de Pontos");
    this->menu->addTextInput(4,0,1,4,3);
    this->menu->addText(3,0,1,4,4,"Ordem");
    this->menu->addButton(5,0,1,2,5,"Mouse");
    this->menu->addButton(5,1,1,2,6,"Teclado");
    this->menu->addClickFunction(teste);
    this->menu->generate();
}





/**
 * @brief Destrutor para a classe Window.
 * 
 */
window::~window()
{
    sceneIterator it;
    std::pair<unsigned int, geometry*> object;
    
    //Itera por todos os objetos da cena atual e os deleta.
    for(it = this->vision->begin(); it!= this->vision->end(); it++)
    {
        object = this->vision->getObject((*it).first);
        delete object.second;
        object.second = nullptr;   
    }

    delete this->vision; //Deleta a cena.

    delete this->selBox;
    //delete this->console;
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
    w->menu->draw();
   // w->console->draw();


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
                this->waitingMouse = 0;
                
            }else{
                B->addControlPoint(this->mouseQueue);
                //std::cout<<this->mouseQueue.size()<<std::endl;
                this->mouseQueue.clear(); // Limpa a fila do mouse.

                if(!this->waitingMouse)
                {
                    bSpline* B = dynamic_cast <bSpline*> (this->selectedShape);
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

            w->menu->mouseClick(button,state,xpos,ypos);

            std::vector<GLfloat> coordinates = {xpos, ypos, 0.0f};

            if(mouseFlag == 0 && !w->waitingMouse)
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

            if(w->waitingMouse)
            { 
                w->mouseQueue.push_back(GLfloat(xpos));
                w->mouseQueue.push_back(GLfloat(ypos));
                w->mouseQueue.push_back(0.0f);
                w->waitingMouse--;
            }
        }

        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
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

            if(mouseFlag == 1 && !w->waitingMouse)
            {
                mouseFlag = 0;
                std::pair<unsigned int, geometry*> temp = w->vision->checkCollision(w->selBox);
                w->select(temp);
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
        glutDetachMenu(GLUT_RIGHT_BUTTON);

    glutPostRedisplay(); //Requere que a tela seja redesenhada.
}

void window::clearSelection()
{
    this->selectedShape = nullptr;
    this->selectedShapeID = 0;

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
        pixels = 2.0f/newHeight;
    }else
    {
        GLfloat aux = GLfloat(newHeight-newWidth)/2.0f;
        glViewport(0, aux, newWidth, newWidth);
        pixels = 2.0f/newWidth;
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

/*
void window::createMenu()
{
    window* w = (window*)glutGetWindowData();

    glutCreateMenu(splineManagement);
    glutAddMenuEntry("Prender", 0);
    glutAddMenuEntry("Desprender", 1);
    glutAddMenuEntry("Mostrar Pontos de Controle", 2);
    glutAddMenuEntry("Esconder Pontos de Controle", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}*/


/*
void window::splineManagement(int option)
{
    window* w = (window*)glutGetWindowData();
    
    if(w->selectedShape)
    {
        bSpline *B = dynamic_cast<bSpline *>(w->selectedShape);;
        switch (option)
        {
        case 0:
            B->makeClamped();
            break;

        case 1:
            B->makeUnclamped();
            break;

        case 2:
            B->setControlPointsVisibility(true);
            break;

        case 3:
            B->setControlPointsVisibility(false);
            break;
        }
        glutPostRedisplay();
    }
    
}*/
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
        //std::cout<<"ATUALIZADO"<<std::endl;

        this->vertices.erase(this->vertices.begin()+3, this->vertices.end());
        std::vector<GLfloat> coordinates = {x, this->vertices[1], this->vertices[2]};
        this->addVertex(coordinates);
        coordinates[1] = y;
        this->addVertex(coordinates);
        coordinates[0] = this->vertices[0];
        this->addVertex(coordinates);

        //for(int i = 0; i<this->vertices.size(); i++)
          //  std::cout<<this->vertices[i]<<" ";
        //std::cout<<std::endl;
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


menu::menu(GLfloat xPos, GLfloat yPos)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->color = {MENU_DEFAULT_R, MENU_DEFAULT_G, MENU_DEFAULT_B};
}

void menu::setText(std::string str)
{
    this->text = str;
}

void menu::addChar(char character)
{
    this->text.push_back(character);
}

void menu::deleteChar()
{
    if(this->text.size())
    {
        this->text.pop_back();
    }
}

void menu::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    this->color = {MENU_DEFAULT_R, MENU_DEFAULT_G, MENU_DEFAULT_B};
}

void menu::draw()
{
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f,1.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(this->color[0],this->color[1],this->color[2], 0.4f);
    glRasterPos2f(this->xPos, this->yPos);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text.c_str()));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}