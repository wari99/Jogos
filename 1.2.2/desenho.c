#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Teste COR!",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_SetRenderDrawColor(ren, 0xFF, 0xB6, 0xC1,0x00); //Fundo rosa bebe 
    SDL_RenderClear(ren);
 
    roundedBoxRGBA(ren, 80,80, 300, 300, 20, 221,160,221,255); // Contorno caixa com bordas redondas
 
    filledTrigonRGBA(ren, 115, 155, 205, 305, 245, 105, 0,0,0,255); // Borda diagonais preta  
    filledTrigonRGBA(ren, 120, 150, 200, 300, 250, 100, 253,253,150, 255);   // Triangulo amarelo

    SDL_SetRenderDrawColor(ren, 0xBF, 0xBC, 0x8B, 0x00); //Retangulo Verde ZEBRA 
    SDL_Rect r1 = {90, 340, 180, 30};
    SDL_RenderFillRect(ren, &r1);
    SDL_RenderPresent(ren);
    
    SDL_SetRenderDrawColor(ren, 0xBF, 0xBC, 0x8B,0x00); //Retangulo Verde PLAZA
    SDL_Rect r0 = { 90,390, 180,30};
    SDL_RenderFillRect(ren, &r0);
    SDL_RenderPresent(ren);

    SDL_SetRenderDrawColor(ren, 0xBF, 0xBC, 0x8B, 0x00); //Retangulo Verde PIZZA
    SDL_Rect r2 = {90, 440, 180, 30};
    SDL_RenderFillRect(ren, &r2);
    SDL_RenderPresent(ren);
    
    SDL_SetRenderDrawColor(ren, 0x80, 0x80, 0x80, 0x00); //Retangulo Cinza
    SDL_Rect rCINZA = {90, 340, 60, 30};
    SDL_RenderFillRect(ren, &rCINZA);
    SDL_RenderPresent(ren);
       
    SDL_SetRenderDrawColor(ren, 0xB2, 0x22, 0x22, 0x00); // Erro Plaza
    SDL_Rect r00 = {130, 390, 60, 30};
    SDL_RenderFillRect(ren, &r00);
    SDL_RenderPresent(ren);
    
    SDL_SetRenderDrawColor(ren, 0xB2, 0x22, 0x22, 0x00); //Erro Zebra
    SDL_Rect r01 = {130, 340, 95, 30};
    SDL_RenderFillRect(ren, &r01);
    SDL_RenderPresent(ren); 

    stringRGBA(ren, 100, 350, " Z   E   B   R   A", 240, 255, 255, 255);    
    stringRGBA(ren, 100, 400, " P   L   A   Z   A", 240, 255, 255, 255);

    stringRGBA(ren, 100, 450, " P   I   Z   Z   A", 240, 255, 255, 255);
    
    stringRGBA(ren, 120,40, "Um desenho qualquer com a biblioteca SDL2_gfx", 168,110,247,255);
    
    thickLineRGBA(ren, 370, 300, 400,400, 100,  186,211,255,255); //Fundo Azul
    filledPieRGBA(ren, 390, 320,  50, 0, 290, 251,218,95,255);
       

    arcRGBA(ren, 400, 300, 20, 40, 100, 106,90,205,255);
    arcRGBA(ren, 400, 300, 50, 40, 100, 238,130,238,255);
    arcRGBA(ren, 400, 300, 35, 5, 180, 144,238,144,255);

    filledCircleRGBA(ren, 200, 160, 13, 178,34,34,255); //Detalhe Pizza
    filledCircleRGBA(ren, 180, 220, 13, 178,34,34,255); //Detalhe Pizza

    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
