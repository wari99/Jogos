#include <assert.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>
#define MAX(x,y) (((x) > (y)) ? (x) : (y))


int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
    Uint32 antes = SDL_GetTicks();
    SDL_FlushEvent(SDL_MOUSEMOTION);
    if (SDL_WaitEventTimeout(evt, *ms)) {
		*ms = MAX(0, *ms - (int)(SDL_GetTicks() - antes));
		return 1;
    } return 0;
}

void mudaCor(SDL_Renderer* ren,SDL_Surface* listaS[],SDL_Texture* listaT[],SDL_Color cor,int i,char nome[],TTF_Font *ourFont){
	listaS[i] = TTF_RenderText_Solid(ourFont, nome,cor);  
	listaT[i] = SDL_CreateTextureFromSurface(ren,listaS[i]);
}


void chamaMenu(SDL_Renderer* ren, bool *menu, bool *running, bool *gameIsRunning){
    TTF_Init();
    SDL_Color padrao = { 0,0,0,255 };
    //SDL_Color focus = { 228,232,112,255 };
    SDL_Color focus = { 255,255,255,255 };
    //TTF_Font *ourFont = TTF_OpenFont("Gameshow.ttf",100);
    TTF_Font *ourFont = TTF_OpenFont("Mont-HeavyDEMO.otf",100);
    struct SDL_Surface* listaSurfaceText[3];
    listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",padrao);  
    listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "About",padrao); 
    listaSurfaceText[2] = TTF_RenderText_Solid(ourFont, "Quit",padrao); 

 	SDL_Texture* bgmenu = IMG_LoadTexture(ren, "niltonsantosalvinegro.jpg");
    struct SDL_Texture* listaTextureText[3];
	listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
    listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
    listaTextureText[2] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[2]);
    
    int i;
    SDL_Point mouse = {0,0};
    SDL_Rect recPlay = {110,210,100,30};
    SDL_Rect recAbout = {220,210,100,30};
    SDL_Rect recQuit = {330,210,100,30};
    
    bool selecionado = false;
    
     /* EXECUÇÃO */
    while (*menu) {
    	SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					*gameIsRunning = false;
					*menu = false;
					*running = false;
				break;
				case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mouse.x,&mouse.y);
						if(SDL_PointInRect(&mouse,&recPlay)){
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,0,"Play",ourFont);
					   	}

					   	else if(SDL_PointInRect(&mouse,&recAbout)){
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,1,"About",ourFont);					   	
					   	}
					   	
					   	else if(SDL_PointInRect(&mouse,&recQuit)){
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,2,"Quit",ourFont);

					   	}
					   	else{
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,0,"Play",ourFont);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,1,"About",ourFont);					   	
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,2,"Quit",ourFont);
							selecionado = false;
					   	}
				break;
				case SDL_MOUSEBUTTONDOWN:	
					if(event.button.button==SDL_BUTTON_LEFT){
						if(SDL_PointInRect(&mouse,&recQuit)) selecionado = true;
						else if(SDL_PointInRect(&mouse,&recPlay)) selecionado = true;
					}
				case SDL_MOUSEBUTTONUP:	
					if(event.button.button==SDL_BUTTON_LEFT){
						if(event.button.state==SDL_RELEASED){
							if(SDL_PointInRect(&mouse,&recQuit) && selecionado) {
								*menu = false;
								*gameIsRunning = false;
								*running = false;
							}
							else if(SDL_PointInRect(&mouse,&recPlay) && selecionado) {
								*menu = false;
								*running = true;
							}
						}
					}
				break;
			}
		}
		SDL_RenderCopy(ren,bgmenu,NULL,NULL);
		SDL_RenderCopy(ren,listaTextureText[0],NULL,&recPlay);
		SDL_RenderCopy(ren,listaTextureText[1],NULL,&recAbout);
		SDL_RenderCopy(ren,listaTextureText[2],NULL,&recQuit);
		SDL_RenderPresent(ren);
	}	

	for(i = 0; i < 3;i++){
		SDL_FreeSurface(listaSurfaceText[i]);
		SDL_DestroyTexture(listaTextureText[i]);
	}
	mouse.x = mouse.y = 0;
	TTF_CloseFont(ourFont);
	SDL_DestroyTexture(bgmenu);
	TTF_Quit();
}

SDL_Window* create_window(void) {
    SDL_Window* win = SDL_CreateWindow("ProjetoP2",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 350, SDL_WINDOW_SHOWN
                      );

    if(win==NULL) {
        printf("Janela não foi criada.\nSDL_Error: %s\n", SDL_GetError());
    }

    return win;
}

