#include <iostream>
#include <stdio.h>
#include<time.h>
#include<cstdlib>

using namespace std;
struct Karta {
    int wartosc;
    char kolor[6];
};

struct Gracz {
    Karta karty[53];                        //maksymalna liczba kart+1 wymagane jedno dodatkowe miejsce przez mechanizm dodawania kart
    int posiadane;
    int wygrane;
};

void wypisz(Gracz Gracz_1, Gracz Gracz_2, Gracz stos_1, Gracz stos_2);          //wypisuje karty w rekach graczy i te polozone na stole
void stworz_talie(struct Karta talia[], int LICZBA_KART);
void tasuj(Karta talia[], int ile);
void przydziel_karty(struct Gracz* Gracz_1, struct Gracz* Gracz_2, struct Karta talia[], int LICZBA_KART);
void usun_karte(Gracz* gracz);                                          //usuwa karte z poczatku talii
void dodaj_karte(Gracz* gracz, Karta karta);            //dodaje karte na koniec talii
int wojna(Gracz* Gracz_1, Gracz* Gracz_2, int wyniki[], int wyniki_p[], int ranking1, int ranking2, int* ruchy, int LICZBA_KART, char tryb);
int sprawdz_gotowosc(Gracz* gracz_1, Gracz* gracz_2);
int wojnaB(Gracz* Gracz_1, Gracz* Gracz_2, int wyniki[], int wyniki_p[], int ranking1, int ranking2, int* ruchy, int LICZBA_KART, char tryb);
void wyjmij_karte(Gracz* gracz, int ktora);                     //usuwa karte o konretnym indeksie
void czysc(Gracz* Gracz_1, Gracz* Gracz_2, int LICZBA_KART);
int licz_ranking(Gracz gracz, int LICZBA_KART);
void wypisz_rankingi(int wyniki[], int wyniki_p[]);
void zamien(Gracz* gracz);                                                              //do madrej wojny, wybiera karte ktora zostanie zagrana
void wybor(Gracz* Gracz_1, Gracz* Gracz_2, int tura);   //do madrej wojny, pozwala wynbrac karte, ktora zostanie zagrana
void tasuj_rozdane(Gracz* gracz, int ile);              //tasuje karty ktore ida do gracza w madrej wojnie
void losowa(Gracz* gracz);
void pokojowa(Gracz* Gracz_ktorego_tura, Gracz* Gracz_inny);                            //strategie
void wsciekla(Gracz* Gracz_ktorego_tura, Gracz* Gracz_inny);

