#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

const int MAX_PREKIU = 100;
const int KATEGORIJU_SK = 3;
const int NUOLAIDOS_RIBA = 10;
const double NUOLAIDOS_DALIS = 0.10;

string apkarpyti(const string& s) {
    size_t pr = s.find_first_not_of(" \t\r\n");
    if (pr == string::npos) return "";
    size_t pb = s.find_last_not_of(" \t\r\n");
    return s.substr(pr, pb - pr + 1);
}

bool arTinkamaKategorija(const string& kat, const string kategorijos[]) {
    for (int i = 0; i < KATEGORIJU_SK; i++)
        if (kat == kategorijos[i]) return true;
    return false;
}

bool tekstasIDouble(const string& tekstas, double& rezultatas) {
    if (tekstas.empty()) return false;
    char* pabaiga = nullptr;
    rezultatas = strtod(tekstas.c_str(), &pabaiga);
    return *pabaiga == '\0';
}

bool tekstasIInt(const string& tekstas, int& rezultatas) {
    if (tekstas.empty()) return false;
    char* pabaiga = nullptr;
    long v = strtol(tekstas.c_str(), &pabaiga, 10);
    if (*pabaiga != '\0') return false;
    if (v < -2147483647L || v > 2147483647L) return false;
    rezultatas = static_cast<int>(v);
    return true;
}

bool nuskaitytiDuomenis(const string& failoVardas, string kategorijos[],
                        string pavadinimai[], double kainos[], int kiekiai[],
                        int& n, const string leidziamosKat[], string& klaida) {
    n = 0;
    ifstream fin(failoVardas.c_str());
    if (!fin.is_open()) {
        klaida = "Nepavyko atidaryti duomenu failo \"" + failoVardas + "\".";
        return false;
    }

    string eilute;
    if (!getline(fin, eilute)) {
        klaida = "Duomenu failas tuscias.";
        return false;
    }
    int kiek;
    if (!tekstasIInt(apkarpyti(eilute), kiek)) {
        klaida = "Pirmoje eiluteje turi buti prekiu irasu skaicius (sveikasis skaicius).";
        return false;
    }
    if (kiek < 1 || kiek > MAX_PREKIU) {
        klaida = "Irasu skaicius turi buti nuo 1 iki " + to_string(MAX_PREKIU) +
                 ", failo reiksme: " + to_string(kiek) + ".";
        return false;
    }

    for (int i = 0; i < kiek; i++) {
        if (!getline(fin, eilute)) {
            klaida = "Nurodyta " + to_string(kiek) + " irasu, bet nuskaityta tik " +
                     to_string(i) + ".";
            return false;
        }
        string eilNr = to_string(i + 2);
        string laukai[4];
        int laukuSk = 0;
        stringstream ss(eilute);
        string laukas;
        while (getline(ss, laukas, ';')) {
            if (laukuSk < 4) laukai[laukuSk] = apkarpyti(laukas);
            laukuSk++;
        }
        if (laukuSk != 4) {
            klaida = "Eilute " + eilNr + ": turi buti 4 laukai, atskirti kabliataskiais.";
            return false;
        }
        if (!arTinkamaKategorija(laukai[0], leidziamosKat)) {
            klaida = "Eilute " + eilNr + ": netinkama kategorija \"" + laukai[0] + "\".";
            return false;
        }
        if (laukai[1].empty()) {
            klaida = "Eilute " + eilNr + ": truksta prekes pavadinimo.";
            return false;
        }
        double kaina;
        if (!tekstasIDouble(laukai[2], kaina) || kaina <= 0) {
            klaida = "Eilute " + eilNr + ": vieneto kaina turi buti teigiamas skaicius.";
            return false;
        }
        int kiekis;
        if (!tekstasIInt(laukai[3], kiekis) || kiekis <= 0) {
            klaida = "Eilute " + eilNr + ": kiekis turi buti teigiamas sveikasis skaicius.";
            return false;
        }
        kategorijos[i] = laukai[0];
        pavadinimai[i] = laukai[1];
        kainos[i] = kaina;
        kiekiai[i] = kiekis;
        n++;
    }
    return true;
}

double eilutesSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}

double skaiciuotiNuolaida(double suma, int kiekis) {
    if (kiekis >= NUOLAIDOS_RIBA) return suma * NUOLAIDOS_DALIS;
    return 0.0;
}

