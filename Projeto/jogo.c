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
}dadosBola;

typedef struct dadosBarra{
	SDL_Rect rect;
	SDL_Texture* texture;
	unsigned short int state;
	struct dadosBola bola;
}dadosBarra;

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

	//cria barra1
	dadosBarra barra1;
	barra1.rect = (SDL_Rect) {156,250,200,140};
	barra1.texture = IMG_LoadTexture(ren, "barra.png");
	barra1.state = on;
	barra1.bola.rect = (SDL_Rect) {200, 300, 40,40};
	barra1.bola.texture = IMG_LoadTexture(ren, "bola.png");
	
	//cria barra2
	dadosBarra barra2;
	barra2.rect = (SDL_Rect) {515,87,140,200};
	barra2.texture = IMG_LoadTexture(ren, "barra2.png");
	barra2.state = off;
	barra2.bola.rect = (SDL_Rect) {570, 150, 40,40};
	barra2.bola.texture = IMG_LoadTexture(ren, "bola.png");
		
	//cria bola
	dadosBola bola;
	SDL_Texture* img = IMG_LoadTexture(ren, "golpngetal.png");
	SDL_Texture* imgbola = IMG_LoadTexture(ren, "bola.png");
	
	//cria torcida
	
	bool selecionado = false;
	
	bool isCountingY = true;
	SDL_Point mouse = {0,0};
	int espera = 200;
	int x3 = 400, y3 = 300;
	
	int statusBall = 0, isMoving = 1; 
	int x,y,dx,dy; 
	Uint32 antes = 0;
	short int cont;
    /* EXECUÇÃO */
    
	SDL_Rect rBX = {x3, y3, 40,40}; // Bola Eixo X
	SDL_Rect rBY = {520,50, 40,40}; // Bola Eixo Y
	
	assert(img != NULL);	
	assert(imgbola != NULL);
		
	int contadorX = 140;
	int contadorY = 87;
	
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
			SDL_RenderCopy(ren, barra1.texture, NULL, &barra1.rect);// BOLA X
			SDL_RenderCopy(ren, barra2.texture, NULL, &barra2.rect);// BOLA X
			SDL_RenderCopy(ren, barra1.bola.texture, NULL, &barra1.bola.rect);// BOLA X
			SDL_RenderCopy(ren, barra2.bola.texture, NULL, &barra2.bola.rect);// BOLA X
					
			
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
						//isCounting = false;
						if(evt.button.button == SDL_BUTTON_LEFT){
							SDL_GetMouseState(&mouse.x, &mouse.y);
		    				
							/*if(SDL_PointInRect(&mouse,&rB)) {
								dx=rB.x-mouse.x;
								dy=rB.y-mouse.y;
								selecionado = true;	
								printf("\nBola clicada");
							}*/
						}
						break;
						case SDL_MOUSEBUTTONUP:
							SDL_GetMouseState(&mouse.x, &mouse.y);
							if(evt.button.button == SDL_BUTTON_LEFT){
								if(evt.button.state==SDL_RELEASED){
									selecionado = false;	
									
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
									rBX.x = contadorX;
									
									if(rBX.x>=11 && rBX.x<=170 && rBX.y>=66 && rBX.y<=152){
										printf("\nEspaco A!");
										statusBall = 1;
										break;
									}
									else if(rBX.x>=171 && rBX.x <= 330 && rBX.y>= 66 && rBX.y<= 152){
										printf("\nEspaco B!");
										statusBall = 2;
										break;
									}
									else if(rBX.x>=331 && rBX.x<=491 && rBX.y>=66 && rBX.y<=152){
										printf("\nEspaco C!");
										statusBall = 3;
										break;
									}

									else if(rBX.x>=11 && rBX.x<=170 && rBX.y>=152 && rBX.y<=238){
										printf("\nEspaco D!");
										statusBall = 4;
										break;
									}
									else if(rBX.x>=171 && rBX.x <= 330 && rBX.y>= 152 && rBX.y<= 238){
										printf("\nEspaco E!");
										statusBall = 5;
										break;
									}
									else if(rBX.x>=331 && rBX.x<=491 && rBX.y>=152 && rBX.y<=238){
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
							if(SDL_PointInRect(&mouse,&rBX) && selecionado){
								rBX.x=mouse.x+dx;
								rBX.y=mouse.y+dy;
							
							}	
							break;
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
					barra1.bola.rect.x += 5 * bolaaux;

			   		if(barra1.bola.rect.x >= 330 || barra1.bola.rect.x <= 140) {
		   				bolaaux *= -1;
				  		printf("\n %d ", barra1.bola.rect.x); 
					}
			   		//barra1.bola.rect.x = contadorX;				   		
		   		}	
		   		
		   		if(barra2.state == on){
		   			barra2.bola.rect.y += 5 * bolaauxY;
		   			if(barra2.bola.rect.y >= (272 - barra2.bola.rect.h)   || barra2.bola.rect.y <= 87) {
		   				printf("\n yyyyyyyyyy %d", barra2.bola.rect.y);
		   				bolaauxY *= -1;
		   			}
		   			//barra2.bola.rect.y = contadorY;
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
				if(goleiro.state == agarrando && goleiro.aux == 5){
					goleiro.aux = 0;
					//caminho do goleiro
					
					switch(goleiro.pos){
						case 0:
							if(goleiro.rect.y >= 90) goleiro.rect.y -= 12;
							break;
						case 1: //Agarrando meio cima
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 12;
							break;
						case 2: //Angulo direito
							if(goleiro.rect.x <= 300) goleiro.rect.x += 16;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 6;
							break;
						case 3: //Angulo esquerdo
							if(goleiro.rect.x >= 0) goleiro.rect.x -= 16;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 6;
							break;
						case 4: //Canto direito	
							if(goleiro.rect.x <= 300) goleiro.rect.x += 16;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 4;
							break;
						case 5: //Canto esquerdo
							if(goleiro.rect.x >= 0) goleiro.rect.x -= 15;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 3;
							
							break;
							
					}
				}	
				(goleiro.aux)++;
				
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
			
			printf("\ngoleiro.pos = %d\n",goleiro.pos);
			
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
    SDL_Window* win = SDL_CreateWindow("ProjetoP2",
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
//srand(time(NULL));
int main (int argc, char* args[])
{
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
