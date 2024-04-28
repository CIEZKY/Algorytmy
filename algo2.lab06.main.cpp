#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <cmath>

using namespace std;

template<typename T>
class lista_dwukierunkowa {
public:
    struct wezel_listy {
        T dane;
        wezel_listy* nast, * poprz;

        wezel_listy(const T& dane, wezel_listy* nast, wezel_listy* poprz)
            : dane(dane), nast(nast), poprz(poprz) {}

        wezel_listy(T&& dane, wezel_listy* nast, wezel_listy* poprz)
            : dane(dane), nast(nast), poprz(poprz)
        {
        }

        ~wezel_listy()
        {
        }
    };

    lista_dwukierunkowa() noexcept
        : glowa(nullptr), ogon(nullptr), dlugosc(0)
    {
    }

    ~lista_dwukierunkowa()
    {
        czysciciel();
    }

    void czysciciel()
    {
        for (unsigned i = 0; i < dlugosc; i++) {
            auto nowa_glowa = glowa->nast;
            glowa->nast = nullptr;
            delete glowa;
            glowa = nowa_glowa;
        }
        glowa = nullptr;
        ogon = nullptr;
        dlugosc = 0;
    }

    unsigned pobierz_dlugosc() const noexcept {
        return dlugosc;
    }

    void wstaw_na_poczatek(const T& dane) {
        auto nowa_glowa = new wezel_listy(dane, glowa, nullptr);
        if (glowa)
            glowa->poprz = nowa_glowa;
        else
            ogon = nowa_glowa;
        glowa = nowa_glowa;
        ++dlugosc;
    }

    void wstaw_na_koniec(const T& dane) {
        auto nowy_ogon = new wezel_listy(dane, nullptr, ogon);
        if (ogon)
            ogon->nast = nowy_ogon;
        else
            glowa = nowy_ogon;
        ogon = nowy_ogon;
        ++dlugosc;
    }

    void usun_glowe() {
        if (glowa == nullptr)
            throw std::domain_error("Pusta lista!");

        if (!glowa->nast) {
            czysciciel();
        }
        else {
            auto nowa_glowa = glowa->nast;
            glowa->nast = nullptr;
            delete glowa;
            glowa = nowa_glowa;
            --dlugosc;
        }
    }

    void usun_ogon() {
        if (ogon == nullptr)
            throw std::domain_error("Pusta lista!");

        if (!ogon->poprz) {
            czysciciel();
        }
        else {
            auto nowy_ogon = ogon->poprz;
            ogon->poprz = nullptr;
            delete ogon;
            ogon = nowy_ogon;
            --dlugosc;
        }
    }

    template<typename Comp>
    wezel_listy* porzadek_ma_byc(const T& el, Comp comp)
    {
        auto nowy_el = new wezel_listy(el, nullptr, nullptr);
        if (!glowa)
        {
            ogon = nowy_el;
            glowa = nowy_el;
            dlugosc++;
            return nowy_el;
        }

        for (auto p = glowa; p; p = p->nast)
        {
            if (comp(el, p->dane))
            {
                nowy_el->nast = p->nast;
                nowy_el->poprz = p;

                if (p->nast)
                {
                    p->nast->poprz = nowy_el;
                }
                else
                {
                    ogon = nowy_el;
                }

                p->nast = nowy_el;
                dlugosc++;
                return nowy_el;
            }
        }

        nowy_el->poprz = ogon;
        ogon->nast = nowy_el;
        ogon = nowy_el;
        dlugosc++;
        return nowy_el;
    }

    template<typename Comp>
    wezel_listy* znajdz_element(const T& el, Comp comp) const
    {
        for (auto p = glowa; p; p = p->nast)
            if (comp(el, p->dane))
                return p;
        return nullptr;
    }

