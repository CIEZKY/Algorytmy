#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <time.h>
using namespace std;


template<typename Tab>
class kopcur {
private:
    Tab* Kopcur;
    unsigned ostatni_index;
    unsigned rozmiar;
    int wspolczynnik_rozszerzenia = 2;

public:
    kopcur() {
        rozmiar = 2;
        ostatni_index = 0;
        Kopcur = new Tab[rozmiar];
    }
    ~kopcur() {
        czysciciel();
    }

    void czysciciel() {
        rozmiar = 2;
        ostatni_index = 0;
        Kopcur = nullptr;
        delete[] Kopcur;
        Kopcur = new Tab[rozmiar];
    }

    unsigned pobierz_rozmiar() const noexcept
    {
        return ostatni_index;
    }

    unsigned pobierz_korzen() const noexcept
    {
        if (ostatni_index == 0) {
            return 0;
        }
        return Kopcur[0];
    }

    template<typename Comp>
    void dodaj_do_kopca(const Tab& dane, Comp comp) {
        if (ostatni_index < rozmiar) {
            Kopcur[ostatni_index] = dane;
            ostatni_index++;
        }
        else {
            rozmiar = rozmiar * wspolczynnik_rozszerzenia;
            Tab* nowa_Tablica = new Tab[rozmiar];
            for (int i = 0; i < ostatni_index; i++) {
                nowa_Tablica[i] = Kopcur[i];
            }
            delete[] Kopcur;
            Kopcur = nowa_Tablica;
            Kopcur[ostatni_index] = dane;
            ostatni_index++;
        }
        przekopcuj_gora(ostatni_index - 1, comp);
    }

    template<typename Comp>
    Tab usun_korzen(Comp comp) {
        if (ostatni_index == 0) {
            return 0;
        }
        int zwrot = pobierz_korzen();
        Kopcur[0] = Kopcur[ostatni_index - 1];
        ostatni_index--;
        przekopcuj_dol(0, comp);
        return zwrot;
    }


    string to_string() const {
        if (ostatni_index == 0)
            return "[]";
        ostringstream str;
        str << "[" << Kopcur[0];
        for (int i = 1; i < ostatni_index; i++)
            str << ", " << Kopcur[i];
        str << "]";
        return str.str();
    }

private:
    int rodzic(int i) const {
        return (i - 1) / 2;
    }

    int lewe(int i) const {
        return 2 * i + 1;
    }

    int prawe(int i) const {
        return lewe(i) + 1;
    }

    /*
    int prawe(int i) const {
        return 2 * i + 2;
    }
    */


    void zmien(int a, int b)
    {
        Tab przechowalnia = Kopcur[a];
        Kopcur[a] = Kopcur[b];
        Kopcur[b] = przechowalnia;
    }

    template<typename Comp>
    void przekopcuj_dol(int index, Comp comp)
    {
        int el = index;
        int dziecko_lewe = lewe(index);
        int dziecko_prawe = prawe(index);

        if (dziecko_lewe < ostatni_index && comp(Kopcur[dziecko_lewe], Kopcur[el]))
            el = dziecko_lewe;

        if (dziecko_prawe < ostatni_index && comp(Kopcur[dziecko_prawe], Kopcur[el]))
            el = dziecko_prawe;

        if (index != el)
        {
            zmien(index, el);
            przekopcuj_dol(el, comp);
        }
    }


    template<typename Comp>
    void przekopcuj_gora(int index, Comp comp)
    {
        if (index > 0 && comp(Kopcur[index], Kopcur[rodzic(index)]))
        {
            zmien(index, rodzic(index));
            przekopcuj_gora(rodzic(index), comp);
        }
    }
};

int main() {
    kopcur<int> kopiec;
    auto komparator = [](int a, int b)
    {
        return a > b;
    };

    default_random_engine gen(time(nullptr));
    uniform_int_distribution<int> dist(0, 10);
    clock_t wlacz_stoper, wylacz_stoper;
    double czas_operacji;
    const int MAX_ORDER = 1;
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);
        wlacz_stoper = clock();
        for (int i = 0; i < n; i++)
        {
            kopiec.dodaj_do_kopca(dist(gen), komparator);
        }
        wylacz_stoper = clock();
        czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
        //cout << kopiec.to_string() << endl;
        printf("Czas operacji dodawania wynosi: %f sekund\n", czas_operacji);

        wlacz_stoper = clock();
        for (int i = 0; i < n; i++)
        {
            cout << kopiec.usun_korzen(komparator) << endl;
        }
        wylacz_stoper = clock();
        czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
        //cout << kopiec.to_string() << endl;
        printf("Czas operacji usuwania wynosi: %f sekund\n", czas_operacji);
    }
    return 0;
    


    kopiec.dodaj_do_kopca(13, komparator);
    kopiec.dodaj_do_kopca(11, komparator);
    kopiec.dodaj_do_kopca(12, komparator);
    kopiec.dodaj_do_kopca(15, komparator);
    kopiec.dodaj_do_kopca(7, komparator);
    kopiec.dodaj_do_kopca(19, komparator);
    kopiec.dodaj_do_kopca(8, komparator);
    kopiec.dodaj_do_kopca(22, komparator);
    kopiec.dodaj_do_kopca(14, komparator);
    kopiec.dodaj_do_kopca(5, komparator);
    kopiec.dodaj_do_kopca(17, komparator);
    kopiec.dodaj_do_kopca(10, komparator);

    cout << kopiec.to_string() << endl;
    cout << kopiec.pobierz_korzen() << endl;
    cout << kopiec.pobierz_rozmiar() << endl;
    kopiec.usun_korzen(komparator);

}