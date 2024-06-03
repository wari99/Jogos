#include <stdint.h>
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
// srand(time(NULL));
//

enum estadoGoleiro {esperando = 0, agarrando};
enum estadoBola {parada = 0, girando};
enum estadoBarra {off = 0, on};
enum estadoTorcida {up = 0, down};
enum estadoPlayer {vezBatedor = 0, vezGoleiro};
enum stateGame{menu = 0,jogo,telaFinal,fim};
typedef struct dadosPonto{
	SDL_Rect r;
	bool state;
	SDL_Texture* img;
}dadosPonto;

typedef struct dadosPontuacao{
    dadosPonto vetPontos[5];
	unsigned short int n;//Numero de tentativas;
	unsigned short int valor;
	SDL_Texture* texture;
	SDL_Rect rect;
	unsigned short int i;
}dadosPontuacao;


typedef struct dadosPlayer{
	uint8_t state;
	unsigned short int pontuacao;
}dadosPlayer;

typedef struct dadosTorcida{
	SDL_Texture* texture;
	uint8_t  state;
	uint8_t aux;
}dadosTorcida;

typedef struct dadosGoleiro{
	SDL_Rect rect;
	SDL_Rect corte;
	SDL_Texture* texture;
	uint8_t  state;
	uint8_t aux;
	uint8_t  pos;
	
}dadosGoleiro;

typedef struct dadosBola{
	SDL_Rect rect;
	SDL_Texture* texture;
	uint8_t state;
	uint8_t aux;
}dadosBola;

typedef struct dadosBarra{
	SDL_Rect rect;
	SDL_Texture* texture;
	uint8_t state;
	struct dadosBola bola;
}dadosBarra;

typedef struct setorGol{
	SDL_Rect rect;
	bool selecionado;
}setorGol;

void constroi(SDL_Rect setor[]){
	setor[0] = (SDL_Rect) {371,152,160,86}; //Meio Baixo
	setor[1] = (SDL_Rect) {371,66,160,86}; //Meio Cima	
	setor[2] = (SDL_Rect) {531,66,160,86}; //Angulo Direito	
	setor[3] = (SDL_Rect) {211,66,160,86}; //Angulo Esquerdo
	setor[4] = (SDL_Rect) {531,152,160,86}; //Canto direito
	setor[5] = (SDL_Rect) {211,152,160,86}; //Canto esquerdo
}

