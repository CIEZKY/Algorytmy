#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <time.h>
using namespace std;


template<typename T>
class RBT
{
public:
	struct wezel_drzewa
	{
		T dane;
		T kolor; // 0 - CZARNY 1 - CZERWONY
		wezel_drzewa* dziecko_lewe, * dziecko_prawe, * rodzic;

		wezel_drzewa(const T& dane, T kolor, wezel_drzewa* dziecko_lewe, wezel_drzewa* dziecko_prawe, wezel_drzewa* rodzic)
			: dane(dane), kolor(kolor), dziecko_lewe(dziecko_lewe), dziecko_prawe(dziecko_prawe), rodzic(rodzic)
		{
		}

		wezel_drzewa(T&& dane, T kolor, wezel_drzewa* dziecko_lewe, wezel_drzewa* dziecko_prawe, wezel_drzewa* rodzic)
			: dane(dane), kolor(kolor), dziecko_lewe(dziecko_lewe), dziecko_prawe(dziecko_prawe), rodzic(rodzic)
		{
		}


		~wezel_drzewa()
		{
			delete dziecko_lewe;
			delete dziecko_prawe;

		}
	};

	RBT() noexcept
		: rozmiar(0), wysokosc(0), korzen(nullptr)
	{
	}

	~RBT()
	{
		czysciciel();
	}

	void czysciciel()
	{
		delete korzen;
		korzen = nullptr;
		rozmiar = 0;
		wysokosc = 0;
	}

	unsigned pobierz_rozmiar() const noexcept
	{
		return rozmiar;
	}

	wezel_drzewa* pobierz_korzen() const noexcept
	{
		return korzen;
	}

	template<typename Comp>
	wezel_drzewa* wstaw(const T& el, Comp comp)
	{
		auto nowy_el = new wezel_drzewa(el, 1, nullptr, nullptr, nullptr);
		if (!korzen)
		{
			korzen = nowy_el;
			nowy_el->kolor = 0; //korzen zawsze czarny
			rozmiar++;
			wysokosc++;
			return nowy_el;
		}
		for (auto p = korzen; p;)
		{
			while (p)
				if (comp(el, p->dane))
				{
					if (p->dziecko_lewe)
					{
						p = p->dziecko_lewe;
					}
					else
					{
						p->dziecko_lewe = nowy_el;
						nowy_el->rodzic = p;
						rozmiar++;
						kolorowanie(nowy_el);
						return nowy_el;
					}
				}
				else
				{
					if (p->dziecko_prawe)
					{
						p = p->dziecko_prawe;
					}
					else
					{
						p->dziecko_prawe = nowy_el;
						nowy_el->rodzic = p;
						rozmiar++;
						kolorowanie(nowy_el);
						return nowy_el;
					}
				}
		}
		return nullptr;
	}


	void kolorowanie(wezel_drzewa* nowy_el)
	{
		while (nowy_el != korzen && nowy_el->rodzic->kolor == 1)
		{
			if (nowy_el->rodzic == nowy_el->rodzic->rodzic->dziecko_lewe)
			{
				wezel_drzewa* sasiad = nowy_el->rodzic->rodzic->dziecko_prawe;

				if (sasiad != nullptr && sasiad->kolor == 1) {
					nowy_el->rodzic->kolor = 0;
					sasiad->kolor = 0;
					nowy_el->rodzic->rodzic->kolor = 1;
					nowy_el = nowy_el->rodzic->rodzic;
				}
				else
				{
					if (nowy_el == nowy_el->rodzic->dziecko_prawe)
					{
						nowy_el = nowy_el->rodzic;
						rotacja(nowy_el, 'L');
					}

					nowy_el->rodzic->kolor = 0;
					nowy_el->rodzic->rodzic->kolor = 1;
					rotacja(nowy_el->rodzic->rodzic, 'P');
				}
			}
			else
			{
				wezel_drzewa* sasiad = nowy_el->rodzic->rodzic->dziecko_lewe;

				if (sasiad != nullptr && sasiad->kolor == 1)
				{
					nowy_el->rodzic->kolor = 0;
					sasiad->kolor = 0;
					nowy_el->rodzic->rodzic->kolor = 1;
					nowy_el = nowy_el->rodzic->rodzic;
				}
				else
				{
					if (nowy_el == nowy_el->rodzic->dziecko_lewe)
					{
						nowy_el = nowy_el->rodzic;
						rotacja(nowy_el, 'P');
					}

					nowy_el->rodzic->kolor = 0;
					nowy_el->rodzic->rodzic->kolor = 1;
					rotacja(nowy_el->rodzic->rodzic, 'L');
				}
			}
		}

		korzen->kolor = 0;
	}


