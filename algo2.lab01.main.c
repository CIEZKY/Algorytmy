#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <time.h>
using namespace std;




template<typename T>
class lista_dwukierunkowa
{
public:
	struct wezel_listy
	{
		T dane;
		wezel_listy *nast, *poprz; 

		wezel_listy(const T& dane, wezel_listy *nast, wezel_listy *poprz)
			: dane(dane), nast(nast), poprz(poprz)
		{
		}

		wezel_listy(T&& dane, wezel_listy *nast, wezel_listy *poprz)
			: dane(dane), nast(nast), poprz(poprz)
		{
		}
		~wezel_listy()
		{
			/*
			if (nast)
				delete nast;
			// nast = nullptr;
			*/
			
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
		for (int i = 0; i < dlugosc; i++ )
		{
			auto nowa_glowa = glowa->nast;
			glowa->nast = nullptr;
			delete glowa;
			glowa = nowa_glowa;
		}
		glowa = nullptr;
		ogon = nullptr;
		dlugosc = 0;
	}
	


	unsigned pobierz_dlugosc() const noexcept
	{
		return dlugosc;
	}

	void wstaw_na_poczatek(const T& dane)
	{
		auto nowa_glowa = new wezel_listy(dane, glowa, nullptr);
		if(glowa)
			glowa->poprz = nowa_glowa;
		else
		ogon = nowa_glowa;
		glowa = nowa_glowa;
		++dlugosc;
	}

	
	void wstaw_na_koniec(const T& dane)
	{
		auto nowy_ogon = new wezel_listy(dane, nullptr, ogon);
		if (ogon)
			ogon->nast = nowy_ogon;
		else
		glowa = nowy_ogon;
		ogon = nowy_ogon;
		++dlugosc;
	}
	

	void usun_glowe()
	{
		if (glowa == nullptr)
			throw domain_error("Pusta lista!");
		
		if (!glowa->nast)
		{
			czysciciel();
		}
		else
		{
			auto nowa_glowa = glowa->nast;
			glowa->nast = nullptr;
			delete glowa;
			glowa = nowa_glowa;
			--dlugosc;
		}
	}

	void usun_ogon()
	{
		if (ogon == nullptr)
			throw domain_error("Pusta lista!");
		
		if (!ogon->poprz)
		{
			czysciciel();
		}
		else
		{
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
				if (p->poprz)
				{
					p->poprz->nast = p->nast;
				}
				else
				{
					if (p->nast)
						glowa->poprz = nullptr;
					else
						ogon = nullptr;
				}
				
				delete p;
				--dlugosc;
				return true;
			}
		}
		return false;
	}
	


	string to_string() const
	{
		if (dlugosc == 0)
			return "[]";
		ostringstream str; 
		str << "[" << glowa->dane;
		for (auto p = glowa->nast; p; p = p->nast)
			str << ", " << p->dane;
		str << "]";
		return str.str();
	}
	
