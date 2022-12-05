#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	Uint32 depois = 0;
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if(isevt){
		depois = (SDL_GetTicks() - antes);
		if(*ms < depois) depois = *ms;
		*ms -= depois;		
	}
	else{
		*ms = 500;
		
	}
	return isevt;
}


int main (int argc, char* args[]){
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("WaitEventTimeout",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         250, 150, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    int espera = 500;
    int v = 1;    
    int x = 40, y = 40;
    int z = 200, w = 110;
    
    SDL_Event evt;
    
    bool continua = true;
	/* EXECUÇÃO */
    while (continua) {
        SDL_SetRenderDrawColor(ren, 0x8B, 0x45, 0x13,0x00);
        SDL_RenderClear(ren);
        
        filledCircleRGBA(ren, x,y,33, 0,0,0,255); //Borda Preta Alvo Cima
        filledCircleRGBA(ren, z,w,33, 0,0,0,255); //Borda Preta Alvo Meio

        filledCircleRGBA(ren, x,y,30, 220,20,60,255); //Vermelho 30 Cima
        filledCircleRGBA(ren, x,y,20, 240,248,255,255); //Branco 20 Cima
        
        filledCircleRGBA(ren, z,w,30, 220,20,60,255); //Vermelho 30 Meio
        filledCircleRGBA(ren, z,w,20, 240,248,255,255); //Branco 20 Meio
        
        if(x % 2 == 0){
            filledCircleRGBA(ren, x, y,10, 220,20,60,255); //Vermelho Cima 10            
            filledCircleRGBA(ren, z, w,10, 220,20,60,255); //Vermelho Meio 10
            
            filledCircleRGBA(ren, x,y,4, 255,255,0,255); //Bullseye Cima
            filledCircleRGBA(ren, z,w,4, 255,255,0,255); //Bullseye Meio
        }
        
        if(x % 2 == 1){
            filledCircleRGBA(ren, x,y, 30, 240, 248,255,255);//Branco Cima 30
            filledCircleRGBA(ren, x,y, 20, 220,20,60,255); //Vermelho Cima 20
            filledCircleRGBA(ren, x,y, 10, 240,248,255,255); //Branco CIma 10

            filledCircleRGBA(ren, z,w, 30, 240, 248,255,255);//Branco Meio 30
            filledCircleRGBA(ren, z,w, 20, 220,20,60,255); //Vermelho Meio 20
            filledCircleRGBA(ren, z,w, 10, 240,248,255,255); //Branco Meio 10

            filledCircleRGBA(ren, x,y,4, 0,255,255,255); //Bullseye Cima
            filledCircleRGBA(ren, z,w,4, 0,255,255,255); //Bullseye Meio
        
        } SDL_RenderPresent(ren);
          
        
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        
        if (!isevt) {
            espera = 500;
		    x += 5 * v;
		    z += 5 * v*(-1);

		    if (x <= 39 || x >= 197){    
		        v = v*(-1);
		    }
        }
        
        /* FINALIZAÇÃO */
        switch (evt.type ) {
            case SDL_QUIT:
            	continua = false;
            	break;
            case SDL_KEYDOWN:
            	if(evt.key.keysym.sym == SDLK_F4){
            		if(evt.key.keysym.mod == KMOD_LALT){
		    		continua = false;
		    		break;	
            		}		
			}
        }   
    }
}
