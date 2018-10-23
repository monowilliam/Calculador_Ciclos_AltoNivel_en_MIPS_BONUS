#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <stdlib.h>
using namespace std;


vector<string> fila;
vector<vector<string> > matriz;
int contadorPipe=4;

vector<string> split_iterator(const string& str){
	vector<string> resultado;
	string::const_iterator itBegin = str.begin();
	string::const_iterator itEnd   = str.end();
	int numItems = 1;
	for( string::const_iterator it = itBegin; it!=itEnd; ++it )
		numItems += *it==' ';
	resultado.reserve(numItems);
	for( string::const_iterator it = itBegin; it!=itEnd; ++it ){        //Divide un texto en varias palabras
		if( *it == ' ' or *it ==','){
	    	resultado.push_back(string(itBegin,it));
	      	itBegin = it+1;
		}
	}
	if( itBegin != itEnd )
		resultado.push_back(string(itBegin,itEnd));
	return resultado;
}


void abrirArchivo(string codigo){
	string linea;
	ifstream a;
	a.open(codigo.c_str());
	while(!a.eof()){
		getline(a,linea);
		for(int i=0; i<split_iterator(linea).size();i++){
			if (split_iterator(linea)[i]!="")
				fila.push_back(split_iterator(linea)[i]);	
		}
		matriz.push_back(fila);	
		fila.clear();	
	}
	a.close();
}


bool etiquetaComentario(int a){
	string palabra = matriz[a][0];
	if (palabra.find(":") != std::string::npos || palabra.find("#") != std::string::npos)
    	return true;
	return false;
}


void contadorDeCiclosPipe(int i){
    if(matriz[i][0] != "lw" and etiquetaComentario(i)!=true)
    	contadorPipe++;
}


int main(void){
	abrirArchivo("codigo.asm");
	
	for (int i = 0; i<matriz.size(); i++) { 
		contadorDeCiclosPipe(i); cout << contadorPipe <<"\t";
        for (int j = 0; j<matriz[i].size(); j++) 
            cout << matriz[i][j] << " "; 
        cout << endl; 
    }
    
    cout << etiquetaComentario(3);
	
	return 0;
}
