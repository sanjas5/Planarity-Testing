#include <iostream>
#include "graf.h"

using namespace std;

int main() {

    /// kocka za testiranje planarnosti:
    Graf K(8);
    K.dodajGranu(0, 1);
    K.dodajGranu(0, 3);
    K.dodajGranu(0, 4);
    K.dodajGranu(1, 5);
    K.dodajGranu(1, 2);
    K.dodajGranu(2, 3);
    K.dodajGranu(2, 6);
    K.dodajGranu(3, 7);
    K.dodajGranu(3, 5);
    K.dodajGranu(4, 5);
    K.dodajGranu(4, 7);
    K.dodajGranu(5, 6);
    K.dodajGranu(6, 7);

    ///stablo za testiranje planarnosti:
    Graf S(8);
    S.dodajGranu(0, 1);
    S.dodajGranu(0, 2);
    S.dodajGranu(1, 3);
    S.dodajGranu(1, 4);
    S.dodajGranu(4, 5);
    S.dodajGranu(4, 6);
    S.dodajGranu(4, 7);


    ///Kuratowski graf K3,3 za testiranje planarnosi:
    Graf K3(6);
    K3.dodajGranu(0, 3);
    K3.dodajGranu(0, 4);
    K3.dodajGranu(0, 5);

    K3.dodajGranu(1, 3);
    K3.dodajGranu(1, 4);
    K3.dodajGranu(1, 5);

    K3.dodajGranu(2, 3);
    K3.dodajGranu(2, 4);
    K3.dodajGranu(2, 5);

    ///Kuratowski graf K5 za testiranje planarnosi:
    Graf K5(6);
    K5.dodajGranu(0, 1);
    K5.dodajGranu(0, 2);
    K5.dodajGranu(0, 3);
    K5.dodajGranu(0, 4);
    K5.dodajGranu(0, 5);
    K5.dodajGranu(1, 2);
    K5.dodajGranu(1, 3);
    K5.dodajGranu(1, 4);
    K5.dodajGranu(1, 5);
    K5.dodajGranu(2, 3);
    K5.dodajGranu(2, 4);
    K5.dodajGranu(2, 5);
    K5.dodajGranu(3, 4);
    K5.dodajGranu(3, 5);
    K5.dodajGranu(4, 5);

    Graf K7(5);
    K7.dodajGranu(0,1);
    K7.dodajGranu(0,2);
    K7.dodajGranu(0,3);
    K7.dodajGranu(0,4);
    K7.dodajGranu(2,1);
    K7.dodajGranu(2,4);
    K7.dodajGranu(2,3);
    K7.dodajGranu(3,4);
    K7.dodajGranu(3,1);
    K7.dodajGranu(4,1);


    /// jedan ciklus za testiranje planarnosti
    Graf N(10);
    N.dodajGranu(0,1);
    N.dodajGranu(1,2);
    N.dodajGranu(2,3);
    N.dodajGranu(3,4);
    N.dodajGranu(4,5);
    N.dodajGranu(5,6);
    N.dodajGranu(6,7);
    N.dodajGranu(8,7);
    N.dodajGranu(8,9);
    N.dodajGranu(0,9);

    Graf F(10);
    F.dodajGranu(0,1);
    F.dodajGranu(0,2);
    F.dodajGranu(0,3);
    F.dodajGranu(1,3);
    F.dodajGranu(1,4);
    F.dodajGranu(2,4);
    F.dodajGranu(5,4);
    F.dodajGranu(6,4);
    F.dodajGranu(7,8);
    F.dodajGranu(0,8);
    F.dodajGranu(9,8);
    F.dodajGranu(7,5);

    Graf Z(5);
    Z.dodajGranu(0,1);
    Z.dodajGranu(0,2);
    Z.dodajGranu(0,3);
    Z.dodajGranu(4,1);
    Z.dodajGranu(4,2);
    Z.dodajGranu(4,3);


    Graf H(8);
    H.dodajGranu(0,1);
    H.dodajGranu(0,2);
    H.dodajGranu(1,2);
    H.dodajGranu(2,3);
    H.dodajGranu(3,0);
    H.dodajGranu(3,4);
    H.dodajGranu(4,1);
    H.dodajGranu(1,7);
    H.dodajGranu(7,5);
    H.dodajGranu(5,6);
    H.dodajGranu(6,3);
    H.dodajGranu(6,0);
    H.dodajGranu(6,1);


    int brRegija = H.daLiJePlanaran();
    if (brRegija)
        cout<<"Planaran, broj regija: " <<brRegija<<endl;
    else
        cout<<"Nije planaran"<<endl;

/*    int provjera = K5.teoremaProvjera();
    if(provjera)
        cout<<"je planaran!!"<<endl;
    else cout<<"nije planaran!!"<<endl;*/

}
