#include <SDL2/SDL.h>
#include <time.h>

typedef struct rects{
	SDL_Rect m;
	int cores[3];

}rects;


int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Quadrados Coloridos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Event evt;
    SDL_Rect r = {40,20, 10,10};
    
    int executando = 1;
    
    int x,y;
    int i = 0; //contar quadrados
    
    rects rec[10];

    while (executando) {
    	
        SDL_SetRenderDrawColor(ren, 0x0, 0x0, 0x0 ,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
       

        SDL_WaitEvent(&evt);
	
	for(int k = 0;k < 10;k++){
		SDL_SetRenderDrawColor(ren, rec[k].cores[0],rec[k].cores[1],rec[k].cores[2],255); // rec.cores
        	SDL_RenderFillRect(ren, &(rec[k].m));	
	}
	
	SDL_RenderPresent(ren);
	if(evt.type == SDL_MOUSEBUTTONDOWN){
	    if(i < 10){
	        int x , y;
	        SDL_GetMouseState(&x, &y);
	        rec[i].m.x = x;
	        rec[i].m.y = y;
	        rec[i].m.h = 10;
	        rec[i].m.w = 10;
	        for(int f=0; f < 3;f++){
	        	rec[i].cores[f] = rand() % 255;
	        }
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
		    
        }
        	        
        switch (evt.type){
            case SDL_QUIT:
                executando = 0;
                break;
            case SDL_KEYDOWN:
            	if(evt.key.keysym.sym == SDLK_F4){
            		if(evt.key.keysym.mod == KMOD_LALT){
            		
            		}
            	}
        }
        
    }
}