    template<typename Comp>
    bool znajdz_element_i_go_usun(const T& el, Comp comp)
    {
        for (auto p = glowa; p; p = p->nast)
        {
            if (comp(el, p->dane))
            {
                if (p->nast)
                {
                    p->nast->poprz = p->poprz;
                }
                else
                {
                    ogon = p->poprz;
                    if (ogon)
                        ogon->nast = nullptr;
                }
                if (p->poprz)
                {
                    p->poprz->nast = p->nast;
                }
                else
                {
                    glowa = p->nast;
                    if (glowa)
                        glowa->poprz = nullptr;
                }

                delete p;
                --dlugosc;
                return true;
            }
        }
        return false;
    }



    wezel_listy* zwroc_element_o_indeksie(int index) const
    {
        if (index < 0 || index > dlugosc - 1) {
            return nullptr;
        }
        auto p = glowa;
        for (int i = 0; i < index; i++)
        {
            p = p->nast;
        }
        return p;
    }

    wezel_listy* zamien_element_o_indeksie(int index, const T& podmiana) const
    {
        if (index < 0 || index > dlugosc - 1) {
            return nullptr;
        }
        auto p = glowa;
        for (int i = 0; i < index; i++)
        {
            p = p->nast;
        }
        p->dane = podmiana;
        return p;
    }

    std::string to_string() const {
        if (dlugosc == 0)
            return "[]";

        std::ostringstream str;
        str << "[" << *(glowa);
        auto p = glowa->nast;

        while (p) {
            str << ", " << *p;
            p = p->nast;
        }

        str << "]";
        return str.str();
    }


    wezel_listy* zwroc_poczatek() const {
        return glowa;
    }


private:
    wezel_listy* glowa, * ogon;
    unsigned dlugosc;
};


template <typename K, typename T>
class Tablica_Haszyszowa {
public:
    struct elementy {
        K klucz;
        T dane;

        elementy(const K& k, const T& t) : klucz(k), dane(t) {}
    };

    Tablica_Haszyszowa() : rozmiar(16), ostatni_index(0), stopien_napelnienia(0.75) {
        tablica = new lista_dwukierunkowa<elementy>[rozmiar];
    }

    ~Tablica_Haszyszowa() {
        delete[] tablica;
    }

    void czysciciel2() {
        for (size_t i = 0; i < rozmiar; i++) {
            tablica[i].czysciciel();
        }
        ostatni_index = 0;
    }

    void wstaw(const K& klucz, const T& wartosc) {
        if (ostatni_index / rozmiar > stopien_napelnienia) {
            przehaszowanie();
        }

        size_t index = Haszowanie(klucz);
        elementy nowy_element(klucz, wartosc);
        tablica[index].wstaw_na_koniec(nowy_element);
        ostatni_index++;
    }

    lista_dwukierunkowa<elementy>::wezel_listy* znajdz_element(const K& klucz) const {
        size_t index = Haszowanie(klucz);
        for (auto p = tablica[index].zwroc_poczatek(); p; p = p->nast) {
            if (p->dane.klucz == klucz) {
                return p;
            }
        }
        return nullptr;
    }

    bool usun_element(const K& klucz) {
        size_t index = Haszowanie(klucz);
        elementy e{ klucz,T{} };
        return tablica[index].znajdz_element_i_go_usun(e, [](const elementy& a, const elementy& b) { return a.klucz == b.klucz; });
    }

    string to_string() const {
        stringstream str;
        str << "Tablica Mieszająca:" << endl;
        str << "Rozmiar: " << ostatni_index << endl;
        str << "Pojemnosc: " << rozmiar << endl;
        str << "tablica:" << endl;
        for (size_t i = 0; i < rozmiar; i++) {
            if (!tablica[i].pobierz_dlugosc()) continue;
            str << i << ": " << tablica[i].to_string() << "; ";
        }
        str << endl;
        to_string_stats();
    }

    string to_string_stats() const {
        stringstream str;
        str << "statystyki:" << endl;
        str << "minimalna dlugosc listy: " << min_list() << endl;
        str << "maksymalna dlugosc listy: " << max_list() << endl;
        str << "listy non-null: " << listy_NN() << endl;
        str << "srednia dlugosc listy: " << srednia_list() << endl;
        return str.str();
    }


