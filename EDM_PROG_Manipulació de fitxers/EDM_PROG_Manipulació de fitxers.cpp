#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Definició d'una estructura per a l'encapçalament GIF
struct CapcaleraGIF {
    char firma[3]; // Signatura del GIF
    char versio[3]; // Versió del GIF
};

// Definició d'una estructura per a la descripció de la pantalla lògica
struct DescPantalla {
    uint16_t amplada; // Amplada de la imatge
    uint16_t alcada; // Alçada de la imatge
    uint8_t campsEmpaquetats; // Camps empaquetats
    uint8_t indexColorFons; // Índex del color de fons
    uint8_t relAspectePixel; // Relació d'aspecte del píxel
};

// Funció per mostrar la informació del GIF
void mostrarInformacio(const CapcaleraGIF& capcalera, const DescPantalla& descPantalla, int midaTaulaColors) {
    cout << "Versio: " << capcalera.versio[0] <<capcalera.versio[1] << capcalera.versio[2]<< endl;
    cout << "Amplada de la imatge: " << descPantalla.amplada << " pixels" << endl;
    cout << "Alçada de la imatge: " << descPantalla.alcada << " pixels" << endl;
    cout << "Quantitat de colors: " << (1 << ((descPantalla.campsEmpaquetats & 0x07) + 1)) << endl;
}

// Funció per duplicar un fitxer GIF
void duplicarGIF(const char* nomFitxerEntrada, const char* nomFitxerSortida) {
    ifstream fitxerEntrada(nomFitxerEntrada, ios::binary); // Fitxer d'entrada en mode binari
    ofstream fitxerSortida(nomFitxerSortida, ios::binary); // Fitxer de sortida en mode binari

    if (!fitxerEntrada || !fitxerSortida) { // Comprovació d'errors en la obertura dels fitxers
        cerr << "Error: No s'ha pogut obrir el fitxer." << endl;
        return;
    }

    vector<char> buffer(4096); // 4 KB de memòria
    while (!fitxerEntrada.eof()) { // Lectura fins al final del fitxer
        fitxerEntrada.read(buffer.data(), buffer.size()); // Lectura del fitxer d'entrada al buffer
        fitxerSortida.write(buffer.data(), fitxerEntrada.gcount()); // Escriure del buffer al fitxer de sortida
    }

    cout << "Fitxer duplicat amb exit." << endl;
}

int main() {
    string nomFitxer;
    cout << "Introdueix el nom del fitxer GIF: ";
    cin >> nomFitxer;

    ifstream fitxer(nomFitxer, ios::binary); // Obertura del fitxer en mode binari
    if (!fitxer) {
        cerr << "No s'ha pogut obrir el fitxer." << endl;
        return 1;
    }

    CapcaleraGIF capcalera;
    fitxer.read(reinterpret_cast<char*>(&capcalera), sizeof(CapcaleraGIF)); // Lectura de l'encapçalament del GIF

    if (strncmp(capcalera.firma, "GIF", 3) != 0) { // Comprovació de la signatura GIF vàlida
        cerr << "El fitxer no te un format GIF valid." << endl;
        return 1;
    }

    DescPantalla descPantalla;
    fitxer.read(reinterpret_cast<char*>(&descPantalla), sizeof(DescPantalla)); // Lectura de la descripció de la pantalla lògica

    int midaTaulaColors = (1 << ((descPantalla.campsEmpaquetats & 0x07) + 1)); // Càlcul de la grandària de la taula de colors

    mostrarInformacio(capcalera, descPantalla, midaTaulaColors); // Mostra la informació del GIF

    char opcio;
    cout << "Vols duplicar el fitxer GIF? (S/N): ";
    cin >> opcio;

    if (opcio == 's' || opcio == 'S') { // Demana si es vol duplicar el fitxer
        string nomFitxerSortida;
        cout << "Introdueix el nom del fitxer de sortida: ";
        cin >> nomFitxerSortida;
        duplicarGIF(nomFitxer.c_str(), nomFitxerSortida.c_str()); // Duplica el fitxer GIF
    }

    return 0;
}
