#include <assert.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

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

//srand(time(NULL));
int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("SOCCER SHOOTOUTS",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 350, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	SDL_Texture* img = IMG_LoadTexture(ren, "campo1.png");
	SDL_Texture* img2 = IMG_LoadTexture(ren, "goleiro.png");
	SDL_Texture* imgbola = IMG_LoadTexture(ren, "bola.png");
	assert(img != NULL);
	 
	int running = 1;

	int espera = 200;
	int x1 = 150;
	int x3 = 400, y3 = 250;
	
	int statusBall = 0;
	int isMoving = 1; 
	int x,y,dx,dy; 
	
    /* EXECUÇÃO */

	while(running){
		SDL_Event evt;
		SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
		SDL_RenderClear(ren);

		SDL_Rect corte = {0,0,500,350};
		SDL_RenderCopy(ren, img, &corte, NULL);
		
		SDL_Rect corte2 = {0,0,210,200};
		SDL_Rect r2 = {x1, 50, 260,200};
	
		SDL_Rect cortebola = {0,0,40,40};
		SDL_Rect rB = {x3, y3, 40,40};
		SDL_RenderCopy(ren, imgbola, &cortebola, &rB);
		
	
        if(AUX_WaitEventTimeoutCount(&evt,&espera)){       	
			switch (evt.type){
				case SDL_QUIT:
					running = 0;
					break;		
				case SDL_MOUSEBUTTONDOWN:
					if(evt.button.button == SDL_BUTTON_LEFT){
						SDL_GetMouseState(&x, &y);
						x3 = x - 20;
						y3 = y - 20;
						//dx = rB.x - x;
						//dy = rB.y - y;

						if(x>=11 && x<=170 && y>=66 && y<=152){
							printf("\nEspaco A!");
							statusBall = 1;
							break;
						}
						else if(x>=171 && x <= 330 && y>= 66 && y<= 152){
							printf("\nEspaco B!");
							statusBall = 2;
							break;
						}
						else if(x>=331 && x<=491 && y>=66 && y<=152){
							printf("\nEspaco C!");
							statusBall = 3;
							break;
						}

						else if(x>=11 && x<=170 && y>=152 && y<=238){
							printf("\nEspaco D!");
							statusBall = 4;
							break;
						}
						else if(x>=171 && x <= 330 && y>= 152 && y<= 238){
							printf("\nEspaco E!");
							statusBall = 5;
							break;
						}
						else if(x>=331 && x<=491 && y>=152 && y<=238){
							printf("\nEspaco F!");
							statusBall = 6;
							break;
						}
						else{
							printf("\nFora!");
							statusBall = 7;
							break;
						}
					}
				//case SDL_MOUSEBUTTONUP: acao goleiro random number + mov bola
					case SDL_MOUSEBUTTONUP:
						if(evt.button.button == SDL_BUTTON_LEFT){
							break;
						}
					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&x, &y);
						
						rB.x = x;
						rB.y = y;
						break;
			}
		}
		else{
			switch(isMoving){
				case 1:
					corte2 = (SDL_Rect) {0,0,210,200};
					x1 = 120;
					break;
				case 2:
					corte2 = (SDL_Rect) {210,0,210,200};
					x1 = 130;
					break;
				case 3:
					isMoving = 0;
					break;
			}isMoving++;

		SDL_RenderCopy(ren, img2, &corte2, &r2);
		SDL_RenderPresent(ren);
		espera = 300;

		}
	}

    /* FINALIZACAO */
 	SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
