#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         250, 150, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    
    int x = 40, y = 40;
    int z = 200, w = 110;
    
    while (x < 200) {
        SDL_SetRenderDrawColor(ren, 0x8B, 0x45, 0x13,0x00);
        SDL_RenderClear(ren);
        
        filledCircleRGBA(ren, x,y,33, 0,0,0,255); //Borda Preta Alvo Cima
        filledCircleRGBA(ren, z,w,33, 0,0,0,255); //Borda Preta Alvo Baixo

        filledCircleRGBA(ren, x,y,30, 220,20,60,255); //Vermelho 30 Cima
        filledCircleRGBA(ren, x,y,20, 240,248,255,255); //Branco 20 Cima
        
        filledCircleRGBA(ren, z,w,30, 220,20,60,255); //Vermelho 30 Baixo
        filledCircleRGBA(ren, z,w,20, 240,248,255,255); //Branco 20 Baixo


        SDL_RenderPresent(ren);

        if(x % 2 == 0){
            filledCircleRGBA(ren, x, y,10, 220,20,60,255); //Vermelho Cima 10            
            filledCircleRGBA(ren, z, w,10, 220,20,60,255); //Vermelho Baixo 10
            
            filledCircleRGBA(ren, x,y,4, 255,255,0,255); //Bullseye Cima
            filledCircleRGBA(ren, z,w,4, 255,255,0,255); //Bullseye Baixo

                       
            SDL_RenderPresent(ren);         
        }
        
        if(x % 2 == 1){
            filledCircleRGBA(ren, x,y, 30, 240, 248,255,255);//Branco Cima 30
            filledCircleRGBA(ren, x,y, 20, 220,20,60,255); //Vermelho Cima 20
            filledCircleRGBA(ren, x,y, 10, 240,248,255,255); //Branco CIma 10

            filledCircleRGBA(ren, z,w, 30, 240, 248,255,255);//Branco Baixo 30
            filledCircleRGBA(ren, z,w, 20, 220,20,60,255); //Vermelho Baixo 20
            filledCircleRGBA(ren, z,w, 10, 240,248,255,255); //Branco Baixo 10

            filledCircleRGBA(ren, x,y,4, 0,255,255,255); //Bullseye Cima
            filledCircleRGBA(ren, z,w,4, 0,255,255,255); //Bullseye Baixo
        
            SDL_RenderPresent(ren);
        }
          
        SDL_Delay(500);
        
        x += 5;
        y += 0;
        
        z -= 5;
        w -= 0;
        
    }
    

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
