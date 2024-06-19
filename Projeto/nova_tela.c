#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <math.h>

const int dimensao_x = 800;
const int dimensao_y = 600;

SDL_Window* window = NULL;
SDL_Renderer* ren = NULL;

typedef struct{
    int x, y; //posicao	
    int raio; //raio da bola
	int velocidade_y; //velocidade eixo y
	
    Uint8 r, g, b, a; //cores
} Bola;

Bola bola = {largura_tela/2, altura_tela/2, 50, 0, 255, 0, 0, 255};
const int gravidade = 1;

void desenharBola(const Bola* bola) {
    filledCircleRGBA(ren, bola->x, bola->y, bola->raio, bola->r, bola->g, bola->b, bola->a);
}

void movimentoBola(Bola* bola) {
    bola->velocidade_y += gravidade;
    bola->y += bola->velocidade_y;

    // Colisão com a parte inferior da tela
    if (bola->y + bola->raio > altura_tela) {
        bola->y = altura_tela - bola->raio;
        bola->velocidade_y = 0;
    }
}

void acaoClique(int mouseX, int mouseY) {
    int dx = mouseX - bola.x;
    int dy = mouseY - bola.y;
    int distanciaFormula = dx * dx + dy * dy; //calculando distancia entre clique e centro da bola
    
	if (distanciaFormula <= bola.raio * bola.raio) {
        if (mouseY < bola.y - bola.raio / 3) bola.velocidade_y = 10;  //parte superior
		else if (mouseY > bola.y + bola.raio / 3) bola.velocidade_y = -10; // parte inferior
		else bola.velocidade_y = 5; // meio
    }
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
        desenharBola(&bola);

        SDL_RenderPresent(ren);

        SDL_Delay(16); // 
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Embaixadinha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largura_tela, altura_tela, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    ren = SDL_Createren(window, -1, SDL_ren_ACCELERATED);
    if (!ren) {
        printf("Erro ao criar ren: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    mainLoop();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
