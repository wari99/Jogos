//1.2.1: Faça um desenho qualquer com linhas, pontos e retângulos, usando cores diversas.

#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Teste COR!",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 300, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_SetRenderDrawColor(ren, 0x19,0x19,0x70,0x00); //Fundo DarkOliveGreen
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 0xB8, 0x86, 0x70,0x00); //Cabelo P
    SDL_Rect r2 = { 80,150, 150,30 };
    SDL_RenderFillRect(ren, &r2);
    SDL_RenderPresent(ren);    
    
    SDL_SetRenderDrawColor(ren, 0xB8, 0x86, 0x70,0x00); //Cabelo Ponta2
    SDL_Rect r22 = { 55,180, 200,30 };
    SDL_RenderFillRect(ren, &r22);
    SDL_RenderPresent(ren);        

    SDL_SetRenderDrawColor(ren, 0xB8, 0x86, 0x70,0x00); //Cabelo Cima
    SDL_Rect r0 = { 120,120, 60,30 };
    SDL_RenderFillRect(ren, &r0);
    SDL_RenderPresent(ren); 
    
    SDL_SetRenderDrawColor(ren, 0xFF, 0x69, 0xB4, 0x00); //Mudando a cor do ponto

    SDL_RenderDrawPoint(ren,80,40);  //Estrelas
    SDL_RenderDrawPoint(ren,25,80);    
    SDL_RenderDrawPoint(ren,120,75);    
    SDL_RenderDrawPoint(ren,55,95); 
       
    SDL_RenderDrawPoint(ren,200,100);
    SDL_RenderDrawPoint(ren, 170, 35);
    SDL_RenderDrawPoint(ren, 190, 25);
    
    SDL_RenderDrawPoint(ren, 280, 55);
    

    SDL_RenderDrawLine(ren, 50, 50, 70, 70);
    SDL_RenderDrawLine(ren, 30, 70, 50, 50);     
    SDL_RenderDrawLine(ren, 30, 55, 70, 55); // Linha reta horizontal estrela
    SDL_RenderDrawLine(ren, 30, 55, 70, 70);
    SDL_RenderDrawLine(ren, 70, 55, 30, 70);
        
    SDL_SetRenderDrawColor(ren, 0xFF, 0xDA, 0xB9,0x00); //Cabeca
    SDL_Rect r1 = { 128,130, 45,55 };
    SDL_RenderFillRect(ren, &r1);
    SDL_RenderPresent(ren);
    
    
    SDL_SetRenderDrawColor(ren, 0xDC, 0xDC, 0xDC, 0x00); //Base Chapeu
    SDL_Rect chap1 = {90, 115,  120, 12}; 
    SDL_RenderFillRect(ren, &chap1);
    SDL_RenderPresent(ren);  
    
    SDL_SetRenderDrawColor(ren, 0xFF, 0xFA, 0xF0, 0x00); //Ponta Chapeu
    SDL_Rect chap2 = {135, 70, 30, 50};
    SDL_RenderFillRect(ren, &chap2);
    SDL_RenderPresent(ren);

    SDL_Delay(5000);


    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
