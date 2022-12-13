#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	Uint32 depois = 0;
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if(isevt){
		depois = (SDL_GetTicks() - antes);
		if(*ms < depois) depois = *ms;
		*ms -= depois;		
	}
	return isevt;
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("uno reverse card flip",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         350, 130, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "cardflip.png");
    assert(img != NULL);

    /* EXECUÇÃO */
    SDL_Event evt;
    int running = 1, isup = 1, x = -50, espera = 100;

    while (running) {
        SDL_SetRenderDrawColor(ren, 254,221,2,255);
        SDL_RenderClear(ren);
        
        SDL_Rect r = { x,0,110,110 };
        SDL_Rect corte;
        
        //int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);
               
        if(AUX_WaitEventTimeoutCount(&evt,&espera)){       	
			switch (evt.type){
				case SDL_QUIT:
					running = 0;
					break;		
			}
		}
		else{   
			switch(isup){
					case 1:
						corte = (SDL_Rect) {0,0, 110,110};
						break;
					case 2:
						corte = (SDL_Rect) {110,0, 110,100};
						break;
					case 3:
						corte = (SDL_Rect) {220,0,110,110};
						break;
					case 4:
						corte = (SDL_Rect) {330,0,110,110}; 
						break;
					case 5:
						corte = (SDL_Rect) {440,0,110,110}; 
						break;
					case 6:
						corte = (SDL_Rect) {550,0,110,110};   		
						break;
			}isup++;
			if(isup == 6) isup = 1;
	
			SDL_RenderCopy(ren, img, &corte, &r);
			SDL_RenderPresent(ren);
			espera = 100;
			
			x = (x + 10) % 350;
		}
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