double eilutesMoketina(double kaina, int kiekis) {
    double suma = eilutesSuma(kaina, kiekis);
    return suma - skaiciuotiNuolaida(suma, kiekis);
}

double kategorijosSuma(const string& kategorija, const string kategorijos[],
                       const double kainos[], const int kiekiai[], int n) {
    double suma = 0.0;
    for (int i = 0; i < n; i++)
        if (kategorijos[i] == kategorija)
            suma += eilutesMoketina(kainos[i], kiekiai[i]);
    return suma;
}

double bendraSumaPriesNuolaidas(const double kainos[], const int kiekiai[], int n) {
    double suma = 0.0;
    for (int i = 0; i < n; i++) suma += eilutesSuma(kainos[i], kiekiai[i]);
    return suma;
}

double bendraNuolaida(const double kainos[], const int kiekiai[], int n) {
    double suma = 0.0;
    for (int i = 0; i < n; i++)
        suma += skaiciuotiNuolaida(eilutesSuma(kainos[i], kiekiai[i]), kiekiai[i]);
    return suma;
}

void spausdintiLenteleAntraste(ostream& out) {
    out << left << setw(4) << "Nr." << setw(12) << "Kategorija"
        << setw(48) << "Pavadinimas"
        << right << setw(9) << "Kaina" << setw(7) << "Kiekis"
        << setw(11) << "Suma" << setw(10) << "Nuolaida"
        << setw(11) << "Moketina" << "\n";
    out << string(112, '-') << "\n";
}

void spausdintiPreke(ostream& out, int nr, const string& kat, const string& pav,
                     double kaina, int kiekis) {
    double suma = eilutesSuma(kaina, kiekis);
    double nuol = skaiciuotiNuolaida(suma, kiekis);
    out << left << setw(4) << nr << setw(12) << kat << setw(48) << pav
        << right << setw(9) << kaina << setw(7) << kiekis << setw(11) << suma
        << setw(10) << nuol << setw(11) << (suma - nuol) << "\n";
}

void spausdintiPrekes(ostream& out, const string& kategorija, const string kategorijos[],
                      const string pavadinimai[], const double kainos[],
                      const int kiekiai[], int n) {
    out << fixed << setprecision(2);
    spausdintiLenteleAntraste(out);
    int rodoma = 0;
    for (int i = 0; i < n; i++) {
        if (kategorija.empty() || kategorijos[i] == kategorija) {
            rodoma++;
            spausdintiPreke(out, i + 1, kategorijos[i], pavadinimai[i], kainos[i], kiekiai[i]);
        }
    }
    if (rodoma == 0) out << "Pasirinktos kategorijos prekiu uzsakyme nera.\n";
}

void spausdintiBendrasSumas(ostream& out, const double kainos[], const int kiekiai[], int n) {
    double pries = bendraSumaPriesNuolaidas(kainos, kiekiai, n);
    double nuol = bendraNuolaida(kainos, kiekiai, n);
    out << fixed << setprecision(2) << left;
    out << setw(32) << "Uzsakymo suma pries nuolaidas:" << pries << " Eur\n";
    out << setw(32) << "Bendra nuolaidu suma:" << nuol << " Eur\n";
    out << setw(32) << "Galutine moketina suma:" << (pries - nuol) << " Eur\n";
}

void spausdintiKategorijuSumas(ostream& out, const string leidziamosKat[],
                               const string kategorijos[], const double kainos[],
                               const int kiekiai[], int n) {
    out << fixed << setprecision(2);
    for (int k = 0; k < KATEGORIJU_SK; k++)
        out << left << setw(12) << leidziamosKat[k] << ": " << right << setw(10)
            << kategorijosSuma(leidziamosKat[k], kategorijos, kainos, kiekiai, n)
            << " Eur\n";
}

void spausdintiAtaskaita(ostream& out, const string leidziamosKat[], const string kategorijos[],
                         const string pavadinimai[], const double kainos[],
                         const int kiekiai[], int n) {
    out << "=== DVIRACIU PARDUOTUVES UZSAKYMO ATASKAITA ===\n\n";
    spausdintiPrekes(out, "", kategorijos, pavadinimai, kainos, kiekiai, n);
    out << "\n--- Viso uzsakymo sumos ---\n";
    spausdintiBendrasSumas(out, kainos, kiekiai, n);
    out << "\n--- Sumos pagal kategorijas (po nuolaidu) ---\n";
    spausdintiKategorijuSumas(out, leidziamosKat, kategorijos, kainos, kiekiai, n);
}

