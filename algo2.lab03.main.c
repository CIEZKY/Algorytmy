#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <time.h>
using namespace std;


template<typename T>
class BST
{
public:
	struct wezel_drzewa
	{
		T dane;
		wezel_drzewa* dziecko_lewe, * dziecko_prawe, * rodzic;

		wezel_drzewa(const T& dane, wezel_drzewa* dziecko_lewe, wezel_drzewa* dziecko_prawe, wezel_drzewa* rodzic)
			: dane(dane), dziecko_lewe(dziecko_lewe), dziecko_prawe(dziecko_prawe), rodzic(rodzic)
		{
		}

		wezel_drzewa(T&& dane, wezel_drzewa* dziecko_lewe, wezel_drzewa* dziecko_prawe, wezel_drzewa* rodzic)
			: dane(dane), dziecko_lewe(dziecko_lewe), dziecko_prawe(dziecko_prawe), rodzic(rodzic)
		{
		}


		~wezel_drzewa()
		{
			delete dziecko_lewe;
			delete dziecko_prawe;

		}
	};

	BST() noexcept
		: rozmiar(0), wysokosc(0), korzen(nullptr)
	{
	}

	~BST()
	{
		czysciciel();
	}

	void czysciciel()
	{
		/*
		for (int i = 0; i < rozmiar; i++)
		{
			usun_znaleziony_element(korzen);
		}
		*/
		delete korzen;
		korzen = nullptr;
		rozmiar = 0;
		wysokosc = 0;
	}

	unsigned pobierz_rozmiar() const noexcept
	{
		return rozmiar;
	}
	/*
	unsigned pobierz_wysokosc() const noexcept
	{
		return wysokosc;
	}
	*/
	wezel_drzewa* pobierz_korzen() const noexcept
	{
		return korzen;
	}

	template<typename Comp>
	wezel_drzewa* wstaw(const T& el, Comp comp)
	{
		auto nowy_el = new wezel_drzewa(el, nullptr, nullptr, nullptr);
		if (!korzen)
		{
			//cout << "Dodaje korzen " << endl;
			korzen = nowy_el;
			rozmiar++;
			wysokosc++;
			return nowy_el;
		}

		//int obecny_poziom = 1;
		//cout << "Dodaje lisc " << endl;
		for (auto p = korzen; p;)
		{
			//obecny_poziom++;
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
						//cout << obecny_poziom << endl;
						/*
						if (!p->dziecko_prawe && obecny_poziom != wysokosc)
						{
							wysokosc++;
						}
						*/
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
						//cout << obecny_poziom << endl;
						/*
						if (!p->dziecko_lewe && obecny_poziom != wysokosc)
						{
							wysokosc++;
						}
						*/
						return nowy_el;
					}
				}
		}
		return nullptr;
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

	bool usun_znaleziony_element(wezel_drzewa* p)
	{
		//cout << "usuwam: " << p->dane << endl;
		//auto rodzic = p->rodzic;
		
		if (p == nullptr)
		{
			//cout << "Brak wskaznika" << endl;
			return false;
		}
		/*
		if (p->dziecko_lewe)
		{
			//cout << "zamieniam " << p->dane << "z najwiekszym elementem po lewo" << endl;
			auto element_zastepczy = zastap(p->dziecko_lewe);
			p->dane = element_zastepczy->dane;
			usun_znaleziony_element(element_zastepczy);
			return true;
		}
		*/
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
	void in_order(wezel_drzewa * p)
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
	std::map<int, int> heights, min_node_data;
	do
	{
		BST<int> t;
		for (int i : v)
			t.wstaw(i, std::less<int>{});
		++heights[t.oblicz_wysokosc(t.pobierz_korzen())];
		for (int i : v)
		{
			++min_node_data[t.skrajny_lewy()];
			t.usun_znaleziony_element(t.znajdz_element(i, std::less<int>{}));
		}
	} while (std::next_permutation(v.begin(), v.end()));
	for (auto [height, cnt] : heights)
		std::cout << height << ": " << cnt << std::endl;
	std::cout << "***" << std::endl;
	for (auto [data, cnt] : min_node_data)
		std::cout << data << ": " << cnt << std::endl;
}
int main()
{
	test();
	//return 0;
	BST<int> drzewo;

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
	/*
	wlacz_stoper = clock();
	for (int i = 0; i < 100000; i++)
	{
		drzewo.wstaw(dist(gen), komparator);
	}
	wylacz_stoper = clock();
	czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);

	wlacz_stoper = clock();
	int hits = 0;
	for (int i = 0; i < 1000; i++)
	{

		if (drzewo.znajdz_element(dist(gen), komparator) != nullptr)
			hits++;
	}
	wylacz_stoper = clock();
	czas_operacji = (wylacz_stoper - wlacz_stoper) / (double)CLOCKS_PER_SEC;
	printf("Czas operacji wynosi: %f sekund\n", czas_operacji);
	cout << "trafienia: " << hits << endl;
	*/
	/*
	int dane;
	BST<int>::wezel_drzewa* point = nullptr;
	int wybor;
	do {
		cout << "Co chcesz zrobic?\n";
		cout << "1-dodaj\n2-znajdz element\n3-usun znaleziony element\n4-wyswietl\n5-wyczysc\n6-pre_order\n7-in_order\n0-wyjscie\n";
		cin >> wybor;

		switch (wybor) {
		case 1:
			cout << "Wpisz co chcesz dodac do drzewa\n";
			cin >> dane;
			system("cls");
			drzewo.wstaw(dane, komparator);
			break;
		case 2:
			cout << "Wpisz element ktory chcesz wyszukac\n";
			cin >> dane;
			system("cls");
			point = drzewo.znajdz_element(dane, komparator);
			if(point != nullptr)
			cout << "znaleziono! " << point->dane << endl;
			break;
		case 3:
			system("cls");
			drzewo.usun_znaleziony_element(point);
			break;
		case 4:
			system("cls");
			drzewo.wyswietl();
			break;
		case 5:
			drzewo.czysciciel();
			system("cls");
			cout << "Usunięto drzewo!\n";
			break;
		case 6:
			system("cls");
			drzewo.pre_order(drzewo.pobierz_korzen());
			break;
		case 7:
			system("cls");
			drzewo.in_order(drzewo.pobierz_korzen());
			break;
		default:
			cout << "Blad!, Wybierz jeszcze raz\n";
			break;
		}
	}while (wybor != 0);
	*/
	drzewo.wstaw(10, komparator);
	drzewo.wstaw(15, komparator);
	drzewo.wstaw(12, komparator);
	drzewo.wstaw(5, komparator);
	drzewo.wstaw(30, komparator);
	drzewo.wstaw(25, komparator);
	drzewo.wstaw(35, komparator);
	drzewo.wstaw(7, komparator);
	drzewo.wstaw(-2, komparator);
	drzewo.wstaw(33, komparator);
	drzewo.wstaw(35, komparator);
	drzewo.wstaw(40, komparator);
	drzewo.wstaw(50, komparator);
	drzewo.wstaw(60, komparator);
	drzewo.usun_znaleziony_element(drzewo.znajdz_element(60, komparator));
	drzewo.wyswietl();
	cout << "\n in-order ";
	drzewo.in_order(drzewo.pobierz_korzen());
	cout << "\n pre-order ";
	drzewo.pre_order(drzewo.pobierz_korzen());


	return 0;
}