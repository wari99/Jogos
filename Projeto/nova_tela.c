#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#include <SDL2/SDL_messagebox.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int recordeSessao = 0;
int recordeGeral = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

typedef struct {
    int x, y;
    int raio;
    float velocidade_Y;
    float velocidade_X; 
	
    Uint8 r, g, b, a;
    int encostaChao; 
} Bola;

Bola bola = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 70, 0, 0, 255, 0, 0, 255, 1}; 

const float GRAVIDADE = 500.0f; 
const float VEL_INICIAL = 600.0f; 
const float VEL_HORIZONTAL = 200.0f;

int cont = 0;

void desenhoBola(const Bola* bola) {
    filledCircleRGBA(renderer, bola->x, bola->y, bola->raio, bola->r, bola->g, bola->b, bola->a);
}

void movimentoBola(Bola* bola, Uint32 deltaTime) {
	
    // Aplicação da gravidade
    bola->velocidade_Y += GRAVIDADE * deltaTime / 1000.0f; // Convertendo deltaTime para segundos
    bola->y += bola->velocidade_Y * deltaTime / 1000.0f;
    bola->x += bola->velocidade_X * deltaTime / 1000.0f;


    if (bola->y + bola->raio > SCREEN_HEIGHT) { //colidir com os "tetos" 
        bola->y = SCREEN_HEIGHT - bola->raio;
        bola->velocidade_Y = 0;
        bola->encostaChao = 1; //bola no chao
		
        cont = 0; //encostou no chao = cont reseta
    } else if (bola->y - bola->raio < 0) {
        bola->y = bola->raio;
        bola->velocidade_Y = 0;
    } else bola->encostaChao = 0; //bola esta no ar


    if (bola->x + bola->raio > SCREEN_WIDTH) { //colidindo com as "paredes"
        bola->x = SCREEN_WIDTH - bola->raio;
        bola->velocidade_X = -bola->velocidade_X; //invertendo velocidade horizontal
		
    } else if (bola->x - bola->raio < 0) {
        bola->x = bola->raio;
        bola->velocidade_X = -bola->velocidade_X; //invertendo velocidade horizontal
    }
}

void acaoClique(int mouseX, int mouseY) {
    //distancia entre o clique e a bola
    int dx = mouseX - bola.x;
    int dy = mouseY - bola.y;
    float distanciaFormula = dx * dx + dy * dy;

    if (distanciaFormula <= bola.raio * bola.raio) {
        if (mouseY < bola.y - bola.raio / 3) {
            bola.velocidade_Y = -VEL_INICIAL * 0.5;  //clique na parte de cima VELINICIAL * um pouco
        } else if (mouseY > bola.y + bola.raio / 3) {
            bola.velocidade_Y = -VEL_INICIAL * 0.8; //clique na parte de baixo VELINICIAL * muito
        } else { 
            bola.velocidade_Y = -VEL_INICIAL; //clique no meio da bola,  somente VELINICIAL
        }

        if (mouseX < bola.x) { 
            bola.velocidade_X = VEL_HORIZONTAL; //clique lado esquerdo = vai p direita
        } else if (mouseX > bola.x) {
            bola.velocidade_X = -VEL_HORIZONTAL; //clique lado direito = vai p esquerda
        }

        cont++; //cont++ se  estiver no ar 
        
        if (cont > recordeSessao) { //att  so o rec da sessao
        	recordeSessao = cont;
        }
        
        if (cont > recordeGeral){ 
        	recordeGeral = cont;
        	
        	FILE* arquivo = fopen("recorde_geral.txt", "w");
        	if(arquivo){
        		fprintf(arquivo, "%d", recordeGeral);
        		
        		//fscanf(arquivo, "%d", &recordeGeral);
        		fclose(arquivo);	
        	}else{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erro!", "Nao foi possivel salvar!", window);
        	}
        	
        }    
    }
}

void renderizaTexto(const char* text, int x, int y, SDL_Color corTexto) {
	
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, corTexto);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_Rect dstRect = { x, y, texW, texH };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    SDL_FlushEvent(SDL_MOUSEMOTION);
    if (SDL_WaitEventTimeout(evt, *ms)){
        *ms = MAX(0, *ms - (int)(SDL_GetTicks() - antes));
        return 1;
    }
    return 0;
}

void mainLoop() {
    Uint32 startTime = SDL_GetTicks(); //tempo  inicial
    Uint32 deltaTime = 0; //tempo dsd ultima att
    Uint32 frameStart = startTime; //tempo inicio do frame

    int in_game = 1;

    while (in_game) {
    	FILE* arquivo = fopen("recorde_geral.txt", "r");
    	
    	if(arquivo){
    		fscanf(arquivo, "%d", &recordeGeral);
    		fclose(arquivo);
    	}else{
    		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erro", "N foi possivel carregar", window);
    	}
    
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        frameStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        movimentoBola(&bola, frameTime); //att bola com base no frameTime
        desenhoBola(&bola); //desenho da bola

        //contador em texto 
        SDL_Color corTexto = { 0, 0, 0, 255 };
        char textoContador[50];
        snprintf(textoContador, sizeof(textoContador), "Embaixadinhas: %d", cont);
        renderizaTexto(textoContador, 10, 10, corTexto);

        SDL_RenderPresent(renderer);

        //espera eventos 
        SDL_Event event;
        Uint32 waitTime = 16; 

        if (AUX_WaitEventTimeoutCount(&event, &waitTime)) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                acaoClique(event.button.x, event.button.y);
            } else if (event.type == SDL_QUIT) {
                in_game = 0; 
            } else if (event.type == SDL_KEYDOWN) {
            	if (event.key.keysym.sym == SDLK_RETURN){
            		// apertando tecla ENTER pressionada 
            		char mensagem[100];
            		snprintf(mensagem, sizeof(mensagem), "Recorde da sessao atual: %d Recorde Geral: %d", recordeSessao, recordeGeral);
            		
            		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Recordes", mensagem, window);
            	};
            }
         
        }

        //calculo do deltaTime global
        deltaTime = SDL_GetTicks() - startTime;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("Embaixadinha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro ao criar renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("Mont-HeavyDEMO.otf", 24);
    if (!font) {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    mainLoop();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
