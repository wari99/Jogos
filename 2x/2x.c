#include <assert.h>
#include <SDL2/SDL2_gfxPrimitives.h>
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

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("click drag drop",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 400, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    int continua = 1, espera = 500; 
    int x,y,dx,dy;
	int selecionou = 0, clicou = 0, arrastou = 0, soltou = 0, cancelou = 0;
	
    SDL_Rect r1 = { 200, 200, 40,40 };    

    /* EXECUÇÃO */
    while (continua) {    
    	SDL_SetRenderDrawColor(ren, 0x93,0x70,0xDB, 0x00);
		SDL_RenderClear(ren);   
	    
	    SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);
		SDL_RenderFillRect(ren, &r1);
		
		roundedBoxRGBA(ren, 140,20, 260,80, 5, 0,0,0,255);
		SDL_Rect placa = {0,20, 400,5};
		SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
		SDL_RenderFillRect(ren, &placa);
		
		stringRGBA(ren, 150,25, "STATUS RECT: ", 255,255,255,255);
		stringRGBA(ren, 150,40, "Clicou!", 55,55,55,255);
		stringRGBA(ren, 150,50, "Arrastando...", 55,55,55,255);
		stringRGBA(ren, 150,60, "Soltou!", 55,55,55,255);
		stringRGBA(ren, 150,70, "Cancelou!", 55,55,55,255);
		
		if(clicou && !arrastou) stringRGBA(ren,150,40,"Clicou!",0,255,0,255);
		if(arrastou) stringRGBA(ren, 150,50, "Arrastando...", 0,255,0,255);
		if(soltou) stringRGBA(ren, 150,60, "Soltou!", 0,255,0,255);
		if(cancelou) stringRGBA(ren, 150,70, "Cancelou!", 0,255,0,255);
		
		SDL_RenderPresent(ren);
		SDL_Event evt;
		
		if(AUX_WaitEventTimeoutCount(&evt, &espera)){
			switch(evt.type){
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&x, &y);
					if(x>=r1.x && x<=r1.x+r1.w && y>=r1.y && y<=r1.y+r1.h){
						selecionou = 1;
						arrastou = soltou = cancelou = 0;
						dx = r1.x - x;
						dy = r1.y - y;
						break;
					}
				case SDL_MOUSEBUTTONUP:
						if(evt.button.button == SDL_BUTTON_LEFT){
							if(arrastou && selecionou){
								soltou = 1;
							}
							else if(selecionou){
								clicou = 1;
							}
						}
						arrastou = selecionou = 0;
						break;
				case SDL_MOUSEMOTION: 
					SDL_GetMouseState(&x, &y);
					if(selecionou == 1){
						clicou = cancelou = 0;
						arrastou = 1;
						r1.x = x + dx;
						r1.y = y + dy;
						break;
					}
				case SDL_KEYDOWN:
					if(evt.key.keysym.sym == SDLK_ESCAPE){
						cancelou = 1;
						selecionou = arrastou = soltou = clicou = 0;
						break;
					}
			}	
		}	
			if(evt.type == SDL_QUIT){
					continua = 0;
					break;
			}
}

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
