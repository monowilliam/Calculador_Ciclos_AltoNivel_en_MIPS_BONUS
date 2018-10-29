#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <stdlib.h>
#include <sstream>
using namespace std;
#ifdef _WIN32
#define  limpiar  system("cls")
#endif
#ifdef linux
#define  limpiar   system("clear");
#endif

vector<string> fila;
vector<vector<string> > matriz;
vector<vector<string> > ciclo1;

int contadorPipe=4;
int contadorMono=0;

vector<string>nombreRegistro; 	//vector donde guardo el nombre de los Registros
vector<int>valorRegistro; 		//vector donde guarda los valores de los respectivos registros
vector <int>posicionesCiclos;	//vectoy que guarda las posiciones de cada ciclo
int ciclos=0;					//contador de los ciclos que van
int offset1;
int offset2;
int cuandoCiclosHay=0;
string operando1Ciclo, operando2Ciclo;

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

namespace patch{
    template < typename T > std::string to_string( const T& n ){
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
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


bool compararEtiquetas(string a, string b){
	if (a.find(":") != std::string::npos){
		if( a.find(b.c_str())  != std::string::npos )
			return true;
		return false;
	}else{
		if( b.find(a.c_str())  != std::string::npos )
			return true;
		return false;
	}
	
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

//Calcula el numero maximo entre dos valores
int min(int num1, int num2) {
   int result;
   if (num1 < num2)
      result = num1;
   else
      result = num2;
   return result;
}


void contadorDeCiclosPipe(int i){
	int operando1;
	int operando2;
	
	//Cuando se encuentra una etiqueta se debe cambiar una bandera indicando un bucle y no sumar nada
	if(etiquetaComentario(i)==true){
		if (ciclos==0){
			cout << " \t "; posicionesCiclos.push_back(i); }
		else if (ciclos==1){
			cout << " \t \t"; posicionesCiclos.push_back(i); }
	}
	
	//Calcula los ciclos que estan fuera de un bucle y no tenga burbujas
    else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jr"and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and ciclos==0){
    	cout << contadorPipe<<" + 1 = ";  
		contadorPipe++;
	}
	
	//Calcula los ciclos cuando estan dentro de un bucle (primero) simple y no tenga burbujas
	else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and ciclos==1){
		cout<<contadorPipe<<" + "<< offset1 <<" = \t";
		contadorPipe = contadorPipe + offset1;
	}
	
	//Calcula los ciclos cuando estan dentro de un bucle (segundo) anidado y no tenga burbujas
	else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and ciclos==2){
		cout<<contadorPipe<<" + "<< offset1 << "x" << offset2 <<" = \t \t";
		contadorPipe= contadorPipe + (offset1 * offset2);
	}
	
	//Calcula los ciclos cuando se encuentra con Jump junto con sus burbujas.
	else if(matriz[i][0] == "j"){
		//Encuentra el j donde salta el primer ciclo
		if(ciclos==1 and compararEtiquetas(matriz[posicionesCiclos[0]][0],matriz[i][1]) == 1){
			ciclos=0;
			cout<<contadorPipe<<" + "<<offset1<<"x2 = ";
			contadorPipe= contadorPipe+ (offset1*2);
		}
		//Encuentra el j donde salta el segundo ciclo anidado
		else if(ciclos==2 and compararEtiquetas(matriz[posicionesCiclos[1]][0],matriz[i][1]) == 1 ){
			ciclos=1;
			cout<<contadorPipe<<" + "<<offset1 << "x" << offset2 <<"x2 = \t";
			contadorPipe = contadorPipe + (offset1*offset2*2);
		}
	}
	
	//Calcula los ciclos cuando se encuetra con un JR
	else if(matriz[i][0] == "jr" and ciclos==0){
		cout<<contadorPipe<<" + 2 = ";
		contadorPipe = contadorPipe + 2;
	}
		
