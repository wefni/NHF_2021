#include "megjelenit.h"
#include "debugmalloc.h"


void sdl_init(rajzolo *r,int szeles, int magas)    //Az egyik legfontosabb fuggveny, enelkul nem mukodik a program, leinicializalja az ablakot es megjeleniti azt
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    r->window = SDL_CreateWindow("Chess Addict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (r->window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    r->renderer = SDL_CreateRenderer(r->window, -1, SDL_RENDERER_SOFTWARE);
    if (r->renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawColor(r->renderer,120, 130, 135,255);
    SDL_RenderClear(r->renderer);
}

void felirat(rajzolo *r,int x,int y)
{
    SDL_Rect hova = { 0, 0, 0, 0 };
    SDL_Color feher = {255, 255, 255};

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("FrontPageNeue.otf", 28);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    r->surface = TTF_RenderUTF8_Solid(font, "RESIGN", feher);
    r->texture = SDL_CreateTextureFromSurface(r->renderer, r->surface);
    hova.x = (x - r->surface->w);
    hova.y = y;
    hova.w = r->surface->w;
    hova.h = r->surface->h;
    SDL_RenderCopy(r->renderer, r->texture, NULL, &hova);
    SDL_FreeSurface(r->surface);
    SDL_DestroyTexture(r->texture);
}

rajzolo *rajzol(Palya *pa)
{
    rajzolo *r = (rajzolo *) malloc(sizeof(rajzolo)); // Az egesz rajzolashoz egy rajzolo meretu memoriat kell lefoglalni csak, nem kell tobbet
    sdl_init(r,1000,800);
    r->p = pa;
    r->texture = IMG_LoadTexture(r->renderer,"pieces.png"); //A babuk kepeinek forrasanak betoltese

    return r;
}

void indul()  //Az osszes meghivas ami a kepernyore valo megjelenitest lehetove teszi azt itt hivodik meg, illetve a malloc-kal lefoglalt memoriat is itt szabaditjuk fel a lefoglalas sorrendjeben.
{
    rajzolo *r = rajzol(Field());
    megjelenites(r);
    bool quit = false;
    bool click = false;
    bool
    felvett = false;
    int xkatt = 0;
    int ykatt = 0;
    Mezo m;
    int db = 2;
    promote = false;

    felirat(r,900,100);

    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        SDL_RenderPresent(r->renderer);
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if(felvett)
                {
                    m.x = event.button.x/100;
                    m.y = event.button.y/100;
                    printf("%d %d\n",m.x,m.y);
                        if(m.y==0 && r->p->palya[xkatt][ykatt].babu.tipus == 5 || m.y==7 && r->p->palya[xkatt][ykatt].babu.tipus == 5){
                            if((xkatt+ykatt)%2==0)
                                boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,221,230,238,255);
                            else
                                boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,123,155,176,255);

                            if((m.x+m.y)%2==0)
                                boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,221,230,238,255);
                            else
                                boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,123,155,176,255);

                            baburajz(r,m.babu.szin,1,m.x,m.y);
                            r->p->palya[xkatt][ykatt].babu.tipus = 6;
                            r->p->palya[xkatt][ykatt].babu.szin = 2;
                            r->p->palya[m.x][m.y].babu.tipus = 1;
                            r->p->palya[m.x][m.y].babu.szin = m.babu.szin;
                            db++;
                            promote = false;
                            felvett = false;
                            break;
                        }
                    //printf("%d\n",m.babu.szin);

                    if(xkatt == m.x && ykatt == m.y) break;
                    if(lephet(xkatt,ykatt,m.x,m.y,m.babu.tipus,m.babu.szin,r->p))
                    {
                        if((xkatt+ykatt)%2==0)
                            boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,221,230,238,255);
                        else
                            boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,123,155,176,255);

                        if((m.x+m.y)%2==0)
                            boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,221,230,238,255);
                        else
                            boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,123,155,176,255);

                        baburajz(r,m.babu.szin,m.babu.tipus,m.x,m.y);
                        r->p->palya[xkatt][ykatt].babu.tipus = 6;
                        r->p->palya[xkatt][ykatt].babu.szin = 2;
                        r->p->palya[m.x][m.y].babu.tipus = m.babu.tipus;
                        r->p->palya[m.x][m.y].babu.szin = m.babu.szin;
                        db++;
                    }
                    felvett = false;

                    break;
                }
                if(event.button.x>800)
                {
                    break;
                }
                xkatt = event.button.x/100;
                ykatt = event.button.y/100;
                click = true;
                if(click)
                {
                    if(db%2==1 && r->p->palya[xkatt][ykatt].babu.szin==1)
                    {
                        m.babu.tipus = r->p->palya[xkatt][ykatt].babu.tipus;
                        m.babu.szin = r->p->palya[xkatt][ykatt].babu.szin;
                        m.x = xkatt;
                        m.y = ykatt;

                        if(m.babu.tipus != 6)
                        {
                            felvett = true;
                        }
                        printf("%d %d\n",m.x,m.y);
                    }

                    if(db%2==0 && r->p->palya[xkatt][ykatt].babu.szin==0)
                    {
                        m.babu.tipus = r->p->palya[xkatt][ykatt].babu.tipus;
                        m.babu.szin = r->p->palya[xkatt][ykatt].babu.szin;
                        m.x = xkatt;
                        m.y = ykatt;

                        if(m.babu.tipus != 6)
                        {
                            felvett = true;
                        }
                        printf("%d %d\n",m.x,m.y);
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                click = false;
            }
            break;

        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    felszabadit(r->p);
    szabadit(r);
}

