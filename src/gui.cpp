#include"gui.h"
#include"geometry.h"
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<GL/glew.h>
#include<vector>
#include<iostream>


frame::frame(GLfloat height, GLfloat width, int numVertical, int numHorizontal, GLfloat xPos, GLfloat yPos, int ID ,float gap):guiElement(width, height,xPos,yPos)
{
    this->grid_w = numHorizontal;
    this->grid_h = numVertical;
    this->vGap = gap*height;
    this->hGap = gap*width;
    this->gap = gap;
    this->ID = ID;
    this->isVisible = false;
    this->element_h = (height-((numVertical+1)*this->vGap))/numVertical;
    this->element_w = (width-((numHorizontal+1)*this->hGap))/numHorizontal;

    this->setColor(FRAME_R, FRAME_G, FRAME_B);

    this->elements = std::vector<std::vector<guiElement*>> (numVertical, std::vector<guiElement*>());
}


void frame::addElement(int vPos, int hPos, guiElement* newElement)
{
    if(!(hPos> (this->grid_w-1) || vPos> (this->grid_h-1)))
    {
        
        while((this->elements[vPos].size())<unsigned(hPos+1))
        {
            this->elements[vPos].push_back(nullptr);
        }

        this->elements[vPos][hPos] = newElement;
    }
}

void frame::addElement(int vPos, int hPos, int vSize, int hSize, int ID)
{
    if(!(hPos> (this->grid_w-1) || vPos> (this->grid_h-1) || hSize > (this->grid_w) || vSize> (this->grid_h)))
    {
        guiElement* newElement = new guiElement(hSize, vSize, ID);//(this->element_w, this->element_h, element_xPos, element_yPos);
        this->addElement(vPos,hPos,newElement);
    }
}

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

int frame::mouseClick(int button, int state, GLfloat xpos, GLfloat ypos)
{
    if(isVisible)
    {
        if(this->collision(xpos,ypos,0.0f))
        {
            std::vector<std::vector<guiElement*>>::iterator it1;
            std::vector<guiElement*>::iterator it2; 

            for(it1 = this->elements.begin(); it1!= this->elements.end(); it1++)
            {
                for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
                {
                    if((*it2)!= nullptr)
                    {
                        if((*it2)->collision(xpos,ypos,0.0f))
                        {
                            if(this->clickFunction)
                                (*it2)->onClick(clickFunction);
                        }else
                        {
                            (*it2)->setClicked(false);
                        }
                    }
                }
            }
        }
    }
}

void frame::addClickFunction(functionPtr fun)
{
    this->clickFunction = fun;
}

//bool guiElement:: collision(GLfloat xpos, GLfloat ypos);
//void guiElement::onClick(void (function*) (int));
void guiElement::generate(GLfloat width, GLfloat height,GLfloat xPos, GLfloat yPos)
{   
    //std::cout<<"generate"<<std::endl;
    this->width = width*hSize;
    this->height = height*vSize;
    this->xPos = xPos;
    this->yPos = yPos;
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


int guiElement::getID()
{
    return this->ID;
}


guiElement::guiElement(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos):square(width, height, xPos, yPos)
{
    this->width = width;
    this->height = height;
    this->xPos = xPos;
    this->yPos = yPos;
    this->clicked = false;
}

guiElement::guiElement(int hSize, int vSize, int ID)
{
    this->hSize = hSize;
    this->vSize = vSize;
    this->ID = ID;
    this->clicked = false;
}

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
                GLfloat width = this->element_w + (((*it2)->getHSize()-1)*hGap/(*it2)->getHSize());
                (*it2)->generate(width,this->element_h, element_xPos, element_yPos);
                element_xPos += (element_w + hGap)*(*it2)->getHSize();
            }else
            {
                element_xPos += this->element_w + this->hGap;
            }
        }
        element_xPos = this->xPos + this->hGap;
        element_yPos -= (this->element_h + this->vGap);
        j++;
    }
}

int guiElement::getHSize()
{
    return this->hSize;
}
int guiElement::getVSize()
{
    return this->vSize;
}


void button::draw()
{
    this->guiElement::draw();

    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f,1.0f,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(0.0f,0.0f,0.0f, 1.0f);

    int length = glutBitmapLength(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text));
    
    glRasterPos2f(this->xPos + ((this->width)-(length*0.00333f))/2.0, this->yPos-this->height/2);

    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void guiElement::draw()
{
    this->square::draw();
}