	//Calcula ciclos cuando se encuentra con branch CICLOS SMPLES Y ANIDADOS
	else if(matriz[i][0] == "beq" or matriz[i][0] == "bne"){
		ciclos++;
		//Si se encuentra el primer ciclo...
		if(matriz[i][0] == "beq" and ciclos==1){
			for (int j=0; j < matriz[i].size(); j++){
				if(matriz[i][1]==nombreRegistro[j])
					operando1=valorRegistro[j];
				if(matriz[i][2]==nombreRegistro[j])
					operando2=valorRegistro[j];
			}
			//tratar de que en el vector se sobrescriban valores. Puede que se estalle aca, hay que cuadrar para que sea dividir operando1 con operando2
			int maximo=max(operando1,operando2);
			int minimo=min(operando1,operando2);
			offset1=maximo-minimo;
			cout << contadorPipe<<" + "<< offset1<< " + 2 = \t";
			//hay burbuja cuando se cumple la condicion +1, y +1 ahi mismo que pregunta por ultima vez y se cumple
			contadorPipe=contadorPipe+1+1+offset1;
		}
		//Si se encuentra con el segundo ciclo para formar el ciclo anidado...................................................
		if(matriz[i][0] == "beq" and ciclos==2){
			for (int j=0; j < matriz[i].size(); j++){
				if(matriz[i][1]==nombreRegistro[j])
					operando1=valorRegistro[j];
				if(matriz[i][2]==nombreRegistro[j])
					operando2=valorRegistro[j];
			}
			//tratar de que en el vector se sobrescriban valores. Puede que se estalle aca, hay que cuadrar para que sea dividir operando1 con operando2
			int maximo=max(operando1,operando2);
			int minimo=min(operando1,operando2);
			offset2=maximo-minimo;
			cout << contadorPipe<<" + "<< offset1 << "+2 x" << offset2 << " = \t";
			//hay burbuja cuando se cumple la condicion +1, y +1 ahi mismo que pregunta y se cumple
			contadorPipe=contadorPipe+((offset1+2)*offset2);
		}
		if(matriz[i][0] == "bne"){
			//terminar...
		}
	}	
	
	//Calcula los ciclos de los lw
	else if(matriz[i][0]=="lw"){
		//Calcular ciclos de lw con o sin dependencias fuera de un ciclo
		if(matriz[i][0]=="lw" and ciclos==0){
			if(matriz[i+1].size()==4){
				if(matriz[i][1]==matriz[i+1][2] or matriz[i][1]==matriz[i+1][3]){
					cout << "(B) " <<contadorPipe << " + 2 = \t";
					contadorPipe = contadorPipe + 1+1;
				}else{
					cout <<contadorPipe << " + 1 = \t";
					contadorPipe++;
				}
			}
			if(matriz[i+1].size()==3){
				if(matriz[i][1]==matriz[i+1][1] or matriz[i][1]==matriz[i+1][2]){
					cout << "(B) " <<contadorPipe << " + 2 = \t";
					contadorPipe = contadorPipe + 1+1;
				}else{
					cout <<contadorPipe << " + 1 = \t";
					contadorPipe++;
				}
			}
		}
		//Calcular ciclos de lw con o sin dependencias dentro de un ciclo simple
		else if(matriz[i][0]=="lw" and ciclos==1){
			if(matriz[i+1].size()==4){
				if(matriz[i][1]==matriz[i+1][2] or matriz[i][1]==matriz[i+1][3]){
					cout << "(B) " <<contadorPipe << " + " << offset1 << "x2 = \t";
					contadorPipe = contadorPipe + (offset1*2);
				}else{
					cout <<contadorPipe << " + " << offset1 << " = \t";
					contadorPipe = contadorPipe + offset1;
				}
			}
			if(matriz[i+1].size()==3){
				if(matriz[i][1]==matriz[i+1][1] or matriz[i][1]==matriz[i+1][2]){
					cout << "(B) " <<contadorPipe << " + " << offset1 << "x2 = \t";
					contadorPipe = contadorPipe + (offset1*2);
				}else{
					cout <<contadorPipe << " + " << offset1 << " = \t";
					contadorPipe = contadorPipe + offset1;
				}
			}
		}
		//Calcular ciclos de lw con o sin dependencias dentro de un ciclo anidado
		else if(matriz[i][0]=="lw" and ciclos==2){
			if(matriz[i+1].size()==4){
				//Busca las dependencias de lw
				if(matriz[i][1]==matriz[i+1][2] or matriz[i][1]==matriz[i+1][3]){
					cout << "(B) " <<contadorPipe << " + " << offset1 << "x" <<offset2 << "x2 = \t";
					contadorPipe = contadorPipe + (offset1*offset2*2);
				}else{
					cout <<contadorPipe << " + " << offset1 << "x" <<offset2 << " = \t";
					contadorPipe = contadorPipe + (offset1*offset2);
				}
			}
			if(matriz[i+1].size()==3){
				//Busca las dependencias de lw
				if(matriz[i][1]==matriz[i+1][1] or matriz[i][1]==matriz[i+1][2]){
					cout << "(B) " <<contadorPipe << " + " << offset1 << "x" <<offset2 << "x2 = \t";
					contadorPipe = contadorPipe + (offset1*offset2*2);
				}else{
					cout <<contadorPipe << " + " << offset1 << "x" <<offset2 << " = \t";
					contadorPipe = contadorPipe + (offset1*offset2);
				}
			}
		}
	}
}