bool issaugotiAtaskaita(const string& failoVardas, const string leidziamosKat[],
                        const string kategorijos[], const string pavadinimai[],
                        const double kainos[], const int kiekiai[], int n) {
    ofstream fout(failoVardas.c_str(), ios::trunc);
    if (!fout.is_open()) return false;
    spausdintiAtaskaita(fout, leidziamosKat, kategorijos, pavadinimai, kainos, kiekiai, n);
    fout.flush();
    if (!fout.good()) return false;
    fout.close();
    return !fout.fail();
}

void spausdintiMeniu() {
    cout << "\n===== MENIU =====\n"
         << "1. Perziureti visas uzsakymo prekes\n"
         << "2. Perziureti pasirinktos kategorijos prekes\n"
         << "3. Perziureti viso uzsakymo sumas\n"
         << "4. Perziureti sumas pagal kategorijas\n"
         << "5. Parodyti visa uzsakymo ataskaita\n"
         << "6. Issaugoti visa ataskaita i faila\n"
         << "7. Iseiti\n";
}

int skaitytiPasirinkima(const string& kvietimas, int min, int max) {
    string eil;
    while (true) {
        cout << kvietimas;
        if (!getline(cin, eil)) return max;
        int v;
        if (tekstasIInt(apkarpyti(eil), v) && v >= min && v <= max) return v;
        cout << "Netinkamas pasirinkimas. Iveskite skaiciu nuo " << min << " iki " << max << ".\n";
    }
}

int main() {
    const string leidziamosKat[KATEGORIJU_SK] = {"Dviraciai", "Dalys", "Aksesuarai"};

    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];
    int n = 0;
    string klaida;

    if (!nuskaitytiDuomenis("uzsakymas.txt", kategorijos, pavadinimai, kainos, kiekiai,
                            n, leidziamosKat, klaida)) {
        cerr << "KLAIDA: " << klaida << "\n";
        cerr << "Programa baigia darba, ataskaita nesudaroma.\n";
        return 1;
    }
    cout << "Sekmingai nuskaityta irasu: " << n << "\n";

    int pasirinkimas;
    do {
        spausdintiMeniu();
        pasirinkimas = skaitytiPasirinkima("Jusu pasirinkimas (1-7): ", 1, 7);
        cout << "\n";
        switch (pasirinkimas) {
            case 1:
                spausdintiPrekes(cout, "", kategorijos, pavadinimai, kainos, kiekiai, n);
                break;
            case 2: {
                cout << "Kategorijos:\n";
                for (int k = 0; k < KATEGORIJU_SK; k++)
                    cout << "  " << (k + 1) << ". " << leidziamosKat[k] << "\n";
                int k = skaitytiPasirinkima("Pasirinkite kategorija (1-3): ", 1, KATEGORIJU_SK) - 1;
                cout << "\n";
                spausdintiPrekes(cout, leidziamosKat[k], kategorijos, pavadinimai, kainos, kiekiai, n);
                cout << fixed << setprecision(2) << "\nKategorijos \""
                     << leidziamosKat[k] << "\" moketina suma: "
                     << kategorijosSuma(leidziamosKat[k], kategorijos, kainos, kiekiai, n)
                     << " Eur\n";
                break;
            }
            case 3:
                spausdintiBendrasSumas(cout, kainos, kiekiai, n);
                break;
            case 4:
                spausdintiKategorijuSumas(cout, leidziamosKat, kategorijos, kainos, kiekiai, n);
                break;
            case 5:
                spausdintiAtaskaita(cout, leidziamosKat, kategorijos, pavadinimai, kainos, kiekiai, n);
                break;
            case 6:
                if (issaugotiAtaskaita("ataskaita.txt", leidziamosKat, kategorijos,
                                       pavadinimai, kainos, kiekiai, n))
                    cout << "Ataskaita sekmingai issaugota faile ataskaita.txt\n";
                else
                    cerr << "KLAIDA: nepavyko irasyti ataskaitos i faila ataskaita.txt\n";
                break;
            case 7:
                cout << "Viso gero!\n";
                break;
        }
    } while (pasirinkimas != 7);

    return 0;
}