    // Funkcje pomocnicze //////////////////////////////////////////////////////////

    size_t min_list() const {
        if (rozmiar == 0) {
            return 0;
        }

        size_t minimalna = tablica[0].pobierz_dlugosc();
        for (int i = 1; i < rozmiar; i++) {
            size_t ostatni_idx = tablica[i].pobierz_dlugosc();
            if (ostatni_idx < minimalna) {
                minimalna = ostatni_idx;
            }
        }
        return minimalna + 1;
    }

    size_t max_list() const {
        size_t maksymalna = 0;
        for (int i = 0; i < rozmiar; i++) {
            size_t ostatni_idx = tablica[i].pobierz_dlugosc();
            if (ostatni_idx > maksymalna) {
                maksymalna = ostatni_idx;
            }
        }
        return maksymalna;
    }

    size_t listy_NN() const {
        size_t licznik = 0;
        for (int i = 0; i < rozmiar; i++) {
            if (tablica[i].pobierz_dlugosc() > 0) {
                licznik++;
            }
        }
        return licznik;
    }

    double srednia_list() const {
        size_t suma = 0;
        size_t licznik = 0;
        for (int i = 0; i < rozmiar; i++) {
            size_t ostatni_idx = tablica[i].pobierz_dlugosc();
            if (ostatni_idx > 0) {
                suma += ostatni_idx;
                licznik++;
            }
        }
        return (licznik == 0) ? 0 : static_cast<double>(suma) / licznik;
    }


private:
    lista_dwukierunkowa<elementy>* tablica;
    unsigned rozmiar;
    unsigned ostatni_index;
    const double stopien_napelnienia;

    size_t Haszowanie(const K& klucz) const {
        size_t hasz = 0;
        for (char c : klucz) {
            hasz = (hasz * 31 + c) % rozmiar;
        }
        return hasz;
    }

    void przehaszowanie() {
        size_t nowy_rozmiar = rozmiar * 2;
        auto nowa_tablica = new lista_dwukierunkowa<elementy>[nowy_rozmiar];

        for (size_t i = 0; i < rozmiar; i++) {
            for (auto p = tablica[i].zwroc_poczatek(); p; p = p->nast) {
                size_t nowy_index = Haszowanie(p->dane.klucz) % nowy_rozmiar;
                nowa_tablica[nowy_index].wstaw_na_koniec(p->dane);
            }
        }

        delete[] tablica;
        tablica = nowa_tablica;
        rozmiar = nowy_rozmiar;
    }
};

string generuj_losowy_klucz(size_t d) {
    //char zakres_gen[] = "0123456789""ABCDEFGHIJKLMNOPQRSTUVWXYZ""abcdefghijklmnopqrstuvwxyz";
    char zakres_gen[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string klucz;
    for (size_t i = 0; i < d; i++) {
        klucz.push_back(zakres_gen[rand() % (sizeof(zakres_gen) - 1)]);
    }
    return klucz;
}

int main()
{
    const int MAX_ORDER = 7;
    Tablica_Haszyszowa<string, int> zielona_tablica;

    clock_t wlacz_stoper, wylacz_stoper;
    double czas_operacji;

    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);

        wlacz_stoper = clock();
        for (int i = 0; i < n; i++)
        {
            zielona_tablica.wstaw(generuj_losowy_klucz(6), i);
        }
        wylacz_stoper = clock();
        czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
        cout << "Czas wstawiania " << czas_operacji << " elementy: " << n << endl;

        const int m = (pow(10, 4));
        int hits = 0;
        wlacz_stoper = clock();
        for (int i = 0; i < m; i++) {
            auto entry = zielona_tablica.usun_element(generuj_losowy_klucz(6));
            if (entry)
                hits++;
        }
        wylacz_stoper = clock();
        czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
        cout << "Czas szukania " << czas_operacji << " elementy: " << m << endl;
        cout << "trafienia: " << hits << endl;

        cout << zielona_tablica.to_string_stats() << endl;

        zielona_tablica.czysciciel2();
    }

    return 0;
};