int main()
{
    int LICZBA_KART;
    char wariant;
    char tryb;
    cout << "Podaj liczbe kart: ";
    cin >> LICZBA_KART;
    cout << "Podaj tryb (n-normalna wojna, m-madra wojna): ";
    cin >> tryb;
    if ((tryb != 'm') && (tryb != 'M') && (tryb != 'n') && (tryb != 'N')) {
        cout << "Brak takiego trybu!";
        return 0;
    }
    else if ((tryb == 'n') || (tryb == 'N')) {
        cout << "Podaj wariant (a lub b): ";
        cin >> wariant;
        if ((wariant != 'a') && (wariant != 'A') && (wariant != 'b') && (wariant != 'B')) {
            cout << "Brak takiego wariantu!";
            return 0;
        }
    }
    else {
        wariant = 'a';
    }

    int wyniki[293] = {
    };  //do ustalania korelacjii sposobow rankingowych
    int wyniki_p[293] = {
    };
    double long remis = 0;
    srand(time(NULL));
    double long licznik = 0;
    Gracz Gracz_1;
    Gracz Gracz_2;
    Gracz stos_1;
    Gracz stos_2;
    Gracz_1.wygrane = 0;
    Gracz_2.wygrane = 0;
    stos_1.posiadane = 0;
    stos_2.posiadane = 0;
    stos_1.wygrane = 0;
    stos_2.wygrane = 0;
    int tura = 1;
    int rezultat = 0;
    czysc(&Gracz_1, &Gracz_2, LICZBA_KART);
    Karta* talia;                           //deklaracja talii
    talia = (Karta*)malloc(LICZBA_KART * sizeof(Karta) + 1);
    stworz_talie(talia, LICZBA_KART);
    tasuj(talia, LICZBA_KART);
    przydziel_karty(&Gracz_1, &Gracz_2, talia, LICZBA_KART);
    int ranking1 = licz_ranking(Gracz_1, LICZBA_KART);                              //do liczenia rankingu
    int ranking2 = licz_ranking(Gracz_2, LICZBA_KART);
    int ruchy = 0;
    char stop;
    wypisz(Gracz_1, Gracz_2, stos_1, stos_2);

    while ((Gracz_1.posiadane != 0) && (Gracz_2.posiadane != 0)) {

        if ((tryb == 'm') || (tryb == 'M')) {
            wybor(&Gracz_1, &Gracz_2, tura);
            //if (tura % 2 == 0)                                                                    // resza 1 dla gracza 2 bo to on robi 1 ruch w ktorym podejmuje decyzje
                                                                                                    //ustawienia madrej wojny(wybor-recznie, losowa, pokojowa, wsciekla-wybor strategii dla symulacji dla kazdego gracza z osobna 
            //if (tura % 2 == 1)
        }
        dodaj_karte(&stos_1, Gracz_1.karty[0]);                         //dodanie kart do stosu
        dodaj_karte(&stos_2, Gracz_2.karty[0]);
        wypisz(Gracz_1, Gracz_2, stos_1, stos_2);

        if (Gracz_1.karty->wartosc > Gracz_2.karty->wartosc) {
            dodaj_karte(&Gracz_1, Gracz_1.karty[0]);                                                //bitwa, ktora wygral gracz 1
            dodaj_karte(&Gracz_1, Gracz_2.karty[0]);
            usun_karte(&Gracz_1);
            usun_karte(&Gracz_2);
            usun_karte(&stos_1);
            usun_karte(&stos_2);
            if ((tryb == 'm') || (tryb == 'M')) {                                                    //losowe rozdawanie w madrej wojnie
                tasuj_rozdane(&Gracz_1, 2);
            }
            ruchy++;
        }
        else if (Gracz_2.karty->wartosc > Gracz_1.karty->wartosc) {
            dodaj_karte(&Gracz_2, Gracz_2.karty[0]);
            dodaj_karte(&Gracz_2, Gracz_1.karty[0]);                                        //bitwa, ktora wygral gracz 2
            usun_karte(&Gracz_2);
            usun_karte(&Gracz_1);
            usun_karte(&stos_1);
            usun_karte(&stos_2);
            if ((tryb == 'm') || (tryb == 'M')) {                                                            //losowe rozdawanie w madrej wojnie
                tasuj_rozdane(&Gracz_2, 2);
            }
            ruchy++;
        }

        else if (Gracz_2.karty->wartosc == Gracz_1.karty->wartosc) {
            if ((Gracz_1.posiadane > 0) && (Gracz_2.posiadane > 0)) {
                int ilekart = sprawdz_gotowosc(&Gracz_1, &Gracz_2);
                for (int i = 1; i < ilekart; i++) {                                                                              //dodawanie odpowiedniej ilosci kart do stosu w zaleznosci od tego jak dluga bedzie wojna
                    dodaj_karte(&stos_1, Gracz_1.karty[i]);
                    dodaj_karte(&stos_2, Gracz_2.karty[i]);
                }
                wypisz(Gracz_1, Gracz_2, stos_1, stos_2);
                if ((wariant == 'A') || (wariant == 'a')) {
                    rezultat = wojna(&Gracz_1, &Gracz_2, wyniki, wyniki_p, ranking1, ranking2, &ruchy, LICZBA_KART, tryb);
                }                                                                                                                                                                                                                                                       //wojna typu A lub typu B
                else if ((wariant == 'B') || (wariant == 'b')) {
                    rezultat = wojnaB(&Gracz_1, &Gracz_2, wyniki, wyniki_p, ranking1, ranking2, &ruchy, LICZBA_KART, tryb);
                }
                for (int i = 0; i < ilekart; i++) {
                    usun_karte(&stos_1);                                                                                                            //sciaganie kart ze stolu po wojnie
                    usun_karte(&stos_2);
                }

                if (rezultat == 1) break;                                                                                                       //rezultat wojny, czy zakonczyla sie, czy zabraklo kart obu graczom
                if (rezultat == 2) {
                    remis++;
                    break;
                }
            }
        }

        wypisz(Gracz_1, Gracz_2, stos_1, stos_2);

        if (Gracz_1.posiadane == 0) {
            cout << "Wygral gracz 2" << endl;
            Gracz_2.wygrane++;
            wyniki[ranking2]++;
            wyniki_p[ranking1]++;
        }                                                                                                                               //zakonczenie gry
        if (Gracz_2.posiadane == 0) {
            cout << "Wygral gracz 1" << endl;
            Gracz_1.wygrane++;
            wyniki[ranking1]++;
            wyniki_p[ranking2]++;
        }

        /*if (ruchy >= 2 * LICZBA_KART * LICZBA_KART) { //ograniczenie ruchow, dozwolona wieksza ilosc dla wiekszej ilosci kart
                ruchy = 0;
                remis++;
                break;
        }*/
        tura++;
        if ((tryb != 'm') && (tryb != 'M')) {
            cout << "Wprowadz dowolny znak aby kontynuowac!: ";
            cin >> stop;
        }
    }
    cout << "Ruchy: " << ruchy;
    licznik += ruchy;
    free(talia);
    cout << endl;
    //wypisz_rankingi(wyniki, wyniki_p);
    return 0;

}

