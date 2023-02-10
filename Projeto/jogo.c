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
// gcc jogo.c -lSDL2 -lSDL2_image -lSDL2_ttf -o jogo.exe

enum estadoGoleiro {esperando = 0, agarrando};
enum estadoBola {parada = 0, girando};
enum estadoBarra {off = 0, on};
enum estadoTorcida {up = 0, down};
enum estadoPlayer {vezBatedor = 0, vezGoleiro,aguardando, venceuDisputa, perdeuDisputa};

typedef struct dadosPlayer{
	unsigned short int state;
	unsigned short int pontuacao;
}dadosPlayer;

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

typedef struct setorGol{
	SDL_Rect rect;
	bool selecionado;
}setorGol;

void constroi(SDL_Rect setor[]){
	setor[3] = (SDL_Rect) {211,66,160,86}; //Angulo Esquerdo
	setor[1] = (SDL_Rect) {371,66,160,86}; //Meio Cima
	setor[2] = (SDL_Rect) {531,66,160,86}; //Angulo Direito
	setor[5] = (SDL_Rect) {211,152,160,86}; //Canto esquerdo
	setor[0] = (SDL_Rect) {371,152,160,86}; //Meio Baixo
	setor[4] = (SDL_Rect) {531,152,160,86}; //Canto direito
}

void calculaXmov(short int *xMov, dadosBarra barra1){ //MUDEI
	int aux = barra1.bola.rect.x + (barra1.bola.rect.w)/2;
	if(aux >= 220 && aux <= 250) *xMov = -8;
	else if(aux > 250 && aux <= 380) *xMov = -6;
	else if(aux > 280 && aux <= 410) *xMov = -4;
	else if(aux > 410 && aux <= 440) *xMov = -2;
	
	else if(aux > 440 && aux <= 470) *xMov = 0;
	
	else if(aux > 470 && aux <= 500) *xMov = 2;
	else if(aux > 500 && aux <= 530) *xMov = 4;
	else if(aux > 530 && aux <= 560) *xMov = 6;
	else if(aux > 560 && aux <= 590) *xMov = 8;
}

void calculaForca(short int *forca, dadosBarra barra2){
	int aux = barra2.bola.rect.y + (barra2.bola.rect.h)/2;
	
	if(aux >= 87 && aux <= 117) *forca = 100;
	else if(aux > 117 && aux <= 147) *forca = 90;
	else if(aux > 147 && aux <= 177) *forca = 80;
	
	else if(aux > 177 && aux <= 207) *forca = 65;
	
	else if(aux > 207 && aux <= 237) *forca = 55;
	else if(aux > 237 && aux <= 267) *forca = 50;
	else if(aux > 267 && aux <= 297) *forca = 40;
	else if(aux > 297 && aux <= 327) *forca = 30;
}

