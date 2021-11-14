#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "tabla.h"
#include "megjelenit.h"
#include "Mezo.h"
#include "debugmalloc.h"


Palya *Field(){
    //Visszateresi erteke a p Palya
    Palya *p = (Palya*) malloc(sizeof(Palya));    //Foglalunk egy palyanyi helyet a p pointernek.
    p->palya = (Mezo**) malloc(8*sizeof(Mezo*));    //Majd foglalunk 8 db Mezo meretu palyat(mezot)

    for(int i = 0;i<8;i++){
        p->palya[i] = (Mezo *) malloc(8*sizeof(Mezo)); //Itt ugyanez majdnem megegyszer 8-szor, mivel egy palya 64 mezobol all (8*8=64)
    }

    for(int i =0;i<8;i++){
        for(int j = 0;j<8;j++){
            poz(i,j,&(p->palya[i][j]));   //Itt beallitjuk a mezoknek az x,y koordinatajat a palyan
        }
    }
    kezdes(p);
    return p;
}

void kezdes(Palya *p){
    for(int i = 0;i<8;i++){
       for(int j = 0;j<8;j++){ //Hozzafuzzuk a kulonbozo mezokhoz a szint illetve babu tipusokat, soronkent es oszloponkent
           if(j>1 && j<6){
            p->palya[i][j].babu.tipus = nincs;
            p->palya[i][j].babu.szin = nincs2;
           }else{
               if(j<2){
                p->palya[i][j].babu.szin = fekete;
                if(j==1){
                    p->palya[i][j].babu.tipus = paraszt;
                }
           }
            if(j>5){
                p->palya[i][j].babu.szin = feher;
            if(j==6){
                 p->palya[i][j].babu.tipus = paraszt;
               }
           }
           }
           if(j==0 || j==7){
                p->palya[0][j].babu.tipus = bastya;
                p->palya[1][j].babu.tipus = lo;
                p->palya[2][j].babu.tipus = futo;
                p->palya[3][j].babu.tipus = vezer;
                p->palya[4][j].babu.tipus = kiraly;
                p->palya[5][j].babu.tipus = futo;
                p->palya[6][j].babu.tipus = lo;
                p->palya[7][j].babu.tipus = bastya;
           }
        }
    }
}

void felszabadit(Palya *p){
    for(int i = 0;i<8;i++){
        free(p->palya[i]);      //Amit fentebb lefoglaltunk azt fel kell oldani, forditott sorrendben, mint ahogy lefoglaltuk!
    }
    free(p->palya);
    free(p);
}
