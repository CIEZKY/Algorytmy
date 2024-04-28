#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <time.h>
using namespace std;

template<typename Tab>
class tablica_dynamo {
private:
    Tab* Tablica;
    Tab dane;
    unsigned ostatni_index;
    unsigned rozmiar;
    int wspolczynnik_rozszerzenia = 2;

public:
    tablica_dynamo() {
        rozmiar = 2;
        ostatni_index = 0;
        Tablica = new Tab[rozmiar];
    }
    ~tablica_dynamo() {
        //czysciciel();
        rozmiar = 2;
        ostatni_index = 0;
        delete[] Tablica;
        Tablica = nullptr;
    }

    void czysciciel() {
        rozmiar = 2;
        ostatni_index = 0;
        delete[] Tablica;

        Tablica = new Tab[rozmiar];
    }

    unsigned pobierz_rozmiar() const noexcept
    {
        return rozmiar;
    }

    void dodaj_do_tablicy(const Tab& dane) {
        if (ostatni_index < rozmiar) {
            Tablica[ostatni_index] = dane;
            ostatni_index++;
            //cout << "dodaje normal\n";
        }
        else {
            //cout << "rozszerzam\n";
            rozmiar = rozmiar * wspolczynnik_rozszerzenia;
            //cout << rozmiar;
            Tab* nowa_Tablica = new Tab[rozmiar];
            for (int i = 0; i < ostatni_index; i++) {
                nowa_Tablica[i] = Tablica[i];
            }
            delete[] Tablica;
            Tablica = nowa_Tablica;
            Tablica[ostatni_index] = dane;
            ostatni_index++;
        }
    }

    Tab* zwroc_element_o_indeksie(int index) const
    {
        if (index < 0 || index > rozmiar - 1) {
            //throw out_of_range("Zły przedział!");
            return nullptr;
        }
        return &Tablica[index];
    }

    bool zamien_element_o_indeksie(int index, const Tab& podmiana) const
    {
        if (index < 0 || index > rozmiar - 1) {
            //throw out_of_range("Zły przedział!");
            return false;
        }
        Tablica[index] = podmiana;
        return true;
    }

    string to_string() const
    {
        if (ostatni_index == 0)
            return "[]";
        ostringstream str;
        str << "[" << Tablica[0];
        for (int i = 1; i < ostatni_index; i++)
            str << ", " << Tablica[i];
        str << "]";
        return str.str();
    }

    template<typename Comp>
    void sortuj(Comp comp) {
        for (int i = 0; i < ostatni_index; i++)
            for (int j = 1; j < ostatni_index - i; j++)
                if (comp(Tablica[j - 1], Tablica[j])) {
                    Tab temp = Tablica[j - 1];
                    Tablica[j - 1] = Tablica[j];
                    Tablica[j] = temp;
                }
    }



};


int main()
{
    tablica_dynamo<int> Tablica;
   // int wybor;
    //dane dane_wpis;
    //dane zamien;
    //int* p;

    auto komparator = [](int& a, int& b)
    {
        return a > b;
    };

    default_random_engine gen(time(nullptr));
    uniform_int_distribution<int> dist(0, 10000);
    clock_t wlacz_stoper, wylacz_stoper;
    double czas_operacji;
    double max_time=0;
    
    
    for (int i = 0; i < 10000000; i++)
    {
        wlacz_stoper = clock();
        Tablica.dodaj_do_tablicy(dist(gen));
        wylacz_stoper = clock();
        czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
        if (czas_operacji > max_time)
        {
            max_time = czas_operacji;
            printf("Czas operacji wynosi: %f sekund,i = %d\n", max_time, i);
        }
    }
    return 0;
    //czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
   //out << Tablica.to_string() << endl;
    //printf("Czas operacji wynosi: %f sekund\n", czas_operacji);

    wlacz_stoper = clock();
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < Tablica.pobierz_rozmiar(); j += 5)
        {
            Tablica.zamien_element_o_indeksie(j, (dist(gen)));
        }
    }
    wylacz_stoper = clock();
    czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
    //cout << Tablica.to_string() << endl;
    printf("Czas operacji wynosi: %f sekund\n", czas_operacji);

    wlacz_stoper = clock();
    Tablica.sortuj(komparator);
    wylacz_stoper = clock();
    czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
    cout << Tablica.to_string() << endl;
    printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
    



    /*
    do {
        cout << "Co chcesz zrobic?\n";
        cout << "1-dodaj\n2-wyswietl element o indeksie\n3-zamien element o indeksie\n4-wyswietl\n5-wyczysc\n6-sortuj\n0-wyjscie\n";
        cin >> wybor;

        switch (wybor) {
        case 1:
            cout << "Wpisz co chcesz dodac do Tablicy\n";
            cin >> dane_wpis;
            system("cls");
            Tablica.dodaj_do_tablicy(dane_wpis);
            cout << Tablica.to_string() << endl;
            break;
        case 2:
            cout << "Wpisz index elementu który chcesz zwrócić\n";
            cin >> dane_wpis;
            system("cls");
            p = Tablica.zwroc_element_o_indeksie(dane_wpis);
            cout << "Zwracany element to : " << *p << endl;
            break;
        case 3:
            cout << "Wpisz index elementu który chcesz zamienic\n";
            cin >> dane_wpis;
            cout << "Wpisz na co chcesz zamienic\n";
            cin >> zamien;
            system("cls");
            cout << Tablica.zamien_element_o_indeksie(dane_wpis, zamien) << endl;
            break;
        case 4:
            system("cls");
            cout << Tablica.to_string() << endl;
            cout << Tablica.pobierz_rozmiar() << endl;
            break;
        case 5:
            Tablica.czysciciel();
            system("cls");
            cout << "Usunięto tablice!\n";
            break;
        case 6:
            system("cls");
            Tablica.sortuj(komparator);
            cout << Tablica.to_string() << endl;
            break;
        default:
            cout << "Blad!, Wybierz jeszcze raz\n";
            break;
        }
    } while (wybor != 0);
    */
    return 0;
}

