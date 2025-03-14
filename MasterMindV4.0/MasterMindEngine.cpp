#include "MasterMindEngine.h"
#include <iostream>
#include <stdio.h>   
#include <stdlib.h>     
#include <time.h>
#include <string>
#include <sstream>

using namespace std;

namespace MasterMindAux {

	using namespace MasterMindEngine;

	string IntToStr(int x) {

		stringstream stream;

		stream << x;

		string xStr;

		stream >> xStr;

		return xStr;
	}

	bool existeEspacio(string entrada, string& errorMessage) {
		for (int i = 0; i < entrada.length();i++) {
			if (entrada[i] == ' ') {
				return true;
			}
		}
		errorMessage = "ERROR: Ingresa solo dos numeros enteros entre 0 y " +
			IntToStr(LARGO_CODIGO) + " separados por un espacio en blanco.\n";
		cout << errorMessage;
		return false;
	}


}

namespace MasterMindEngine {

	using namespace MasterMindAux;

	void generarCodigo(TCodigo& c) {
		srand(time(NULL));
		for (int i = 0; i < LARGO_CODIGO; i++) {
			c.codigo[i] = rand() % (ULTIMA_LETRA - PRIMERA_LETRA + 1) + PRIMERA_LETRA;
		}
	}

	bool leerCodigo(TCodigo& c, string& errorMessage) {
		string codigo;
		cin >> codigo;
		int contadorLargo = 0;
		
		for (int i = 0; i < codigo.length(); i++) {
			if (codigo[i] < 'A' || codigo[i] > 'H') {
				errorMessage = "Solo puedes ingresar letras entre " + string(1, PRIMERA_LETRA) + " y " + string(1, ULTIMA_LETRA) + ".\n";
				return false;
			}
			contadorLargo++;
			if (contadorLargo > 4) {
				errorMessage = "El largo del codigo debe ser de " + IntToStr(LARGO_CODIGO) + " letras.\n";
				return false;
			}
		}
		for (int i = 0; i < LARGO_CODIGO; i++) {
			c.codigo[i] = codigo[i];
		}
		return true;
	}

	bool leerNotas(short int& b, short int& r, string& errorMessage) {
		string entrada;
		string puntosBuenos;
		string puntosRegulares;
		int contadorIndex = 0;
		getline(cin, entrada);

		if (!existeEspacio(entrada, errorMessage)) {
			b = 0;
			r = 0;
			return false;
		}

		for (int i = 0; i < entrada.length();i++) {
			if (isdigit(entrada[i]) && (entrada[i] - '0' >= 0 && entrada[i] - '0' <= LARGO_CODIGO)) {
				puntosBuenos.push_back(entrada[i]);
			}
			else if (entrada[i] == ' ') {
				contadorIndex = i + 1;
				break;
			}
			else {
				b = 0;
				r = 0;
				errorMessage = "ERROR: Ingresa solo dos numeros enteros entre 0 y " +
					IntToStr(LARGO_CODIGO) + " separados por un espacio en blanco.\n";
				cout << errorMessage;
				return false;
			}
		}

		for (int i = contadorIndex; i < entrada.length();i++) {
			if (isdigit(entrada[i]) && (entrada[i] - '0' >= 0 && entrada[i] - '0' <= LARGO_CODIGO)) {
				puntosRegulares.push_back(entrada[i]);
			}
			else {
				b = 0;
				r = 0;
				errorMessage = "ERROR: Ingresa solo dos numeros enteros entre 0 y " +
					IntToStr(LARGO_CODIGO) + " separados por un espacio en blanco.\n";
				cout << errorMessage;
				return false;
			}
		}

		b = stoi(puntosBuenos);
		r = stoi(puntosRegulares);

		if (b + r > LARGO_CODIGO || (b == (LARGO_CODIGO - 1) && r >= 1)) {
			errorMessage = "ERROR: Las notas no son correctas, por favor verifica los  valores. \n";
			cout << errorMessage;
			return false;
		}
		return true;


	}

	void calcularNota(const TCodigo& codAdivinador, const TCodigo& codPensador,
		short int& buenos, short int& regulares) {

		buenos = 0;
		regulares = 0;

		bool usadoPensador[LARGO_CODIGO]{ false },
			usadoAdivinador[LARGO_CODIGO] = { false };


		for (int j = 0; j < LARGO_CODIGO;j++) {
			if (codAdivinador.codigo[j] == codPensador.codigo[j]) {
				buenos++;
				usadoPensador[j] = true;
				usadoAdivinador[j] = true;
			}
		}

		for (int j = 0; j < LARGO_CODIGO;j++) {
			if (!usadoAdivinador[j]) {
				for (int k = 0; k < LARGO_CODIGO;k++) {
					if (!usadoPensador[k] && codAdivinador.codigo[j] == codPensador.codigo[k]) {
						regulares++;
						usadoPensador[k] = true;
						break;
					}
				}
			}
		}
	}

	void imprimirCodigo(const TCodigo& c) {
		for (int i = 0; i < LARGO_CODIGO; i++) {
			cout << c.codigo[i];
		}
	}

	

	void siguienteCodigo(TCodigo& c) {
		for (int i = LARGO_CODIGO - 1; i >= 0; i--) {
			if (c.codigo[i] == 'H') {
				c.codigo[i] = 'A';
			}
			else {
				c.codigo[i] = c.codigo[i] + 1;
				break;
			}
		}
	}

	THistoria crearHistoria() {
		THistoria historiaVacia;
		historiaVacia.tope = -1;
		return historiaVacia;
	}

	bool esHistoriaVacia(const THistoria& h) {
		if (h.tope == -1) {
			return true;
		}
		return false;
	}

	bool esHistoriaLlena(const THistoria& h) {
		if (h.tope == MAX_INTENTOS - 1) {
			return true;
		}
		return false;
	}

	void guardarNota(THistoria& h, const TCodigo& c, short int buenos, short int
		regulares) {
		if (!esHistoriaLlena(h)) {
			h.tope++;
			h.info[h.tope].buenos = buenos;
			h.info[h.tope].regulares = regulares;
			h.info[h.tope].codigo = c;
		}
	}

	bool esAdecuado(const TCodigo& c, const THistoria& h) {
		short int buenos=0;
		short int regulares=0;
		for (int i = 0; i <= h.tope; i++) {
			buenos = 0;
			regulares = 0;
			calcularNota(c, h.info[i].codigo, buenos, regulares);
			if (buenos != h.info[i].buenos || regulares != h.info[i].regulares) {
				return false;
			}
		}
		return true;
	}

	void iniciarJuego(TModoJuego modoPartida, TPartida& j) {
		j.historial = crearHistoria();
		j.intentoActual = 0;
		j.modo = modoPartida;
		j.estado = INICIADO;
	}

	bool sonIguales(TCodigo& c1, TCodigo& c2) {
		for (int i = 0; i < LARGO_CODIGO; i++) {
			if (c2.codigo[i] != c1.codigo[i]) {
				return false;
			}
		}
		return true;
	}

	void presentarCodigo(TCodigo codigo, short int b, short int r, TPartida& j) {
		cout << "El codigo ";
		imprimirCodigo(codigo);
		cout << " obtuvo " << b << " puntos buenos y " << r << " puntos regulares \n";
		
		if (b == LARGO_CODIGO) {
			j.estado = GANO;
		}
		else if (j.intentoActual<MAX_INTENTOS-1) {
			j.intentoActual++;
			j.estado = INICIADO;
		}
		else {
			j.estado = PERDIO;
		}

		
		
	}
}