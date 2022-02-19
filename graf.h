#ifndef GRAF_H
#define GRAF_H

#include <vector>
#include <deque>
#include <list>
#include <utility>
#include <unordered_set>

using namespace std;

class Graf
{
    vector<unordered_set<int>> lista_susjedstva;
    //bitno da se modifikuje, tj. da kad dodamo neke grane, da ih brisemo odavde
    int brojGrana;
    int brojVrhova;
    bool usmjeren;

    public:
        Graf(int n, bool usmjeren = false);
        void dodajGranu(int i, int j);
        void IspisiGraf();
        vector<int> Put(int pocetni, int krajnji);
        vector<int> drugaStaza(vector<unordered_set<int>> planarna_lista_susjedstva, int pocetni,
            int krajnji, int regija, vector<unordered_set<int>> regijeGrana,
            unordered_set<int> bridovi_na_prvoj_stazi);
        int brojiGrane(vector<int>& broj_grana_svakog_vrha);
        vector<int> nadjiCiklus();
        int ubaciDodatak(int prvi, int drugi, int brRegija, int zajednickaRegija, vector<int>& staza,
            unordered_set<int>& ubaceniVrhovi, unordered_set<int>& ubaceneGrane,
            vector<unordered_set<int>>& pripadnostVrhovaRegijama,
            vector<unordered_set<int>>& vrhoviuRegijama, vector<unordered_set<int>>& zajednickeRegije,
            vector<unordered_set<int>>& regijeGrana, vector<unordered_set<int>>& planarna_lista_susjedstva);
        int daLiJePlanaran();


        /// Dodatak
        int teoremaProvjera();
        int brojRegija();


};



#endif // GRAF_H
