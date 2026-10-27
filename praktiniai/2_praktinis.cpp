#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main() {

    const int MAX_MOKINIU = 100;
    const int MAX_PAZYMIU = 10;

    string vardai[MAX_MOKINIU];
    int pazymiai[MAX_MOKINIU][MAX_PAZYMIU];
    int pazymiuSkaicius[MAX_MOKINIU];
    int mokiniuSkaicius = 0;

    int meniuPasirinkimas;
    int mokinioNr;
    int m;
    int pazymioNr;
    int pazymys;
    int kiekis;
    int suma;
    double vidurkis;

    cout << fixed << setprecision(2);

    do {
        cout << endl << "--- Mokiniu pazymiu sistema ---" << endl;
        cout << "1. Ivesti mokini ir jo pazymius" << endl;
        cout << "2. Perziureti visu mokiniu pazymius" << endl;
        cout << "3. Perziureti konkretaus mokinio pazymius" << endl;
        cout << "4. Atnaujinti pazymi" << endl;
        cout << "5. Apskaiciuoti mokinio vidurki" << endl;
        cout << "6. Pasalinti mokini" << endl;
        cout << "7. Iseiti" << endl;
        cout << "Pasirinkite skaiciu: ";
        cin >> meniuPasirinkimas;

        switch (meniuPasirinkimas) {
            case 1:
                if (mokiniuSkaicius >= MAX_MOKINIU) {
                    cout << "Pasiektas mokiniu limitas (" << MAX_MOKINIU << ")." << endl;
                }
                else {
                    cout << "Iveskite mokinio varda: ";
                    cin >> vardai[mokiniuSkaicius];

                    cout << "Kiek pazymiu ivesite (0-" << MAX_PAZYMIU << "): ";
                    cin >> kiekis;
                    while (kiekis < 0 || kiekis > MAX_PAZYMIU) {
                        cout << "Neteisingas kiekis. Iveskite skaiciu nuo 0 iki " << MAX_PAZYMIU << ": ";
                        cin >> kiekis;
                    }

                    for (int i = 0; i < kiekis; i++) {
                        cout << "Iveskite " << i + 1 << " pazymi: ";
                        cin >> pazymys;
                        while (pazymys < 1 || pazymys > 10) {
                            cout << "Pazymys turi buti nuo 1 iki 10. Bandykite dar karta: ";
                            cin >> pazymys;
                        }
                        pazymiai[mokiniuSkaicius][i] = pazymys;
                    }

                    pazymiuSkaicius[mokiniuSkaicius] = kiekis;
                    mokiniuSkaicius++;
                    cout << "Mokinys pridetas." << endl;
                }
                break;

            case 2:
                if (mokiniuSkaicius == 0) {
                    cout << "Sarasas tuscias. Pirmiausia ivesite mokini." << endl;
                }
                else {
                    cout << endl << "Nr.  Vardas          Pazymiai" << endl;
                    cout << "-------------------------------------" << endl;
                    for (int i = 0; i < mokiniuSkaicius; i++) {
                        cout << i + 1 << ".   " << vardai[i] << "   ";
                        if (pazymiuSkaicius[i] == 0) {
                            cout << "(pazymiu nera)";
                        }
                        for (int j = 0; j < pazymiuSkaicius[i]; j++) {
                            cout << pazymiai[i][j] << " ";
                        }
                        cout << endl;
                    }
                }
                break;

            case 3:
                if (mokiniuSkaicius == 0) {
                    cout << "Sarasas tuscias. Pirmiausia ivesite mokini." << endl;
                }
                else {
                    cout << "Iveskite mokinio numeri (1-" << mokiniuSkaicius << "): ";
                    cin >> mokinioNr;

                    if (mokinioNr < 1 || mokinioNr > mokiniuSkaicius) {
                        cout << "Neteisingas mokinio numeris. Iveskite skaiciu nuo 1 iki " << mokiniuSkaicius << "." << endl;
                    }
                    else {
                        m = mokinioNr - 1;
                        cout << endl << vardai[m] << ": ";
                        if (pazymiuSkaicius[m] == 0) {
                            cout << "pazymiu nera";
                        }
                        for (int j = 0; j < pazymiuSkaicius[m]; j++) {
                            cout << pazymiai[m][j] << " ";
                        }
                        cout << endl;
                    }
                }
                break;

            case 4:
                if (mokiniuSkaicius == 0) {
                    cout << "Sarasas tuscias. Pirmiausia ivesite mokini." << endl;
                }
                else {
                    cout << "Iveskite mokinio numeri (1-" << mokiniuSkaicius << "): ";
                    cin >> mokinioNr;

                    if (mokinioNr < 1 || mokinioNr > mokiniuSkaicius) {
                        cout << "Neteisingas mokinio numeris. Iveskite skaiciu nuo 1 iki " << mokiniuSkaicius << "." << endl;
                    }
                    else {
                        m = mokinioNr - 1;
                        if (pazymiuSkaicius[m] == 0) {
                            cout << "Sis mokinys pazymiu neturi." << endl;
                        }
                        else {
                            cout << "Iveskite pazymio numeri (1-" << pazymiuSkaicius[m] << "): ";
                            cin >> pazymioNr;

                            if (pazymioNr < 1 || pazymioNr > pazymiuSkaicius[m]) {
                                cout << "Neteisingas pazymio numeris. Iveskite skaiciu nuo 1 iki " << pazymiuSkaicius[m] << "." << endl;
                            }
                            else {
                                cout << "Iveskite nauja pazymi (1-10): ";
                                cin >> pazymys;
                                while (pazymys < 1 || pazymys > 10) {
                                    cout << "Pazymys turi buti nuo 1 iki 10. Bandykite dar karta: ";
                                    cin >> pazymys;
                                }
                                pazymiai[m][pazymioNr - 1] = pazymys;
                                cout << "Pazymys atnaujintas." << endl;
                            }
                        }
                    }
                }
                break;

            case 5:
                if (mokiniuSkaicius == 0) {
                    cout << "Sarasas tuscias. Pirmiausia ivesite mokini." << endl;
                }
                else {
                    cout << "Iveskite mokinio numeri (1-" << mokiniuSkaicius << "): ";
                    cin >> mokinioNr;

                    if (mokinioNr < 1 || mokinioNr > mokiniuSkaicius) {
                        cout << "Neteisingas mokinio numeris. Iveskite skaiciu nuo 1 iki " << mokiniuSkaicius << "." << endl;
                    }
                    else {
                        m = mokinioNr - 1;
                        if (pazymiuSkaicius[m] == 0) {
                            cout << "Sis mokinys pazymiu neturi, vidurkio nera." << endl;
                        }
                        else {
                            suma = 0;
                            for (int j = 0; j < pazymiuSkaicius[m]; j++) {
                                suma += pazymiai[m][j];
                            }
                            vidurkis = (double)suma / pazymiuSkaicius[m];
                            cout << vardai[m] << " pazymiu vidurkis: " << vidurkis << endl;
                        }
                    }
                }
                break;

            case 6:
                if (mokiniuSkaicius == 0) {
                    cout << "Sarasas tuscias. Pirmiausia ivesite mokini." << endl;
                }
                else {
                    cout << "Iveskite mokinio numeri (1-" << mokiniuSkaicius << "): ";
                    cin >> mokinioNr;

                    if (mokinioNr < 1 || mokinioNr > mokiniuSkaicius) {
                        cout << "Neteisingas mokinio numeris. Iveskite skaiciu nuo 1 iki " << mokiniuSkaicius << "." << endl;
                    }
                    else {
                        m = mokinioNr - 1;
                        for (int i = m; i < mokiniuSkaicius - 1; i++) {
                            vardai[i] = vardai[i + 1];
                            pazymiuSkaicius[i] = pazymiuSkaicius[i + 1];
                            for (int j = 0; j < MAX_PAZYMIU; j++) {
                                pazymiai[i][j] = pazymiai[i + 1][j];
                            }
                        }
                        mokiniuSkaicius--;
                        cout << "Mokinys pasalintas." << endl;
                    }
                }
                break;

            case 7:
                cout << "Programa baige darba." << endl;
                break;

            default:
                cout << "Tokio pasirinkimo nera." << endl;
                break;
        }

    } while (meniuPasirinkimas != 7);

    return 0;
}