void stworz_talie(Karta talia[], int LICZBA_KART) {
    for (int i = 0, j = 15 - (LICZBA_KART / 4); i < LICZBA_KART; i++, j++) {          //15 pasuje do algorytmu wyznaczania najmniejsze wartosci karty dla danej ich ilosci w zadanych przypadkach
        if (j <= 14) {
            talia[i].wartosc = j;
        }
        else {
            j = 15 - (LICZBA_KART / 4);
            talia[i].wartosc = j;
        }
        if (i < (LICZBA_KART / 4)) sprintf_s(talia[i].kolor, "pik");
        if ((i >= (LICZBA_KART / 4)) && (i < (LICZBA_KART / 2)))sprintf_s(talia[i].kolor, "kier");
        if ((i >= (LICZBA_KART / 2)) && (i < (LICZBA_KART * 3 / 4)))sprintf_s(talia[i].kolor, "trefl");
        if ((i >= (LICZBA_KART * 3 / 4)) && (i < LICZBA_KART))sprintf_s(talia[i].kolor, "karo");
    }
}

void tasuj(Karta talia[], int ile) {

    Karta x, y;
    for (int i = ile - 1; i > 0; i--) {              //tasowanie polegajace na zamianie ostatniej karty z losowo wybrana wczesniejsza, nastepnie przedostatniej itd.
        int j = rand() % (i + 1);
        x = talia[i];
        y = talia[j];
        talia[i] = y;
        talia[j] = x;
    }
}

void przydziel_karty(struct Gracz* Gracz_1, struct Gracz* Gracz_2, struct Karta talia[], int LICZBA_KART) {
    for (int i = 0; i < LICZBA_KART / 2; i++) {
        Gracz_1->karty[i] = talia[i];
        Gracz_1->posiadane = LICZBA_KART / 2;
    }
    for (int i = LICZBA_KART / 2, j = 0; i < LICZBA_KART; i++, j++) {
        Gracz_2->karty[j] = talia[i];
        Gracz_2->posiadane = LICZBA_KART / 2;
    }
}

void usun_karte(Gracz* gracz) {

    int indeks = gracz->posiadane;
    for (int i = 0; i < indeks; i++) {
        gracz->karty[i] = gracz->karty[i + 1];
    }
    gracz->posiadane--;
}

void dodaj_karte(Gracz* gracz, Karta karta) {
    gracz->karty[gracz->posiadane] = karta;
    gracz->posiadane++;
}

