#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    
    SDL_Rect r = { 40,20, 10,10 };
    
    int x = 20, y = 20;
    
    while (r.x < 70) {
        SDL_SetRenderDrawColor(ren, 0xFF, 0xB6,0xC1,0x00);
        SDL_RenderClear(ren);
        
        filledCircleRGBA(ren, x,y,10, 253,253,150,255);
                
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        if(r.x % 2 == 0){
            SDL_SetRenderDrawColor(ren, 0xDC, 0x14, 0x3C, 0x00);
            SDL_RenderFillRect(ren, &r);
            SDL_RenderPresent(ren);   
            
            filledCircleRGBA(ren, x, y,10, 220,20,60,255);
            SDL_RenderPresent(ren);
        }
        
        
        SDL_Delay(500);
        r.x += 2;
        r.y += 0;
        
        x += 2;
        y += 3;
        
    }
    

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
