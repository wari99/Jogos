#include <assert.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

enum estadoGoleiro {esperando = 0, agarrando};
enum estadoBola {parada = 0, girando};
enum estadoBarra {off = 0, on};
enum estadoTorcida {up = 0, down};
enum estadoPlayer {vezBatedor = 0, vezGoleiro, aguardando, venceuDisputa, perdeuDisputa};

/*typedef struct dadosPlayer{
	unsigned short int state;
	SDL_Texture* texture;
	struct dadosGoleiro *gol; // Goleiro 1 e Goleiro 2;
}dadosPlayer;*/

typedef struct dadosTorcida{
	SDL_Texture* texture;
	unsigned short int state;
	int aux;
}dadosTorcida;

typedef struct dadosGoleiro{
	SDL_Rect rect;
	SDL_Rect corte;
	SDL_Texture* texture;
	unsigned short int state;
	unsigned short int aux;
	unsigned short int pos;
	
}dadosGoleiro;

typedef struct dadosBola{
	SDL_Rect rect;
	SDL_Texture* texture;
	unsigned short int state;
	int aux;
}dadosBola;

typedef struct dadosBarra{
	SDL_Rect rect;
	SDL_Texture* texture;
	unsigned short int state;
	struct dadosBola bola;
}dadosBarra;

//220b1

void calculaXmov(short int *xMov, dadosBarra barra1){
	int aux = barra1.bola.rect.x + (barra1.bola.rect.w)/2;
	if(aux >= 120 && aux <= 150) *xMov = -8;
	else if(aux > 150 && aux <= 180) *xMov = -6;
	else if(aux > 180 && aux <= 210) *xMov = -4;
	else if(aux > 210 && aux <= 240) *xMov = -2;
	
	else if(aux > 240 && aux <= 270) *xMov = 0;
	
	else if(aux > 270 && aux <= 300) *xMov = 2;
	else if(aux > 300 && aux <= 330) *xMov = 4;
	else if(aux > 330 && aux <= 360) *xMov = 6;
	else if(aux > 360 && aux <= 390) *xMov = 8;
}

void calculaForca(short int *forca, dadosBarra barra2){
	int aux = barra2.bola.rect.y + (barra2.bola.rect.h)/2;
	if(aux >= 87 && aux <= 117) *forca = 75;
	else if(aux > 117 && aux <= 147) *forca = 70;
	else if(aux > 147 && aux <= 177) *forca = 65;
	
	else if(aux > 177 && aux <= 207) *forca = 60;
	
	else if(aux > 207 && aux <= 237) *forca = 55;
	else if(aux > 237 && aux <= 267) *forca = 50;
	else if(aux > 267 && aux <= 297) *forca = 40;
	else if(aux > 297 && aux <= 327) *forca = 30;
}

void calculaStatusBall(int *statusBall, dadosBola bola){ //0E 1B 2C 3A 4F 5D 
	int auxX = bola.rect.x - (bola.rect.w)/2;
	int auxY = bola.rect.y - (bola.rect.h)/2;
	if(auxX>=11 && auxX<=170 && auxY>=66 && auxY<=152){
		//printf("\nEspaco A!");
		*statusBall = 3;
	}
	else if(auxX>=171 && auxX <= 330 && auxY>= 66 && auxY<= 152){
		//printf("\nEspaco B!");
		*statusBall = 1;
	} //
	else if(auxX>=331 && auxX<=491 && auxY>=66 && auxY<=152){
		//printf("\nEspaco C!");
		*statusBall = 2;
	}

	else if(auxX >=11 && auxX<=170 && auxY>=152 && auxY<=238){
		//printf("\nEspaco D!");
		*statusBall = 5;
	}
	else if(auxX>=171 && auxX <= 330 && auxY>= 152 && auxY<= 238){
		//printf("\nEspaco E!");
		*statusBall = 0;
	}
	else if(auxX>=331 && auxX<=491 && auxY>=152 && auxY<=238){
		//printf("\nEspaco F!");
		*statusBall = 4;
	}
	else{
		//printf("\nFora!");
		*statusBall = 6;
	}	
}

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