int wojna(Gracz* Gracz_1, Gracz* Gracz_2, int wyniki[], int wyniki_p[], int ranking1, int ranking2, int* ruchy, int LICZBA_KART, char tryb) {
    int sprawdz = sprawdz_gotowosc(Gracz_1, Gracz_2);
    if (sprawdz > (LICZBA_KART) / 2) {                                                       //sytuacja, gdy zabraklo by kart do wojny 
        ruchy += sprawdz;
        return 2;
    }

    if ((Gracz_1->posiadane >= sprawdz) && (Gracz_2->posiadane >= sprawdz)) {
        int licznik = 3;                                                                                                                                        //zliczanie ile kart bedzie bralo udzial w wojnie
        while ((Gracz_1->posiadane >= licznik) && (Gracz_2->posiadane >= licznik)) {
            for (int i = 2; (i < Gracz_1->posiadane) && (i < Gracz_2->posiadane); i = i + 2) {
                if (Gracz_1->karty[i].wartosc > Gracz_2->karty[i].wartosc) {
                    for (int j = 0; j < licznik; j++) {
                        dodaj_karte(Gracz_1, Gracz_1->karty[0]);
                        usun_karte(Gracz_1);
                    }
                    for (int j = 0; j < licznik; j++) {
                        dodaj_karte(Gracz_1, Gracz_2->karty[0]);
                        usun_karte(Gracz_2);
                    }
                    if ((tryb == 'm') || (tryb == 'M')) {                                                    //losowe rozdawanie w madrej wojnie
                        tasuj_rozdane(Gracz_1, (2 * licznik));
                    }
                    (*ruchy) += licznik;
                    return 0;
                }
                else if (Gracz_2->karty[i].wartosc > Gracz_1->karty[i].wartosc) {
                    for (int j = 0; j < licznik; j++) {
                        dodaj_karte(Gracz_2, Gracz_2->karty[0]);
                        usun_karte(Gracz_2);
                    }
                    for (int j = 0; j < licznik; j++) {
                        dodaj_karte(Gracz_2, Gracz_1->karty[0]);
                        usun_karte(Gracz_1);
                    }
                    if ((tryb == 'm') || (tryb == 'M')) {                                                    //losowe rozdawanie w madrej wojnie
                        tasuj_rozdane(Gracz_2, (2 * licznik));
                    }
                    (*ruchy) += licznik;
                    return 0;
                }
                else if (Gracz_2->karty[i].wartosc == Gracz_1->karty[i].wartosc) {
                    licznik = licznik + 2;
                }
            }
        }
        return 0;
    }
    else if (Gracz_1->posiadane < sprawdz) {                                         //gdy graczowi 1 zabraklo kart
        Gracz_2->wygrane++;
        wyniki[ranking2]++;
        wyniki_p[ranking1]++;
        (*ruchy) += sprawdz;
        cout << "Wygral gracz 2\n";
        return 1;
    }
    else if (Gracz_2->posiadane < sprawdz) {                                         // gdy graczowi 2 zabraklo kart
        Gracz_1->wygrane++; ;
        wyniki[ranking1]++;
        wyniki_p[ranking2]++;
        (*ruchy) += sprawdz;
        cout << "Wygral gracz 1\n";
        return 1;
    }
    else return 1;
}

int sprawdz_gotowosc(Gracz* gracz_1, Gracz* gracz_2) {                                                                                   //sprawdza czy starczy kart do wojny
    int licznik = 3;
    for (int i = 2; (i < gracz_1->posiadane) && (i < gracz_2->posiadane); i = i + 2) {
        if (gracz_1->karty[i].wartosc != gracz_2->karty[i].wartosc) {
            break;
        }
        if (gracz_1->karty[i].wartosc == gracz_2->karty[i].wartosc) {
            licznik += 2;
        }
    }
    return licznik;
}

int wojnaB(Gracz* Gracz_1, Gracz* Gracz_2, int wyniki[], int wyniki_p[], int ranking1, int ranking2, int* ruchy, int LICZBA_KART, char tryb) {
    int sprawdz = sprawdz_gotowosc(Gracz_1, Gracz_2);

    if (Gracz_1->posiadane == (sprawdz - 1)) {
        if (Gracz_2->posiadane == 1) {
            Gracz_2->wygrane++; wyniki[ranking2]++; wyniki_p[ranking1]++; (*ruchy) += sprawdz;  return 1;
        }
        else {
            dodaj_karte(Gracz_1, Gracz_2->karty[sprawdz - 1]);
            wyjmij_karte(Gracz_2, sprawdz - 1);
            (*ruchy) += 1;
        }
    }
    else if (Gracz_1->posiadane == (sprawdz - 2)) {                                                                          //jesli nie starcza kart do 1wszej wojny, uzupelnia talie przeciwnika
        if (Gracz_2->posiadane == 2) {
            Gracz_2->wygrane++; wyniki[ranking2]++; wyniki_p[ranking1]++; (*ruchy) += sprawdz;  return 1;
        }
        else {
            dodaj_karte(Gracz_1, Gracz_2->karty[sprawdz - 2]);
            wyjmij_karte(Gracz_2, sprawdz - 2);
            dodaj_karte(Gracz_1, Gracz_2->karty[sprawdz - 2]);
            wyjmij_karte(Gracz_2, sprawdz - 2);
            (*ruchy) += 1;

        }
    }
    else if (Gracz_2->posiadane == (sprawdz - 1)) {
        if (Gracz_1->posiadane == 1) {
            Gracz_1->wygrane++;
            wyniki[ranking1]++;
            wyniki_p[ranking2]++;
            (*ruchy) += sprawdz;
            return 1;
        }
        else {
            dodaj_karte(Gracz_2, Gracz_1->karty[sprawdz - 1]);
            wyjmij_karte(Gracz_1, sprawdz - 1);
            (*ruchy) += 1;
        }
    }
    else if (Gracz_2->posiadane == (sprawdz - 2)) {
        if (Gracz_1->posiadane == 2) {
            Gracz_1->wygrane++;
            wyniki[ranking1]++;
            wyniki_p[ranking2]++;
            (*ruchy) += sprawdz;
            return 1;
        }
        else {
            dodaj_karte(Gracz_2, Gracz_1->karty[sprawdz - 2]);
            wyjmij_karte(Gracz_1, sprawdz - 2);
            dodaj_karte(Gracz_2, Gracz_1->karty[sprawdz - 2]);
            wyjmij_karte(Gracz_1, sprawdz - 2);
            (*ruchy) += 1;
        }
    }
    int spr = wojna(Gracz_1, Gracz_2, wyniki, wyniki_p, ranking1, ranking2, ruchy, LICZBA_KART, tryb);      //dalej toczy sie wojna, zwracany jest rezultat wojny
    return spr;
}

