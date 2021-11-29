#ifndef MEGJELENIT
#define MEGJELENIT
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
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

bool lepes;
bool shortSideFeher;
bool longSideFeher;
bool shortSideFekete;
bool longSideFekete;
bool isCheck;
bool hibas;
bool kastely;
int bKiralyX;
int bKiralyY;
int fKiralyX;
int fKiralyY;
int teszt[8];

void sdl_init(rajzolo *r,int szeles, int magas);
void felirat(rajzolo *r,int x,int y,char *szoveg,int bMeret,int red,int g,int b,char *srcFile);
rajzolo *rajzol(Palya *p);
void indul();
bool ideOda(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2);
bool keresztbe(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2);
bool lovacska(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2);
bool lephet(int elozoX,int elozoY,int mostX,int mostY,Tipus tBabu,Babuszin szBabu,Palya *p,rajzolo *r,FILE *fp);
void megjelenites(rajzolo *r);
void baburajz(rajzolo *r,Babuszin szin,Tipus t,int i,int j);
void szabadit(rajzolo *r);

#endif // MEGJELENIT