	wezel_listy* zwroc_element_o_indeksie(int index) const
	{
		if (index < 0 || index > dlugosc-1) {
			throw domain_error("Zły przedział!");
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
		if (index < 0 || index > dlugosc-1) {
			throw domain_error("Zły przedział!");
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

	

private:
	wezel_listy *glowa, *ogon;
	unsigned dlugosc;
};


int main()
{
	
	{
		lista_dwukierunkowa<int> l;
		l.wstaw_na_koniec(0);
		l.wstaw_na_koniec(1);
		l.znajdz_element_i_go_usun(1, std::equal_to<int>{});
		l.znajdz_element_i_go_usun(0, std::equal_to<int>{});
		return 0;
		for (int i = 0; i < 1000; i++)
		{
			l.wstaw_na_koniec(0);
			cout << l.to_string() << endl;
			l.usun_glowe();
			cout << l.to_string() << endl;
		}
		for (int i = 0; i < 1000; i++)
		{
			l.wstaw_na_poczatek(1);
			cout << l.to_string() << endl;
			l.usun_ogon();
			cout << l.to_string() << endl;
		}

		return 0;
		for (int i = 0; i < 10000; i++)
			l.wstaw_na_koniec(0);

		cout << l.to_string() << endl;
		l.czysciciel();
		cout << "dlugosc listy to: ";
		cout << l.pobierz_dlugosc() << endl;
		cout << l.to_string() << endl;
		l.wstaw_na_koniec(1);
		l.wstaw_na_poczatek(2);
		cout << "dlugosc listy to: ";
		cout << l.pobierz_dlugosc() << endl;
		cout << l.to_string() << endl;

		return 0;

		/*
		cout << l.to_string() << endl;
		l.znajdz_element_i_go_usun(0, std::equal_to<int>{});
		cout << l.to_string() << endl;
		l.wstaw_na_koniec(1);
		l.wstaw_na_koniec(2);
		l.wstaw_na_koniec(4);
		l.wstaw_na_poczatek(8);
		cout << l.to_string() << endl;
		l.czysc_liste();
		cout << l.to_string() << endl;
		
		return 0;
		*/
	}
	
	default_random_engine gen(time(nullptr));
	uniform_int_distribution<int> dist(0, 10000);
	clock_t wlacz_stoper, wylacz_stoper;
	double czas_operacji;
	/*
	lista_dwukierunkowa<int> lista;

	auto komparator = [](const int& a, const int& b)
	{
		return a == b;
	};

	wlacz_stoper = clock();
	for (int i = 0; i < 100; i++)
	{
		lista.wstaw_na_poczatek(dist(gen));
	}
	wylacz_stoper = clock();
	czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
	cout << lista.to_string() << endl;

	wlacz_stoper = clock();
	for (int i = 0; i < 200; i++)
	{
		lista.porzadek_ma_byc(dist(gen),komparator);
	}
	wylacz_stoper = clock();
	czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
	cout << lista.to_string() << endl;
	
	wlacz_stoper = clock();
	for (int i = 0; i < 500; i++)
	{
		if (lista.znajdz_element_i_go_usun(dist(gen), komparator))
			printf("zniszczono element\n");
	}
	wylacz_stoper = clock();
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
	cout << lista.to_string() << endl;
	lista.czyscieciel();
	cout << lista.to_string() << endl;

	
	/*
	wlacz_stoper = clock();
	for (int i = 0; i < 500; i++)
	{
		lista.porzadek_ma_byc(dist(gen), komparator);
	}
	wylacz_stoper = clock();
	czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
	cout << lista.to_string() << endl;

	return 0;
	*/

	/*
	wlacz_stoper = clock();
	wylacz_stoper = clock();
	czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
	*/
	

	/*
	lista_dwukierunkowa<int> lista;

	auto komparator = [](const int& a, const int& b)
	{
		return a == b; 
	};

	auto komparator_sort = [](const int& a, const int& b)
	{
		less<int>();
	};

	lista.wstaw_na_poczatek(30);
	lista.wstaw_na_poczatek(30);
	lista.wstaw_na_poczatek(30);
	lista.wstaw_na_poczatek(30);
	lista.~lista_dwukierunkowa();
	lista.wstaw_na_poczatek(10);
	lista.wstaw_na_poczatek(9);
	lista.usun_glowe();
	lista.wstaw_na_poczatek(5);
	lista.wstaw_na_koniec(2);
	lista.usun_ogon();
	lista.wstaw_na_koniec(8);
	lista.wstaw_na_koniec(9);
	cout << "czy na liscie jest 8?: " << endl;
	cout << lista.znajdz_element(40, komparator) << endl;
	cout << lista.to_string() << endl;
	cout << "dlugosc listy to: ";
	cout << lista.pobierz_dlugosc() << endl;
	cout << "zwroc indeks 3: " << endl;
	cout << lista.zwroc_element_o_indeksie(3)->dane << endl;
	cout << "zamien indeks 1 na liczbe 2: " << endl;
	lista.zamien_element_o_indeksie(1, 2);
	cout << lista.to_string() << endl;
	cout << lista.znajdz_element_i_go_usun(2, komparator) << endl;
	cout << lista.to_string() << endl;
	cout << "dlugosc listy to: ";
	cout << lista.pobierz_dlugosc() << endl;
	cout << lista.to_string() << endl;
	lista.czysc_liste();
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(1, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(1, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(144, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(144, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(5, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(1, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(144, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(5, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(5, komparator);
	cout << lista.to_string() << endl;
	lista.porzadek_ma_byc(451, komparator);
	cout << lista.to_string() << endl;
	return 0;
	*/
}