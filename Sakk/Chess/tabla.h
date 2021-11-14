#ifndef INIT_GAME
#define INIT_GAME
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <SDL2_gfxPrimitives.h>
#include "Mezo.h"

typedef struct Palya{
    Mezo **palya;    //Maga a tablanak a mezoi ezzel a strukturaval valosul meg, igy lehet x,y vagyis i,j koordinataja egy mezonek
}Palya;

Palya *Field();
void kezdes(Palya *p);  //A fuggvenyeket azert kellett ide is beirni, hogy a tobbi modulbol is elerhetoek legyenek, igy van ez a tobbi headerben is.
void felszabadit(Palya *p);
#endif // INIT_GAME
