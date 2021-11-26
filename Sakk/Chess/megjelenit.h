#ifndef MEGJELENIT
#define MEGJELENIT
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "tabla.h"
#include "debugmalloc.h"


typedef struct rajzolo{
    SDL_Texture *texture;
    SDL_Window *window;        //Maga a rajzolo struktura itt kapott helyet, koncepcio ugyanaz mint eddig, inicializatlunk valtozokat, amik egy blokkba kell hogy tartozzanak
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    Palya *p;
}rajzolo;

bool promote;

void sdl_init(rajzolo *r,int szeles, int magas);
rajzolo *rajzol(Palya *p);
void indul();
void lepes(rajzolo *r,SDL_Event event);
bool lephet(int elozoX,int elozoY,int mostX,int mostY,Tipus tBabu,Babuszin szBabu,Palya *p);
Mezo *mezopoz(Palya *p,SDL_Event e);
void megjelenites(rajzolo *r);
void baburajz(rajzolo *r,Babuszin szin,Tipus t,int i,int j);
void szabadit(rajzolo *r);

#endif // MEGJELENIT
