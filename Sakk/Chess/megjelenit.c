#include "megjelenit.h"
#include "debugmalloc.h"


void sdl_init(rajzolo *r,int szeles, int magas) {  //Az egyik legfontosabb fuggveny, enelkul nem mukodik a program, leinicializalja az ablakot es megjeleniti azt
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    r->window = SDL_CreateWindow("Chess Addict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (r->window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    r->renderer = SDL_CreateRenderer(r->window, -1, SDL_RENDERER_SOFTWARE);
    if (r->renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(r->renderer);
}


rajzolo *rajzol(Palya *pa){
    rajzolo *r = (rajzolo *) malloc(sizeof(rajzolo)); // Az egesz rajzolashoz egy rajzolo meretu memoriat kell lefoglalni csak, nem kell tobbet
    sdl_init(r,800,800);
    r->p = pa;
    r->texture = IMG_LoadTexture(r->renderer,"pieces.png"); //A babuk kepeinek forrasanak betoltese
    return r;
}

void indul(){ //Az osszes meghivas ami a kepernyore valo megjelenitest lehetove teszi azt itt hivodik meg, illetve a malloc-kal lefoglalt memoriat is itt szabaditjuk fel a lefoglalas sorrendjeben.
    rajzolo *r = rajzol(Field());
    megjelenites(r);
    SDL_Event event;
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) {
    }
    felszabadit(r->p);
    szabadit(r);
}

void megjelenites(rajzolo *r){
    for(int i = 0;i<8;i++){
        for(int j=0;j<8;j++){
            if((i+j)%2==0){ //Megnezzuk, hogyha a i+j osszege oszthato-e kettovel maradek nelkul, akkor feher legyen a mezo, ha meg nem akkor meg fekete, illetve itt rajzoljuk ki a babukat a tablara
                 boxRGBA(r->renderer,100+100*i,100+100*j,100*i,100*j,221,230,238,255);
                 baburajz(r,r->p->palya[i][j].babu.szin,r->p->palya[i][j].babu.tipus,i,j);
            }else{
                boxRGBA(r->renderer,100+100*i,100+100*j,100*i,100*j,123,155,176,255);
                baburajz(r,r->p->palya[i][j].babu.szin,r->p->palya[i][j].babu.tipus,i,j);
            }
        }
    }
    SDL_RenderPresent(r->renderer);
}


void baburajz(rajzolo *r,Babuszin sz,Tipus t,int i,int j){
    if(t != nincs){
        SDL_Rect src = {(t%6)*62+10, (sz%2)*60+10, MERET, MERET}; //A koncepcio az, hogy a forraskepbol pixelek es szin alapjan kiszedjuk a megfelelo babut
        SDL_Rect dest = {100*i,100*j,MERET+40,MERET+40};
        SDL_RenderCopy(r->renderer,r->texture,&src,&dest);
    }
}

void szabadit(rajzolo *r){
    SDL_DestroyRenderer(r->renderer); //Felszabadito fuggveny, amit fent lefoglaltunk memoriat felszabaditjuk, de elobb a beepitett SDL konyvtarbol a renderer-t, az ablakot, illetve a texturat megsemmisitjuk a memoria pazarlast elkerulve.
    SDL_DestroyTexture(r->texture);
    SDL_DestroyWindow(r->window);
    free(r);
}

