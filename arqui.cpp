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

vector<string>nombreRegistro;
vector<int>valorRegistro;
int banderaCiclo1=0;
int banderaCiclo2=0;
int posBandera;
int indice;


//Funcion de Split donde identifica y separa las palabras entre espacios y elimina comas
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


//Abre el archivo, quita comentarios y crea una matriz de vectores.
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
		//elimina los comentarios de todo el codigo
		for(int i=0; i<fila.size(); i++){ 
			string a=fila[i];
			if(a.find("#") != std::string::npos)
				fila.erase(fila.begin()+i,fila.begin()+fila.size());
		}
		matriz.push_back(fila);	
		fila.clear();	
	}
	a.close();
}


//Busca y guarda en dos vectores donde se inicialzar los vectores y con que valor quedan.
void guardarRegistros(){
	for(int i=0; i<matriz.size(); i++){
		for (int j = 0; j<matriz[i].size(); j++){
			string palabra = matriz[i][j];
			if (palabra.find("add") != std::string::npos ){
				string registro = matriz[i][1];
				string op1 = matriz[i][2];
				string op2 = matriz[i][3];
				int operando1 = atoi(op1.c_str());
				int operando2 = atoi(op2.c_str());
				nombreRegistro.push_back(registro);
				valorRegistro.push_back(operando1+operando2);
			}
			if (palabra.find("sub") != std::string::npos ){
				string registro = matriz[i][1];
				string op1 = matriz[i][2];
				string op2 = matriz[i][3];
				int operando1 = atoi(op1.c_str());
				int operando2 = atoi(op2.c_str());
				nombreRegistro.push_back(registro);
				valorRegistro.push_back(operando1-operando2);
			}
		}
	}
}


//Busca si existe etiquetas en el codigo
bool etiquetaComentario(int a){
	string palabra = matriz[a][0];
	if (palabra.find(":") != std::string::npos)
    	return true;
	return false;
}

//Bandera para los ciclos principales
void banderaCiclos1(){
	if(banderaCiclo1==0)
		banderaCiclo1=1;
	else
		banderaCiclo1=0;
}


//Bandera para los ciclos anidados
void banderaCiclos2(){
	if(banderaCiclo2==0)
		banderaCiclo2=1;
	else
		banderaCiclo2=0;
}


//Calcula el numero maximo entre dos valores
int max(int num1, int num2) {
   int result;
   if (num1 > num2)
      result = num1;
   else
      result = num2;
   return result;
}


void contadorDeCiclosPipe(int i){
	int operando1;
	int operando2;
	
	//Calcula los ciclos que no estan fuera de un bucle y no tenga burbujas
    if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and banderaCiclo1==0 and banderaCiclo2==0){
    	cout << contadorPipe<<" + 1 = ";
		contadorPipe++;
	}
	//Cuando se encuentra una etiqueta se debe cambiar una bandera indicando un bucle y no sumar nada
	else if(etiquetaComentario(i)==true){
		if (banderaCiclo1==0 and banderaCiclo2==0){
			banderaCiclos1();
			posBandera=i;
		}
		if (banderaCiclo1==1 and banderaCiclo2==0){
			banderaCiclos2();
		}
	}
	/*
	//Calcula ciclos cuando se encuentra con branch
	else if(banderaCiclo1==1 and (matriz[i][0] == "beq" or matriz[i][0] == "bne")){
		if(matriz[i][0] == "beq"){
			for (int j=0; j < matriz[i].size(); j++){
				if(matriz[i][1]==nombreRegistro[j])
					operando1=valorRegistro[j];
				if(matriz[i][2]==nombreRegistro[j])
					operando2=valorRegistro[j];
			}
			//tratar de que en el vector se sobrescriban valores. Puede que se estalle aca, hay que cuadrar para que sea dividir operando1 con operando2
			indice=max(operando1,operando2);
			cout << contadorPipe<<" + "<< indice<< "+2 = ";
			//hay burbuja cuando se cumple la condicion +1, y +1 ahi mismo que pregunta y se cumple
			contadorPipe=contadorPipe+1+1+indice;
		}
		if(matriz[i][0] == "bne"){
			//terminar...
		}
	}
	//Calcula los ciclos cuando estan dentro de un bucle y no tenga burbujas
	else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and banderaCiclo1==1){
		cout<<contadorPipe<<" + "<<indice<<" = ";
		contadorPipe= contadorPipe+indice;
	}
	//Calcula los ciclos cuando se encuentra con Jump junto con sus burbujas.
	else if(matriz[i][0] == "j" and banderaCiclo1==1){
		cout<<contadorPipe<<"+"<<indice<<"(2) = ";
		contadorPipe= contadorPipe+(indice*2);
	}	
	//Calcular ciclos de lw con o sin dependencias fuera de un ciclo
	else if(matriz[i][0]=="lw" and banderaCiclo1==0){
		//terminar...
	}
	//Calcular ciclos de lw con o sin dependencias dentro de un ciclo
	else if(matriz[i][0]=="lw" and banderaCiclo1==1){
		//terminar...
	}*/
}


//imprime los ciclos y la matriz
void imprimirCiclosYMatrizPipelined(){
	for (int i = 0; i<matriz.size(); i++) { 
		contadorDeCiclosPipe(i); cout << contadorPipe <<"\t \t";
        for (int j = 0; j<matriz[i].size(); j++) 
            cout << matriz[i][j] << " "; 
        cout << endl; 
    }
}


int main(void){
	abrirArchivo("codigoProfe.asm");
	guardarRegistros();
	imprimirCiclosYMatrizPipelined();
	return 0;
}