bool ideOda(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2)
{
    int db = 0;
    int i = 0;
    if(mostX == elozoX && mostY == elozoY) return false;
    if(mostX<8)
    {
        if(mostX == elozoX)
        {
            if((elozoY-mostY)<0)
            {
                i = 1;
                elozoY++;
            }
            else
            {
                i = -1;
                elozoY--;
            }

            while(elozoY != mostY)
            {
                if(p->palya[mostX][elozoY].babu.tipus != 6 && p->palya[mostX][elozoY].babu.szin < 2)
                {
                    db++;
                }
                elozoY = elozoY+i;
            }
            if(p->palya[mostX][mostY].babu.szin != szin2 && db==0)return true;
        }
        else if(mostY == elozoY)
        {
            if((elozoX-mostX)<0)
            {
                i = 1;
                elozoX++;
            }
            else
            {
                i = -1;
                elozoX--;
            }

            while(elozoX != mostX)
            {
                if(p->palya[elozoX][mostY].babu.tipus != 6 && p->palya[elozoX][mostY].babu.szin < 2)
                {
                    db++;
                }
                elozoX = elozoX+i;
            }
            if(p->palya[mostX][mostY].babu.szin != szin2 && db==0)return true;
        }
    }
    return false;
}

bool keresztbe(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2)
{
    if(elozoX != mostX && elozoY != mostY && mostX<8)
    {
        int meredekseg = ((mostX-elozoX)/(mostY-elozoY));
        int tangens = abs(meredekseg);
        int dir = elozoY-mostY;
        int db = 0;

        if(tangens == 1)
        {
            if(dir>0) //Pozitiv oldal
            {
                if(meredekseg==-1)
                {
                    while(mostX != elozoX && mostY != elozoY)
                    {
                        elozoX++;
                        elozoY--;
                        if(p->palya[elozoX][elozoY].babu.szin == szin2) db++;
                    }
                    if(db ==0)return true;
                }
                else
                {
                    while(mostX != elozoX && mostY != elozoY)
                    {
                        elozoX--;
                        elozoY--;
                        if(p->palya[elozoX][elozoY].babu.szin == szin2) db++;
                    }
                    if(db ==0)return true;
                }
            }
            else
            {
                if(meredekseg==-1)
                {
                    while(mostX != elozoX && mostY != elozoY)
                    {
                        elozoX--;
                        elozoY++;
                        if(p->palya[elozoX][elozoY].babu.szin == szin2) db++;
                    }
                    if(db ==0)return true;
                }
                else
                {
                    while(mostX != elozoX && mostY != elozoY)
                    {
                        elozoX++;
                        elozoY++;
                        if(p->palya[elozoX][elozoY].babu.szin == szin2) db++;
                    }
                    if(db ==0)return true;
                }
            }
        }
        return false;
    }
    return false;
}