void calculaXmov(short int *xMov, dadosBarra barra1){ 
	int aux = barra1.bola.rect.x + (barra1.bola.rect.w)/2;
	
	if(aux >= 220 && aux <= 250) *xMov = -8;
	else if(aux > 250 && aux <= 310) *xMov = -6;
	else if(aux > 310 && aux <= 410) *xMov = -4;
	else if(aux > 410 && aux <= 440) *xMov = -2;
	else if(aux > 440 && aux <= 470) *xMov = 0;
	else if(aux > 470 && aux <= 500) *xMov = 2;
	else if(aux > 500 && aux <= 530) *xMov = 4;
	else if(aux > 530 && aux <= 560) *xMov = 6;
	else if(aux > 560 && aux <= 595) *xMov = 8;
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

void calculaStatusBall(short int *statusBall, dadosBola bola){ 
	unsigned short int auxX = bola.rect.x - (bola.rect.w)/2;
	int auxY = bola.rect.y - (bola.rect.h)/2;
	
	if(auxX>=211 && auxX<=400 && auxY>=66 && auxY<=152) *statusBall = 3;
	else if(auxX>=401 && auxX <= 500 && auxY>= 66 && auxY<= 152)*statusBall = 1;
	else if(auxX>=401 && auxX<=691 && auxY>=66 && auxY<=152) *statusBall = 2;
	else if(auxX >=211 && auxX<=400 && auxY>=152 && auxY<=238) *statusBall = 5;	
	else if(auxX>=401 && auxX <= 500 && auxY>= 152 && auxY<= 238) *statusBall = 0;
	else if(auxX>=400 && auxX<=691 && auxY>=152 && auxY<=238) *statusBall = 4;
	else *statusBall = 6;	
	
}

void constroiPont(SDL_Renderer *ren,dadosPontuacao* pontuacao, int x,int y, char *endereco){
	pontuacao->n = 0;
	pontuacao->rect = (SDL_Rect) {x,y,240,40};
	
	short int aux = pontuacao->rect.x + 40;
	
	pontuacao->i = 0;
    pontuacao->texture = IMG_LoadTexture(ren, endereco);
    pontuacao->valor = 0;
    
	for(short int i = 0; i<5;i++){
		pontuacao->vetPontos[i].img = NULL;
	 	pontuacao->vetPontos[i].state = false;
	 	SDL_Rect recAux = {aux,y,40,40};
	 	pontuacao->vetPontos[i].r = recAux;
	 	
	 	aux+=40;
    }
}

void chamaPont(SDL_Renderer* ren, dadosPontuacao pontuacao){
	SDL_RenderCopy(ren, pontuacao.texture, NULL, &pontuacao.rect);
	for(int i = 0; i<5;i++){
		SDL_RenderCopy(ren, pontuacao.vetPontos[i].img, NULL, &pontuacao.vetPontos[i].r);
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

void rodaJogo(SDL_Renderer* ren, int *screen,int *vencedor){

	//cria torcida
	dadosTorcida torcida;
	torcida.texture = IMG_LoadTexture(ren, "torcidaarquibancada1.png");
	torcida.state = up;
	torcida.aux = 0;
	
	//cria goleiro
	dadosGoleiro goleiro;
	goleiro.rect = (SDL_Rect) {350, 100, 200,170}; 
	goleiro.corte = (SDL_Rect) {0,0,200,170};
	goleiro.texture = IMG_LoadTexture(ren, "goleiros200x170.png");	
	goleiro.state = esperando;
	goleiro.aux = 0;

	//cria barra1
	dadosBarra barra1;
	barra1.rect = (SDL_Rect) {320,250,270,140}; 
	barra1.texture = IMG_LoadTexture(ren, "barra.png");
	barra1.state = on;
	barra1.bola.rect = (SDL_Rect) {300, 300, 40,40}; 
	barra1.bola.texture = IMG_LoadTexture(ren, "bola.png");
	
	//cria barra2
	dadosBarra barra2;
	barra2.rect = (SDL_Rect) {715,87,140,240};
	barra2.texture = IMG_LoadTexture(ren, "barra2.png");
	barra2.state = off;
	barra2.bola.rect = (SDL_Rect) {765, 150, 40,40}; 
	barra2.bola.texture = IMG_LoadTexture(ren, "bola.png");

	//cria bola
	dadosBola bola;
	bola.rect = (SDL_Rect) {420, 300, 40,40}; 
	bola.texture = IMG_LoadTexture(ren, "bola.png");
	bola.state = parada;
	bola.aux = 0;
	
	SDL_Texture* imgGol = IMG_LoadTexture(ren, "golpngetal.png");
	SDL_Texture* imgBola = IMG_LoadTexture(ren, "bola.png");
	
	//cria Player
	dadosPlayer player;
	player.state = vezBatedor;
	player.pontuacao = 0;
	
	//setores do gol
	struct SDL_Rect setor[6];
	constroi(setor);

	SDL_Point mouse = {0,0};
	
	short int statusBall = -1, isMoving = 1, cont, forca = 1, xMov = 0; 
	int x,y, espera = 200; 	
	bool aguardando = false;
	
	Uint32 antes = 0;
	unsigned short int posBola = 6;
	
	//cria pontuacao1
	dadosPontuacao pontuacao1;
	constroiPont(ren,&pontuacao1,0,260,"timeA.png");
	dadosPontuacao pontuacao2;
	constroiPont(ren,&pontuacao2,0,305,"timeB.png");
	
    /* EXECUÇÃO */
	assert(imgGol != NULL);	
	assert(imgBola != NULL);
				
	short int contadorX = 140, contadorY = 87, aguardandoAux = 1, i = 0;
	bool setorSelecionado = false;
	short int setorAux = -1, bolaaux = 1, bolaauxY = 1;
	
	SDL_Rect rBX = {171,66,160,86};	
	SDL_Rect rectGol = {241,96,420,142};
	
	while(*screen == jogo){
			espera = MAX(espera - (int)(SDL_GetTicks() - antes), 0);
		  	SDL_Event evt; 
		  	antes = SDL_GetTicks();
		  	
			SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
			SDL_RenderClear(ren);
			SDL_RenderCopy(ren, torcida.texture, NULL, NULL);//FUNDO DE TELA
			SDL_RenderCopy(ren, imgGol, NULL, NULL);//FUNDO DE TELA
			
            chamaPont(ren,pontuacao1);
			chamaPont(ren,pontuacao2);
			
	    	SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);
	    	
			if(goleiro.state == esperando){
				if(player.state == vezGoleiro){						
					SDL_SetRenderDrawColor(ren, 255,0,0,255);	
					SDL_RenderDrawRect (ren, &setor[0]);
					SDL_RenderDrawRect (ren, &setor[1]);
					SDL_RenderDrawRect (ren, &setor[2]);				
					SDL_RenderDrawRect (ren, &setor[3]);
					SDL_RenderDrawRect (ren, &setor[4]);
					SDL_RenderDrawRect (ren, &setor[5]);
					
				if(setorSelecionado){
					SDL_SetRenderDrawColor(ren, 255,255,255,255);	
					SDL_RenderFillRect (ren, &setor[setorAux]);
					}
				}	
			}
			SDL_RenderCopy(ren, goleiro.texture, &goleiro.corte, &goleiro.rect);
			
			if(player.state == vezBatedor){
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
			
			//TRATAR O GOL DO vezBatedor
			if(bola.aux == forca){	
				forca = -1;		
				calculaStatusBall(&statusBall, bola);
				printf("\n *Status ball: %d* *goleiro.pos: %d* ",statusBall,goleiro.pos);
				if(SDL_HasIntersection(&bola.rect, &goleiro.rect)){
					printf("Agarrou!!");
					pontuacao1.vetPontos[pontuacao1.n].img = IMG_LoadTexture(ren, "perdeu.png");
				}
				else if(!SDL_HasIntersection(&bola.rect, &goleiro.rect) && SDL_HasIntersection(&bola.rect, &rectGol)){
					printf("Gol!!!");
					pontuacao1.vetPontos[pontuacao1.n].img = IMG_LoadTexture(ren, "acertou.png");
					pontuacao1.valor++;
				}
				else if(!SDL_HasIntersection(&bola.rect, &rectGol)){
					printf("Fora!!!");
					pontuacao1.vetPontos[pontuacao1.n].img = IMG_LoadTexture(ren, "perdeu.png");
				}
				aguardando = true;
				(pontuacao1.n)++;
			}
			
			//TRATAR O GOL vezGoleiro
			if(player.state == vezGoleiro && goleiro.state == agarrando){
				aguardando = true;
				bola.state = parada;
			}
			
			SDL_RenderPresent(ren);
			
		  	int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);    
		    if(isevt){ 	
				switch (evt.type){
					case SDL_WINDOWEVENT:
		            	if (SDL_WINDOWEVENT_CLOSE == evt.window.event){
							*screen = fim;
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
								*screen = menu;
						}
						break;
					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mouse.x,&mouse.y);
						if(goleiro.state == esperando){//Otimização de recurso
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
			   		if(bola.state == girando && bola.aux <= forca){ //Até a bola alcançar a força correspondente (movimentacoes)
						bola.rect.x += xMov;
						bola.rect.y -= 3;
						bola.aux++;
					}
				}				
				
				else if(player.state == vezGoleiro && goleiro.state == agarrando){
					switch(posBola){ 
						case 0: //Meio baixo
							if(bola.rect.y >= 205) bola.rect.y -= 3;
							if(bola.rect.y < 205) bola.state = parada;
							break;
						case 1: //Meio cima
							if(bola.rect.y >= 75) bola.rect.y -= 3;
							if(bola.rect.y < 75) bola.state = parada;
							break;
						case 2: //Angulo Direito
							if(bola.rect.x <= 640) bola.rect.x += 3;
							if(bola.rect.y >= 78) bola.rect.y -= 3;
							if(bola.rect.x > 640 && bola.rect.y < 78) bola.state = parada;
							break;
						case 3: //Angulo Esquerdo
							if(bola.rect.x >= 270) bola.rect.x -= 2;
							if(bola.rect.y >= 75) bola.rect.y -= 3;
							if(bola.rect.x < 640 && bola.rect.y < 75) bola.state = parada;
							break;					
						case 4: //Canto Direito
							if(bola.rect.x <= 640) bola.rect.x += 3;
							if(bola.rect.y >= 160) bola.rect.y -= 2;
							if(bola.rect.x > 640 && bola.rect.y < 160) bola.state = parada;
							break;
						case 5: //Canto Esquerdo
							if(bola.rect.x >= 220) bola.rect.x -= 3;
							if(bola.rect.y >= 170) bola.rect.y -= 2;
							if(bola.rect.x < 220 && bola.rect.y < 170) bola.state = parada;
							break;
					}	
				
				}
				if(aguardando){//ESPERANDO
					aguardandoAux++;
					if(aguardandoAux >= 100 && player.state == vezBatedor ){
						aguardando = false;
						player.state = vezGoleiro;
						bola.state = parada;
						bola.rect.x = 420; //MUDEI
						bola.rect.y = 300;
						
						goleiro.texture = IMG_LoadTexture(ren, "outrogoleiro200x170.png");
						goleiro.corte = (SDL_Rect) {0,0,200,170};
						goleiro.rect.x = 350; //MUDEI
						goleiro.rect.y = 100;
						goleiro.state = esperando;
						aguardandoAux = 0;

					}
					else if(aguardandoAux >= 100 && player.state == vezGoleiro){
						aguardando = false;
						player.state = vezBatedor;
						bola.state = parada;
						bola.rect.x = 420;
						bola.rect.y = 300;
						if(posBola == goleiro.pos && bola.state == parada){
							pontuacao2.vetPontos[pontuacao2.n].img = IMG_LoadTexture(ren, "perdeu.png");
						}
						else if(bola.state == parada){
							pontuacao2.vetPontos[pontuacao2.n].img = IMG_LoadTexture(ren, "acertou.png");
							pontuacao2.valor++;
						}
						(pontuacao2.n)++;
						goleiro.texture = IMG_LoadTexture(ren, "goleiros200x170.png");
						goleiro.corte = (SDL_Rect) {0,0,200,170};
						goleiro.rect.x = 350;
						goleiro.rect.y = 100;
						goleiro.state = esperando;
						aguardandoAux = 0;
						bola.aux = 0;
						barra1.state = on;
						barra2.state = off;
						setorAux = 0;
					}
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
						case 2: //Angulo direito
							if(goleiro.rect.x <= 500) goleiro.rect.x += 3;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 1;
							break;
						case 3: //Angulo esquerdo 
							if(goleiro.rect.x >= 200) goleiro.rect.x -= 3;
							if(goleiro.rect.y >= 50) goleiro.rect.y -= 1;
							break;
						case 4: //Canto direito	 
							if(goleiro.rect.x <= 480) goleiro.rect.x += 3;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 1;
							break;
						case 5: //Canto esquerdo
							if(goleiro.rect.x >= 210) goleiro.rect.x -= 3;
							if(goleiro.rect.y >= 70) goleiro.rect.y -= 1;
							break;
					}
				}	
				(goleiro.aux)++;

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
		if(pontuacao1.n == 5 && pontuacao2.n == 5){
			if(pontuacao1.valor > pontuacao2.valor) *vencedor = 1;
			else if(pontuacao1.valor < pontuacao2.valor) *vencedor = 2;
			else if(pontuacao1.valor == pontuacao2.valor) *vencedor = 0;
			*screen = telaFinal;
			break;
		}
	}
	SDL_DestroyTexture(pontuacao1.texture);
	SDL_DestroyTexture(pontuacao2.texture);
	for(i = 0;i<5;i++){
		SDL_DestroyTexture(pontuacao1.vetPontos[i].img);
		SDL_DestroyTexture(pontuacao2.vetPontos[i].img);
	}
	SDL_DestroyTexture(imgGol);
 	SDL_DestroyTexture(imgBola);

}

void chamaMenu(SDL_Renderer* ren,int *screen){
    TTF_Init();
    SDL_Color padrao = { 0,0,0,255 };
    SDL_Color focus = { 255,255,255,255 };
    
    TTF_Font *ourFont = TTF_OpenFont("Mont-HeavyDEMO.otf",100);
    struct SDL_Surface* listaSurfaceText[3];
    listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",padrao);  
    listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "About",padrao); 
    listaSurfaceText[2] = TTF_RenderText_Solid(ourFont, "Quit",padrao); 

 	SDL_Texture* bgmenu = IMG_LoadTexture(ren, "telamenu.png");
    struct SDL_Texture* listaTextureText[3];
	listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
    listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
    listaTextureText[2] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[2]);
    
    int i;
    SDL_Point mouse = {0,0};
    SDL_Rect recPlay = {320,210,100,30};
    SDL_Rect recAbout = {410,210,100,30};
    SDL_Rect recQuit = {500,210,100,30};
    
    bool selecionado = false;
 	int espera = 0;    
	Uint32 antes = 0;
	  
     /* EXECUÇÃO */
    while (*screen == menu) {
    	SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);
		espera = MAX(espera - (int)(SDL_GetTicks() - antes), 0);
	  	SDL_Event event; int isevt = AUX_WaitEventTimeoutCount(&event,&espera);    
	  	antes = SDL_GetTicks();
	  	if(isevt){   
			switch(event.type){
				case SDL_QUIT:
					*screen = fim;
				break;
				case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mouse.x,&mouse.y);
						if(SDL_PointInRect(&mouse,&recPlay)){
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,0,"Play",ourFont);
					   	}
					   	
					   	else if(SDL_PointInRect(&mouse,&recQuit)){
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,2,"Quit",ourFont);

					   	}
					   	else{
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,0,"Play",ourFont);
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
								*screen = fim;
							}
							else if(SDL_PointInRect(&mouse,&recPlay) && selecionado) {
								*screen = jogo;
							}
						}
					}
				break;
			}
		}
		SDL_RenderCopy(ren,bgmenu,NULL,NULL);
		SDL_RenderCopy(ren,listaTextureText[0],NULL,&recPlay);
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

