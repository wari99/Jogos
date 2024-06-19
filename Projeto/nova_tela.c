#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <math.h>

const int largura_tela = 800;
const int altura_tela = 600;

SDL_Window* window = NULL;
SDL_Renderer* ren = NULL;

typedef struct {
    int x, y;
    int raio;
    float velocidade_Y;
	
    Uint8 r, g, b, a;
} Bola;

Bola bola = {largura_tela / 2, altura_tela / 2, 70, 0, 255, 0, 0, 255};
const float GRAVIDADE_CONST = 0.5;

void drawBola(const Bola* bola) {
    filledCircleRGBA(ren, bola->x, bola->y, bola->raio, bola->r, bola->g, bola->b, bola->a);
}

void movimentoBola(Bola* bola) {
    bola->velocidade_Y += GRAVIDADE_CONST;
    bola->y += bola->velocidade_Y;

    if (bola->y + bola->raio > altura_tela) {
        bola->y = altura_tela - bola->raio;
        bola->velocidade_Y = 0;
    }
}

void acaoClique(int mouseX, int mouseY) {
    int dx = mouseX - bola.x;
    int dy = mouseY - bola.y;
	
    int distanciaFormula = dx * dx + dy * dy;
	
    if (distanciaFormula <= bola.raio * bola.raio) {
        if (mouseY < bola.y - bola.raio / 3) {
            bola.velocidade_Y = -12; 
        } else if (mouseY > bola.y + bola.raio / 3) {
            bola.velocidade_Y = -17; 
        } else {
            bola.velocidade_Y = -20; 
        }
    }
}

void mainLoop() {
    SDL_Event evt;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) {
                running = 0;
            } else if (evt.type == SDL_MOUSEBUTTONDOWN) {
                acaoClique(evt.button.x, evt.button.y);
            }
        }

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        movimentoBola(&bola);
        drawBola(&bola);

        SDL_RenderPresent(ren);

        SDL_Delay(16); //retirar dps
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

    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
