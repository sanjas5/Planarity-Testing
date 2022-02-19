#include "graf.h"
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

Graf::Graf(int n, bool usmjeren): usmjeren(usmjeren)
{
    brojVrhova = n;
    brojGrana = 0;
    lista_susjedstva.resize(n);
}

void Graf::dodajGranu(int i, int j)
{
    brojGrana++;
    lista_susjedstva[i].insert(j);
    if(!usmjeren)
        lista_susjedstva[j].insert(i);
}

void Graf::IspisiGraf()
{
    for(int i=0;i<lista_susjedstva.size();i++)
    {
        cout<<i<<": ";
        for(auto it = lista_susjedstva[i].begin();it!=lista_susjedstva[i].end();it++)
            cout<<*it<<" ";
        cout<<endl;
  }
}

vector<int> Graf::Put(int pocetni, int krajnji)
{
    int n = lista_susjedstva.size();
    vector<int> udaljenosti(n,-1);
    vector<int> roditelj(n,-1);
    queue<int> red;
    red.push(pocetni);
    udaljenosti[pocetni] = 0;
    roditelj[pocetni] = -2;

    while(!red.empty()) {
        int trenutni = red.front();
        //cout<<"Obradjujem "<<trenutni<<endl;
        red.pop();
        for(auto it = lista_susjedstva[trenutni].begin();it != lista_susjedstva[trenutni].end();it++)
            if(udaljenosti[*it] == -1)
            {
                red.push(*it);
                udaljenosti[*it] = udaljenosti[trenutni] + 1;
                roditelj[*it] = trenutni;
            }
    }


    vector<int> put;
    int trenutni = krajnji;

    while(roditelj[trenutni] != -2)
    {
        put.push_back(trenutni);
        trenutni = roditelj[trenutni];
    }

    put.push_back(pocetni);
    std::reverse(put.begin(), put.end());
    return put;
}

vector<int> Graf::drugaStaza(vector<unordered_set<int>> planarna_lista_susjedstva, int pocetni,
    int krajnji, int regija, vector<unordered_set<int>> regijeGrana,
    unordered_set<int> grane_na_prvoj_stazi)
{
    int n = planarna_lista_susjedstva.size();
    vector<int> udaljenosti(n, -1);
    vector<int> roditelj(n, -1);
    queue<int> red;
    red.push(pocetni);
    udaljenosti[pocetni] = 0;
    roditelj[pocetni] = -2;

    while (!red.empty())
    {
        int trenutni = red.front();
        //cout<<"Obradjujem "<<trenutni<<endl;
        red.pop();
        for (auto it = planarna_lista_susjedstva[trenutni].begin();
            it != planarna_lista_susjedstva[trenutni].end(); it++)
        {
            auto grana = trenutni * brojVrhova + *it;
            if (grane_na_prvoj_stazi.find(grana) != grane_na_prvoj_stazi.end())
                continue; // grane na prvoj stazi su "odspojene"
            if (udaljenosti[*it] == -1)
            {
                if (regijeGrana[grana].find(regija) != regijeGrana[grana].end())
                {
                    // trazimo stazu po granama cija je (jedna) regija == regija
                    red.push(*it);
                    udaljenosti[*it] = udaljenosti[trenutni] + 1;
                    roditelj[*it] = trenutni;
                }
            }
        }
    }


    vector<int> put;
    int trenutni = krajnji;

    while (roditelj[trenutni] != -2)
    {
        put.push_back(trenutni);
        trenutni = roditelj[trenutni];
    }

    put.push_back(pocetni);
    std::reverse(put.begin(), put.end());
    return put;
}




/// broji stepene svakog vrha i grane u grafu
int Graf::brojiGrane(vector<int>& broj_grana_svakog_vrha)
{
    int brojac = 0;
    for(int i=0; i<brojVrhova; i++)
    {
        broj_grana_svakog_vrha.push_back(lista_susjedstva[i].size());
        brojac += lista_susjedstva[i].size();
    }

    if(!usmjeren)
        brojac = brojac/2;

    return brojac;

}