void wyjmij_karte(Gracz* gracz, int ktora) {
    int ilosc = gracz->posiadane;
    for (int i = ktora; i < ilosc; i++) {
        gracz->karty[i] = gracz->karty[i + 1];
    }
    gracz->posiadane--;
}

void wypisz(Gracz Gracz_1, Gracz Gracz_2, Gracz stos_1, Gracz stos_2) {
    cout << "Karty gracza 1:" << endl;
    for (int i = Gracz_1.posiadane - 1; i >= stos_1.posiadane; i--) {       //i=0
        cout << Gracz_1.karty[i].kolor << " " << Gracz_1.karty[i].wartosc << endl;
    }
    cout << endl;
    cout << "Stos gracza 1:" << endl;
    for (int i = stos_1.posiadane - 1; i >= 0; i--) {
        if (stos_1.karty[i].wartosc > 1)
            cout << stos_1.karty[i].kolor << " " << stos_1.karty[i].wartosc << endl;
    }
    cout << endl;
    cout << "Stos gracza 2:" << endl;
    for (int i = 0; i < stos_2.posiadane; i++) {
        if (stos_2.karty[i].wartosc > 1)
            cout << stos_2.karty[i].kolor << " " << stos_2.karty[i].wartosc << endl;
    }
    cout << endl;
    cout << "Karty gracza 2:" << endl;
    for (int i = stos_2.posiadane; i < Gracz_2.posiadane; i++) {
        cout << Gracz_2.karty[i].kolor << " " << Gracz_2.karty[i].wartosc << endl;
    }
    cout << "--------------------------------";
    cout << endl;
}

void czysc(Gracz* Gracz_1, Gracz* Gracz_2, int LICZBA_KART) {                            //usuwa wartosci kart z rak graczy
    for (int i = 0; i <= LICZBA_KART; i++) {
        Gracz_1->karty[i].wartosc = 0;
        Gracz_2->karty[i].wartosc = 1;
    }
    return;
}

int licz_ranking(Gracz gracz, int LICZBA_KART) {
    int licznik = 0;

    for (int i = 0; i < LICZBA_KART / 2; i++) {
        licznik += gracz.karty[i].wartosc;
    }
    return licznik;
}

void wypisz_rankingi(int wyniki[], int wyniki_p[]) {
    for (int i = 0; i <= 293; i++) {
        float a = 0;
        cout << i << " ";
        if (wyniki_p[i] + wyniki[i] != 0)  a = (100 * wyniki[i] / (wyniki_p[i] + wyniki[i]));

        /*cout <<i<<"."<<" Zwyciestwa " << " : " << wyniki[i] << " Przegrane:"<<" " << wyniki_p[i] << " % zwyciestw: ";*/
        if (wyniki_p[i] + wyniki[i] != 0) cout << a << endl;
        else cout << endl;
    }
}

void zamien(Gracz* gracz) {
    Karta pom1;
    Karta pom2;
    pom1 = gracz->karty[0];
    pom2 = gracz->karty[1];
    gracz->karty[0] = pom2;
    gracz->karty[1] = pom1;
}

