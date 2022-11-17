#include <SDL2/SDL.h>
//Removido Poll; Testes abaixo do waitevent
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
    SDL_Event evt;
    SDL_Rect r = {40,20, 10,10};
    
    int x,y;
    int i = 0; //Contar quadrados
    
    typedef struct estruturaquadrado{
        int posicaox;
        int posicaoy;
        //SDL_Rect r1 = {posicaox, posicaoy, 10, 10};

    }estruturaquadrado;
    
    estruturaquadrado quadrados[10];

    while (1) {
    	
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);
	
	if(evt.type == SDL_MOUSEBUTTONDOWN){
	    if(i < 10){
	        SDL_GetMouseState(&x, &y);
	        quadrados[i].posicaox = x;
	        quadrados[i].posicaoy = y;
	    }
	    i++;

	    SDL_RenderPresent(ren);
	}        
        else if (evt.type == SDL_KEYDOWN) {
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
        
        else if (evt.type == SDL_QUIT){
            break;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