/// Funkcija koja vraća neki ciklus u grafu ako ga ima, a ako ga nema vraća prazan vektor.
vector<int>Graf::nadjiCiklus()
{
    int n = lista_susjedstva.size();
    int pocetni = 0;
    vector<int> koGaJeOtkrio(n, -1);
    queue<int> red;
    vector<int> ciklus;
    red.push(pocetni);
    koGaJeOtkrio[pocetni] = -2;

    while(!red.empty())
    {
        int trenutni = red.front();
        red.pop();
        for(auto it=lista_susjedstva[trenutni].begin(); it!=lista_susjedstva[trenutni].end(); it++)
            if(koGaJeOtkrio[*it] == -1){
                red.push(*it);
                koGaJeOtkrio[*it] = trenutni;
            }
            else if(koGaJeOtkrio[trenutni] != *it){
                /// Pratimo put od trenutnog
                vector<int> putOdTrenutnog;
                putOdTrenutnog.push_back(trenutni);
                while(koGaJeOtkrio[putOdTrenutnog[putOdTrenutnog.size()-1]] != -2)
                    putOdTrenutnog.push_back(koGaJeOtkrio[putOdTrenutnog[putOdTrenutnog.size()-1]]);

                /// Pratimo put od susjeda
                vector<int> putOdSusjeda;
                putOdSusjeda.push_back(*it);
                while(koGaJeOtkrio[putOdSusjeda[putOdSusjeda.size()-1]] != -2)
                    putOdSusjeda.push_back(koGaJeOtkrio[putOdSusjeda[putOdSusjeda.size()-1]]);

                /// Roditelj
                int indeksPresjeka = -1;
                for(int i=0; i<putOdSusjeda.size(); i++){
                    ciklus.push_back(putOdSusjeda[i]);
                    for(int j=0; j<putOdTrenutnog.size(); j++)
                        if(putOdTrenutnog[j] == putOdSusjeda[i])
                            indeksPresjeka = j;
                    if(indeksPresjeka != -1)
                        break;
                }
                /// Detekcija ciklusa unazad
                for(int i=indeksPresjeka-1; i>=0; i--)
                    ciklus.push_back(putOdTrenutnog[i]);

                return ciklus;
        }
    }

    return ciklus;
}