void contadorDeMonociclos(int i){
	int operando1;
	int operando2;
	//Cuando se encuentra una etiqueta se debe cambiar una bandera indicando un bucle y no sumar nada
	if(etiquetaComentario(i)==true){
		if (ciclos==0)
			posicionesCiclos.push_back(i);
		else if (ciclos==1)
			posicionesCiclos.push_back(i);
	}
	//Calcula los ciclos que estan fuera de un bucle y no tenga burbujas
    else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jr"and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and ciclos==0){
    	contadorMono++;
	}
	//Calcula los ciclos cuando estan dentro de un bucle (primero) simple y no tenga burbujas
	else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and ciclos==1){
		contadorMono = contadorMono + offset1;
	}
	//Calcula los ciclos cuando estan dentro de un bucle (segundo) anidado y no tenga burbujas
	else if(matriz[i][0] != "lw" and matriz[i][0] != "j" and matriz[i][0] != "jal" and matriz[i][0] != "beq" and matriz[i][0] != "bne" and etiquetaComentario(i)!=true and ciclos==2){
		contadorMono= contadorMono + (offset1 * offset2);
	}
	//Calcula los ciclos cuando se encuentra con Jump junto con sus burbujas.
	else if(matriz[i][0] == "j"){
		//Encuentra el j donde salta el primer ciclo
		if(ciclos==1 and compararEtiquetas(matriz[posicionesCiclos[0]][0],matriz[i][1]) == 1){
			ciclos=0;
			contadorMono = contadorMono + offset1;
		}
		//Encuentra el j donde salta el segundo ciclo anidado
		else if(ciclos==2 and compararEtiquetas(matriz[posicionesCiclos[1]][0],matriz[i][1]) == 1 ){
			ciclos=1;
			contadorMono = contadorMono + (offset1*offset2);
		}
	}
	//Calcula los ciclos cuando se encuetra con un JR
	else if(matriz[i][0] == "jr" and ciclos==0){
		contadorMono++;
	}	
	//Calcula ciclos cuando se encuentra con branch CICLOS SMPLES Y ANIDADOS
	else if(matriz[i][0] == "beq" or matriz[i][0] == "bne"){
		ciclos++;
		//Si se encuentra el primer ciclo...
		if(matriz[i][0] == "beq" and ciclos==1){
			for (int j=0; j < matriz[i].size(); j++){
				if(matriz[i][1]==nombreRegistro[j])
					operando1=valorRegistro[j];
				if(matriz[i][2]==nombreRegistro[j])
					operando2=valorRegistro[j];
			}
			//tratar de que en el vector se sobrescriban valores. Puede que se estalle aca, hay que cuadrar para que sea dividir operando1 con operando2
			int maximo=max(operando1,operando2);
			int minimo=min(operando1,operando2);
			offset1=maximo-minimo;
			contadorMono=contadorMono+1+offset1;
		}
		//Si se encuentra con el segundo ciclo para formar el ciclo anidado...................................................
		if(matriz[i][0] == "beq" and ciclos==2){
			for (int j=0; j < matriz[i].size(); j++){
				if(matriz[i][1]==nombreRegistro[j])
					operando1=valorRegistro[j];
				if(matriz[i][2]==nombreRegistro[j])
					operando2=valorRegistro[j];
			}
			//tratar de que en el vector se sobrescriban valores. Puede que se estalle aca, hay que cuadrar para que sea dividir operando1 con operando2
			int maximo=max(operando1,operando2);
			int minimo=min(operando1,operando2);
			offset2=maximo-minimo;
			contadorMono=contadorMono+1+(offset1*offset2);
		}
		if(matriz[i][0] == "bne"){
			//terminar...
		}
	}	
	//Calcular ciclos de lw con o sin dependencias fuera de un ciclo
	else if(matriz[i][0]=="lw" and ciclos==0){
		contadorMono++;
	}
	//Calcular ciclos de lw con o sin dependencias dentro de un ciclo simple
	else if(matriz[i][0]=="lw" and ciclos==1){
		contadorMono = contadorMono + offset1;
	}
	//Calcular ciclos de lw con o sin dependencias dentro de un ciclo anidado
	else if(matriz[i][0]=="lw" and ciclos==2){
		contadorMono = contadorMono + (offset1*offset2);
	}
}


