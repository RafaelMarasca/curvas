#ifndef GUI_H
#define GUI_H

#include"geometry.h"
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<GL/glew.h>
#include<vector>
#include<string>
#include<map>

#define FRAME_R 0.2156f
#define FRAME_G 0.1451f
#define FRAME_B 0.3333f

#define BUTTON_R 0.9529f
#define BUTTON_G 0.7922f
#define BUTTON_B 0.2510f

#define TOGGLE_ON_R 0.5921f
#define TOGGLE_ON_G 1.0f
#define TOGGLE_ON_B 0.5647f

#define TOGGLE_OFF_R 0.8549f
#define TOGGLE_OFF_G 0.2667f
#define TOGGLE_OFF_B 0.3137f

#define TEXT_R 1.0f
#define TEXT_G 1.0f
#define TEXT_B 1.0f

typedef void (*functionPtr) (int);


class guiElement : public square
{
    protected:
        int vSize;
        int hSize;
        int ID;
        GLfloat width;
        GLfloat height;
        bool clicked;
        
    public:
        GLfloat xPos;
        GLfloat yPos;
        guiElement(int hSize, int vSize, int ID);
        guiElement(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos);
        virtual void onClick(void (*) (int));
        virtual void onKeyPress(int);
        virtual void generate(GLfloat width, GLfloat height, GLfloat xpos, GLfloat yPos);
        int getHSize();
        int getVSize();
        int getID();
        virtual void setClicked(bool state);
        virtual void draw();
        virtual void clear();
};


class frame : public guiElement
{
    private:
        int grid_w;
        int grid_h;
        GLfloat element_h;
        GLfloat element_w;
        GLfloat vGap;
        GLfloat hGap;
        GLfloat gap;
        functionPtr clickFunction;
        
        bool isVisible; 

        std::vector<std::vector<guiElement*>> elements;

        void addElement(int vPos, int hPos, guiElement* element);

    public:
        frame(GLfloat width, GLfloat height, int numVertical, int numHorizontal, GLfloat xPos, GLfloat yPos, int ID = -1, float gap = 0.05f);
        frame(int vsize, int hsize, int numHorizontal, int numVertical,float gap = 0.05f);
        virtual ~frame();
        //void addElement(int grid_w, int grid_h, guiElement* element);
        void addElement(int vPos, int hPos, int vSize, int hSize, int ID);

        void show();
        void draw();
        void hide();
        void generate();
        void generate(GLfloat width, GLfloat height, GLfloat xpos, GLfloat yPos);

        void addButton(int vpos, int hpos, int vsize,int hsize, int ID, const char* text);
        void addButton(int vpos, int hpos, int vsize,int hsize, int ID, const char* text, std::vector<GLfloat>color);
        void addToggleButton(int vpos, int hpos, int vsize,int hsize, int ID, bool state);
        void addText(int vpos, int hpos, int vsize,int hsize, int ID, const char* text);
        void addTextInput(int vpos, int hpos, int vsize,int hsize, int ID);
        void addSubFrame(int vpos, int hpos, int vsize, int hsize, int numHorizontal, int numVertical, int ID, float gap = 0.05f);

        void addClickFunction(functionPtr fun);
        int mouseClick(int button, int state, GLfloat xpos, GLfloat ypos);
        void keyPress(int key);
        bool visible();
        void clear();

        std::string getTextInput(int ID);
        bool getState(int ID);
        
        //frame* getSubFrame(int ID);
        
        
};


class button : public guiElement
{
    private:
        std::string text;
        std::vector<GLfloat> color;

    public:
        button(int vsize,int hsize, int ID, const char* text,std::vector<GLfloat>color);
        button(int vsize,int hsize, int ID, const char* text);
        void setText(const char*);
        void draw();
};

class textDisp : public guiElement
{
    private:
        std::string text;
        std::vector<GLfloat> textColor;

    public:
        textDisp(int vsize,int hsize, int ID, const char* text, std::vector<GLfloat>color);
        textDisp(int vsize,int hsize, int ID, const char* text);
        void onClick(functionPtr fun);
        void setTextColor(GLfloat r, GLfloat g, GLfloat b);
        void draw();
};

class textInput : public guiElement
{
    private:
        std::string text;
        std::vector<GLfloat> textColor;

    public:
        textInput(int vsize,int hsize, int ID);
        void onKeyPress(int);
        void onClick(functionPtr fun);
        void addChar(char character);
        void deleteChar();
        void draw();
        void setClicked(bool state);
        void clear();
        std::string getText();
};

class toggleButton : public button
{
    private:
        bool state;
    public:
        toggleButton(int vsize,int hsize, int ID,bool state);
        bool getState();
        void setState(bool state);
        void onClick(functionPtr fun);
        void toogle();
};



#endif //GUI_H