	void rotacja(wezel_drzewa* nowy_el, int kierunek)
	{
		wezel_drzewa* marszalek_rotacyjny = nullptr;
		if (kierunek == 'L')
		{
			marszalek_rotacyjny = nowy_el->dziecko_prawe;
			if (marszalek_rotacyjny != nullptr)
			{
				nowy_el->dziecko_prawe = marszalek_rotacyjny->dziecko_lewe;

				if (marszalek_rotacyjny->dziecko_lewe != nullptr)
				{
					marszalek_rotacyjny->dziecko_lewe->rodzic = nowy_el;
				}

				marszalek_rotacyjny->rodzic = nowy_el->rodzic;
				if (nowy_el == korzen)
				{
					korzen = marszalek_rotacyjny;
				}
				else if (nowy_el == nowy_el->rodzic->dziecko_prawe)
				{
					nowy_el->rodzic->dziecko_prawe = marszalek_rotacyjny;
				}
				else
				{
					nowy_el->rodzic->dziecko_lewe = marszalek_rotacyjny;
				}
				marszalek_rotacyjny->dziecko_lewe = nowy_el;
				nowy_el->rodzic = marszalek_rotacyjny;
			}
		}
		else if (kierunek == 'P')
		{
			marszalek_rotacyjny = nowy_el->dziecko_lewe;
			if (marszalek_rotacyjny != nullptr)
			{
				nowy_el->dziecko_lewe = marszalek_rotacyjny->dziecko_prawe;

				if (marszalek_rotacyjny->dziecko_prawe != nullptr)
				{
					marszalek_rotacyjny->dziecko_prawe->rodzic = nowy_el;
				}

				marszalek_rotacyjny->rodzic = nowy_el->rodzic;
				if (nowy_el == korzen)
				{
					korzen = marszalek_rotacyjny;
				}
				else if (nowy_el == nowy_el->rodzic->dziecko_lewe)
				{
					nowy_el->rodzic->dziecko_lewe = marszalek_rotacyjny;
				}
				else
				{
					nowy_el->rodzic->dziecko_prawe = marszalek_rotacyjny;
				}
				marszalek_rotacyjny->dziecko_prawe = nowy_el;
				nowy_el->rodzic = marszalek_rotacyjny;
			}
		}
		else
			return;
	}


	template<typename Comp>
	wezel_drzewa* znajdz_element(const T& el, Comp comp) const
	{
		for (auto p = korzen; p;)
		{
			//if (comp(el, p->dane))
			if (el == p->dane)
				return p;
			else
				if (comp(el, p->dane))
				{
					if (p->dziecko_lewe)
					{
						p = p->dziecko_lewe;
					}
					else
					{
						break;
					}
				}
				else
				{
					if (p->dziecko_prawe)
					{
						p = p->dziecko_prawe;
					}
					else
					{
						break;
					}
				}
		}
		//cout << "Brak elementu " << endl;
		return nullptr;
	}

	wezel_drzewa* zastap(wezel_drzewa* p) {
		for (; p->dziecko_prawe; p = p->dziecko_prawe) {}
		return p;
	}