void rodaJogo(SDL_Renderer* ren, bool *menu, bool *running, bool *gameIsRunning){
	//cria torcida
	dadosTorcida torcida;
	torcida.texture = IMG_LoadTexture(ren, "torcidaarquibancada1.png");
	torcida.state = up;
	torcida.aux = 0;
	//cria goleiro
	dadosGoleiro goleiro;
	goleiro.rect = (SDL_Rect) {150, 100, 200,170};
	goleiro.corte = (SDL_Rect) {0,0,200,170};
	goleiro.texture = IMG_LoadTexture(ren, "goleiros200x170.png");
	goleiro.state = esperando;
	goleiro.aux = 0;
	
	//if(vezGoleiro) goleiro.texture = IMG_LoadTexture(ren, "outrogoleiro200x170.png");

	//cria barra1
	dadosBarra barra1;
	barra1.rect = (SDL_Rect) {120,250,270,140};
	barra1.texture = IMG_LoadTexture(ren, "barra.png");
	barra1.state = on;
	barra1.bola.rect = (SDL_Rect) {200, 300, 40,40};
	barra1.bola.texture = IMG_LoadTexture(ren, "bola.png");
	
	//cria barra2
	dadosBarra barra2;
	barra2.rect = (SDL_Rect) {515,87,140,240};
	barra2.texture = IMG_LoadTexture(ren, "barra2.png");
	barra2.state = off;
	barra2.bola.rect = (SDL_Rect) {570, 150, 40,40};
	barra2.bola.texture = IMG_LoadTexture(ren, "bola.png");

	//cria bola
	dadosBola bola;
	bola.rect = (SDL_Rect) {220, 300, 40,40};
	bola.texture = IMG_LoadTexture(ren, "bola.png");
	bola.state = parada;
	bola.aux = 0;
	
	SDL_Texture* img = IMG_LoadTexture(ren, "golpngetal.png");
	SDL_Texture* imgbola = IMG_LoadTexture(ren, "bola.png");
	
	
	bool isDoneJumping = false;
	SDL_Point mouse = {0,0};
	int espera = 200;
	int x3 = 400, y3 = 300;
	short int forca = 1;
	short int xMov = 0;
	int statusBall = 0, isMoving = 1; 
	int x,y,dx,dy; 
	Uint32 antes = 0;
	short int cont;
	
	unsigned int goalCountT1 = 0; 
	unsigned int saveCountT1 = 0;
	
	unsigned int goalCountT2 = 0;
	unsigned int saveCountT2 = 0;
    /* EXECUÇÃO */
    
	//SDL_Rect rBX = {x3, y3, 40,40}; // Bola Eixo X
	//SDL_Rect rBY = {520,50, 40,40}; // Bola Eixo Y
	
	
	assert(img != NULL);	
	assert(imgbola != NULL);
		
	int contadorX = 140;
	int contadorY = 87;
	SDL_Rect rBX = {x3, y3, 40,40}; // Bola Eixo X
	
	short int bolaaux = 1;
	short int bolaauxY = 1;
	
	while(*running){
			espera = MAX(espera - (int)(SDL_GetTicks() - antes), 0);
		  	SDL_Event evt; 
			if(torcida.state == up) torcida.texture = IMG_LoadTexture(ren, "torcidaarquibancada1.png");
			else torcida.texture = IMG_LoadTexture(ren, "torcidaarquibancada2.png");
		  	antes = SDL_GetTicks();
			SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
			SDL_RenderClear(ren);
			SDL_RenderCopy(ren, torcida.texture, NULL, NULL);//FUNDO DE TELA
			SDL_RenderCopy(ren, img, NULL, NULL);//FUNDO DE TELA
			
			SDL_RenderCopy(ren, goleiro.texture, &goleiro.corte, &goleiro.rect);
			if(barra1.state == on){
				SDL_RenderCopy(ren, barra1.texture, NULL, &barra1.rect);// BOLA X
				SDL_RenderCopy(ren, barra1.bola.texture, NULL, &barra1.bola.rect);// BOLA X
			}else{
				SDL_RenderCopy(ren, bola.texture, NULL, &bola.rect);// BOLA do jogo
			}
			
			if(barra1.state == on || barra2.state == on){
				SDL_RenderCopy(ren, barra2.texture, NULL, &barra2.rect);// BOLA Y
				SDL_RenderCopy(ren, barra2.bola.texture, NULL, &barra2.bola.rect);// BOLA Y
			}

			//bola.rect = (SDL_Rect) {100, 100, 40,40};
			//bola.texture = IMG_LoadTexture(ren, "bola.png");
			if(bola.aux == forca){
				
				calculaStatusBall(&statusBall, bola);
				printf("\n *Status ball: %d* *goleiro.pos: %d* ",statusBall,goleiro.pos);
				if(statusBall == goleiro.pos) printf("\nAgarrou!!!\n");
				else if((statusBall >= 0 && statusBall < 6) && statusBall != goleiro.pos ) printf("Gol!!!");
				else if(statusBall == 6) printf("\nFora!!! -\n\n");
			}
				
			SDL_RenderPresent(ren);
		  	int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);    
		    if(isevt){ 	
				switch (evt.type){
					case SDL_WINDOWEVENT:
		            	if (SDL_WINDOWEVENT_CLOSE == evt.window.event){
							*running = false;
							*gameIsRunning = false;
							*menu = false;
						} break;	
					case SDL_MOUSEBUTTONDOWN:
						if(evt.button.button == SDL_BUTTON_LEFT){
							SDL_GetMouseState(&mouse.x, &mouse.y);
						}
						break;
						case SDL_MOUSEBUTTONUP:
							SDL_GetMouseState(&mouse.x, &mouse.y);
							if(evt.button.button == SDL_BUTTON_LEFT){
								if(evt.button.state==SDL_RELEASED){									
									if(barra2.state == on) barra2.state = off;	
											
								 	if(barra1.state == on){
								 		barra1.state = off;
								 		barra2.state = on;
								 	}
									if(goleiro.state == agarrando) {
										goleiro.state = esperando;
										goleiro.rect.x = 150;
										goleiro.rect.y = 100;
									}
									break;
								}
							}
						case SDL_KEYDOWN:
							if(evt.key.keysym.sym == SDLK_ESCAPE){
								case SDLK_ESCAPE:
								*menu = true;
								*running = false;
							}
							break;
				}
			}
			else{
				if(barra1.state == on){
					//isDoneJumping = false;
					barra1.bola.rect.x += 5 * bolaaux;

			   		if(barra1.bola.rect.x >= 370 || barra1.bola.rect.x <= 100) {
		   				bolaaux *= -1;
				  		
					}
		   		}	
		   		
		   		if(barra2.state == on){
		   			barra2.bola.rect.y += 5 * bolaauxY;
		   			if(barra2.bola.rect.y >= 307   || barra2.bola.rect.y <= 87) {
		   				bolaauxY *= -1;
		   			}
		   		}
		   		
		   		if(barra1.state == off && barra2.state == off && bola.state != girando){
		   			bola.state = girando;
		   			calculaXmov(&xMov, barra1);
		   			calculaForca(&forca,barra2); 
		   		}
		   		
		   		if(bola.state == girando && bola.aux <= forca){// && bola.aux <= forca
					bola.rect.x += xMov;
					bola.rect.y -= 3;
					bola.aux++;
				}
		   		
		   		if(goleiro.aux == 30 && goleiro.state == esperando){
					switch(isMoving){
						case 1:
							goleiro.corte = (SDL_Rect) {0,0,200,170};
							break;
						case 2:
							goleiro.corte = (SDL_Rect) {200,0,200,170};
							break;
						default:
							isMoving = 0;
							break;
					}isMoving++;
					goleiro.aux = 0;
				}
				if(goleiro.state == agarrando && goleiro.aux == 2){
					goleiro.aux = 0;
					//caminho do goleiro
					switch(goleiro.pos){ 
						case 0: //Meio baixo
							if(goleiro.rect.y >= 90) goleiro.rect.y -= 9;
							break;
						case 1: //Meio cima
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 9;
							break;
						case 2: //Angulo direito
							if(goleiro.rect.x <= 300) goleiro.rect.x += 15;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 5;
							break;
						case 3: //Angulo esquerdo
							if(goleiro.rect.x >= 0) goleiro.rect.x -= 15;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 5;
							break;
						case 4: //Canto direito	
							if(goleiro.rect.x <= 280) goleiro.rect.x += 15;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 4;
							break;
						case 5: //Canto esquerdo
							if(goleiro.rect.x >= 10) goleiro.rect.x -= 15;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 4;
							break;
						//isDoneJumping = true;
					}
				}	
				(goleiro.aux)++;

				/* //Verificando se agarrou. 				
				switch(statusBall){
					case 1: //A
						if(goleiro.pos == 3) saveCountT1++;
						else goalCountT1++;
						break;
					case 2: //B
						if(goleiro.pos == 1) saveCountT1++;
						else goalCountT1++;
						break;
					case 3: //C
						if(goleiro.pos == 2) saveCountT1++;
						else goalCountT1++;
						break;					
					case 4: //D
						if(goleiro.pos == 5) saveCountT1++; 
						else goalCountT1++;
						break;					
					case 5: //E
						if(goleiro.pos == 0) saveCountT1++;
						else goalCountT1++;
						break;					
					case 6: //F
						if(goleiro.pos == 5) saveCountT1++;
						else goalCountT1++;
						break;					
					case 7: //Fora
						saveCountT1++; 
						break;
					}				
				
				}*/
				
				if(torcida.aux == 15){			
					if(torcida.state == down) {
						torcida.state = up;
					}
					else {
						torcida.state = down;
					}
					torcida.aux = 0;
				}
				(torcida.aux)++;
				espera = 20;
			
			}
		if(goleiro.state == esperando && barra1.state == off && barra2.state == off ){
			goleiro.state = agarrando;
			goleiro.aux = 0;
			goleiro.pos = rand() % 6;	
			if(goleiro.pos == 0) goleiro.corte = (SDL_Rect) {0,170,200,170};
			else if(goleiro.pos == 1) goleiro.corte = (SDL_Rect) {200,170,200,170};
			else if(goleiro.pos == 2) goleiro.corte = (SDL_Rect) {0,340,200,170};
			else if(goleiro.pos == 3) goleiro.corte = (SDL_Rect) {200,340,200,170};
			else if(goleiro.pos == 4) goleiro.corte = (SDL_Rect) {0,510,200,170};
			else if(goleiro.pos == 5) goleiro.corte = (SDL_Rect) {200,510,200,170};		
		}
	}

	SDL_DestroyTexture(img);
 	SDL_DestroyTexture(imgbola);

}

