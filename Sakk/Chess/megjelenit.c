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

void felirat(rajzolo *r,int x,int y,char *szoveg,int bMeret,int red,int g,int b,char *srcFile) //Lenyegeben egy tetszoleges szoveget ir ki a kepernyore, a tetszoleges szinben es betutipussal
{
    SDL_Rect hova = { 0, 0, 0, 0 };
    SDL_Color feher = {red, g, b};

    TTF_Init();
    TTF_Font *font = TTF_OpenFont(srcFile, bMeret);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    r->surface = TTF_RenderUTF8_Solid(font, szoveg, feher);
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

    felirat(r,950,170,"RESIGN",28,255, 250, 250,"fonts/CollegiateBlackFLF.ttf");//Fent
    felirat(r,950,600, "RESIGN",28,255, 250, 250,"fonts/CollegiateBlackFLF.ttf");//Lent

    felirat(r,996,340, "CHESS",60,255, 255, 255,"fonts/Fine College.ttf");
    felirat(r,998,390, "ADDICT",55,255, 255, 255,"fonts/Fine College.ttf");

    felirat(r,943,270,"GAME",30,255,250,250,"fonts/CollegiateBlackFLF.ttf");
    felirat(r,990,500,"PREVIOUS GAME", 20,255,250,250,"fonts/CollegiateBlackFLF.ttf");

    r->p = pa;
    r->texture = IMG_LoadTexture(r->renderer,"asset/pieces.png"); //A babuk kepeinek forrasanak betoltese

    return r;
}

