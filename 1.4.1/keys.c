#include <SDL2/SDL.h>
#include <time.h>
//Removido Poll; Testes abaixo do waitevent; Testes no switch; Até 10 quadrados; Todos mudam de cor com movimento do mouse 
int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Quadrados Coloridos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 200, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Event evt;
    SDL_Rect r = {40,20, 10,10};
    
    int executando = 1;
    int x,y;
    int i = 0; //Contar quadrados
    
    struct SDL_Rect rec[10];

    while (executando) {
    	
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
       

        SDL_WaitEvent(&evt);
	
	for(int k = 0;k < 10;k++){
		SDL_SetRenderDrawColor(ren, rand() % 255,rand() % 255,rand() % 255,255);
        	SDL_RenderFillRect(ren, &rec[k]);	
	}
	
	SDL_RenderPresent(ren);
	if(evt.type == SDL_MOUSEBUTTONDOWN){
	    if(i < 10){
	        int x , y;
	        SDL_GetMouseState(&x, &y);
	        rec[i].x = x;
	        rec[i].y = y;
	        rec[i].h = 10;
	        rec[i].w = 10;
	    }
	    i++;

	    SDL_RenderPresent(ren);
	}        
        switch (evt.type){
            case SDL_KEYDOWN: 
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
		    
	    case SDL_QUIT:
	        executando = 0;
	        break;
        }
        
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
