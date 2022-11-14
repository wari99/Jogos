#include <SDL2/SDL.h>

int continua = 500;
int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
    if (SDL_WaitEventTimeout(evt, continua)){
    	continua -= (SDL_GetTicks() - *ms);
    	return 1;
    } else {
    	continua = 500;
    	return 0;
    }
}

int main (int argc, char* args[]){
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("WaitEventTimeout",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         250, 200, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    
    SDL_Rect r = { 10,10, 10,10 };

    while (1) {
    
        SDL_SetRenderDrawColor(ren, 0x0, 0x0, 0x0,0x00);
        SDL_RenderClear(ren);
        
        SDL_Rect r0 = {10,10, 230,180};
        SDL_SetRenderDrawColor(ren, 0x94, 0x0, 0xD3, 0x00);
        SDL_RenderFillRect(ren, &r0);
        
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFA, 0xF0,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        
        Uint32 time = SDL_GetTicks();
        int isevt = AUX_WaitEventTimeoutCount(&evt, &time);
        
        if (isevt){
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        r.y -= 5;
                        break;
                    case SDLK_DOWN:
                        r.y += 5;
                        break;
                    case SDLK_LEFT:
                        r.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        r.x += 5;
                        break;
                }
            }
            
            else if (evt.type == SDL_QUIT){
                break;
            }
        } 
        
        else {
            r.x += 2;
            r.y += 2;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