void indul()  //Az osszes meghivas ami a kepernyore valo megjelenitest lehetove teszi azt itt hivodik meg, illetve a malloc-kal lefoglalt memoriat is itt szabaditjuk fel a lefoglalas sorrendjeben.
{
    rajzolo *r = rajzol(Field()); //Legeneralja az egesz tablat
    FILE *fp;

    megjelenites(r); //Megjeleniti vizualisan a tablat es a babukat
    bool quit = false;
    bool meccs = true;
    bool click = false;
    bool resign = false;
    bool felvett = false; //A program mukodesehez szuksege valtozok inicalizalasa
    isCheck = false;
    kastely = false;
    bKiralyX = 4;
    bKiralyY = 0;
    fKiralyX = 4;
    fKiralyY = 7;
    int xkatt = 0; //Ahova eloszor kattintott x koordinataja
    int ykatt = 0; //Ahova eloszor kattintott y koordinataja
    Mezo m;  //Szukseges volt egy temporary mezo, ahhoz, hogy a lepeseket veghez vigyuk
    int db = 0; //A darab valtozo a felvaltott lepeshez szukseges

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
                if(event.button.x>=830 && event.button.x<=960 && event.button.y>=257 && event.button.y<=303)
                {
                    meccs = true;
                    quit = true;
                }
                else if((event.button.x>=805 && event.button.x<=991 && event.button.y>=490 && event.button.y<=525))
                {
                    meccs = false;
                    quit = true;
                }
            }
            break;
        }
    }
    quit = false;
    if(meccs)
    {
        fp = fopen("meccs.txt","wt");
        if(fp == NULL)
        {
            perror("Nem sikerült megnyitni a fájlt!");
        }

        while (!quit)
        {

            SDL_Event event;
            SDL_WaitEvent(&event);
            SDL_RenderPresent(r->renderer);
            switch (event.type)
            {

            case SDL_MOUSEBUTTONDOWN:

                if(resign) break; //Ha feladta az egyik versenyzo, ne lehessen osszevissza kattintgatni es lepkedni, ezzel a sorral nem tud mar semmit se csinalni, csak kilepni a programbol
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    //*******************************GOMBOK***********************************
                    if(event.button.x>=820 && event.button.x<=969 && event.button.y>=150 && event.button.y<=217)  //Ha ebbe a tartomanyba kattint akkor a fekete feladja
                    {

                        boxRGBA(r->renderer,0,0,800,800,145, 137, 137,120);
                        felirat(r,575,350,"WHITE WINS!",40,255, 255, 255,"fonts/CollegiateBlackFLF.ttf");
                        resign = true;
                    }
                    else if(event.button.x>=820 && event.button.x<=969 && event.button.y>=579 && event.button.y<=644)  //Ha ebbe a tartomanyba kattint akkor a feher feladja
                    {
                        boxRGBA(r->renderer,0,0,800,800,145, 137, 137,120);
                        felirat(r,575,350,"BLACK WINS!",40,0, 0, 0,"fonts/CollegiateBlackFLF.ttf");
                        resign = true;
                    }

                    //*******************************GOMBOK***********************************

                    //****************************Masodik kattintas***************************
                    if(felvett) //Felvett bool valtozo felel azert, hogy az eloszor rakattintotatt jelezze itt
                    {
                        if(event.button.x>800) break; //Ne tudjon lehelyezni babut a tablan kivulre, mivel 1000*800-as a tabla, ezert csak a x koordinatat kell vizsgalni
                        m.x = event.button.x/100;
                        m.y = event.button.y/100; //Az elobb inicializalt temporary m mezonek, egy x,y koordinatat adunk, ez a mezo lesz majd ahova a jatekos le akarja rakni a babut

                        //PROMOTE ESETEN
                        if(m.y==0 && r->p->palya[xkatt][ykatt].babu.tipus == 5 && r->p->palya[xkatt][ykatt].babu.szin != 1 || m.y==7 && r->p->palya[xkatt][ykatt].babu.tipus == 5 && r->p->palya[xkatt][ykatt].babu.szin != 0)  //Meg kellett vizsgalni, ha az egyik paraszt eleri a
                        {
                            if((xkatt+ykatt)%2==0)                                                                                         //szamara vegallomast, akkor "atalakuljon", ez minden esetben egy kiralyno lesz
                                boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,221,230,238,255);                         //mert az a legerosebb babu a jatekban es igy a leghasznosabb is
                            else
                                boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,123,155,176,255);     //Mivel az adat struktura nem enged egyesevel torolni babut a tablarol
                            if((m.x+m.y)%2==0)                                                                        //igy minden lepesnel lehelyezunk egy ugyanakkora negyzetet a szinehez megfeleloen
                                boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,221,230,238,255);            //hogy ne maradjon babu az elmozgataskor
                            else
                                boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,123,155,176,255);

                            baburajz(r,m.babu.szin,1,m.x,m.y);          //Kiralyno kirajzolasa
                            r->p->palya[xkatt][ykatt].babu.tipus = 6;
                            r->p->palya[xkatt][ykatt].babu.szin = 2;    //Atallitjuk a forras mezo ertekeit 6 ra es 2 re, mert az enumban ezek azok az ertekek, amik azt jelentik hogy a mezon nincs semmi
                            r->p->palya[m.x][m.y].babu.tipus = 1;       //Majd a cel mezonek az 1 es erteket adjuk, mert az enumban az a kiralyno, majd a szint is atadjuk a mezonek
                            r->p->palya[m.x][m.y].babu.szin = m.babu.szin;
                            db^=1;                                       //Itt a db-t noveljuk, hogy a fekete kovetkezzen
                            fprintf(fp,"%d %d %d %d %d %d\n",xkatt,ykatt,m.x,m.y,m.babu.szin,1);
                            felvett = false;                            //Majd hamisra allitjuk a felvettet, hogy ujra ki tudja jelolni a mozgatni kivant babut
                            break;
                        }

                        //SIMA LEPES ESETEN
                        if(xkatt == m.x && ykatt == m.y) break;                 //Itt kikuszoboljuk hogy ha ugyanarra a mezore szeretne-e lepni a jatekos, igy nem fagy ki a jatek
                        if(lephet(xkatt,ykatt,m.x,m.y,m.babu.tipus,m.babu.szin,r->p,r,fp)) //A lephet fuggvenyrol lentebb lesz szo, ez a fuggveny diohejban egy bool-t ad vissza, ha igaz engedi a lepest, ha nem, nem
                        {
                            if(hibas)  break;

                            if(kastely)  //Feher rovid sánc
                            {
                                db^=1;
                                kastely = false;
                                felvett = false;
                                break;
                            }

                            if((xkatt+ykatt)%2==0)
                                boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,221,230,238,255);
                            else
                                boxRGBA(r->renderer,xkatt*100,ykatt*100,100*xkatt+99,100*ykatt+99,123,155,176,255); //Ezek ugyanazok mint fent, csak nem a kiralynot rajzoljuk ki, hanem a kijelolt babut

                            if((m.x+m.y)%2==0)
                                boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,221,230,238,255);
                            else
                                boxRGBA(r->renderer,m.x*100,m.y*100,100*m.x+99,100*m.y+99,123,155,176,255);

                            baburajz(r,m.babu.szin,m.babu.tipus,m.x,m.y);
                            r->p->palya[xkatt][ykatt].babu.tipus = 6;
                            r->p->palya[xkatt][ykatt].babu.szin = 2;
                            r->p->palya[m.x][m.y].babu.tipus = m.babu.tipus;
                            r->p->palya[m.x][m.y].babu.szin = m.babu.szin;
                            db^=1;
                            fprintf(fp,"%d %d %d %d %d %d\n",xkatt,ykatt,m.x,m.y,m.babu.szin,m.babu.tipus);
                        }
                        felvett = false;
                        break;
                    }


                    //**************************Masodik kattintas vege************************


                    //****************************Elso kattintas******************************

                    xkatt = event.button.x/100;     //Megadjuk a forras mezo x erteket, azzal hogy elosztjuk 100-al a koordinatakat maradeknelkul ossze tudjuk hasonlitani majd kesobb a forras es cel mezo koordinatait
                    ykatt = event.button.y/100;
                    click = true;
                    if(click)
                    {
                        if(event.button.x>800) break;

                        if(db==1 && r->p->palya[xkatt][ykatt].babu.szin==1)   //Ha a fekete jon
                        {
                            m.babu.tipus = r->p->palya[xkatt][ykatt].babu.tipus; //Beallitjuk a temporary mezonek, hogy mi a tipusa az adott mezonek amire kattintott majd atadjuk a temporary mezonek
                            m.babu.szin = r->p->palya[xkatt][ykatt].babu.szin; //Itt a szint adjuk at
                            m.x = xkatt;    //Itt pedig a koordinatakat
                            m.y = ykatt;

                            if(m.babu.tipus < 6) //Ha nem ures mezore kattintottunk, akkor ervenyes a kijeloles
                            {
                                felvett = true;
                            }
                        }

                        if(db==0 && r->p->palya[xkatt][ykatt].babu.szin==0)//Ha pedig a feher jon
                        {
                            m.babu.tipus = r->p->palya[xkatt][ykatt].babu.tipus;
                            m.babu.szin = r->p->palya[xkatt][ykatt].babu.szin; //Ugyanaz mint feljebb
                            m.x = xkatt;
                            m.y = ykatt;

                            if(m.babu.tipus < 6)
                            {
                                felvett = true;
                            }
                        }
                    }
                }
                break;
            //****************************Elso kattintas vege******************************

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    click = false;
                }
                break;

            case SDL_QUIT:
                quit = true; //Itt a while ciklus kilepese
                break;
            }
        }


    }
    else
    {
        fp = fopen("meccs.txt","r");
        if(fp ==NULL)
        {
            perror("Nem sikerült megnyitni a fájlt!");
        }
        while (!quit)
        {
            SDL_Event event;
            SDL_WaitEvent(&event);
            SDL_RenderPresent(r->renderer);

            bool rajzoltam = false;

            switch (event.type)
            {
            /* eger kattintas */
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    int x,y,szin,tip,elozox,elozoy;
                    fscanf(fp,"%d %d %d %d %d %d\n",&elozox,&elozoy,&x,&y,&szin,&tip);
                    if((elozox+elozoy)%2==0)                                                                                         //szamara vegallomast, akkor "atalakuljon", ez minden esetben egy kiralyno lesz
                        boxRGBA(r->renderer,elozox*100,elozoy*100,100*elozox+99,100*elozoy+99,221,230,238,255);                         //mert az a legerosebb babu a jatekban es igy a leghasznosabb is
                    else
                        boxRGBA(r->renderer,elozox*100,elozoy*100,100*elozox+99,100*elozoy+99,123,155,176,255);     //Mivel az adat struktura nem enged egyesevel torolni babut a tablarol
                    if((x+y)%2==0)                                                                        //igy minden lepesnel lehelyezunk egy ugyanakkora negyzetet a szinehez megfeleloen
                        boxRGBA(r->renderer,x*100,y*100,100*x+99,100*y+99,221,230,238,255);            //hogy ne maradjon babu az elmozgataskor
                    else
                        boxRGBA(r->renderer,x*100,y*100,100*x+99,100*y+99,123,155,176,255);

                    baburajz(r,szin,tip,x,y);
                }
                break;

            case SDL_QUIT:
                quit = true;
                break;
            }
        }
    }

    fclose(fp);
    felszabadit(r->p); //Mivel itt vege a programnak, felszabaditjuk a lefoglalt memoriat
    szabadit(r);
}


