#ifndef MEZO
#define MEZO
#include "debugmalloc.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

typedef enum Tipus{
    kiraly,vezer,bastya,futo,lo,paraszt,nincs     //A bábuk tipusai, illetve egy nincs tipus az ures mezok megkulonboztetesere
}Tipus;

typedef enum Babuszin{
    feher,fekete,nincs2   //Szinek, ugyanigy a nincs2 az ures mezore vonatkozik
}Babuszin;

typedef struct Babu{
    Tipus tipus;
    Babuszin szin;
}Babu;

typedef struct Mezo{
    Babu babu;         //Egy mezo alapvetoen egy babubol es egy x,y koordinatabol all
    int x,y;
}Mezo;

enum {MERET=52};     //A babuknak a forras fajlban levo merete

void p(int x,int y,Mezo *m);

#endif // MEZO
