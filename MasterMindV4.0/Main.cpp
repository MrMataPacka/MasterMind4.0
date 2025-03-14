#include <iostream>
#include <string>
#include "MasterMindEngine.h"

using namespace MasterMindEngine;
using namespace std;

const char TECLA_1 = '1';
const char TECLA_2 = '2';
const char TECLA_0 = '0';

char key;
bool invalidKey;
TPartida juego;

bool readKey(char& k) {
    string entrada;
    getline(cin, entrada);
    if (entrada.length() != 1) {
        return false;
    }
    else if ((entrada[0] != '1') && (entrada[0] != '2') && (entrada[0] != '0')) {
        return false;
    }
    else {
        k = entrada[0];
        return true;
    }
}

void modoAdivinador(TPartida& juego) {
    string mensaje;
    int maxFOR;
    short int buenos, regulares;

    mensaje = "Escribe tu codigo y presiona ENTER.";

    iniciarJuego(ADIVINADOR, juego);
    generarCodigo(juego.pensador);

    cout << "|----------------------------- MODO ADIVINADOR ------------------------------|\n";
    cout << "Tienes un total de " << MAX_INTENTOS << " intentos para adivinar.\n\n";
    do {
        cout << juego.intentoActual + 1 << ") Escribe tu codigo >> ";
        while (!leerCodigo(juego.adivinador, mensaje)) {
            cout << mensaje << "\nEscribe tu codigo >> ";
        }

        calcularNota(juego.adivinador, juego.pensador, buenos, regulares);

        cout << "Buenos= " << buenos << " | Regulares= " << regulares << "\n";

        presentarCodigo(juego.adivinador, buenos, regulares, juego);
    } while (juego.estado == INICIADO);

    if (juego.estado == GANO) {
        cout << "*****************\n|   GANASTE!!!   |\n*****************\n";
    }
    else {
        cout << "*****************\n|   PERDISTE!!!  |\n*****************\n";
        cout << "El codigo era: ";
        imprimirCodigo(juego.pensador);
        cout << "\n\n";
    }
}

void modoPensador(TPartida& juego) {
    TCodigo firstCode;
    bool firstAgain;
    short int buenos, regulares;
    string mensaje;

    iniciarJuego(PENSADOR, juego);
    generarCodigo(juego.adivinador);
    firstCode = juego.adivinador;
    firstAgain = false;

    cout << "|----------------------------- MODO PENSADOR ------------------------------|\n";
    cout << "Queda un total de " << MAX_INTENTOS << " intentos para adivinar. Escribe las notas [B R].\n\n";
    do {
        while (!esAdecuado(juego.adivinador, juego.historial) && (!firstAgain)) {
            siguienteCodigo(juego.adivinador);
            firstAgain = sonIguales(firstCode, juego.adivinador);
        }

        if (firstAgain) {
            juego.estado = TRAMPA;
            break;
        }

        cout << juego.intentoActual + 1 << ") El codigo podria ser: ";
        imprimirCodigo(juego.adivinador);
        cout << "... >> ";

        while (!leerNotas(buenos, regulares, mensaje)) {
            cout << mensaje << "\nIngresa nuevamente >> ";
        }

        guardarNota(juego.historial, juego.adivinador, buenos, regulares);

        presentarCodigo(juego.adivinador, buenos, regulares, juego);
    } while (juego.estado == INICIADO);

    if (juego.estado == GANO) {
        cout << "*****************\n|  TE VENCI!!!  |\n*****************\n";
    }
    else if (juego.estado == PERDIO) {
        cout << "*****************\n|   GANASTE!!!  |\n*****************\n";
    }
    else {
        cout << "*****************\n|   TRAMPA!!!   |\n*****************\n";
        cout << "El codigo: "; imprimirCodigo(juego.pensador);
    }
}


void mainMenu(TPartida& juego) {
    do {
        cout << "******************************************************************************\n";
        cout << "                          MasterMind V4.0 - KA EduSoft                        \n";
        cout << "******************************************************************************\n\n";
        cout << "                     ***  Selecciona el modo de juego ***                     \n\n";
        cout << "                                1 - ADIVINADOR                                \n";
        cout << "                                2 - PENSADOR                                  \n";
        cout << "                                0 - SALIR                                     \n\n";
        cout << ">> ";
        while (!readKey(key)) {
            cout << "ERROR. Ingresa una opcion valida >> ";
        }

        switch (key) {
        case TECLA_1:
            modoAdivinador(juego);
            break;
        case TECLA_2:
            modoPensador(juego);
            break;
        case TECLA_0:
            cout << "Finalizando partida.";
            break;
        }

    } while (key != TECLA_0);

}



int main() {
    mainMenu(juego);
}