	T skrajny_lewy() {
		wezel_drzewa* p = korzen;
		for (; p->dziecko_lewe; p = p->dziecko_lewe) {}
		return p->dane;
	}

	/*
	bool usun_znaleziony_element(wezel_drzewa* p)
	{
		//cout << "usuwam: " << p->dane << endl;
		//auto rodzic = p->rodzic;

		if (p == nullptr)
		{
			//cout << "Brak wskaznika" << endl;
			return false;
		}
		if (p->dziecko_lewe)
		{
			//cout << "zamieniam " << p->dane << "z najwiekszym elementem po lewo" << endl;
			auto element_zastepczy = zastap(p->dziecko_lewe);
			p->dane = element_zastepczy->dane;
			usun_znaleziony_element(element_zastepczy);
			return true;
		}
		else if (p->dziecko_prawe)
		{
			if (!p->rodzic)
			{
				korzen = p->dziecko_prawe;
				p->dziecko_prawe->rodzic = nullptr;
				//cout << "usuwam korzen ktory ma prawe dziecko " << prawe->dane << endl;
			}
			else
			{
				if (p == p->rodzic->dziecko_lewe)
					p->rodzic->dziecko_lewe = p->dziecko_prawe;
				else
					p->rodzic->dziecko_prawe = p->dziecko_prawe;
				p->dziecko_prawe->rodzic = p->rodzic;
			}
			p->dziecko_prawe = p->dziecko_lewe = nullptr;
			delete p;
		}
		else
		{
			if (!p->rodzic)
			{
				korzen = nullptr;
				//cout << "usuwam korzen ktory nie ma dzieci " << endl;
			}
			else
			{
				if (p == p->rodzic->dziecko_lewe)
					p->rodzic->dziecko_lewe = p->dziecko_prawe;
				else
					p->rodzic->dziecko_prawe = p->dziecko_prawe;
			}
			p->dziecko_prawe = p->dziecko_lewe = nullptr;
			delete p;
		}
		//delete p;
		rozmiar--;
		return true;
	}
	*/

	void pre_order(wezel_drzewa* p)
	{
		if (!p)
		{
			cout << "puste drzewo";
		}
		else
		{
			cout << '[' << p->dane << ']';
			if (p->dziecko_lewe != nullptr)
				pre_order(p->dziecko_lewe);
			if (p->dziecko_prawe != nullptr)
				pre_order(p->dziecko_prawe);
		}
	}
	void in_order(wezel_drzewa* p)
	{
		if (!p)
		{
			cout << "puste drzewo";
		}
		else
		{
			if (p->dziecko_lewe != nullptr)
				in_order(p->dziecko_lewe);
			cout << '[' << p->dane << ']';
			if (p->dziecko_prawe != nullptr)
				in_order(p->dziecko_prawe);
		}
	}
	string to_string(wezel_drzewa* p)
	{
		if (!p) {
			return "[]";
		}
		else
		{
			ostringstream str;
			if (korzen == p)
				str << "korzen ";
			else
				str << "lisc ";
			if (p->kolor == 0)
				str << "kolor: czarny ";
			else
				str << "kolor: czerwony ";
			str << "[" << p->dane << "] ";
			if (p->rodzic)
				str << "Rodzic : [" << p->rodzic->dane << "] ";
			else
				str << "Rodzic : [NULL] ";
			if (p->dziecko_lewe)
				str << " Dziecko lewe : [" << p->dziecko_lewe->dane << "] ";
			else
				str << " Dziecko lewe : [NULL] ";
			if (p->dziecko_prawe)
				str << "Dziecko prawe : [" << p->dziecko_prawe->dane << "]" << endl;
			else
				str << "Dziecko prawe : [NULL]" << endl;
			if (p->dziecko_lewe != nullptr)
				str << to_string(p->dziecko_lewe);
			if (p->dziecko_prawe != nullptr)
				str << to_string(p->dziecko_prawe);

			return str.str();
		}
	}
	void wyswietl()
	{
		auto korzen_w = pobierz_korzen();
		cout << "rozmiar: " << pobierz_rozmiar() << endl;
		cout << "wysokosc: " << oblicz_wysokosc(korzen_w) << endl;
		cout << "{\n" << to_string(korzen_w) << "}" << endl;
	}
	int oblicz_wysokosc(wezel_drzewa* p) {
		if (!p) {
			return 0;
		}
		else
		{
			int lewa_wysokosc = oblicz_wysokosc(p->dziecko_lewe);
			int prawa_wysokosc = oblicz_wysokosc(p->dziecko_prawe);
			return porownaj_wysokosci(lewa_wysokosc, prawa_wysokosc) + 1;
		}
	}
	int porownaj_wysokosci(int a, int b) {
		if (a > b)
			return a;
		else
			return b;
	}



private:
	wezel_drzewa* korzen;
	unsigned rozmiar;
	unsigned wysokosc;
};

