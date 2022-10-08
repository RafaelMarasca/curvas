/**
 * @file main.cpp
 * @author Rafael Marasca Martins e Lucas Carvalho
 * @brief Função principal, inicializa o programa.
 * @version 0.1
 * @date 2022-09
 */

#include"scene.h"
#include"window.h"
#include<thread>
#include<iostream>


int main(int argc, char**argv)
{

    //Try catch para capturar as possíveis exceções geradas pela classe window
    try{
        //Cria uma janela com tamanho inicial 250x250
        window a(600,600, std::string("Hello Curves").c_str(), &argc, argv);
        //Inicializa a janela e a exibe na tela
        a.init(); 
    }catch(std::string b){
        //Exibe as exceções no terminal
        std::cout<<b<<std::endl;
    };
    
    return 0;
}