void frame::addButton(int vpos, int hpos, int vsize,int hsize, int ID, char* text)
{
    button* b = new button(vsize, hsize, ID, text);
    this->addElement(vpos,hpos,b);
}

button::button(int vsize,int hsize, int ID, char* text):guiElement(hsize, vsize, ID)
{
    this->text = text;
    this->setColor(BUTTON_R, BUTTON_G, BUTTON_B);
}


textDisp::textDisp(int vsize,int hsize, int ID, char* text):guiElement(hsize, vsize, ID)
{
    this->text = text;
    this->setColor(FRAME_R, FRAME_G, FRAME_B);
    this->textColor = {TEXT_R, TEXT_G, TEXT_B};
}

void textDisp::setTextColor(GLfloat r, GLfloat g, GLfloat b)
{
    this->textColor[0] = r;
    this->textColor[1] = g;
    this->textColor[2] = b;
}

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

    int length = glutBitmapLength(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text));

    glRasterPos2f(this->xPos + ((this->width)-(length*0.00333f))/2.0, this->yPos-this->height/2);

    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)(this->text));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

}

void frame::addText(int vpos, int hpos, int vsize,int hsize, int ID, char* text)
{
    textDisp* t = new textDisp(vsize, hsize, ID, text);
    this->addElement(vpos,hpos,t);
}

void frame::addTextInput(int vpos, int hpos, int vsize,int hsize, int ID)
{
    textInput* t = new textInput(vsize, hsize, ID);
    this->addElement(vpos,hpos,t);
}


textInput::textInput(int vsize,int hsize, int ID):guiElement(hsize, vsize, ID)
{
    this->setColor(1.0f,1.0f,1.0f);
}

void textInput::addChar(char character)
{
    this->text.push_back(character);
}

void textInput::deleteChar()
{
    if(this->text.size())
        this->text.pop_back();
}

void textInput::draw()
{
    this->guiElement::draw();
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


frame::frame( int vsize, int hsize, int numHorizontal, int numVertical,float gap) : guiElement(hsize,vsize,-1)
{
    this->grid_w= numHorizontal;
    this->grid_h = numVertical;
    this->gap = gap;
    this->clickFunction = nullptr;
    this->setColor(FRAME_R, FRAME_G, FRAME_B);
    this->elements = std::vector<std::vector<guiElement*>> (numVertical, std::vector<guiElement*>());
}

void frame::addSubFrame(int vpos, int hpos, int vsize, int hsize, int numHorizontal, int numVertical, int ID, float gap)
{
    frame* f = new frame(vsize, hsize, numHorizontal, numVertical,gap);
    f->ID = ID;
    this->addElement(vpos, hpos,f);
}

void frame::generate(GLfloat width, GLfloat height, GLfloat xpos, GLfloat ypos)
{
    this->guiElement::generate(width,height,xpos,ypos);
    this->hGap = this->width*this->gap;
    this->vGap = this->height*this->gap;
    this->element_h = (this->height-((this->grid_h+1)*this->vGap))/this->grid_h;
    this->element_w = (this->width-((this->grid_w+1)*this->hGap))/this->grid_w;
    this->generate();
}

frame* frame::getSubFrame(int ID)
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
                    return dynamic_cast<frame*>(*it2);
                }
            }
        }
    }
    return nullptr;
}

void guiElement::onClick(void (*fun) (int))
{
    fun(this->ID);
}

void guiElement::onKeyPress(int){return;}


void textDisp::onClick(functionPtr fun)
{
    this->setClicked(true);
    fun(this->ID);
}

std::string frame::getTextInput(int ID)
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
    
                    textInput* t = dynamic_cast<textInput*>(*it2);
                    if(t)
                        return t->getText();
                }
            }
        }
    }

    return std::string("falhaaaaaa");
}

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
                    std::cout<<"tecla: "<<key<<std::endl; 
                }
            }
        }
    }
}


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

void textInput::onClick(functionPtr fun)
{
    this->setClicked(true);
    fun(this->ID);
}


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


void guiElement::setClicked(bool state)
{
    this->clicked = state;
}

std::string textInput::getText()
{
    return this->text;
}

void frame::hide()
{
    this->isVisible = false;
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

void frame::show()
{
    this->isVisible = true;
}

bool frame::visible()
{
    return this->isVisible;
}
void guiElement::clear()
{
    this->setClicked(false);
}

void textInput::clear()
{
    this->guiElement::clear();
    this->text.clear();
}