void chamaMenu(SDL_Renderer* ren, bool *menu, bool *running, bool *gameIsRunning){
    TTF_Init();
    SDL_Color padrao = { 0,0,0,255 };
    SDL_Color focus = { 255,255,255,255 };
    
    
    TTF_Font *ourFont = TTF_OpenFont("Mont-HeavyDEMO.otf",100);
    struct SDL_Surface* listaSurfaceText[3];
    listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",padrao);  
    listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "About",padrao); 
    listaSurfaceText[2] = TTF_RenderText_Solid(ourFont, "Quit",padrao); 

 	SDL_Texture* bgmenu = IMG_LoadTexture(ren, "menufundo.png");
    struct SDL_Texture* listaTextureText[3];
	listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
    listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
    listaTextureText[2] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[2]);
    
    int i;
    SDL_Point mouse = {0,0};
    SDL_Rect recPlay = {200,210,100,30};
    SDL_Rect recAbout = {310,210,100,30};
    SDL_Rect recQuit = {420,210,100,30};
    
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
    SDL_Window* win = SDL_CreateWindow("Soccer: Match Decider",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         700, 350, SDL_WINDOW_SHOWN
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

int main (int argc, char* args[]){
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
  	SDL_Window* win = create_window();
    SDL_Renderer* ren = create_renderer(win);

	bool menu = true;
	bool running = false;
	bool gameIsRunning = true;
	
    while(gameIsRunning){
    
		chamaMenu(ren,&menu,&running,&gameIsRunning);
		rodaJogo(ren,&menu,&running,&gameIsRunning);
		
	}
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
	