void crearMatriz2(int i, int numDesenrrollado){
	if(ciclos==1 and cuandoCiclosHay==1){
		fila.push_back(patch::to_string(ciclos));	
		//Busca el registro que esta de contador tomando los dos operandos el branch
		if(matriz[i][0]== "beq" or matriz[i][0]== "bne"){
			operando1Ciclo= matriz[i][1];
			operando2Ciclo= matriz[i][2];
		}
		//Mete los valores la matriz fila
		for (int j = 0; j<matriz[i].size(); j++)
			fila.push_back(matriz[i][j]);
		//Cambia el valor del contador por la multiplicacion del desenrrollado
		if( (fila[2]==operando1Ciclo and fila[3]==operando1Ciclo) or (fila[2]==operando2Ciclo and fila[3]==operando2Ciclo) ){
			string a=fila[4];
			int b = atoi(a.c_str());
			int c = b*numDesenrrollado;
			fila[4]= patch::to_string(c);
		}
		ciclo1.push_back(fila);
		fila.clear();
	}
	if(ciclos==2 and cuandoCiclosHay==2){
		fila.push_back(patch::to_string(ciclos));	
		//Busca el registro que esta de contador tomando los dos operandos el branch
		if(matriz[i][0]== "beq" or matriz[i][0]== "bne"){
			operando1Ciclo= matriz[i][1];
			operando2Ciclo= matriz[i][2];
		}
		//Mete los valores la matriz fila
		for (int j = 0; j<matriz[i].size(); j++)
			fila.push_back(matriz[i][j]);
		//Cambia el valor del contador por la multiplicacion del desenrrollado
		if( (fila[2]==operando1Ciclo and fila[3]==operando1Ciclo) or (fila[2]==operando2Ciclo and fila[3]==operando2Ciclo) ){
			string a=fila[4];
			int b = atoi(a.c_str());
			int c = b*numDesenrrollado;
			fila[4]= patch::to_string(c);
		}
		ciclo1.push_back(fila);
		fila.clear();
	}
}


void desenrrollado(int i, int numDesenrrollado){
	//Cuando se encuentra una etiqueta se debe cambiar una bandera indicando un bucle y no sumar nada
	if(etiquetaComentario(i)==true){
		if (ciclos==0)
			posicionesCiclos.push_back(i);
		else if (ciclos==1)
			posicionesCiclos.push_back(i);
	}
	//Calcula los ciclos cuando se encuentra con Jump junto con sus burbujas.
	else if(matriz[i][0] == "j"){
		//Encuentra el j donde salta el primer ciclo
		if(ciclos==1 and compararEtiquetas(matriz[posicionesCiclos[0]][0],matriz[i][1]) == 1)
			ciclos=0;
		//Encuentra el j donde salta el segundo ciclo anidado
		else if(ciclos==2 and compararEtiquetas(matriz[posicionesCiclos[1]][0],matriz[i][1]) == 1 )
			ciclos=1;
	}
	//Calcula ciclos cuando se encuentra con branch CICLOS SMPLES Y ANIDADOS
	else if(matriz[i][0] == "beq" or matriz[i][0] == "bne")
		ciclos++;
	crearMatriz2(i,numDesenrrollado);
}

void calculadorDeCiclos(){
	for (int i = 0; i<matriz.size(); i++) { 
		for (int j = 0; j<matriz[i].size(); j++){
			if(matriz[i][j] == "beq" or matriz[i][j] == "bne")
				cuandoCiclosHay++;
		}
	}
}



//imprime los ciclos y la matriz en pipelined
void imprimirCiclosYMatrizPipelined(){
	for (int i = 0; i<matriz.size(); i++) { 
		contadorDeCiclosPipe(i); cout << contadorPipe <<"\t"; cout<<" | " << ciclos << " | " ;
		for (int j = 0; j<matriz[i].size(); j++) 
            cout << matriz[i][j] << " "; 
        cout << endl; 
    }
}



//imprime los ciclos y la matriz en monociclo
void imprimirCiclosYMatrizMonociclo(){
	for (int i = 0; i<matriz.size(); i++) { 
		contadorDeMonociclos(i); cout << contadorMono <<"\t"; cout<<" | " << ciclos << " | " ;
		for (int j = 0; j<matriz[i].size(); j++) 
            cout << matriz[i][j] << " "; 
        cout << endl; 
    }
}