void wybor(Gracz* Gracz_1, Gracz* Gracz_2, int tura) {
    int wyb2;
    int wyb1;
    if (tura % 2 == 1) {

        cout << "Tura gracza 2, prosze wybrac karte 1 lub karte 2: ";
        cout << endl;
        cin >> wyb2;
        if (wyb2 == 2) {
            zamien(Gracz_2);
        }
        else if ((wyb2 != 1) && (wyb2 != 2)) {
            cout << "Nieprawidlowy numer! Wybrano zamist tego karte 1!\n";
        }
    }
    if (tura % 2 == 0) {
        cout << "Tura gracza 1, prosze wybrac karte 1 lub karte 2: ";
        cout << endl;
        cin >> wyb1;
        if (wyb1 == 2) {
            zamien(Gracz_1);
        }
        else if ((wyb1 != 1) && (wyb1 != 2)) {
            cout << "Nieprawidlowy numer! Wybrano zamist tego karte 1!\n";
        }
    }
}

void tasuj_rozdane(Gracz* gracz, int ile) {
    Karta x;
    Karta y;
    for (int i = (gracz->posiadane - 1); i > (gracz->posiadane - ile); i--) {                                //tasuje konkretna ilosc kart zaczynajac od ostatniej
        int k = ile;
        int j = rand() % (k);
        x = gracz->karty[i];
        y = gracz->karty[gracz->posiadane - j - 1];
        gracz->karty[i] = y;
        gracz->karty[gracz->posiadane - j - 1] = x;
        k--;
    }
}

void losowa(Gracz* gracz) {
    int wyb;
    wyb = (rand() % 2) + 1;
    if (wyb == 2) {
        zamien(gracz);
    }
}

void pokojowa(Gracz* Gracz_ktorego_tura, Gracz* Gracz_inny) {
    int wyb = 1;

    if (Gracz_ktorego_tura->karty[0].wartosc == Gracz_inny->karty[0].wartosc)
        wyb = 2;
    else {
        if ((Gracz_ktorego_tura->karty[0].wartosc > Gracz_ktorego_tura->karty[1].wartosc) && (Gracz_ktorego_tura->karty[0].wartosc > Gracz_inny->karty[0].wartosc) && (Gracz_ktorego_tura->karty[1].wartosc > Gracz_inny->karty[0].wartosc))
            wyb = 2;
        else if ((Gracz_ktorego_tura->karty[0].wartosc > Gracz_inny->karty[0].wartosc) && (Gracz_ktorego_tura->karty[1].wartosc < Gracz_inny->karty[0].wartosc))
            wyb = 2;
        /*else if ((Gracz_ktorego_tura->karty[0].wartosc > Gracz_ktorego_tura->karty[1].wartosc) && (Gracz_inny->karty[0].wartosc > Gracz_ktorego_tura->karty[0].wartosc))
                wyb = 2;*/ // niejasny przypadek
    }
    if (wyb == 2) {
        zamien(Gracz_ktorego_tura);
    }
}

void wsciekla(Gracz* Gracz_ktorego_tura, Gracz* Gracz_inny) {
    int wyb = 1;
    if (Gracz_ktorego_tura->karty[0].wartosc == Gracz_inny->karty[0].wartosc)
        wyb = 1;
    else if ((Gracz_ktorego_tura->karty[1].wartosc) == (Gracz_inny->karty[0].wartosc))
        wyb = 2;
    else {
        if ((Gracz_ktorego_tura->karty[0].wartosc > Gracz_ktorego_tura->karty[1].wartosc) && (Gracz_ktorego_tura->karty[1].wartosc > Gracz_inny->karty[0].wartosc)) {
            wyb = 2;
        }
        else if ((Gracz_ktorego_tura->karty[0].wartosc > Gracz_ktorego_tura->karty[1].wartosc) && (Gracz_ktorego_tura->karty[0].wartosc < Gracz_inny->karty[0].wartosc))
            wyb = 2;
        else if ((Gracz_ktorego_tura->karty[1].wartosc > Gracz_inny->karty[0].wartosc) && (Gracz_ktorego_tura->karty[0].wartosc < Gracz_inny->karty[0].wartosc))
            wyb = 2;
    }

    if (wyb == 2) {
        zamien(Gracz_ktorego_tura);
    }
}
