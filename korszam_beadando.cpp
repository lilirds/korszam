#include <iostream>
#include <numeric> 
#include <cmath>




// Itt van elnevezve (deklarálva) minden, ami az osztály része

class Rational{
 
	private:
	
		int szamlalo;
		int nevezo;
		 
		void egysz(); 			// Az egyszerűbb alakra hozás
	 
	public:
	 
		Rational(int A, int B); // Tört létrehozása
		 
		void print() const; // Kiíratás
		 
		// Itt az osztályra felülírjuk a +-*/ szimbólumokat, hogy azt csinálják, amit mi akarunk
		Rational operator+(const Rational &other) const;
		Rational operator-(const Rational &other) const;
		Rational operator*(const Rational &other) const;
		Rational operator/(const Rational &other) const;
		Rational operator^(int C) const;
		
		//Ez pedig a gyökvonás lesz
		Rational gyok(int P, int N) const;
	  
};



// Itt meg maguk a függvények

void Rational::egysz() {
	//Az egyszerűsítés

    int LNKO = std::gcd(szamlalo, nevezo); //Beépített függvény legnagyobb közös osztóra
	
    szamlalo /= LNKO;	//Elosztjuk vele a számlálót
    nevezo /= LNKO;		//És a nevezőt


	//Ha a nevező negatív, akkor megszorozza mindkettőt minusz eggyel, mert az szebb, amikor kiírjuk
	
	if(nevezo < 0){
 
		szamlalo *= -1;
		nevezo *= -1;
	 
	}
	
	
}

Rational::Rational(int A, int B) {
	//Megcsinál egy törtet

	szamlalo = A;
	nevezo = B;
	//Számlálót és a nevezőt beadja


	//Ha nulla a nevező, akkor hibát ír
    if (B == 0) {
        throw std::invalid_argument("Nullával osztás.");
    }
	
	//És legegyszerűbb alakra hozza
    egysz();
}


Rational Rational::operator+(const Rational& other) const {
	//Összeadás

	//Kiszámoljuk az összeg számlálóját és nevezőjét
    int ujSzamlalo = (szamlalo * other.nevezo) + (other.szamlalo * nevezo);
	int ujNevezo = nevezo * other.nevezo;
	 
	 
	//Majd csinálunk belőlük egy új törtet
	Rational uj(ujSzamlalo, ujNevezo);
	 
	return uj;
}

Rational Rational::operator-(const Rational& other) const {
	//Kivonás, ugyanígy

    int ujSzamlalo = (szamlalo * other.nevezo) - (other.szamlalo * nevezo);
	int ujNevezo = nevezo * other.nevezo;
	 
	 
	Rational uj(ujSzamlalo, ujNevezo);
	 
	return uj;
}


Rational Rational::operator*(const Rational& other) const {
	//Szorzás, ugyanígy

    int ujSzamlalo = szamlalo * other.szamlalo;
	int ujNevezo = nevezo * other.nevezo;
	 
	 
	Rational uj(ujSzamlalo, ujNevezo);
	 
	return uj;
}

Rational Rational::operator/(const Rational& other) const {
	//Osztás, ugyanígy

    int ujSzamlalo = szamlalo * other.nevezo;
	int ujNevezo = nevezo * other.szamlalo;
	 
	 
	Rational uj(ujSzamlalo, ujNevezo);
	 
	return uj;
}

Rational Rational::operator^(int C) const {
	//Hatványozás, C-re emelve
	//Ez annyiban tér el, hogy más, ha negatív a kitevő
	
	int ujSzamlalo;
	int ujNevezo;
	
	if (C > 0) {
    	ujSzamlalo = std::pow(szamlalo, C);
    	ujNevezo = std::pow(nevezo, C);
	} else {
	    //Ha negatív, akkor felcseréli őket és pozitívnak veszi C-t

    	ujSzamlalo = std::pow(nevezo, -C);
    	ujNevezo = std::pow(szamlalo, -C);
	}

	Rational uj(ujSzamlalo, ujNevezo);
	 
	return uj;
}

Rational Rational::gyok(int P = 2, int N = 10) const {
	//P-edik gyökvonás, ez picit komplikáltabb
	
	int kitev;
	
	//Először a határesetek
	//Ha negatív a tört, akkor hibát ír
    if (szamlalo < 0) {
        throw std::invalid_argument("Negatív a gyök alatt.");
    }
    //Ha nulladik gyököt kérünk, hibát ír
    if (P == 0) {
        throw std::invalid_argument("Nulladik gyök nincs.");
    }
    
    
    double Q; //Ő lesz maga a törtünk, double-lé konvertálva
    
    //Ha a kitevő pozitív, az az alapvető eset
    if (P>0) {
        Q = (double)szamlalo / (double)nevezo; 	//Konvertálás
        kitev = P;
    //Ha a kitevő negatív:
    } else {
        Q =  (double)nevezo / (double)szamlalo;  //A szám fordítottjával dolgozunk
        kitev = -P; //És a kitevőt pozitívnak vesszük
    }
    

	double GyokQ = Q;	//Aztán első tippnek megadjuk magát a számot
	
	//Elkezdünk iterálni
	for (int c = 0; c < N; c++) {
        GyokQ = GyokQ - (std::pow(GyokQ, kitev) - Q) / (kitev * std::pow(GyokQ, kitev - 1));	//Ez maga a Newton képlet
    }
	
	
	//Itt feltételeztem, hogy a kimenetnek is Rational-t akarunk.
	//Ha nem, akkor simán return GyokQ lenne, és Rational helyett Double típussal lenne a függvény

	int ujNevezo = (int)(std::pow(10, (int)(std::log10(GyokQ)*(-1)+5)));			//A nevező legyen egy nagy szám, kb. a megoldás nagyságrendje +5 (ennyi tizedesjegy pontosságot akartam)
    	//Példa: ha GyokQ = 0.001, akkor log10(GyokQ) = -3. Ekkor 10^3-nal kell megszorozni az első értékes jegyért.
    	
    int ujSzamlalo = (int)(GyokQ*ujNevezo);	//A számláló az a GyökQ lesz, megszorozva a nevezővel
	
    Rational uj(ujSzamlalo, ujNevezo);

	return uj;
}




void Rational::print() const {
	//Kiíratás

    std::cout << szamlalo << "/" << nevezo << std::endl;
}


//Tesztelés
int main(){
 
 Rational Q1(9, 5);
 Rational Q2(27, 343);
 
 Rational QA = Q1 + Q2;
 Rational QB = Q1 - Q2;
 Rational QC = Q1 * Q2;
 Rational QD = Q1 / Q2;
 
 Rational QE = Q1^2;
 Rational QF = Q1^(-2);
 Rational QG = Q1.gyok();
 Rational QH = Q2.gyok(-3, 100);

 
 QA.print();
 QB.print();
 QC.print();
 QD.print();
 QE.print();
 QF.print();
 QG.print();
 QH.print();
 
}