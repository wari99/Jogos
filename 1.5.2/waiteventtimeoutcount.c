#include <SDL2/SDL.h>
#include <stdbool.h>

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	Uint32 depois = 0;
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if(isevt){
		depois = (SDL_GetTicks() - antes);
		if(*ms < depois) depois = *ms;
		*ms -= depois;		
	}
	else{
		*ms = 500;
		
	}
	return isevt;
}


int main (int argc, char* args[]){
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("WaitEventTimeout",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 300, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    int espera = 500;
    SDL_Rect r1 = { 40,20, 10,10 };
    SDL_Rect r2 = { 80,40, 20,20 };
    SDL_Rect r3 = { 50,100, 30,30 };
    SDL_Event evt;
    bool continua = true;
    int x, y;
	/* EXECUÇÃO */
    while (continua) {
            	
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

       
        
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        
       if (isevt) {
            switch (evt.type ) {
                case SDL_MOUSEMOTION:
                	SDL_GetMouseState(&x, &y);
                	r3.x = x;
                	r3.y = y;
                case SDL_KEYDOWN:
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
        }
        
        else {
            espera = 300;
            r2.x += 3;
            r2.y += 3;
            SDL_RenderPresent(ren);
        }
        
        /* FINALIZAÇÃO */
        switch (evt.type ) {
            case SDL_QUIT:
            	continua = false;
            	break;
            case SDL_KEYDOWN:
            	if(evt.key.keysym.sym == SDLK_F4){
            		if(evt.key.keysym.mod == KMOD_LALT){
		    		continua = false;
		    		break;	
            			}		
			}
        }
        
    }
    
}