int Graf::ubaciDodatak(int prvi, int drugi, int brRegija,
    int zajednickaRegija, vector<int>& staza,
    unordered_set<int>& ubaceniVrhovi, unordered_set<int>& ubaceneGrane,
    vector<unordered_set<int>>& pripadnostVrhovaRegijama, vector<unordered_set<int>>& vrhoviuRegijama,
    vector<unordered_set<int>>& zajednickeRegije, vector<unordered_set<int>>& regijeGrana,
    vector<unordered_set<int>>& planarna_lista_susjedstva)
{
    /// nalazimo put izmedju dva vrha koji se nalaze izvan ciklusa/obojenihh regija
    /// tri promjene koje se desavaju prilikom dodavanja novoga vrha:
    ///prva promjena: svi vrhovi na novoj stazi dobijaju novu regiju
    /// i zajednicku regiju dva krajnja vrha (imamo zapamcenu zajednicku regiju preko koje se islo)
   /// druga promjena: uklanjamo iz prvobitnog grafa sve grane nove staze

    unordered_set<int> grane_na_prvoj_stazi;
    vrhoviuRegijama.resize(brRegija + 1);
    for (auto prviVrh : staza)
    {
        for (auto drugiVrh : staza)
        {
            if (drugiVrh == prviVrh)
                continue;
            zajednickeRegije[prviVrh * brojVrhova + drugiVrh].insert(brRegija);
            pripadnostVrhovaRegijama[prviVrh].insert(brRegija);
            pripadnostVrhovaRegijama[drugiVrh].insert(brRegija);
            ubaceniVrhovi.insert(prviVrh);
            ubaceniVrhovi.insert(drugiVrh);
            vrhoviuRegijama[brRegija].insert(prviVrh);
            vrhoviuRegijama[brRegija].insert(drugiVrh);
            zajednickeRegije[prviVrh * brojVrhova + drugiVrh].insert(zajednickaRegija);
            pripadnostVrhovaRegijama[prviVrh].insert(zajednickaRegija);
            pripadnostVrhovaRegijama[drugiVrh].insert(zajednickaRegija);
            vrhoviuRegijama[zajednickaRegija].insert(prviVrh);
            vrhoviuRegijama[zajednickaRegija].insert(drugiVrh);
        }
    }
    for (int i=0;i!=staza.size();i++)
    {
        auto prviVrh = staza[i];
        auto drugiVrh = staza[(i + 1) % staza.size()];
        planarna_lista_susjedstva[prviVrh].insert(drugiVrh);
        planarna_lista_susjedstva[drugiVrh].insert(prviVrh);
        lista_susjedstva[prviVrh].erase(drugiVrh);
        lista_susjedstva[drugiVrh].erase(prviVrh);
        auto grana1 = prviVrh * brojVrhova + drugiVrh; // prviVrh -> drugiVrh
        auto grana2 = drugiVrh * brojVrhova + prviVrh; // drugiVrh -> prviVrh
        // prvi --> drugi i drugi-->prvi ne ubacujemo!
        if ((prviVrh != prvi || drugiVrh != drugi) && (prviVrh != drugi || drugiVrh != prvi))
        {

            regijeGrana[grana1].insert(zajednickaRegija);
            regijeGrana[grana2].insert(zajednickaRegija);
            regijeGrana[grana1].insert(brRegija);
            regijeGrana[grana2].insert(brRegija);

            ubaceneGrane.insert(grana1);
            ubaceneGrane.insert(grana2);
            grane_na_prvoj_stazi.insert(grana1);
            grane_na_prvoj_stazi.insert(grana2);
        }
        // azuriramo zajednickeRegije za sve parove gdje je prvi vrh na novoj stazi
        // a drugi vrh u regiji zajednickaRegija
        for (auto vrh_u_regiji : vrhoviuRegijama[zajednickaRegija])
        {
            if (vrh_u_regiji != prviVrh)
            {
                zajednickeRegije[prviVrh * brojVrhova + vrh_u_regiji].insert(zajednickaRegija);
                zajednickeRegije[vrh_u_regiji * brojVrhova + prviVrh].insert(zajednickaRegija);
            }
            if (vrh_u_regiji != drugiVrh)
            {
                zajednickeRegije[drugiVrh * brojVrhova + vrh_u_regiji].insert(zajednickaRegija);
                zajednickeRegije[vrh_u_regiji * brojVrhova + drugiVrh].insert(zajednickaRegija);
            }
        }
    }

    // trazimo *jednu* stazu prvi->drugi, takvu da svaka grana ima regiju (u regijeGrana)
    // jednaku zajednickaRegija (iz trazenja izuzimamo vec dodanu stazu)
    // svakoj grani na nadjenoj stazi dodajemo novu regiju;
    // ako je broj njenih regija presao dva, moramo joj oduzeti regiju zajednickaRegija
    // promijeniti zajednickeRegije tako da se izbrise regija koja vise nije zajednicka
    // pripadnostVrhovaRegijama
    // vrhoviuRegijama

    auto druga_staza = drugaStaza(planarna_lista_susjedstva, prvi, drugi, zajednickaRegija,
        regijeGrana, grane_na_prvoj_stazi);
    cout<<"Idem preko staze: ";
    for(int i=0; i<druga_staza.size(); i++)
        cout<<druga_staza[i]<<" ";
    cout<<" i regije "<<zajednickaRegija<<endl<<endl;
    for (int i = 0; i != druga_staza.size()-1; i++)
    {
        int prviVrh = druga_staza[i];
        int drugiVrh = druga_staza[(i + 1) % druga_staza.size()];
        int grana1 = prviVrh * brojVrhova + drugiVrh;
        int grana2 = drugiVrh * brojVrhova + prviVrh;
        regijeGrana[grana1].insert(brRegija);
        regijeGrana[grana2].insert(brRegija);
        zajednickeRegije[grana1].insert(brRegija);
        zajednickeRegije[grana2].insert(brRegija);
        pripadnostVrhovaRegijama[prviVrh].insert(brRegija);
        pripadnostVrhovaRegijama[drugiVrh].insert(brRegija);
        vrhoviuRegijama[brRegija].insert(prviVrh);
        vrhoviuRegijama[brRegija].insert(drugiVrh);
        // azuriramo zajednickeRegije za sve parove gdje je prvi vrh na novoj stazi
        // a drugi vrh u regiji brRegija
        for (auto vrh_u_regiji : vrhoviuRegijama[brRegija])
        {
            if (vrh_u_regiji != prviVrh)
            {
                zajednickeRegije[prviVrh * brojVrhova + vrh_u_regiji].insert(brRegija);
                zajednickeRegije[vrh_u_regiji * brojVrhova + prviVrh].insert(brRegija);
            }
            if (vrh_u_regiji != drugiVrh)
            {
                zajednickeRegije[drugiVrh * brojVrhova + vrh_u_regiji].insert(brRegija);
                zajednickeRegije[vrh_u_regiji * brojVrhova + drugiVrh].insert(brRegija);
            }
        }
        if (regijeGrana[grana1].size() > 2)
        {
            regijeGrana[grana1].erase(zajednickaRegija);
            regijeGrana[grana2].erase(zajednickaRegija); // uklanjamo grane iz zajednicke regije

            if (planarna_lista_susjedstva[prviVrh].size() < pripadnostVrhovaRegijama[prviVrh].size())
            {
                // uklanjamo prvi vrh iz regije zajednickaRegija
                pripadnostVrhovaRegijama[prviVrh].erase(zajednickaRegija);
                vrhoviuRegijama[zajednickaRegija].erase(prviVrh);
                for (auto vrh_u_regiji : vrhoviuRegijama[zajednickaRegija])
                {
                    zajednickeRegije[prviVrh * brojVrhova + vrh_u_regiji].erase(zajednickaRegija);
                    zajednickeRegije[vrh_u_regiji * brojVrhova + prviVrh].erase(zajednickaRegija);

                }
            }
        }

    }
    return brRegija + 1;
}