void rodaTelaFinal(SDL_Renderer* ren,int *vencedor,int *screen){
	SDL_Texture *telaF;
	if(*vencedor == 0) telaF = IMG_LoadTexture(ren, "telaempate.png");
	else if(*vencedor == 1) telaF = IMG_LoadTexture(ren, "telaganhou.png");
	else if(*vencedor == 2) telaF = IMG_LoadTexture(ren, "telaperdeu.png");
	int espera = 0;    
	Uint32 antes = 0;
	while(*screen == telaFinal){
		SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren,telaF,NULL,NULL);
		SDL_RenderPresent(ren);
		espera = MAX(espera - (int)(SDL_GetTicks() - antes), 0);
	  	SDL_Event evento; int isevt = AUX_WaitEventTimeoutCount(&evento,&espera);    
	  	antes = SDL_GetTicks();	
		if(isevt){ 
	  		switch(evento.type){
	  			case SDL_KEYUP:
	  				if(evento.key.keysym.sym == SDLK_ESCAPE) *screen = fim;
	  				break;
	  			case SDL_QUIT:
	  				*screen = fim;
	  				break;
	  			default:
	  				SDL_FlushEvent(evento.type);
	  		}
		}
	}
}

SDL_Window* create_window(void) {
    SDL_Window* win = SDL_CreateWindow("SOCCER SHOOTOUTS",
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
	int vencedor = -1;
	int screen = menu;
	
    while(screen != fim){
		switch (screen) {
            case menu:
				chamaMenu(ren,&screen);
            case jogo:  
				rodaJogo(ren,&screen,&vencedor);
            case telaFinal:
            	rodaTelaFinal(ren,&vencedor,&screen);
        } 
	}
	
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
	