bool lovacska(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2)
{
    int X[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int Y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    int db = 0;

    for (int i = 0; i < 8; i++)
    {
        if(mostX == elozoX + X[i] && mostY == elozoY+Y[i]&&p->palya[mostX][mostY].babu.szin != szin2) return true;
    }
    return false;
}

bool lephet(int elozoX,int elozoY,int mostX,int mostY,Tipus tBabu,Babuszin szBabu,Palya *p)
{
    int db = 0; // Castle
    int db2 = 2;
    bool lepett = false;
    switch(tBabu)
    {
    case 0://KIRALY
        if(szBabu==0)
        {
            if(abs(mostX-elozoX)<=1 && abs(mostY-elozoY)<=1 && p->palya[mostX][mostY].babu.szin != p->palya[elozoX][elozoY].babu.szin) return true;
        }
        else
        {
            if(abs(mostX-elozoX)<=1 && abs(mostY-elozoY)<=1 && p->palya[mostX][mostY].babu.szin != p->palya[elozoX][elozoY].babu.szin) return true;
        }
        break;
    case 1: //KIRALYNO
        if(szBabu==0)
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,0))return true;
            if(keresztbe(elozoX,elozoY,mostX,mostY,p,0))return true;
        }
        else
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,1))return true;
            if(keresztbe(elozoX,elozoY,mostX,mostY,p,0))return true;
        }
        break;
    case 2://BASTYA
        if(szBabu==0)
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,0))return true;
        }
        else
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,1))return true;
        }
        break;
    case 3: //FUTO
        if(szBabu==0)
        {
            if(keresztbe(elozoX,elozoY,mostX,mostY,p,0))return true;
        }
        else
        {
            if(keresztbe(elozoX,elozoY,mostX,mostY,p,1))return true;
        }
        break;
    case 4://LO
        if(szBabu==0)
        {
            if(lovacska(elozoX,elozoY,mostX,mostY,p,0)) return true;
        }
        else
        {
            if(lovacska(elozoX,elozoY,mostX,mostY,p,1)) return true;
        }
        break;
    case 5://PARASZT
        if(szBabu == 0)
        {
            lepett = true;
            if(lepett)
            {
                db2++;
                if(elozoY == 6)
                {
                    if(elozoY>mostY && mostX == elozoX && elozoY-mostY < db2)return true;
                }
                if(abs(mostX-elozoX)==1 && abs(mostY-elozoY)==1 && p->palya[mostX][mostY].babu.szin==1) return true;
                if(elozoY-mostY==1 && p->palya[mostX][mostY].babu.tipus == 6 && mostX == elozoX) return true;
            }
        }
        else
        {
            lepett = true;
            if(lepett)
            {
                db2++;
                if(elozoY == 1)
                {
                    if(elozoY<mostY && mostX == elozoX && mostY-elozoY < db2)return true;
                }
                if(abs(mostX-elozoX)==1 && abs(mostY-elozoY)==1 && p->palya[mostX][mostY].babu.szin==0) return true;
                if(mostY-elozoY==1 && p->palya[mostX][mostY].babu.tipus == 6 && mostX == elozoX) return true;
            }
        }
        break;
    }
    return false;
}

void megjelenites(rajzolo *r)
{
    char betuk[] = "A";
    char szamok[] = "8";

    for(int i = 0; i<8; i++) //Vegig megyunk a 8 soron és oszlopon
    {
        for(int j=0; j<8; j++)
        {
            if((i+j)%2==0)  //Megnezzuk, hogyha a i+j osszege oszthato-e kettovel maradek nelkul, akkor feher legyen a mezo, ha meg nem akkor meg fekete, illetve itt rajzoljuk ki a babukat a tablara
            {
                boxRGBA(r->renderer,100+100*i,100+100*j,100*i,100*j,221,230,238,255); //Ez pedig magáért a mezőkért felelős, hogy megjelenitse oket
                baburajz(r,r->p->palya[i][j].babu.szin,r->p->palya[i][j].babu.tipus,i,j); //Meghivjuk a baburajz fuggvenyt, hogy szin es tipus alapjan megjelenitsuk oket az elore meghatarozott helyukre
            }
            else
            {
                boxRGBA(r->renderer,100+100*i,100+100*j,100*i,100*j,123,155,176,255);
                baburajz(r,r->p->palya[i][j].babu.szin,r->p->palya[i][j].babu.tipus,i,j);
            }
        }
        //stringRGBA(r->renderer,i*100+45,790,betuk,0,0,0,255);
        //stringRGBA(r->renderer,5,i*100+45,szamok,0,0,0,255);
        //betuk[0]++;
        //szamok[0]--;
    }
    thickLineRGBA(r->renderer,801,0,801,801,4,0,0,0,255);
    thickLineRGBA(r->renderer,801,2,999,2,4,0,0,0,255);
    thickLineRGBA(r->renderer,998,2,998,798,4,0,0,0,255);
    thickLineRGBA(r->renderer,998,798,801,798,4,0,0,0,255);
    //felirat(r,850,25);
}


void baburajz(rajzolo *r,Babuszin sz,Tipus t,int i,int j)
{
    if(t != nincs)
    {
        SDL_Rect src = {(t%6)*62+10, (sz%2)*60+10, MERET, MERET}; //A koncepcio az, hogy a forraskepbol pixelek es szin alapjan kiszedjuk a megfelelo babut
        SDL_Rect dest = {100*i,100*j,MERET+40,MERET+40};
        SDL_RenderCopy(r->renderer,r->texture,&src,&dest);
    }
}

void szabadit(rajzolo *r)
{
    SDL_DestroyRenderer(r->renderer); //Felszabadito fuggveny, amit fent lefoglaltunk memoriat felszabaditjuk, de elobb a beepitett SDL konyvtarbol a renderer-t, az ablakot, illetve a texturat megsemmisitjuk a memoria pazarlast elkerulve.
    SDL_DestroyTexture(r->texture);
    SDL_DestroyWindow(r->window);
    free(r);
}
