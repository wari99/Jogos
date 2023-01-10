#include <assert.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

void mudaCor(SDL_Renderer* ren,SDL_Surface* listaS[],SDL_Texture* listaT[],SDL_Color cor,int i,char nome[],TTF_Font *ourFont){
	listaS[i] = TTF_RenderText_Solid(ourFont, nome,cor);  
	listaT[i] = SDL_CreateTextureFromSurface(ren,listaS[i]);
}


int main (int argc, char* args[])
{

    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Contando o Tempo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 350, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    TTF_Init();
    SDL_Color padrao = { 138,138,138,255 };
    SDL_Color focus = { 228,232,112,255 };
    char fonte[7] = "ka1.ttf";
    TTF_Font *ourFont = TTF_OpenFont("Gameshow.ttf",100);
    
    struct SDL_Surface* listaSurfaceText[3];
    listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "play",padrao);  
    listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "about",padrao); 
    listaSurfaceText[2] = TTF_RenderText_Solid(ourFont, "quit",padrao); 

     
    struct SDL_Texture* listaTextureText[3];
	listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
    listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
    listaTextureText[2] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[2]);
    
    int i;
    SDL_Point mouse = {0,0};
    SDL_Rect recPlay = {200,180,100,30};
    SDL_Rect recAbout = {200,220,100,30};
    SDL_Rect recQuit = {200,260,100,30};
    
    bool selecionado = false;
    bool gameIsRunning = true;
    
     /* EXECUÇÃO */
    while (gameIsRunning) {
    	SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					gameIsRunning = false;
				break;
				case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mouse.x,&mouse.y);
						if(SDL_PointInRect(&mouse,&recPlay)){
							//listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",focus);  
					   		//listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,0,"play",ourFont);
					   	}

					   	else if(SDL_PointInRect(&mouse,&recAbout)){
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,1,"about",ourFont);					   	
					   	}
					   	
					   	else if(SDL_PointInRect(&mouse,&recQuit)){
					   		//listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "Quit",focus);  
					   		//listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,2,"quit",ourFont);

					   	}
					   	else{
							/*listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",padrao);  
							listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
							listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "Quit",padrao);  
							listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);*/
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,0,"play",ourFont);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,1,"about",ourFont);					   	
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,2,"quit",ourFont);
							selecionado = false;
					   	}
				break;
				case SDL_MOUSEBUTTONDOWN:	
					if(event.button.button==SDL_BUTTON_LEFT){
						if(SDL_PointInRect(&mouse,&recQuit)) selecionado = true;
					}
				case SDL_MOUSEBUTTONUP:	
					if(event.button.button==SDL_BUTTON_LEFT){
						if(event.button.state==SDL_RELEASED){
							if(SDL_PointInRect(&mouse,&recQuit) && selecionado) gameIsRunning = false;
						}
					}
				break;
			}
		}

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
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win); 
	TTF_Quit();
	SDL_Quit(); 
}