void calculaStatusBall(int *statusBall, dadosBola bola){ //0E 1B 2C 3A 4F 5D  //MUDEI
	int auxX = bola.rect.x - (bola.rect.w)/2;
	int auxY = bola.rect.y - (bola.rect.h)/2;
	if(auxX>=211 && auxX<=370 && auxY>=66 && auxY<=152){
		//printf("\nEspaco A!");
		*statusBall = 3;
	}
	else if(auxX>=371 && auxX <= 530 && auxY>= 66 && auxY<= 152){
		//printf("\nEspaco B!");
		*statusBall = 1;
	} //
	else if(auxX>=531 && auxX<=691 && auxY>=66 && auxY<=152){
		//printf("\nEspaco C!");
		*statusBall = 2;
	}

	else if(auxX >=211 && auxX<=370 && auxY>=152 && auxY<=238){
		//printf("\nEspaco D!");
		*statusBall = 5;
	}
	else if(auxX>=371 && auxX <= 530 && auxY>= 152 && auxY<= 238){
		//printf("\nEspaco E!");
		*statusBall = 0;
	}
	else if(auxX>=531 && auxX<=691 && auxY>=152 && auxY<=238){
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
	//goleiro.rect = (SDL_Rect) {350, 190, 160,86}; //MUDEI
	goleiro.rect = (SDL_Rect) {350, 100, 200,170}; //MUDEI
	goleiro.corte = (SDL_Rect) {0,0,200,170};
	//goleiro.texture = IMG_LoadTexture(ren, "gordogoleiros200x170.png");
	goleiro.texture = IMG_LoadTexture(ren, "goleiros200x170.png");
	goleiro.state = esperando;
	goleiro.aux = 0;
	
	//if(vezGoleiro) goleiro.texture = IMG_LoadTexture(ren, "outrogoleiro200x170.png");

	//cria barra1
	dadosBarra barra1;
	barra1.rect = (SDL_Rect) {320,250,270,140}; //MUDEI 320
	barra1.texture = IMG_LoadTexture(ren, "barra.png");
	barra1.state = on;
	barra1.bola.rect = (SDL_Rect) {300, 300, 40,40}; //MUDEI 300
	barra1.bola.texture = IMG_LoadTexture(ren, "bola.png");
	
	//cria barra2
	dadosBarra barra2;
	barra2.rect = (SDL_Rect) {715,87,140,240}; //MUDEI 715
	barra2.texture = IMG_LoadTexture(ren, "barra2.png");
	barra2.state = off;
	barra2.bola.rect = (SDL_Rect) {765, 150, 40,40}; //MUDEI 770
	barra2.bola.texture = IMG_LoadTexture(ren, "bola.png");

	//cria bola
	dadosBola bola;
	bola.rect = (SDL_Rect) {420, 300, 40,40}; //MUDEI 420
	bola.texture = IMG_LoadTexture(ren, "bola.png");
	bola.state = parada;
	bola.aux = 0;
	
	SDL_Texture* img = IMG_LoadTexture(ren, "golpngetal.png");
	SDL_Texture* imgbola = IMG_LoadTexture(ren, "bola.png");
	//cria Player
	dadosPlayer player;
	player.state = vezBatedor;
	player.pontuacao = 0;
	//setores do gol
	struct SDL_Rect setor[6];
	constroi(setor);

	SDL_Point mouse = {0,0};
	
	int espera = 200;
	short int forca = 1;
	short int xMov = 0;
	
	int statusBall = 0, isMoving = 1; 
	int x,y,dx,dy; 
	Uint32 antes = 0;
	short int cont;
	unsigned short int posBola = 6;
	unsigned int goalCountT1 = 0; 
	unsigned int saveCountT1 = 0;
	
	unsigned int goalCountT2 = 0;
	unsigned int saveCountT2 = 0;
    /* EXECUÇÃO */
    
	assert(img != NULL);	
	assert(imgbola != NULL);
	int aguardandoAux = 1;
	int contadorX = 140;
	int contadorY = 87;
	
	SDL_Rect rBX = {171,66,160,86}; // Bola Eixo X
	bool setorSelecionado = false;
	short int setorAux = -1;
	short int bolaaux = 1;
	short int bolaauxY = 1;
	int i = 0;
	while(*running){
			espera = MAX(espera - (int)(SDL_GetTicks() - antes), 0);
		  	SDL_Event evt; 
			if(torcida.state == up) {
				torcida.texture = NULL;
				torcida.texture = IMG_LoadTexture(ren, "torcidaarquibancada1.png");
			}
			else {
				torcida.texture = NULL;
				torcida.texture = IMG_LoadTexture(ren, "torcidaarquibancada2.png");
			}
		  	antes = SDL_GetTicks();
			//SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
			//SDL_RenderClear(ren);
			SDL_RenderCopy(ren, torcida.texture, NULL, NULL);//FUNDO DE TELA
			SDL_RenderCopy(ren, img, NULL, NULL);//FUNDO DE TELA
			
	    	SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);				
						
			//09/12/2023
			if(goleiro.state == esperando){
				if(player.state == vezGoleiro){						
					SDL_SetRenderDrawColor(ren, 255,0,0,255);	
					
					SDL_RenderDrawRect (ren, &setor[0]);
					SDL_RenderDrawRect (ren, &setor[1]);
					SDL_RenderDrawRect (ren, &setor[2]);				
					SDL_RenderDrawRect (ren, &setor[3]);
					SDL_RenderDrawRect (ren, &setor[4]);
					SDL_RenderDrawRect (ren, &setor[5]);
				}
				if(setorSelecionado && player.state == vezGoleiro){
					SDL_SetRenderDrawColor(ren, 255,255,255,255);	
					SDL_RenderFillRect (ren, &setor[setorAux]);
				}
			}
			SDL_RenderCopy(ren, goleiro.texture, &goleiro.corte, &goleiro.rect);
			
			if(player.state  == vezBatedor){
				if(barra1.state == on){
					SDL_RenderCopy(ren, barra1.texture, NULL, &barra1.rect);// BOLA X
					SDL_RenderCopy(ren, barra1.bola.texture, NULL, &barra1.bola.rect);// BOLA X
				}
				if(barra1.state == on || barra2.state == on){
					SDL_RenderCopy(ren, barra2.texture, NULL, &barra2.rect);// BOLA Y
					SDL_RenderCopy(ren, barra2.bola.texture, NULL, &barra2.bola.rect);// BOLA Y
				}
			}
			if(barra1.state != on){
				SDL_RenderCopy(ren, bola.texture, NULL, &bola.rect);// BOLA do jogo
			}
	
			if(bola.aux == forca){	
				forca = 0;		
				calculaStatusBall(&statusBall, bola);
				printf("\n *Status ball: %d* *goleiro.pos: %d* ",statusBall,goleiro.pos);
				if(statusBall == goleiro.pos){
					printf("Agarrou!!");
				}
				else if((statusBall >= 0 && statusBall < 6) && statusBall != goleiro.pos ){
					printf("Gol!!!");
				}
				else if(statusBall == 6){ 
					printf("Fora!!!");
				}
				player.state = aguardando;
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
								/*if(goleiro.state == agarrando) {
									goleiro.state = esperando;
									goleiro.rect.x = 350; //MUDEI
									goleiro.rect.y = 190;
								}*/
								
								if(player.state == vezGoleiro){
									SDL_GetMouseState(&mouse.x, &mouse.y);
									if(SDL_PointInRect(&mouse, &setor[setorAux]) && goleiro.state == esperando){
										goleiro.state = agarrando;
										goleiro.pos = setorAux;
										posBola = rand() % 6;
										printf("SetorAux = %d", setorAux);
									}
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
					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mouse.x,&mouse.y);
						if(!SDL_PointInRect(&mouse,&setor[setorAux]) && goleiro.state == esperando){//Otimização de recurso
							for(i = 0; i < 6;i++){
								if(SDL_PointInRect(&mouse,&setor[i])){
									setorSelecionado = true;
									setorAux = i;
									break;
								}
							}
						}
						
				}
			}
			else{
				if(player.state == vezBatedor){//VEZ BATEDOR
					if(barra1.state == on){
						barra1.bola.rect.x += 5 * bolaaux;

				   		if(barra1.bola.rect.x >= 570 || barra1.bola.rect.x <= 300) { //MUDEI os 2
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
				}
				
				else if(player.state == aguardando){//ESPERANDO
					aguardandoAux++;
					if(aguardandoAux >= 100){
						player.state = vezGoleiro;
						bola.state = parada;
						bola.rect.x = 420; //MUDEI
						bola.rect.y = 300;
						
						goleiro.texture = IMG_LoadTexture(ren, "outrogoleiro200x170.png");
						goleiro.corte = (SDL_Rect) {0,0,200,170};
						goleiro.rect.x = 350; //MUDEI
						goleiro.rect.y = 100;
						goleiro.state = esperando;
					}
				}
				
				else if(player.state == vezGoleiro && goleiro.state == agarrando){
				bola.state = girando;
				
				//unsigned short int posBola = rand() % 6;
				switch(posBola){ // Indo para o mesmo lugar que o goleiro
					case 0: //Meio baixo
						if(bola.rect.y >= 165) bola.rect.y -= 3;
						break;
					case 1: //Meio cima
						if(bola.rect.y >= 75) bola.rect.y -= 3;
						break;
					case 2: //Angulo Direito
						if(bola.rect.x <= 640) bola.rect.x += 3;
						if(bola.rect.y >= 78) bola.rect.y -= 3;
						break;
					case 3: //Angulo Esquerdo
						if(bola.rect.x >= 270) bola.rect.x -= 2;
						if(bola.rect.y >= 75) bola.rect.y -= 3;
						break;					
					case 4: //Canto Direito
						if(bola.rect.x <= 640) bola.rect.x += 3;
						if(bola.rect.y >= 160) bola.rect.y -= 2;
						break;
					case 5: //Canto Esquerdo
						if(bola.rect.x >= 220) bola.rect.x -= 3;
						if(bola.rect.y >= 170) bola.rect.y -= 2;
						break;
				}	
				bola.aux++;
									
				}
				
/*
		if(goleiro.state == esperando && barra1.state == off && barra2.state == off && player.state == vezBatedor){
			goleiro.state = agarrando;
			goleiro.aux = 0;
			goleiro.pos = rand() % 6;		
		}*/				
				
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

				if(goleiro.state == agarrando){
					goleiro.aux = 0;
					//caminho do goleiro
					switch(goleiro.pos){ 
						case 0: //Meio baixo
							if(goleiro.rect.y >= 90) goleiro.rect.y -= 3;
							break;
						case 1: //Meio cima
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 3;
							break;
						case 2: //Angulo direito //MUDEI
							if(goleiro.rect.x <= 500) goleiro.rect.x += 3;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 1;
							break;
						case 3: //Angulo esquerdo //MUDEI
							if(goleiro.rect.x >= 200) goleiro.rect.x -= 3;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 1;
							break;
						case 4: //Canto direito	 //MUDEI
							if(goleiro.rect.x <= 480) goleiro.rect.x += 3;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 1;
							break;
						case 5: //Canto esquerdo //MUDEI
							if(goleiro.rect.x >= 210) goleiro.rect.x -= 3;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 1;
							break;
					}
				}	
				(goleiro.aux)++;
				/*				
				if(goleiro.state == agarrando && goleiro.aux == 2){
					goleiro.aux = 0;
					switch(goleiro.pos){ 
						case 0: //Meio baixo
							if(goleiro.rect.y >= 90) goleiro.rect.y -= 9;
							break;
						case 1: //Meio cima
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 9;
							break;
						case 2: //Angulo direito //MUDEI
							if(goleiro.rect.x <= 500) goleiro.rect.x += 15;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 5;
							break;
						case 3: //Angulo esquerdo //MUDEI
							if(goleiro.rect.x >= 200) goleiro.rect.x -= 15;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 5;
							break;
						case 4: //Canto direito	 //MUDEI
							if(goleiro.rect.x <= 480) goleiro.rect.x += 15;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 4;
							break;
						case 5: //Canto esquerdo //MUDEI
							if(goleiro.rect.x >= 210) goleiro.rect.x -= 15;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 4;
							break;
					}
				}	
				(goleiro.aux)++;*/
				
				/*if(torcida.aux == 15){			
					if(torcida.state == down) {
						torcida.state = up;
					}
					else {
						torcida.state = down;
					}
					torcida.aux = 0;
				}
				(torcida.aux)++;*/
				espera = 20;
			
			}	
	

		if(goleiro.state == esperando && barra1.state == off && barra2.state == off && player.state == vezBatedor){
			goleiro.state = agarrando;
			goleiro.aux = 0;
			goleiro.pos = rand() % 6;		
		}
		if(player.state == vezGoleiro && goleiro.state == agarrando){
			goleiro.pos = setorAux;
						
		}
		if(goleiro.state == agarrando){
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
    SDL_Rect recPlay = {300,210,100,30};
    SDL_Rect recAbout = {410,210,100,30};
    SDL_Rect recQuit = {520,210,100,30};
    
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
                         900, 350, SDL_WINDOW_SHOWN
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
	
