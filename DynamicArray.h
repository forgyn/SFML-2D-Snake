#pragma once
#include <iostream>
#define ERROR(text) std::cout << text << std::endl;
#define LOOP(n) for(int i = 0;i<n;i++)
#define RLOOP(n) for(int i = n-1;i>=0;i--)
#define SLOOP(n1,n2) for(int i = n1;i<n2;i++)
#define INPUT int input = _getwch() 
namespace fl {
	template<class T>struct DynamicArray {
		DynamicArray() {
			maxVelikost = 0;
			poleUkazatelu = new T*[1];
		};
		DynamicArray(DynamicArray &copy) {
			if (copy.max_size() != 0) {
				maxVelikost = copy.max_size();
				poleUkazatelu = new T*[copy.max_size()];
				LOOP(copy.max_size()) poleUkazatelu[i] = new T(copy[i]);
			}
			else {
				velikost = 0;
				maxVelikost = 0;
				poleUkazatelu = new T*[1];
				LOOP(copy.size())(*this)(copy[i]);
			}
		}
		DynamicArray(const int &size) {
			maxVelikost = size;
			poleUkazatelu = new T*[maxVelikost];
		}
		~DynamicArray() {
			if (maxVelikost != 0)LOOP(maxVelikost)delete poleUkazatelu[i];
			else LOOP(velikost) delete poleUkazatelu[i];
			delete[] poleUkazatelu;
			poleUkazatelu = nullptr;
		}
		int size() const { return velikost; }
		int max_size() { return maxVelikost; }
		//T give_back(const int &n) { if (n < velikost && n >= 0)return *poleUkazatelu[n]; }
		T& operator[] (const int &n) const
		{
			if (n < velikost && n >= 0) { return *poleUkazatelu[n]; }
			else ERROR("ERROR: Spatne zadany parametr n!")
				if (velikost == 0) ERROR("ERROR: Pole je prazdne !")
		}
		T& end() const
		{
			if (velikost == 0) ERROR("ERROR: Pole je prazdne !")
			else return *poleUkazatelu[velikost - 1];
		}
		void operator()(const T &prvek) {
			if (velikost < maxVelikost) {
				poleUkazatelu[velikost++] = new T(prvek);
			}
			else if (maxVelikost == 0) {
				if (velikost == 0) {
					poleUkazatelu[0] = new T(prvek);
					velikost++;
				}
				else {
					//zkopirovani minuleho pole do noveho
					T **tempPoleUkazatelu = new T*[velikost];
					LOOP(velikost) { tempPoleUkazatelu[i] = new T(*poleUkazatelu[i]); delete poleUkazatelu[i]; }
					delete[] poleUkazatelu;

					velikost++;
					poleUkazatelu = new T*[velikost];
					//LOOP(velikost) poleUkazatelu[i] = new T;
					LOOP(velikost - 1) { poleUkazatelu[i] = new T(*tempPoleUkazatelu[i]); delete tempPoleUkazatelu[i]; tempPoleUkazatelu[i] = nullptr; }
					delete[] tempPoleUkazatelu; tempPoleUkazatelu = nullptr;
					poleUkazatelu[velikost - 1] = new T(prvek);
				}
			}
			else ERROR("ERROR: Velikost pole nesmi presahnout jeho maximalni hodnotu!")
		}
		void operator- (const int &prvek) {
			if (prvek >= 0 && prvek < velikost) {
				//preskladani prvku
				for (int i = prvek; i < velikost; i++) {
					if (i != (velikost - 1)) {
						*poleUkazatelu[i] = *poleUkazatelu[i + 1];
					}
					else {
						delete poleUkazatelu[i];
					}
				}
				velikost--;
			}
			else ERROR("ERROR: Nelze odstranit takovy prvek!")
		}
		void operator=(DynamicArray &x) {
			LOOP(velikost) delete poleUkazatelu[i];
			delete[] poleUkazatelu;
			if (x.size() < maxVelikost || maxVelikost == 0) {
				velikost = x.size();
			}
			else if (maxVelikost != 0 && x.size() > maxVelikost) {
				velikost = maxVelikost;
			}
			poleUkazatelu = new T*[velikost];
			LOOP(velikost) { poleUkazatelu[i] = new T(x[i]); }
		}
		void set(const int &n, const T &x) {
			if (n < velikost && n >= 0) {
				*poleUkazatelu[n] = x;
			}
			else ERROR("ERROR: Spatne zadany parametr n! Parametr musi byt vetsi nez 0 a mensi nez velikost vektoru!")
		}
		void clear(uint16_t start = 0, uint16_t end = 0) {
			if (start == 0 && end == 0) {
				if (poleUkazatelu != nullptr) {
					if (maxVelikost != 0)LOOP(maxVelikost)delete poleUkazatelu[i];
					else LOOP(velikost) delete poleUkazatelu[i];
					delete[] poleUkazatelu;
					poleUkazatelu = new T*[1];
				}
			}
			else {
				if (start >= end) {
					//if (maxVelikost != 0)LOOP(maxVelikost)delete poleUkazatelu[i];		
					T * *temp_pole_ukazatelu = new T*[velikost - (end - start)];
					//pøesune prvky od 0 do start
					LOOP(start)temp_pole_ukazatelu[i] = new T(*poleUkazatelu[i]);
					//pøesune prvky od konce pole do end
					SLOOP(end, velikost)temp_pole_ukazatelu[i] = new T(*poleUkazatelu[i]);
					//smaže bývalé pole
					LOOP(velikost) delete poleUkazatelu[i];
					delete[] poleUkazatelu;
					//zmìní velikost
					velikost = velikost - (end - start);
					//vytvoøí nové pole
					poleUkazatelu = new T*[velikost];
					LOOP(velikost)poleUkazatelu[i] = new T(*temp_pole_ukazatelu[i]);
					//smaže doèasné pole
					LOOP(velikost) delete temp_pole_ukazatelu[i];
					delete[] temp_pole_ukazatelu;
				}
				else {
					ERROR("ERROR=KONEC NEMUZE BYT VETSI NEZ ZACATEK MAZANI");
				}
			}
		}
	private:
		T **poleUkazatelu = nullptr;
		int velikost = 0, maxVelikost;
	};
}