void castle(int szin,rajzolo *r,int kezdoX,int kezdoY,int mostX,int mostY,bool rovid,FILE *fp)
{

    //Ha a szin 0 akkor fekete, ha 1 akkor feher, fel kellett cserelni, hogy kijojjenek az ertekek a rajzolasnal
    if(rovid)
    {
        if(szin==0)
        {
            boxRGBA(r->renderer,400,szin*700,499,szin*700+99,221,230,238,255);
            baburajz(r,1,0,6,szin*7); //Kiraly
            fprintf(fp,"%d %d %d %d %d %d\n",4,0,6,szin*7,1,0);
            boxRGBA(r->renderer,700,0,799,99,123,155,176,255);
            baburajz(r,1,2,5,0); //BastyaFekete
            fprintf(fp,"%d %d %d %d %d %d\n",7,0,5,0,1,2);
        }
        else
        {
            boxRGBA(r->renderer,400,szin*700,499,szin*700+99,123,155,176,255);
            baburajz(r,0,0,6,szin*7); //Kiraly
            fprintf(fp,"%d %d %d %d %d %d\n",4,7,6,szin*7,0,0);
            boxRGBA(r->renderer,700,700,799,799,221,230,238,255);
            baburajz(r,r->p->palya[7][7].babu.szin,r->p->palya[7][7].babu.tipus,5,7); //BastyaFeher
            fprintf(fp,"%d %d %d %d %d %d\n",7,7,5,7,0,2);
        }

        r->p->palya[kezdoX][kezdoY].babu.szin=2;
        r->p->palya[kezdoX][kezdoY].babu.tipus=6;
        if(szin==0) r->p->palya[mostX][mostY].babu.szin = 1;
        else r->p->palya[mostX][mostY].babu.szin = 0;
        r->p->palya[mostX][mostY].babu.tipus = 0;

        if(szin==0) r->p->palya[5][szin*7].babu.szin = 1;
        else r->p->palya[5][szin*7].babu.szin = 0;
        r->p->palya[5][szin*7].babu.tipus = 2;

        r->p->palya[7][szin*7].babu.szin = 2;
        r->p->palya[7][szin*7].babu.tipus = 6;
    }
    else
    {
        if(szin==0)
        {
            boxRGBA(r->renderer,400,0,499,99,221,230,238,255);
            baburajz(r,r->p->palya[4][0].babu.szin,r->p->palya[4][0].babu.tipus,2,0); //Kiraly
            fprintf(fp,"%d %d %d %d %d %d\n",4,0,2,0,1,0);
            boxRGBA(r->renderer,0,0,99,99,221,230,238,255);
            baburajz(r,r->p->palya[0][0].babu.szin,r->p->palya[0][0].babu.tipus,3,0); //Bastya
            fprintf(fp,"%d %d %d %d %d %d\n",0,0,3,0,1,2);
        }
        else
        {
            boxRGBA(r->renderer,400,700,499,799,123,155,176,255);
            baburajz(r,r->p->palya[4][7].babu.szin,r->p->palya[4][7].babu.tipus,2,7); //Kiraly
            fprintf(fp,"%d %d %d %d %d %d\n",4,7,2,7,0,0);
            boxRGBA(r->renderer,0,700,99,999,123,155,176,255);
            baburajz(r,r->p->palya[0][7].babu.szin,r->p->palya[0][7].babu.tipus,3,7); //Bastya
            fprintf(fp,"%d %d %d %d %d %d\n",0,7,3,7,0,2);
        }

        r->p->palya[kezdoX][kezdoY].babu.szin=2;
        r->p->palya[kezdoX][kezdoY].babu.tipus=6;

        if(szin==0) r->p->palya[mostX][mostY].babu.szin = 1;
        else r->p->palya[mostX][mostY].babu.szin = 0;
        r->p->palya[mostX][mostY].babu.tipus = 0;

        if(szin==0) r->p->palya[3][szin*7].babu.szin = 1;
        else r->p->palya[3][szin*7].babu.szin = 0;
        r->p->palya[3][szin*7].babu.tipus = 2;

        r->p->palya[0][szin*7].babu.szin = 2;
        r->p->palya[0][szin*7].babu.tipus = 6;
    }
}

