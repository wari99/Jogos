#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo com SETA",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 150, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    int v = 1;    
    int x = 40, y = 40;
    int z = 200, w = 110;
    int continua = 1;
    
    SDL_Event evt;
    
    while (continua) {
        while (SDL_PollEvent(&evt) != 0){
            if(evt.type == SDL_QUIT){
                continua = 0;
            }
        }
        SDL_SetRenderDrawColor(ren, 0x8B, 0x45, 0x13,0x00);
        SDL_RenderClear(ren);
        
        SDL_SetRenderDrawColor(ren, 0x0, 0x0, 0x0, 0x00);
        SDL_Rect r = { 265, 115, 30,30};
        
        SDL_SetRenderDrawColor(ren, 0xA9, 0xA9, 0xA9, 0x00); //Cinza
        SDL_Rect r0 = {270, 120, 20, 20};
        SDL_RenderFillRect(ren, &r0);        
        
        stringRGBA(ren, 275, 125, "->", 0,0,0,255);
        
        filledCircleRGBA(ren, x,y,33, 0,0,0,255); //Borda Preta Alvo Cima
        filledCircleRGBA(ren, z,w,33, 0,0,0,255); //Borda Preta Alvo Meio

        filledCircleRGBA(ren, x,y,30, 220,20,60,255); //Vermelho 30 Cima
        filledCircleRGBA(ren, x,y,20, 240,248,255,255); //Branco 20 Cima
        
        filledCircleRGBA(ren, z,w,30, 220,20,60,255); //Vermelho 30 Meio
        filledCircleRGBA(ren, z,w,20, 240,248,255,255); //Branco 20 Meio
        

        if(x % 2 == 0){
            filledCircleRGBA(ren, x, y,10, 220,20,60,255); //Vermelho Cima 10            
            filledCircleRGBA(ren, z, w,10, 220,20,60,255); //Vermelho Meio 10
            
            filledCircleRGBA(ren, x,y,4, 255,255,0,255); //Bullseye Cima
            filledCircleRGBA(ren, z,w,4, 255,255,0,255); //Bullseye Meio        
        }
        
        if(x % 2 == 1){
            filledCircleRGBA(ren, x,y, 30, 240, 248,255,255);//Branco Cima 30
            filledCircleRGBA(ren, x,y, 20, 220,20,60,255); //Vermelho Cima 20
            filledCircleRGBA(ren, x,y, 10, 240,248,255,255); //Branco CIma 10

            filledCircleRGBA(ren, z,w, 30, 240, 248,255,255);//Branco Meio 30
            filledCircleRGBA(ren, z,w, 20, 220,20,60,255); //Vermelho Meio 20
            filledCircleRGBA(ren, z,w, 10, 240,248,255,255); //Branco Meio 10

            filledCircleRGBA(ren, x,y,4, 0,255,255,255); //Bullseye Cima
            filledCircleRGBA(ren, z,w,4, 0,255,255,255); //Bullseye Meio
        }
        
        SDL_RenderPresent(ren);
        
        SDL_WaitEvent(&evt);
        
        if(evt.type == SDL_KEYDOWN){
            switch (evt.key.keysym.sym){
                case SDLK_RIGHT:
                    z -= 5;
                    x += 5;
                    
                    SDL_SetRenderDrawColor(ren, 0xFF, 0xD7, 0x0, 0x00);
                    SDL_Rect r1 = {270, 120, 20, 20};
                    SDL_RenderFillRect(ren, &r1);
                    
                    stringRGBA(ren, 275, 125, "->", 0,0,0,255);    
            }
        }
        SDL_RenderPresent(ren);
        SDL_Delay(500);
    }
    
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
