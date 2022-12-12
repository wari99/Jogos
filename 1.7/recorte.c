#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	Uint32 depois = 0;
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if(isevt){
		depois = (SDL_GetTicks() - antes);
		if(*ms < depois) depois = *ms;
		*ms -= depois;		
	}
	return isevt;
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Recorte",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 110, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "cardflip.png");
    assert(img != NULL);

    /* EXECUÇÃO */
    int isup = 1;
    int x = -100;
    int running = 1, espera = 500;
 	SDL_Event evt;
    
    int cont = 0;
    while (running) {
    	Uint32 antes = SDL_GetTicks();
		int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
		
        SDL_SetRenderDrawColor(ren, 254,221,2,255);
        SDL_RenderClear(ren);
        
        SDL_Rect r = { x,0, 110,110 };
        SDL_Rect c;

 		if(isevt){       
		    if(evt.type == SDL_QUIT){
		    	running = 0;
		    	break;
		    }
    	}
/*
    		c = (SDL_Rect) {0,0, 110,110}; 
    		 		
    		c = (SDL_Rect) {110,0, 100,110};
    		
    		c = (SDL_Rect) {220,0, 300, 110};*/
    	
    	printf("\n*%d*", isup);
    
    	if(isup < 20){
    		if(isup ==1 || isup == 7 || isup == 12) c = (SDL_Rect) {0,0, 110,110};
    		if(isup ==2 || isup == 8 || isup == 13) c = (SDL_Rect) {110,0, 110,100};
    		if(isup ==3 || isup == 9 || isup == 14) c = (SDL_Rect) {220,0,110,110};
    		if(isup ==4 || isup == 10 || isup == 15) c = (SDL_Rect) {330,0,110,110};    		
    		if(isup ==5 || isup == 11 || isup == 16) c = (SDL_Rect) {440,0,110,110};    		
    		if(isup ==6 || isup == 12 || isup == 17) c = (SDL_Rect) {550,0,110,110}; 
    		isup++;
        }
    	else{
    		break;
    	}
    	/*
    	switch(isup){
    		case
    	}*/
   
        SDL_RenderCopy(ren, img, &c, &r);
        SDL_RenderPresent(ren);
        SDL_Delay(200);
 //       x = (x + 10) % 400;
 		x = (x + 25) % 400;
        //isup = !isup;
 		
    }
