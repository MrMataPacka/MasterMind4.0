#pragma once
#include <string>

using namespace std;

namespace MasterMindAux {
	string IntToStr(int x);
	bool existeEspacio(string entrada, string& errorMessage);
}

namespace MasterMindEngine {


	const int MAX_INTENTOS = 10;
	const char LARGO_CODIGO = 4;
	const char PRIMERA_LETRA = 'A';
	const char ULTIMA_LETRA = 'H';

	struct TCodigo {
		char codigo[LARGO_CODIGO];
	};


	struct TRegistroNota {
		TCodigo codigo;
		short int buenos, regulares;
	};


	struct THistoria {
		TRegistroNota info[MAX_INTENTOS];
		short int tope;
	};


	enum TModoJuego { ADIVINADOR, PENSADOR };


	enum TEstadoJuego { INICIADO, GANO, PERDIO, TRAMPA };


	struct TPartida {
		THistoria historial;
		unsigned int intentoActual;
		TCodigo adivinador, pensador;
		TModoJuego modo;
		TEstadoJuego estado;
	};

	void generarCodigo(TCodigo& c);

	bool leerCodigo(TCodigo& c, string& errorMessage);


	bool leerNotas(short int& b, short int& r, string& errorMessage);


	void calcularNota(const TCodigo& codAdivinador, const TCodigo& codPensador, short int& buenos, short int& regulares);


	void imprimirCodigo(const TCodigo& c);


	void siguienteCodigo(TCodigo& c);

	THistoria crearHistoria();


	bool esHistoriaVacia(const THistoria& h);


	bool esHistoriaLlena(const THistoria& h);

	void guardarNota(THistoria& h, const TCodigo& c, short int buenos, short int regulares);


	bool esAdecuado(const TCodigo& c, const THistoria& h);

	void iniciarJuego(TModoJuego modoPartida, TPartida& j);

	bool sonIguales(TCodigo& c1, TCodigo& c2);

	void presentarCodigo(TCodigo codigo, short int b, short int r, TPartida& j);

}