void imprimirDesenrrollado(int numDesenrrollado){
	//crea la matriz del ciclo a desenrrollar
	for (int i = 0; i<matriz.size(); i++)
		desenrrollado(i,numDesenrrollado);
	//imprime el primer ciclo desenrrollado
	for (int i = 0; i<ciclo1.size()-1; i++) { 
		cout <<"\t";
		for (int j = 1; j<ciclo1[i].size(); j++)
				cout << ciclo1[i][j] << " ";
        cout << endl; 
	}cout << endl;
	//imprime los ciclos de la mitad
	for(int k=0; k<numDesenrrollado-2; k++){
		for (int i = 1; i<ciclo1.size()-1; i++) { 
			cout <<"\t";
			for (int j = 1; j<ciclo1[i].size(); j++)
					cout << ciclo1[i][j] << " ";
	        cout << endl; 
	    }
		cout << endl;
	}
	//imprime el ultimo ciclo desenrrollado
	for (int i = 1; i<ciclo1.size(); i++) { 
		cout <<"\t";
		for (int j = 1; j<ciclo1[i].size(); j++)
				cout << ciclo1[i][j] << " ";
        cout << endl; 
	}cout << endl;
}


string obtenerString(){
	string a;
	cin>>a;								//Funcion para no usar cin
	return a;
}

void textoMenu(){
	cout<<endl ;
	cout<<"   ษอออออออออออออออออออออออออออออออออออออออออออออออออออป"<<endl;
	cout<<"   บ     Calculador de Ciclos Pipelined, Monociclos    บ "<<endl ;
	cout<<"   บ  y Desenrrollado en MIPS - William Aguirre Zapata บ "<<endl ;
	cout<<"   ศอออออออออออออออออออออออออออออออออออออออออออออออออออผ "<<endl<<endl;
	cout<<"	 NOTAS Y CRITERIOS DE DISEัO DEL CODIGO MIPS "<<endl;
	cout<<" El codigo en MIPS no puede contener espacios en las entrelineas, ni una linea"<<endl;
	cout<<" solamente con comentarios '#...'; el opcode de las instrucciones debe ser en"<<endl;
	cout<<" minuscula 'addi $s1, $s1, 1' y los registros separados por ',' comas"<<endl;
	cout<<" o espacios ' '; las etiquetas 'label:' deben ir en un entrelineado sin mas porcion"<<endl;
	cout<<" de codigo; el numero a desenrrollar debe ser multiplo de dos. El maximo numero de"<<endl;
	cout<<" ciclos anidados permitidos son dos."<<endl<<endl;
	cout<<" 1 - Calculador de ciclos de PIPELINED en MIPS"<<endl <<endl;
	cout<<" 2 - Calculador de ciclos de MONOCICLO en MIPS" <<endl<<endl;
	cout<<" 3 - DESENRROLLADO de codigo en MIPS" <<endl<<endl;
	cout<<" 0 - Salir"<<endl<<endl;
	cout << "Ingrese la opcion que desea realizar ";
}

void menu(){
	limpiar;
	//string nombreproy;
	int numeroDesenrrollado;
	string version;
		do{
			matriz.clear();
			ciclo1.clear();
			contadorPipe=4;
			contadorMono=0;
			nombreRegistro.clear(); 	//vector donde guardo el nombre de los Registros
			valorRegistro.clear();		//vector donde guarda los valores de los respectivos registros
			posicionesCiclos.clear();	//vectoy que guarda las posiciones de cada ciclo
			ciclos=0;					//contador de los ciclos que van
			offset1=0;
			offset2=0;
			cuandoCiclosHay=0;
			operando1Ciclo="";
			operando2Ciclo="";
			abrirArchivo("codigoProfe.asm");
			calculadorDeCiclos();
			guardarRegistros();
			
			limpiar;
			textoMenu();
			string pl;
			pl=obtenerString();
			if( pl == "1"){
				limpiar;
				imprimirCiclosYMatrizPipelined();
				cout << endl; 
				system("pause");
			}
			else if( pl == "2"){
				limpiar;
				imprimirCiclosYMatrizMonociclo();
				cout << endl; 
				system("pause");
			}
			else if( pl == "3"){
				limpiar;
				cout <<endl <<" Digite el numero total de desenrrollamientos multiplo de 2: ";
				cin >> numeroDesenrrollado; cout << endl;
				imprimirDesenrrollado(numeroDesenrrollado);
				cout << endl; 
				system("pause");
			}
			else if( pl == "0"){
				limpiar; cout << endl;
				cout <<"Hasta pronto!"<< endl;
				system("pause");
				exit(1);
			}
			else{
				cout << endl;
				cout << "Opcion invalida, intenta nuevamente"<< endl<<endl;
				system("pause");
				limpiar;
			}
		}while(1);
}


int main(void){
	menu();
	return 0;
}