#include <map>
#include <vector>
#include <numeric>
#include <algorithm>
void test()
{
	std::vector<int> v(8);
	std::iota(v.begin(), v.end(), 0);
	std::map<int, int> heights, root_data;
	do
	{
		RBT<int> t;
		for (int i : v)
			t.wstaw(i, std::less<int>{});
		++heights[t.oblicz_wysokosc(t.pobierz_korzen())];
		++root_data[t.pobierz_korzen()->dane];
	} while (std::next_permutation(v.begin(), v.end()));
	for (auto [height, cnt] : heights)
		std::cout << height << ": " << cnt << std::endl;
	std::cout << "***" << std::endl;
	for (auto [data, cnt] : root_data)
		std::cout << data << ": " << cnt << std::endl;
}

int main()
{
	test();
	//return 0;
	RBT<int> drzewo;

	default_random_engine gen(time(nullptr));
	uniform_int_distribution<int> dist(0, 100000);
	clock_t wlacz_stoper, wylacz_stoper;
	double czas_operacji;

	auto komparator = [](const int& a, const int& b)
	{
		return a < b;
	};

	auto porownywacz = [](const int& a, const int& b)
	{
		return a == b;
	};

	const int MAX_ORDER = 7;
	for (int o = 1; o <= MAX_ORDER; o++)
	{
		const int n = pow(10, o);

		wlacz_stoper = clock();
		for (int i = 0; i < n; i++)
		{
			drzewo.wstaw(dist(gen), komparator);
		}
		wylacz_stoper = clock();
		czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
		printf("Czas operacji wstawiania wynosi: %f sekund\nwysokosc = %d \n\n", czas_operacji, drzewo.oblicz_wysokosc(drzewo.pobierz_korzen()));
		

		wlacz_stoper = clock();
		int hits = 0;
		for (int i = 0; i < 10000; i++)
		{

			if (drzewo.znajdz_element(dist(gen), komparator) != nullptr)
				hits++;
		}
		wylacz_stoper = clock();
		czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
		printf("Czas operacji szukania wynosi: %f sekund\n", czas_operacji);
		cout << "trafienia: " << hits << endl;

	}
	return 0;


	drzewo.wstaw(10, komparator);
	drzewo.wstaw(15, komparator);
	drzewo.wstaw(12, komparator);
	drzewo.wstaw(8, komparator);
	drzewo.wstaw(20, komparator);
	drzewo.wstaw(5, komparator);
	drzewo.wstaw(18, komparator);
	drzewo.wstaw(25, komparator);
	drzewo.wstaw(7, komparator);
	drzewo.wstaw(14, komparator);
	drzewo.wstaw(22, komparator);
	drzewo.wstaw(3, komparator);
	drzewo.wstaw(17, komparator);


	drzewo.wyswietl();
	cout << "\n in-order ";
	drzewo.in_order(drzewo.pobierz_korzen());
	cout << "\n pre-order ";
	drzewo.pre_order(drzewo.pobierz_korzen());


	return 0;
}