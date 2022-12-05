#include <assert.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("corrida entre retangulos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 300, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    int continua = 1, telacorrida = 1, telafinal = 0;    
    SDL_Rect r1 = { 10, 40, 20,20 };
    SDL_Rect r2 = { 10, 120, 20,20 };
    SDL_Rect r3 = { 10, 210, 20,20 };
    
    SDL_Rect r11 = {12,42, 16,16};
	SDL_Rect r22 = {12,122, 16,16};
	SDL_Rect r33 = {12,212, 16, 16};
	    
    int espera = 200;
	int auxlinha = 0, vencedor = 0, segundolugar = 0, terceirolugar = 0;
    SDL_Event evt;
    Uint32 antes = SDL_GetTicks();
    int isevt = SDL_WaitEventTimeout(&evt, espera);    

    /* EXECUÇÃO */
    while (continua) {
		while (telacorrida) {        
		    SDL_SetRenderDrawColor(ren, 0x55, 0x6B, 0x2F, 0x00);
		    SDL_RenderClear(ren);

			int cont = 0;
			for (cont; cont < 30; cont++){
				if(cont % 2 != 0){
					SDL_SetRenderDrawColor (ren, 0xFF, 0xFF, 0xFF, 0x0);
					SDL_Rect brancopreto1 = {260, auxlinha, 10,10};
					SDL_RenderFillRect(ren, &brancopreto1);

					SDL_SetRenderDrawColor (ren, 0x0, 0x0, 0x0, 0x0);
					SDL_Rect brancopreto2 = {270, auxlinha, 10,10};
					SDL_RenderFillRect(ren, &brancopreto2);
					
					SDL_SetRenderDrawColor (ren, 0xFF, 0xFF, 0xFF, 0x0);
					SDL_Rect brancopreto3 = {280, auxlinha, 10,10};
					SDL_RenderFillRect(ren, &brancopreto3);

					SDL_SetRenderDrawColor (ren, 0x0, 0x0, 0x0, 0x0);
					SDL_Rect brancopreto4 = {290, auxlinha, 10,10};
					SDL_RenderFillRect(ren, &brancopreto4);			
				}	
				else{
						SDL_SetRenderDrawColor (ren, 0x0, 0x0, 0x0, 0x0);
						SDL_Rect pretobranco1 = {260, auxlinha, 10,10};
						SDL_RenderFillRect(ren, &pretobranco1);
						
						SDL_SetRenderDrawColor (ren, 0xFF, 0xFF, 0xFF, 0x0);
						SDL_Rect pretobranco2 = {270, auxlinha, 10,10};
						SDL_RenderFillRect(ren, &pretobranco2);
						
						SDL_SetRenderDrawColor (ren, 0x0, 0x0, 0x0, 0x0);
						SDL_Rect pretobranco3 = {280, auxlinha, 10, 10};
						SDL_RenderFillRect(ren, &pretobranco3);
						
						SDL_SetRenderDrawColor (ren, 0xFF, 0xFF, 0xFF, 0x0);
						SDL_Rect pretobranco4 = {290, auxlinha, 10,10};
						SDL_RenderFillRect(ren, &pretobranco4);
				}
				auxlinha +=10;
				if(auxlinha >= 300)auxlinha = 0;
			} cont = 0;
		    
		    SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
		    SDL_RenderFillRect(ren, &r1);
		    
		    SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
		    SDL_RenderFillRect(ren, &r2);
		    
		    SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
		    SDL_RenderFillRect(ren, &r3);
		    
		    SDL_SetRenderDrawColor(ren, 0x93, 0x70, 0xDB, 0x00);
		    SDL_RenderFillRect(ren, &r22);
		    
		    SDL_SetRenderDrawColor(ren, 0xFF, 0x0, 0xFF,0x00);
		    SDL_RenderFillRect(ren, &r11);

		    SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);
		    SDL_RenderFillRect(ren, &r33);
		    
		    if(r1.x >= 260 && !vencedor){ //Resetar vencedor **
		    	vencedor = 1;
		    }
		    else if(r2.x >= 260 && !vencedor){
		    	vencedor = 2;
		    }
		    else if(r3.x >= 260 && !vencedor){
		    	vencedor = 3;
		    }
		    		
			if(r1.x >= 260 && r2.x >=260 && r3.x >=260){ //Se passar da linha
				continua = 0;
				telacorrida = 0;
				telafinal = 1;
				
				SDL_RenderClear(ren);
			}
						
			SDL_RenderPresent(ren);
			Uint32 antes = SDL_GetTicks();
			
			int isevt = SDL_WaitEventTimeout(&evt, espera);

			if(isevt){
				espera -= (SDL_GetTicks() - antes);
				if(espera < 0)espera =0;
				switch(evt.type){
					case SDL_MOUSEMOTION:
						int x, y;
		            	SDL_GetMouseState(&x, &y);
		            	r3.x = x;
		            	r33.x = x + 2;
					case SDL_KEYDOWN:
						switch(evt.key.keysym.sym){
							case SDLK_RIGHT:
								r1.x += 5;
								r11.x += 5;
								break;
						}
				}
			}

		    else {
		        espera = 100;
		        r2.x += 3;
		        r22.x += 3;
			
		    }SDL_RenderPresent(ren);
		
			if(evt.type == SDL_QUIT){
				continua = 0;
				break;
			}
		}
		
		while(telafinal){  
			SDL_SetRenderDrawColor(ren, 0x4B, 0x0, 0x82, 0x00);
			SDL_RenderClear(ren);
			SDL_WaitEvent(&evt);
			
			SDL_SetRenderDrawColor(ren, 0xFF, 0xE4, 0xE1, 0x00);
			SDL_Rect fundo = {10,10,280,280};
			SDL_RenderFillRect(ren, &fundo);
			
			thickLineRGBA(ren, 135,40, 145,35,40, 0,0,0,255);		
			thickLineRGBA(ren, 165,40, 155,35,40, 0,0,0,255);			
			filledCircleRGBA(ren, 150,50, 17, 0,0,0,255);			
			filledCircleRGBA(ren, 150,50, 15,  255,215,0, 255);
			stringRGBA(ren, 140, 50, "1st", 0,0,0,255);	
						
			thickLineRGBA(ren, 65,40, 75,35,40, 0,0,0,255);
			thickLineRGBA(ren, 95,40, 85,35,40, 0,0,0,255);	    
			filledCircleRGBA(ren, 80,50, 17, 0,0,0,255);
			filledCircleRGBA(ren, 80,50, 15, 220,220,220, 255);
			stringRGBA(ren, 70, 50, "2nd", 0,0,0,255);	
					
			thickLineRGBA(ren, 205,40, 215,35,40, 0,0,0,255);		
			thickLineRGBA(ren, 235,40, 225,35,40, 0,0,0,255);			
			filledCircleRGBA(ren, 220,50, 17, 0,0,0,255);			
			filledCircleRGBA(ren, 220,50, 15,  218,165,32, 255);
			stringRGBA(ren, 210, 50, "3rd", 0,0,0,255);				
			
			SDL_Rect podio1 = {120,142, 60,65};
			SDL_Rect podio2 = {70,162, 60,45};
			SDL_Rect podio3 = {180,172, 60,35};
			SDL_SetRenderDrawColor(ren, 0xCD, 0x85, 0x3F, 0x00);
			SDL_RenderFillRect(ren, &podio1);
			SDL_RenderFillRect(ren, &podio2);
			SDL_RenderFillRect(ren, &podio3);
			
			SDL_Rect segundo = { 80,130, 30,30};
			SDL_Rect terceiro = { 190,140, 30,30};
			
			SDL_SetRenderDrawColor(ren, 0x0, 0x0, 0x0, 0x00);
			SDL_RenderFillRect(ren, &segundo);
			SDL_RenderFillRect(ren, &terceiro);

			stringRGBA(ren, 30, 250, "Aperte ESC para reiniciar...", 0,0,0,255);		
			
			if(vencedor == 1){
				SDL_Rect r1 = { 130, 100, 40,40 };
				SDL_Rect r11 = {132,102, 36,36};
				SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
				SDL_RenderFillRect(ren, &r1);
				SDL_SetRenderDrawColor(ren, 0xFF, 0x0, 0xFF,0x00);
				SDL_RenderFillRect(ren, &r11);	
			}
			
			else if(vencedor == 2){
				SDL_Rect r2 = { 130, 100, 40,40 };
				SDL_Rect r22 = {132,102, 36,36};
				SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
				SDL_RenderFillRect(ren, &r2);
		    	SDL_SetRenderDrawColor(ren, 0x93, 0x70, 0xDB, 0x00);
				SDL_RenderFillRect(ren, &r22);	
			}
			
			else if(vencedor == 3){
				SDL_Rect r3 = { 130, 100, 40,40 };
				SDL_Rect r33 = {132,102, 36,36};
				SDL_SetRenderDrawColor(ren, 0x0,0x0,0x0,0x00);
				SDL_RenderFillRect(ren, &r3);
			  SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF,0x00);
				SDL_RenderFillRect(ren, &r33);	
			}
						
			SDL_RenderPresent(ren);
			if (isevt) {
				espera -= (SDL_GetTicks() - antes);
				if (espera < 0) {
				    espera = 0;
				}
				    
		        if (evt.type == SDL_QUIT){
		        	continua = 0;
		        	break;
		        }
		        
		        if (evt.type == SDL_KEYDOWN){
		        	switch(evt.key.keysym.sym){
		        		case SDLK_ESCAPE:
		        			telafinal = 0; //apaga final 
		        			telacorrida = 1; //inicia a corrida
		        			continua = 1; //continua exibicao
		        	}
		        }
		}
	}
}

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
