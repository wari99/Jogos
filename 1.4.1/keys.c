#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Event evt;
//    srand(time(NULL));
    
    int continua = 1;
    SDL_Event evento;
    
    int cont = 0;
    int x,y;
    
    struct quadrado{
        SDL_Rect r;
        //int r, g, b;
    };
    struct quadrado quadrados[10];
    
    while (continua) {
    	while(SDL_PollEvent(&evento)){
    		if(evento.type == SDL_QUIT){
    			continua = 0;
    		}
    	}
    	
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

	if(evento.type == SDL_MOUSEBUTTONDOWN){
		x = evento.button.x;
		y = evento.button.y;
		
		SDL_Rect quadrado = {x, y, 10, 10};
		
		SDL_RenderPresent(ren);
		
	}

        SDL_WaitEvent(&evt);
        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    if(r.y> 0){
                    	r.y -= 5;
                    }
                    break;
                case SDLK_DOWN:
                    if(r.y< 90){
                    	r.y += 5;
                    }
                    break;
                case SDLK_LEFT:
                    if(r.x> 0){
                    	r.x -= 5;
                    }
                    break;
                case SDLK_RIGHT:
                    if(r.x < 190){
                    	r.x += 5;
                    }
                    break;
            }
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