int Graf::daLiJePlanaran()
{

     /* OPIS POMOCNIH STRUKTURA:
        ubaceniVrhovi = skup vrhova koji su vec u ravni
        ubaceneGrane = skup grana (parova vrhova), koji su vec u ravni
        pripadnostVrhovaRegijama = za svaki vrh X vec u ravni skup regija u kojima se nalazi
        zajednickeRegije = skup zajednickih regija za svaki par vrhova
                            koji se vec nalazi u ravni!
        vrhoviuRegijama = vektor skupova vrhova koji pripadaju svakoj pojedinoj regiji
        regijeGrana - regije kojima pripada svaka grana koja je ubacena (najvise dvije regije po grani!)

        */

    unordered_set<int> ubaceniVrhovi; //ovdje staviti unordered unordered_set
    unordered_set<int> ubaceneGrane; //isto
    vector<unordered_set<int>> pripadnostVrhovaRegijama(brojVrhova);
    vector<unordered_set<int>> zajednickeRegije(brojVrhova * brojVrhova);//matrica
    vector<unordered_set<int>> vrhoviuRegijama(2);
    vector<int> stepeni;
    vector<unordered_set<int>> regijeGrana(brojVrhova*brojVrhova);
    vector<unordered_set<int>> planarna_lista_susjedstva(brojVrhova);
    int brRegija = 1;

    vector<int> ciklus = nadjiCiklus();

    /// ako ne postoji ciklus - graf je stablo i planaran je!
    if(ciklus.size() == 0)
    {
        cout<<"Stablo!!";
        return brRegija;
    }
    brRegija = 2;

    brojGrana = brojiGrane(stepeni);

    cout<<"Nasao proizvoljan ciklus: "<<endl;
    for(int i=0; i<ciklus.size(); i++)
        cout<<ciklus[i]<<" ";
    cout<<endl;


    /// popunjavanje:
    /// prolazeci kroz ciklus
    /// petlja po vrhovima koji se nalaze u ciklusu
    for(int i=0; i<ciklus.size(); i++)
    {
        int vrh = ciklus[i];
        int sljedeciVrh = ciklus[(i+1)%ciklus.size()];

        planarna_lista_susjedstva[vrh].insert(sljedeciVrh);
        planarna_lista_susjedstva[sljedeciVrh].insert(vrh);

        pripadnostVrhovaRegijama[vrh].insert(0);
        pripadnostVrhovaRegijama[vrh].insert(1); //fali za sljedecivrh
        pripadnostVrhovaRegijama[sljedeciVrh].insert(0);
        pripadnostVrhovaRegijama[sljedeciVrh].insert(1); //fali za sljedecivrh

        ubaceniVrhovi.insert(vrh);

        auto grana1 = vrh * brojVrhova + sljedeciVrh;
        auto grana2 = sljedeciVrh * brojVrhova + vrh;

        regijeGrana[grana1].insert(0);
        regijeGrana[grana1].insert(1);
        regijeGrana[grana2].insert(0);
        regijeGrana[grana2].insert(1);

        ubaceneGrane.insert(grana1);
        ubaceneGrane.insert(grana2);

        vrhoviuRegijama[0].insert(vrh);
        vrhoviuRegijama[1].insert(vrh); //fali za sljedeci
        vrhoviuRegijama[0].insert(sljedeciVrh);
        vrhoviuRegijama[1].insert(sljedeciVrh); //fali za sljedeci

        /// izbacujemo one grane koj su u ciklusu
        lista_susjedstva[vrh].erase(sljedeciVrh);
        lista_susjedstva[sljedeciVrh].erase(vrh);
    }

    /// zajednicke regije za one vrhove koji su vec u ciklusu
    for (int i = 0; i != ciklus.size(); i++)
    {
        int vrh1 = ciklus[i];
        for (int j = i + 1; j != ciklus.size(); j++)
        {
            int vrh2 = ciklus[j];
            zajednickeRegije[vrh1 * brojVrhova + vrh2].insert(0); //ovdje matricu
            zajednickeRegije[vrh1 * brojVrhova + vrh2].insert(1);
            zajednickeRegije[vrh2 * brojVrhova + vrh1].insert(0);
            zajednickeRegije[vrh2 * brojVrhova + vrh1].insert(1);
        }
    }

    cout<<"Ubaceni vrhovi su: "<<endl;
    for (const auto& element : ubaceniVrhovi) {
        cout<<element<<" ";
    }
    cout<<endl;
    cout<<"Ubacene grane su: "<<endl;
    for (const auto& element : ubaceneGrane) {
        cout<<element/brojVrhova<<" ";
    }
    cout<<endl;
    /*cout<<"zajednickeRegije  su: "<<endl;
    for(int i=0; i<zajednickeRegije.size(); i++){
        for(const auto& element : zajednickeRegije[i])
            cout<<element<<" ";
    }
    cout<<endl;
    cout<<"Pripadnost vrhova regijama: "<<endl;
    for(int i=0; i<pripadnostVrhovaRegijama.size(); i++){
        for(const auto& element : pripadnostVrhovaRegijama[i])
            cout<<element<<" ";
    }
    cout<<endl;*/

    cout<<"Ubacenih grana je sada: "<<ubaceneGrane.size()<<", a ukupan broj grana je 2 * "<< brojGrana<<endl;

    /// petlja koja se izvrsava sve dok nisu sve grane ubacene
    while(ubaceneGrane.size() < 2*brojGrana)
    {
        /// provjeravamo kriterij zaustavljanja
        /// provjeravamo da li postoji bilo koji vrh koji je vec ubacen, a da nema zajednicku regiju
        /// idemo po parovima ubacenih vrhova!

        /// 1. provjera ima li vrhova bez zajednickih regija
        for (auto prvi : ubaceniVrhovi)
        {
            /// vrhovi moraju imati grane izvan ciklusa!
            if (lista_susjedstva[prvi].size() > 0)
            {
                for (auto drugi : ubaceniVrhovi)
                {
                    if (drugi == prvi || lista_susjedstva[drugi].size() == 0)
                        continue;
                    int brZajednickihRegija =
                        zajednickeRegije[prvi * brojVrhova + drugi].size(); //matrica
                    //cout<<"Broj zajendickih regija je: "<<brZajednickihRegija<<endl;
                    if (brZajednickihRegija == 0)    /// nije planaran, blokiran put
                    {
                        auto put1 = Put(prvi, drugi);
                        cout<<"Kritican put: "<<endl;
                        for(int i=0; i<put1.size(); i++)
                            cout<<put1[i]<<" ";
                        cout<<endl<<endl;
                        if (put1.size())
                            return 0;
                    }
                }
            }
        }

        /// 2. ima li forsiranih grana?
        bool ubaceno = false;
        for(auto prvi : ubaceniVrhovi)
        {
            /// vrhovi moraju imati grane izvan ciklusa!
            if (lista_susjedstva[prvi].size() == 0)
                continue;
            for(auto drugi : ubaceniVrhovi)
            {
                if (drugi == prvi || lista_susjedstva[drugi].size() == 0)
                    continue;
                int brZajednickihRegija =
                    zajednickeRegije[prvi*brojVrhova + drugi].size();
                if (brZajednickihRegija == 1)
                {
                    /// provjeravamo da li grane imaju vrhove izvan ciklusa
                    /// forsirani dodatak

                    /// put od prvog do drugog koji ne prolazi kroz vec neku dodanu granu
                    vector<int> staza = Put(prvi, drugi);

                     ///njihova zajednicka regija, skup zajednickih regija
                    int zajednickaRegija = *(zajednickeRegije[prvi * brojVrhova + drugi].begin());

                    ///dodamo stazu unutra
                    if (staza.size() > 1)
                    {

                        cout<<"Regija je: "<<brRegija<<endl;
                        cout<<endl;
                        cout<<"Pripadnost vrhova regijama: "<<endl;
                        for(int i=0; i<pripadnostVrhovaRegijama.size(); i++){
                            cout<<i<<": ";
                            for(const auto& element : pripadnostVrhovaRegijama[i])
                                cout<<element<<" ";
                        }
                        cout<<endl;

                        cout<<"Nova staza forsiranih: "<<endl;
                        for(int i=0; i<staza.size(); i++)
                            cout<<staza[i]<<" ";
                        cout<<endl;

                        brRegija = ubaciDodatak(prvi, drugi, brRegija, zajednickaRegija,
                            staza, ubaceniVrhovi, ubaceneGrane, pripadnostVrhovaRegijama,
                            vrhoviuRegijama, zajednickeRegije,regijeGrana,planarna_lista_susjedstva);
                        ubaceno = true;
                        break;
                    }   //end if staza.size()>1
                }
            }   //end for vrh2
            if (ubaceno)
                break;
        }   //end for vrh1
        if (ubaceno)
            continue;


        /// 3. ostale grane
        for (auto prvi : ubaceniVrhovi)
        {
            /// vrhovi moraju imati grane izvan ciklusa!
            if (lista_susjedstva[prvi].size() == 0)
                continue;
            for (auto drugi : ubaceniVrhovi)
            {
                if (drugi == prvi || lista_susjedstva[drugi].size() == 0)
                    continue;
                int brZajednickihRegija =
                    zajednickeRegije[prvi * brojVrhova + drugi].size();
                if (brZajednickihRegija < 2) /// ako ne postoji put izvan ciklusa izmedju dva vrha, idemo dalje
                    continue;

                /// Moramo traziti dva neka vrha tj. neki dodatak koji nije forsiran..
                /// stazu stavimo u prvu zajednicku regiju
                /// Treba odabrati pravu zajednicku regiju, ne prvu
                /// Kako? Najveci stepen nekog vrha u regiji u ostatku orginalnog grafa?
                auto prva_zajednicka = *(zajednickeRegije[prvi * brojVrhova + drugi].begin());
                vector<int> staza = Put(prvi, drugi);
                if (staza.size() > 1)
                {
                    cout<<"Regija je: "<<brRegija<<endl;
                    cout<<"Pripadnost vrhova regijama: "<<endl;
                        for(int i=0; i<pripadnostVrhovaRegijama.size(); i++){
                            cout<<i<<": ";
                            for(const auto& element : pripadnostVrhovaRegijama[i])
                                cout<<element<<" ";
                        }
                        cout<<endl;

                    cout<<"Nova staza neforsiranih: "<<endl;
                        for(int i=0; i<staza.size(); i++)
                            cout<<staza[i]<<" ";
                        cout<<endl;
                   brRegija = ubaciDodatak(prvi, drugi, brRegija, prva_zajednicka,
                       staza, ubaceniVrhovi, ubaceneGrane, pripadnostVrhovaRegijama,
                            vrhoviuRegijama, zajednickeRegije,regijeGrana,planarna_lista_susjedstva);
                   ubaceno = true;
                   break;
                }   //end if staza.size()>1
            }   //end for vrh2
            if (ubaceno)
                break;
        }   //end for vrh1
    }   //end while
    return brRegija;
}




/**Teorem: [Eulerova formula] Za povezan, planaran, jednostavan graf G=(V,E) sa e=|E| i v=|V|,
ako je r broj regija koje se kreiraju prilikom crtanja planarne reprezentacije grafa, onda je r = e-v+2 .
*/
int Graf::brojRegija()
{
    int r = brojGrana - brojVrhova + 2;

    return r;
}

int Graf::teoremaProvjera(){
    cout<<"Broj regija u grafu je "<<brojRegija()<<", i on ";
    if(brojVrhova >= 3 && (brojGrana <= 3*brojVrhova - 6) && (brojGrana <= 2*brojVrhova - 4))
        return 1;
    return 0;
}
