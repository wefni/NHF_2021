#ifndef MEGJELENIT
#define MEGJELENIT
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <SDL2_gfxPrimitives.h>
#include "InitializeGame.h"
#include "debugmalloc.h"
#include <stdbool.h>

typedef struct rajzolo{
    SDL_Texture *texture;
    SDL_Window *window;        //Maga a rajzolo struktura itt kapott helyet, koncepcio ugyanaz mint eddig, inicializatlunk valtozokat, amik egy blokkba kell hogy tartozzanak
    SDL_Renderer *renderer;
    Palya *p;
}rajzolo;

void sdl_init(rajzolo *r,int szeles, int magas);
rajzolo *rajzol(Palya *p);
void megjelenites(rajzolo *r);
void mRajz(int i,int j,rajzolo *r,bool b);
void baburajz(rajzolo *r,Babuszin szin,Tipus t,int i,int j);
void szabadit(rajzolo *r);

#endif // MEGJELENIT
