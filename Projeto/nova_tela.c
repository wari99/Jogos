#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL; 

typedef struct {
    int x, y;
    int raio;
	
    float velocidade_Y;	
    float velocidade_X; //adiciona a velocidade pro eixo X
	
    Uint8 r, g, b, a;
    int encostaChao; 
} Bola;

Bola bola = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 70, 0, 0, 255, 0, 0, 255, 1}; 

//Ajuste de Gravidade/Vel inicial/Vel Horzioontal
const float GRAVIDADE = 900.0f; 
const float VEL_INICIAL = 500.0f; 
const float VEL_HORIZONTAL = 200.0f; 

void desenhoBola(const Bola* bola) {
    filledCircleRGBA(renderer, bola->x, bola->y, bola->raio, bola->r, bola->g, bola->b, bola->a);
}

void movimentoBola(Bola* bola, Uint32 deltaTime) { // 1000px/seg
    bola->velocidade_Y += GRAVIDADE * deltaTime / 1000.0f; 
    bola->y += bola->velocidade_Y * deltaTime / 1000.0f;
    bola->x += bola->velocidade_X * deltaTime / 1000.0f;

    //colidir com a parte iferior
    if (bola->y + bola->raio > SCREEN_HEIGHT) {
        bola->y = SCREEN_HEIGHT - bola->raio;
        bola->velocidade_Y = 0;
        bola->encostaChao = 1; //bola no chao
		
        cont = 0; //reseta cont
		
    } else if (bola->y - bola->raio < 0) {
        bola->y = bola->raio;
        bola->velocidade_Y = 0;
    } else {
        bola->encostaChao = 0; //bola esta no ar 
    }

    //colidir com laterais
    if (bola->x + bola->raio > SCREEN_WIDTH) {
        bola->x = SCREEN_WIDTH - bola->raio;
        bola->velocidade_X = -bola->velocidade_X; //inverter velocidade horizontal
    } else if (bola->x - bola->raio < 0) {
        bola->x = bola->raio;
        bola->velocidade_X = -bola->velocidade_X; //inverter velocidade horizontal
    }
}

void acaoClique(int mouseX, int mouseY) {
    int dx = mouseX - bola.x;
    int dy = mouseY - bola.y;
	
    float distanciaFormula = dx * dx + dy * dy;

    if (distanciaFormula <= bola.raio * bola.raio) {
        if (mouseY < bola.y - bola.raio / 3) {
            bola.velocidade_Y = -VEL_INICIAL * 0.5;  // parte superior sobe com vel menor
        } else if (mouseY > bola.y + bola.raio / 3) {
            bola.velocidade_Y = -VEL_INICIAL * 0.8; //parte inferior vel mais alta
        } else {
            bola.velocidade_Y = -VEL_INICIAL; //parte media vel media 
        }

        if (mouseX < bola.x) { inverter velocidade horizontal
            bola.velocidade_X = VEL_HORIZONTAL; //click lado esquerdo = vai pra direita
        } else if (mouseX > bola.x) {
            bola.velocidade_X = -VEL_HORIZONTAL; //click lado direito = vai pra esquerda
        }

        cont++; //se a bola n tiver no chao ++ 
    }
}

void renderText(const char* text, int x, int y, SDL_Color textColor) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
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
    if (SDL_WaitEventTimeout(evt, *ms)) {
        *ms = MAX(0, *ms - (int)(SDL_GetTicks() - antes));
        return 1;
    }
    return 0;
}

void mainLoop() {
    Uint32 startTime = SDL_GetTicks(); 
    Uint32 deltaTime = 0; 
    Uint32 frameStart = startTime; 

    int in_game = 1;

    while (in_game) {
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        frameStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        //att a bola com base no  deltaTime
        movimentoBola(&bola, frameTime);
        desenhoBola(&bola);


        SDL_Color textColor = { 0, 0, 0, 255 };
        char textoContador[50];
        snprintf(textoContador, sizeof(textoContador), "Embaixadinhas: %d", cont);
        renderText(textoContador, 10, 10, textColor);

        SDL_RenderPresent(renderer);


        SDL_Event event;
        Uint32 waitTime = 16; 
        if (AUX_WaitEventTimeoutCount(&event, &waitTime)) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                acaoClique(event.button.x, event.button.y);
            } else if (event.type == SDL_QUIT) {
                in_game = 0; 
            }
        }

        //calculo do deltaTime
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
