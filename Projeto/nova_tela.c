#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* ren = NULL;
TTF_Font* font = NULL; 

typedef struct {
    int x, y;
    int raio;
    float velocidade_Y;
	
    Uint8 r, g, b, a;
	
    int encostaChao; 
} Bola;

Bola bola = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 70, 0, 255, 0, 0, 255, 1}; // Inicia a bola no chão
const float GRAVIDADE = 0.5;
int cont = 0;

void desenhoBola(const Bola* bola) {
    filledCircleRGBA(ren, bola->x, bola->y, bola->raio, bola->r, bola->g, bola->b, bola->a);
}

void movimentoBola(Bola* bola) {
    bola->velocidade_Y += GRAVIDADE;
    bola->y += bola->velocidade_Y;

	//colidir com o interior da tela
    if (bola->y + bola->raio > SCREEN_HEIGHT) {
        bola->y = SCREEN_HEIGHT - bola->raio;
        bola->velocidade_Y = 0;
        bola->encostaChao = 1; 
    } else {
        bola->encostaChao = 0;
    }

	//se encosta no chao reseta 
    if (bola->encostaChao && cont > 0) {
        cont = 0;
    }
}

void acaoClique(int mouseX, int mouseY) {
    int dx = mouseX - bola.x;
    int dy = mouseY - bola.y;
	
    float distancia = sqrt(dx * dx + dy * dy); //dist entre o ponto do clique e o centro da bola
    
	if (distancia <= bola.raio && !bola.encostaChao) {
        if (mouseY < bola.y - bola.raio / 3) {
            bola.velocidade_Y = -12; 
        } else if (mouseY > bola.y + bola.raio / 3) {
            bola.velocidade_Y = -17; 
        } else {
            bola.velocidade_Y = -20;
        }
        cont++; 
    }
}

void renderizarTexto(const char* text, int x, int y, SDL_Color textColor) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_Rect dstRect = { x, y, texW, texH };
    SDL_RenderCopy(ren, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

void mainLoop() {
    SDL_Event evt;
    int in_game = 1;

    while (in_game) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) {
                in_game = 0;
            } else if (evt.type == SDL_MOUSEBUTTONDOWN) {
                acaoClique(evt.button.x, evt.button.y);
            }
        }

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        movimentoBola(&bola);
        desenhoBola(&bola);

        SDL_Color textColor = { 0, 0, 0, 255 };
        char textoContador[50];
		
        snprintf(textoContador, sizeof(textoContador), "Embaixadinhas: %d", cont);
        
		renderizarTexto(textoContador, 10, 10, textColor);

        SDL_RenderPresent(ren);

        SDL_Delay(16); 
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

    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        printf("Erro ao criar ren: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("Mont-HeavyDEMO.otf", 24); 
    if (!font) {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    mainLoop();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