bool ideOda(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2) //ideOda fuggvenyt lenyegeben megnezi, hogy tud-e egy sorban vagy oszlopban odalepni ahova akart a jatekos, a visszateresi erteke egy bool
{
    int db = 0;
    int i = 0;
    if(mostX == elozoX && mostY == elozoY) return false; //Nem lephet ugyanoda, ahol all
    if(mostX<8)
    {
        if(mostX == elozoX)                             //Ha a kattintas csak fel- vagy lefele tortent, akkor csak az Y koordinata valtozik, az X nem
        {
            if((elozoY-mostY)<0)                        //Megnezzuk hogy milyen iranyu a mozgas
            {
                i = 1;                                  //Itt az i-t 1-re allitjuk, mert ezzel szeretnenk inkrementalni a forras Y koordinatajat
                elozoY++;                               //Nem akarjuk megvizsgalni ott ahol pont all a babu, ezert megnoveljuk egyel az Y koordinatajat es onnan kezdjuk
            }
            else
            {
                i = -1;                                 //Itt -1 az i, mert a mozgas lefele tortenik
                elozoY--;                               //Nem akarjuk megvizsgalni ott ahol pont all a babu, ezert csokkentjuk egyel az Y koordinatajat es onnan kezdjuk
            }

            while(elozoY != mostY) //Addig megyunk, ameddig a forras Y koordinataja nem lesz egyenlo a cel Y koordinatajaval
            {
                if(p->palya[mostX][elozoY].babu.tipus != 6 && p->palya[mostX][elozoY].babu.szin < 2) db++; //Ha mozgas soran olyat tapasztalunk, hogy a mezonek amin at akarunk menni, az nem ures, akkor nem tudunk atmasirozni rajta, ezert kell ez a db valtozo, aminek noveljuk egyel az erteket ha ilyen tortenik
                elozoY = elozoY+i; //Itt noveljuk vagy csokkentjuk a forras Y koordinatajat attol fuggoen hogy fel vagy le tortenik a mozgas
            }
            if(p->palya[mostX][mostY].babu.szin != szin2 && db==0)return true; //Ha nem volt olyan hogy nem ures a mezo a mozgas soran es a cel mezonek a szine nem olyan mint a forras mezo babujanak a szine akkor a lepes megtortenhet, amugy nem
        }
        else if(mostY == elozoY) //Itt ugyanaz a tortenet, csak az X koordinata valtozik es a Y nem
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

bool keresztbe(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2) //Atlos mozgast teszi lehetove, bool-t ad vissza
{
    if(elozoX != mostX && elozoY != mostY && mostX<8) //Itt is kikuszoboljuk a lehetseges rossz lepeseket
    {
        float X = mostX-elozoX;
        float Y = mostY-elozoY;
        float meredekseg = X/Y; //Valojaban, ami itt tortenik az az, hogy megnezzuk hogy milyen meredek az egyenes a ket mezo kozott. Mivel ha kozeprol nezzuk akkor a ket atlo a 360 fokos belso szogeit a negyzetnek 4 reszre osztja plusz ha odakepzeljuk az X es Y tengelyt, akkor mar 8 reszre, es ha a 360-at elosztjuk 8-cal az 45, es tangens 45 fok az 1
        float tangens = abs(meredekseg);                    //ez azert jo, mert akkor tudjuk a ket mezo az egy egyenesen van rajta, aminek a meredeksege ±1
        int dir = elozoY-mostY;                           //Meg kellett neznunk az iranyat, ha minusz akkor a fenti feleben van a negyzetnek, ha plusz akkor a lentiben, mivel a (0,0) koordinata a bal felso sarokban van
        int db = 0;
        if(meredekseg<-1 || meredekseg>1) return false;
        if(tangens == 1.0)
        {
            if(dir>0) //Pozitiv oldal
            {
                if(meredekseg==-1.0)
                {
                    while(elozoX!=mostX &&elozoY!=mostY)
                    {
                        elozoX++;
                        elozoY--;
                        if(elozoX==mostX &&elozoY==mostY)
                        {
                            break;
                        }
                        if(p->palya[elozoX][elozoY].babu.szin < 2)db++;
                    }
                    if(db ==0)return true;
                }
                else
                {
                    while(elozoX!=mostX &&elozoY!=mostY)
                    {
                        elozoX--;
                        elozoY--;
                        if(elozoX==mostX &&elozoY==mostY)
                        {
                            break;
                        }
                        if(p->palya[elozoX][elozoY].babu.szin < 2)db++;
                    }
                    if(db ==0)return true;
                }
            }
            else
            {
                if(meredekseg==-1.0)
                {
                    while(elozoX!=mostX &&elozoY!=mostY)
                    {
                        elozoX--;
                        elozoY++;
                        if(elozoX==mostX &&elozoY==mostY)
                        {
                            break;
                        }
                        if(p->palya[elozoX][elozoY].babu.szin < 2)db++;
                    }
                    if(db ==0)return true;
                }
                else
                {
                    while(elozoX!=mostX &&elozoY!=mostY)
                    {
                        elozoX++;
                        elozoY++;
                        if(elozoX==mostX &&elozoY==mostY)
                        {
                            break;
                        }
                        if(p->palya[elozoX][elozoY].babu.szin < 2)db++;
                    }
                    if(db ==0)return true;
                }
            }
        }
        return false;
    }
    return false;
}

bool lovacska(int elozoX,int elozoY,int mostX,int mostY,Palya *p,int szin2) //Lovacska fuggveny a lovagot segiti a szabalyok betartasaban
{
    int X[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int Y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    //Lenyegeben ezek az osszes lehetseges modok, ahogyan a novelni vagy eppen csokkenteni kell a koordinatakat egy for ciklus segitsegevel

    for (int i = 0; i < 8; i++)
    {
        if(mostX == elozoX + X[i] && mostY == elozoY+Y[i]&&p->palya[mostX][mostY].babu.szin != szin2) return true;
    }
    return false;
}

bool sakk(Palya *p,int mostX,int mostY,int szin,int szin2)
{

    for(int i =0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(p->palya[j][i].babu.szin==szin2)
            {
                if(p->palya[j][i].babu.tipus==1)
                {
                    if(keresztbe(j,i,mostX,mostY,p,szin) || ideOda(j,i,mostX,mostY,p,szin))
                    {
                        hibas = true;
                        return true;
                    }
                }
                if(p->palya[j][i].babu.tipus == 2)
                {
                    if(ideOda(j,i,mostX,mostY,p,szin))
                    {
                        hibas = true;
                        return true;
                    }
                }
                if(p->palya[j][i].babu.tipus == 3)
                {
                    if(keresztbe(j,i,mostX,mostY,p,szin))
                    {
                        hibas = true;
                        return true;
                    }
                }
                if(p->palya[j][i].babu.tipus == 4)
                {
                    if(lovacska(j,i,mostX,mostY,p,szin))
                    {
                        hibas = true;
                        return true;
                    }
                }
                if(p->palya[j][i].babu.tipus == 5)
                {
                    if(j-1==mostX && i+1 == mostY || j+1==mostX && i+1==mostY)
                    {
                        hibas = true;
                        return true;
                    }
                }
            }
        }
    }

}

bool lephet(int elozoX,int elozoY,int mostX,int mostY,Tipus tBabu,Babuszin szBabu,Palya *p, rajzolo *r,FILE *fp) //Fent emlitett lephet fuggveny megnezni a lepes szabalyossagat es egy bool-t ad vissza
{
    lepes = false;

    switch(tBabu)
    {
    case 0://KIRALY     Minden babunak ket valtozata van, egy feher es egy fekete, az szBabu = 0 a feher, az else agban pedig a fekete kapott helyet
        if(szBabu==0)
        {
            hibas = false;
            sakk(p,mostX,mostY,0,1);

            if(abs(mostX-elozoX)<=1 && abs(mostY-elozoY)<=1 && p->palya[mostX][mostY].babu.szin != 0)
            {
                fKiralyX = mostX;
                fKiralyY = mostY;
                isCheck = false;
                lepes = true;
                return true;
            }

            if( mostX==6&&mostY==7 && p->palya[mostX-1][mostY].babu.szin == 2 &&p->palya[7][7].babu.tipus==2)
            {
                fKiralyX = mostX;
                fKiralyY = mostY;
                castle(1,r,elozoX,elozoY,mostX,mostY,true,fp);
                kastely = true;
                return true;
            }
            else if(mostX==2&&mostY==7 && p->palya[mostX+1][mostY].babu.szin == 2)
            {
                fKiralyX = mostX;
                fKiralyY = mostY;
                castle(1,r,elozoX,elozoY,mostX,mostY,false,fp);
                kastely = true;
                return true;
            }

        }
        else
        {
            hibas = false;
            sakk(p,mostX,mostY,1,0);

            if(abs(mostX-elozoX)<=1 && abs(mostY-elozoY)<=1 && p->palya[mostX][mostY].babu.szin != 1)
            {
                bKiralyX = mostX;
                bKiralyY = mostY;
                isCheck = false;
                lepes = true;
                return true;
            }

            if( mostX==6&&mostY==0 && p->palya[mostX-1][mostY].babu.szin == 2 &&p->palya[7][7].babu.tipus==2)
            {
                bKiralyX = mostX;
                bKiralyY = mostY;
                castle(0,r,elozoX,elozoY,mostX,mostY,true,fp);
                kastely = true;
                return true;
            }
            else if(mostX==2&&mostY==0 && p->palya[mostX+1][mostY].babu.szin == 2)
            {
                bKiralyX = mostX;
                bKiralyY = mostY;
                castle(0,r,elozoX,elozoY,mostX,mostY,false,fp);
                kastely = true;
                return true;
            }
        }
        break;
    case 1: //KIRALYNO
        if(szBabu==0)
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,0) && p->palya[mostX][mostY].babu.tipus != 0 && p->palya[mostX][mostY].babu.szin != 0|| keresztbe(elozoX,elozoY,mostX,mostY,p,0)&& p->palya[mostX][mostY].babu.tipus != 0&& p->palya[mostX][mostY].babu.szin != 0) //Osszraktuk a futot es bastyat es lett belole egy kiralyno
            {
                if(ideOda(mostX,mostY,bKiralyX,bKiralyY,r->p,0) || keresztbe(mostX,mostY,bKiralyX,bKiralyY,r->p,0))
                {
                    isCheck = true;
                }
                return true;
            }
        }

        else
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,1) && p->palya[mostX][mostY].babu.szin != 1 && p->palya[mostX][mostY].babu.tipus != 0|| keresztbe(elozoX,elozoY,mostX,mostY,p,1)&& p->palya[mostX][mostY].babu.szin != 1&& p->palya[mostX][mostY].babu.tipus != 0) //Osszraktuk a futot es bastyat es lett belole egy kiralyno
            {
                if(ideOda(mostX,mostY,fKiralyX,fKiralyY,r->p,1) || keresztbe(mostX,mostY,fKiralyX,fKiralyY,r->p,1))
                {
                    isCheck = true;
                }
                return true;
            }
        }
        break;
    case 2://BASTYA
        if(szBabu==0)
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,0) && p->palya[mostX][mostY].babu.tipus != 0)  //Az elozoX es elozoY a forras mezonek a koordinatai, a mostX,mostY a cel mezo koordinataja, a p az palya maga, a szam utana pedig, hogy milyen szinu az adott babu
            {
                if(ideOda(mostX,mostY,bKiralyX,bKiralyY,r->p,0))
                {
                    isCheck = true;
                }
                return true;
            }
        }
        else
        {
            if(ideOda(elozoX,elozoY,mostX,mostY,p,1)&& p->palya[mostX][mostY].babu.tipus != 0)
            {
                if(ideOda(mostX,mostY,fKiralyX,fKiralyY,r->p,1))
                {
                    isCheck = true;
                }
                return true;
            }
        }
        break;
    case 3: //FUTO
        if(szBabu==0)
        {
            if(keresztbe(elozoX,elozoY,mostX,mostY,p,0)&& p->palya[mostX][mostY].babu.tipus != 0)
            {
                if(keresztbe(mostX,mostY,bKiralyX,bKiralyY,r->p,0)) //Parameterek ugyanazok mint a bastyanal csak itt atlosan nezi meg oket
                {
                    isCheck = true;
                }
                return true;
            }
        }
        else
        {
            if(keresztbe(elozoX,elozoY,mostX,mostY,p,1)&& p->palya[mostX][mostY].babu.tipus != 0)
            {
                if(keresztbe(mostX,mostY,fKiralyX,fKiralyY,r->p,1))
                {
                    isCheck = true;
                }
                return true;
            }
        }
        break;
    case 4://LO
        if(szBabu==0)
        {
            if(lovacska(elozoX,elozoY,mostX,mostY,p,0)&& p->palya[mostX][mostY].babu.tipus != 0)
            {
                if(lovacska(mostX,mostY,bKiralyX,bKiralyY,r->p,0)) //Parameterek ugyanazok mint a bastyanal csak itt atlosan nezi meg oket
                {
                    isCheck = true;
                }
                return true;
            }
        }
        else
        {
            if(lovacska(elozoX,elozoY,mostX,mostY,p,1)&& p->palya[mostX][mostY].babu.tipus != 0)
            {
                if(lovacska(mostX,mostY,fKiralyX,fKiralyY,r->p,1)) //Parameterek ugyanazok mint a bastyanal csak itt atlosan nezi meg oket
                {
                    isCheck = true;
                }
                return true;
            }
        }
        break;
    case 5://PARASZT
        if(szBabu == 0)
        {
            if(elozoY == 6)
            {
                if(elozoY>mostY && mostX == elozoX && elozoY-mostY < 3 && p->palya[mostX][elozoY-1].babu.szin == 2) //Ha meg nem lepett akkor lephet egybol kettot es megnezi hogy nincs elotte semmi ami blokkolna
                {
                    p->palya[elozoX][elozoY].babu.szin = 2;
                    p->palya[elozoX][elozoY].babu.tipus = 6;
                    return true;
                }
            }


            if(abs(mostX-elozoX)==1 && abs(mostY-elozoY)==1 && p->palya[mostX][mostY].babu.szin==1 && p->palya[mostX][mostY].babu.tipus != 0)  //Itt tud atlosan utni egyet
            {
                p->palya[elozoX][elozoY].babu.szin = 2;
                p->palya[elozoX][elozoY].babu.tipus = 6;
                return true;
            }

            if(elozoY-mostY==1 && p->palya[mostX][mostY].babu.tipus == 6 && mostX == elozoX)     //Ha pedig ures az elotte levo mezo, akkor lephet, illetve ha a cel mezo-nek es a forras mezonek az Y koordinataja 1, mivel igy tudjuk hogy 1-et szeretne csak lepni
            {
                p->palya[elozoX][elozoY].babu.szin = 2;
                p->palya[elozoX][elozoY].babu.tipus = 6;
                return true;
            }
        }
        else if(szBabu == 1)
        {
            if(elozoY == 1)
            {
                if(elozoY<mostY && mostX == elozoX && mostY-elozoY < 3 && p->palya[mostX][elozoY+1].babu.szin == 2)
                {
                    p->palya[elozoX][elozoY].babu.szin = 2;
                    p->palya[elozoX][elozoY].babu.tipus = 6;
                    return true;
                }
            }

            if(abs(mostX-elozoX)==1 && abs(mostY-elozoY)==1 && p->palya[mostX][mostY].babu.szin==0 && p->palya[mostX][mostY].babu.tipus != 0)
            {
                p->palya[elozoX][elozoY].babu.szin = 2;
                p->palya[elozoX][elozoY].babu.tipus = 6;
                return true;
            }

            if(mostY-elozoY==1 && p->palya[mostX][mostY].babu.tipus == 6 && mostX == elozoX)
            {
                p->palya[elozoX][elozoY].babu.szin = 2;
                p->palya[elozoX][elozoY].babu.tipus = 6;
                return true;
            }
        }
        break;
    }
    return false;
}

void megjelenites(rajzolo *r)
{

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
    }
    thickLineRGBA(r->renderer,801,0,801,801,4,0,0,0,255);
    thickLineRGBA(r->renderer,801,2,999,2,4,0,0,0,255);
    thickLineRGBA(r->renderer,998,2,998,798,4,0,0,0,255);
    thickLineRGBA(r->renderer,998,798,801,798,4,0,0,0,255);
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