SDL_Renderer* create_renderer(SDL_Window* win) {
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    if(win==NULL) {
        printf("Janela não foi criada.\nSDL_Error: %s\n", SDL_GetError());
    }

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    return ren;
}
//srand(time(NULL));
int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
  	SDL_Window* win = create_window();
    SDL_Renderer* ren = create_renderer(win);
	SDL_Texture* img = IMG_LoadTexture(ren, "campo1.png");
	SDL_Texture* img2 = IMG_LoadTexture(ren, "goleiro.png");
	SDL_Texture* imgbola = IMG_LoadTexture(ren, "bola.png");
	/*assert(img != NULL);	
	assert(img2 != NULL);	
	assert(imgbola != NULL);*/
	bool menu = true;
	bool running = false;
	bool gameIsRunning = true;
	bool selecionado = false;
	SDL_Point mouse = {0,0};
	int espera = 200;
	int x1 = 200;
	int x3 = 400, y3 = 250;
	int statusBall = 0;
	int isMoving = 1; 
	int x,y,dx,dy; 
	Uint32 antes = 0;
	short int cont;
    /* EXECUÇÃO */
	SDL_Rect rB = {x3, y3, 40,40};
	SDL_Rect corte2 = {0,0,100,150};
	SDL_Rect r2 = {x1, 100, 100,150};
    while(gameIsRunning){
    
		chamaMenu(ren,&menu,&running,&gameIsRunning);
		
		while(running){
			espera = MAX(espera - (int)(SDL_GetTicks() - antes), 0);
		  	SDL_Event evt; 

		  	antes = SDL_GetTicks();
			SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
			SDL_RenderClear(ren);

			SDL_RenderCopy(ren, img, NULL, NULL);//FUNDO DE TELA
			
			SDL_RenderCopy(ren, img2, &corte2, &r2);
			SDL_RenderCopy(ren, imgbola, NULL, &rB);// FUNDO BOLA
			SDL_RenderPresent(ren);
		  	int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);    
		    if(isevt){       	
				switch (evt.type){
					case SDL_WINDOWEVENT:
		            	if (SDL_WINDOWEVENT_CLOSE == evt.window.event){
							running = false;
							gameIsRunning = false;
							menu = false;
						} break;	
					case SDL_MOUSEBUTTONDOWN:
						if(evt.button.button == SDL_BUTTON_LEFT){
							SDL_GetMouseState(&mouse.x, &mouse.y);
							if(SDL_PointInRect(&mouse,&rB)) {
								dx=rB.x-mouse.x;
								dy=rB.y-mouse.y;
								selecionado = true;	
								printf("\nBola clicada");
							}
						}
						break;
					//case SDL_MOUSEBUTTONUP: acao goleiro random number + mov bola
						case SDL_MOUSEBUTTONUP:
							SDL_GetMouseState(&mouse.x, &mouse.y);
							if(evt.button.button == SDL_BUTTON_LEFT){
								if(evt.button.state==SDL_RELEASED){
									selecionado = false;
									if(rB.x>=11 && rB.x<=170 && rB.y>=66 && rB.y<=152){
										printf("\nEspaco A!");
										statusBall = 1;
										break;
									}
									else if(rB.x>=171 && rB.x <= 330 && rB.y>= 66 && rB.y<= 152){
										printf("\nEspaco B!");
										statusBall = 2;
										break;
									}
									else if(rB.x>=331 && rB.x<=491 && rB.y>=66 && rB.y<=152){
										printf("\nEspaco C!");
										statusBall = 3;
										break;
									}

									else if(rB.x>=11 && rB.x<=170 && rB.y>=152 && rB.y<=238){
										printf("\nEspaco D!");
										statusBall = 4;
										break;
									}
									else if(rB.x>=171 && rB.x <= 330 && rB.y>= 152 && rB.y<= 238){
										printf("\nEspaco E!");
										statusBall = 5;
										break;
									}
									else if(rB.x>=331 && rB.x<=491 && rB.y>=152 && rB.y<=238){
										printf("\nEspaco F!");
										statusBall = 6;
										break;
									}
									else{
										printf("\nFora!");
										statusBall = 7;
										break;
									}
									break;
								}
							}
						case SDL_MOUSEMOTION:
							SDL_GetMouseState(&mouse.x, &mouse.y);
							if(SDL_PointInRect(&mouse,&rB) && selecionado){
								rB.x=mouse.x+dx;
								rB.y=mouse.y+dy;
							
							}	
							break;
						case SDL_KEYDOWN:
							if(evt.key.keysym.sym == SDLK_ESCAPE){
								case SDLK_ESCAPE:
								menu = true;
								running = false;
							}
							break;
				}
			}
			else{	
				switch(isMoving){
					case 1:
						corte2 = (SDL_Rect) {0,0,100,150};
						x1 = 220;
						break;
					case 2:
						corte2 = (SDL_Rect) {100,0,100,150};
						x1 = 230;
						break;
					default:
						isMoving = 0;
						break;
				}isMoving++;
			espera = 400;
			
			}
		
		}
	}
    /* FINALIZACAO */
 	SDL_DestroyTexture(img);
 	SDL_DestroyTexture(img2);
 	SDL_DestroyTexture(imgbola);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
