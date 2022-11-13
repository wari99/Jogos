#include <assert.h>
#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Contando o Tempo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 300, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    int continua = 1;  
    SDL_Rect r1 = { 40,20, 10,10 };
    SDL_Rect r2 = { 80,40, 20,20 };
    SDL_Rect r3 = { 50,100, 30,30 };
    int espera = 500;

    SDL_Event evt;
    Uint32 antes = SDL_GetTicks();
    int isevt = SDL_WaitEventTimeout(&evt, espera);    

    while (continua) {
        while(SDL_PollEvent(&evt) != 0){ //Fechar o programa
            if(evt.type == SDL_QUIT){
                continua = 0;
            }
        }
            	
        SDL_SetRenderDrawColor(ren, 0x0, 0x0, 0x0,0x00);
        SDL_RenderClear(ren);
        
        SDL_SetRenderDrawColor(ren, 0x4B, 0x0, 0x82, 0x00);
        SDL_Rect r0 = {10,10, 280,280};
        SDL_RenderFillRect(ren, &r0);
        
        SDL_SetRenderDrawColor(ren, 0xFF, 0x0, 0xFF,0x00);
        SDL_RenderFillRect(ren, &r1);
        
        SDL_SetRenderDrawColor(ren, 0x93, 0x70, 0xDB, 0x00);
        SDL_RenderFillRect(ren, &r2);
        
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);
        SDL_RenderFillRect(ren, &r3);
        
        SDL_RenderPresent(ren);

        Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);
        
        if (isevt) {
            espera -= (SDL_GetTicks() - antes);
            if (espera < 0) {
                espera = 0;
            }
            
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        r1.y -= 5;
                        break;
                    case SDLK_DOWN:
                        r1.y += 5;
                        break;
                    case SDLK_LEFT:
                        r1.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        r1.x += 5;
                        break;
                }
            }
            
            if(evt.type == SDL_MOUSEMOTION){
                int x, y;
                SDL_GetMouseState(&x, &y);
                r3.x = x;
                r3.y = y;
            }
     
        }
        
        else {
            espera = 500;
            r2.x += 1;
            r2.y += 1;
            SDL_RenderPresent(ren);
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
