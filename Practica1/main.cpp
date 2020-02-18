#include <curses.h>
#include <iostream>
#include <string>
#include <windows.h>
#define _tmain main
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdlib>
using namespace std;
int FilaGeneral;
int ColumnaGeneral;
int Ancho;
int Alto;
boolean Permiso=false;
string PalB;
string PalR;

int wherex(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}
int wherey(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
}
string ConString(int i){      //   int to string en c++
       string num;
       int temp;
       while(i / 10!=0){
         temp=i%10;
         i= i/10;
         temp =temp + 48;
         num = (char)temp + num;
       }
       i=i+48;
       num = (char)i + num ;
       return num;
}

void Resolucion(){
    Ancho = (GetSystemMetrics(SM_CXSCREEN) / 8) - 2;
    Alto = (GetSystemMetrics(SM_CYSCREEN) / 19) + 1;}
void ALTENTER(){
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
return;
}

//          Estructuras
struct Caracteres{
	  string Caracter;
	  int Fila;
	  int Columna;
	  struct Caracteres *sig=NULL;
	  struct Caracteres *ant=NULL;};
typedef struct Caracteres *LCD;
LCD InicioLDC=NULL;
LCD FinLDC=NULL;
struct Rutas{
    string Ruta;
    string Nombre;
    struct Rutas *sig=NULL;};
typedef struct Rutas *R;
R InicioRutasLCS=NULL;
R FinRutasLCS=NULL;
struct PilaCambios{
    string PB="NULL";
    string PR="NULL";
    string Estado;
    string Palabra="NULL";
    int Fila;
    int Columna;
    struct PilaCambios *sig=NULL;
    struct PilaCambios *ant=NULL;
};
typedef struct PilaCambios *PC;
PC InicioPC=NULL;
PC FinPC=NULL;
PC InicioPR=NULL;
PC FinPR=NULL;
boolean PermisoBuscarYRemplazar=false;

//                      LISTA DOBLE CARACTERES

void InsertarALFinal(LCD &Inicio, LCD &Fin, string Caracter){
    LCD Nuevo=new (struct Caracteres);
    Nuevo->Caracter=Caracter;Nuevo->Fila=wherey();Nuevo->Columna=wherex();
    if(Inicio==NULL){
        Inicio=Nuevo;
        Fin=Inicio;
    }else{
        Fin->sig=Nuevo;
        Nuevo->ant=Fin;
        Fin=Nuevo;
    }
    cout<<Caracter;
}

void RemplazarNodo_Agregar(LCD &PrimerLetra,LCD &Espacio2,string PR){
    LCD Espacio;
    if(PrimerLetra==InicioLDC){
    }
    else{
        Espacio=PrimerLetra->ant;
    }
    for(int i=0;i<(int)PR.size();i++){
        LCD Nuevo=new (struct Caracteres);
        Nuevo->Caracter=PR[i];
        Espacio->sig=Nuevo;
        Nuevo->ant=Espacio;
        Nuevo->sig=Espacio2;
        Espacio2->ant=Nuevo;
        Espacio=Nuevo;
        }
}
void BuscarRempl(LCD &Inicio){
    InsertarALFinal(InicioLDC,FinLDC," ");
    LCD Aux=Inicio;
    LCD PRIMERCARACTER;
    string PB,PR;
    if(PermisoBuscarYRemplazar==false){
        cout<<"INGRESE LA PALABRA A REEMPLAZAR Y LUEGO LA PALABRA CON LA QUE SE REEMPLAZARA\n";
        cout<<"EJEMPLO:(HOLA HOLA2)";
        cin>>PB>>PR;
        PalB=PB;PalR=PR;
    }else{
    PB=PalB;PR=PalR;
    }
    string PalEncontrada="";
    while(true){
        if(Aux->Caracter==" "||Aux->Caracter=="enter"){
            if(PalEncontrada==PB){
                RemplazarNodo_Agregar(PRIMERCARACTER,Aux,PR);}
            PalEncontrada="";
        }else{
            if(PalEncontrada==""){PRIMERCARACTER=Aux;}
            PalEncontrada+=Aux->Caracter;
        }
        Aux=Aux->sig;
        if(Aux==NULL){break;}
        FinLDC=Aux;
    }
}
//Log

void AgregarCambio(string Pb,string Pr){
    PC Nuevo=new (struct PilaCambios);
    Nuevo->PB=Pb;
    Nuevo->PR=Pr;
    Nuevo->Estado="NO REVERTIDO";
    if(InicioPC==NULL){
        InicioPC=Nuevo;
        FinPC=Nuevo;
    }else{
        FinPC->sig=Nuevo;
        Nuevo->ant=FinPC;
        FinPC=Nuevo;
    }
}
void AgregarPilaRevertido(string Pb,string Pr){
    PC Nuevo=new (struct PilaCambios);
    Nuevo->PB=Pb;
    Nuevo->PR=Pr;
    Nuevo->Estado="REVERTIDO";
    if(FinPR==NULL){
        FinPR=Nuevo;
        InicioPR=Nuevo;
    }else{
        FinPR->sig=Nuevo;
        Nuevo->ant=FinPR;
        FinPR=Nuevo;
    }
}
void QuitarCambio(){
    if(InicioPC!=NULL){
        if(FinPC==InicioPC){
            AgregarPilaRevertido(FinPC->PB,FinPC->PR);
            PalR=FinPC->PB;PalB=FinPC->PR;
            PermisoBuscarYRemplazar=true;
            BuscarRempl(InicioLDC);
            PermisoBuscarYRemplazar=false;
            FinPC=NULL;
            InicioPC=NULL;
        }else{
            AgregarPilaRevertido(FinPC->PB,FinPC->PR);
            PalR=FinPC->PB;PalB=FinPC->PR;
            PermisoBuscarYRemplazar=true;
            BuscarRempl(InicioLDC);
            PermisoBuscarYRemplazar=false;
            FinPC=FinPC->ant;
            FinPC->sig=NULL;
        }
    }

}

void QuitarPilaRevertido(){
    if(InicioPR!=NULL){
        if(FinPR==InicioPR){
            AgregarCambio(FinPR->PB,FinPR->PR);
            //buscar y remplazar normal
            PalR=FinPR->PR;PalB=FinPR->PB;
            PermisoBuscarYRemplazar=true;
            BuscarRempl(InicioLDC);
            PermisoBuscarYRemplazar=false;
            FinPR=NULL;
            InicioPR=NULL;
        }else{
            AgregarCambio(FinPR->PB,FinPR->PR);
            //buscar y remplazar normal
            PalR=FinPR->PR;PalB=FinPR->PB;
            PermisoBuscarYRemplazar=true;
            BuscarRempl(InicioLDC);
            PermisoBuscarYRemplazar=false;
            FinPR=FinPR->ant;
            FinPR->sig=NULL;
    }
    }
}

//agregar a lista circular
void AgregarRuta(string ruta){
    R Nuevo=new (struct Rutas);
    Nuevo->Ruta=ruta;
    if(InicioRutasLCS==NULL){
        InicioRutasLCS=Nuevo;
        FinRutasLCS=Nuevo;
        FinRutasLCS->sig=InicioRutasLCS;
    }else{
        FinRutasLCS->sig=Nuevo;
        FinRutasLCS=Nuevo;
        FinRutasLCS->sig=InicioRutasLCS;
    }
}


void BorrarUltimo(LCD &Inicio,LCD &Fin){
    LCD Aux=Fin;
    if(Fin->ant==NULL && Fin->sig==NULL){
        Fin=NULL;
        Inicio=NULL;
    }else{
        Fin=Fin->ant;
        Fin->sig=NULL;
        Aux->ant=NULL;
    }
}
void ImprimirLista(LCD &Inicio){
    if(Permiso==false){move(0,0);refresh();}else{move(1,0);refresh();}
    LCD Aux=Inicio;
    while(true){
        if(wherex()==Ancho-1){
            Aux->Fila=wherey()+1;Aux->Columna=0;
        }else{Aux->Fila=wherey();Aux->Columna=wherex();}
        if(Aux->Caracter=="enter"){cout<<"\n";}
        else{cout<<Aux->Caracter;}
        Aux=Aux->sig;
        if(Aux==NULL){
            int c=Ancho-2;
            if(FinLDC->Columna==c){
                move(int(FinLDC->Fila+1),0);refresh();
            }else{
                move(FinLDC->Fila,FinLDC->Columna+1);refresh();
            }
        break;}
    }
}

void MenuOpcionEditor(){
    move(0,0);
    refresh();
    cout<<"^W BUSCAR Y REEMPLAZAR   ";
    cout<<"^C REPORTES  ";
    cout<<"^S GUARDAR   ";
    move(0,0);
    refresh();
}



void GuardarArchivo(string ruta){
    LCD aux=InicioLDC;
    ofstream fichero;
    fichero.open(ruta.c_str());
    if(!fichero.fail()){
        while(true){
            if(aux->Caracter=="enter"){fichero<<"\n";}
            else{fichero<<aux->Caracter;}
            aux=aux->sig;
            if(aux==NULL){break;}
        }
        fichero.close();
    }
}


//Generacion de Reportes
void REPRUTAS(){
		ofstream Reporte4("C:\\Users\\Usuario\\Desktop\\Rutas.dot");
		R aux = InicioRutasLCS;
		//Inicio Grafiz
		Reporte4<< "digraph G" << endl;
		Reporte4<< "{" << endl;
		Reporte4<< "node [shape = box, fontname = Arial, color = cyan];" <<endl;
		int n = 0;
		string Ordenador;
		string Flecha;
		if(InicioRutasLCS != NULL)
		{
			do
			{
			    //crear caja de ruta
				Reporte4<<"R"<<ConString(n)<< " [label = \""<< aux -> Ruta <<"\"]" <<endl;
				//guarda el nombre del nodo ruta
				n++;
				aux = aux->sig;
			}
			while(aux != InicioRutasLCS);
            for(int i=0;i<n;i++){
                Ordenador=Ordenador+"R"+ConString(i)+" ";
            }
            for(int i=0;i<n;i++){
                Flecha=Flecha+"R"+ConString(i)+"->";
            }
            Reporte4 << "{ rank = same "+Ordenador+"}"<<endl;
            Reporte4 <<Flecha+"R0"<<endl;
            Reporte4 <<"}";
			Reporte4.close();
			//Generar Imagen
			system("C:\\\"Program Files (x86)\"\\Graphviz2.38\\bin\\dot.exe  -Tpng C:\\Users\\Usuario\\Desktop\\Rutas.dot -o C:\\Users\\Usuario\\Desktop\\Rutas.png");
			//Abrir Imagen
			system("C:\\Users\\Usuario\\Desktop\\Rutas.png &");
		}
	}

	void REPCARACTERES(){
        ofstream Reporte("C:\\Users\\Usuario\\Desktop\\caracteres.dot");
        Reporte<< "digraph G" << endl;
		Reporte<< "{" << endl;
		Reporte<< "node [shape = box, fontname = Arial, color = black];" <<endl;
		LCD Aux=InicioLDC;
		int n=0;
		string Ordenador;
		string Flecha;
		string Car;
		while(true){
            if(Aux->Caracter=="\n"){Car="enter";}else{Car=Aux->Caracter;}
            Reporte<<"R"<<ConString(n)<< " [label = \""<< Car <<"\"]"<<endl;
            n++;
            Aux=Aux->sig;
            if(Aux==NULL){break;}
		}
		for(int i=0;i<n;i++){
            Ordenador=Ordenador+"R"+ConString(i)+" ";
        }
        for(int i=0;i<n;i++){
            Flecha=Flecha+"R"+ConString(i)+"->";
        }
        Reporte << "{ rank = same "+Ordenador+"}"<<endl;
        for(int i=n-2;i>=0;i--){
            if(i==0){Flecha=Flecha+"R"+ConString(i);}
            else{Flecha=Flecha+"R"+ConString(i)+"->";}
        }
        Reporte <<Flecha<<endl;
        Reporte <<"}";
        Reporte.close();
        //Generar Imagen
        system("C:\\\"Program Files (x86)\"\\Graphviz2.38\\bin\\dot.exe  -Tpng C:\\Users\\Usuario\\Desktop\\caracteres.dot -o C:\\Users\\Usuario\\Desktop\\caracteres.png");
        //Abrir Imagen
        system("C:\\Users\\Usuario\\Desktop\\caracteres.png &");
	}

	void mostrarArchivosRecientes(){
        move(0,0);refresh();
        cout<<"GENERAR REPORTES PRECIONE X"<<endl<<endl;
        R Aux=InicioRutasLCS;
        if(InicioRutasLCS!=NULL){
            do{
                cout<<"     "<<Aux->Ruta<<endl;
                Aux=Aux->sig;
            }while(Aux!=InicioRutasLCS);
        }
        int c;
        c=getch();
        if(c==120){REPRUTAS();}
	}

void MenReportes(){
    string Opcion;
    printf("REPORTES:\n");
    printf("1.LISTA CARACTERES   2.PALABRAS BUSCADAS     3.PALABRAS ORDENADAS\n");
    int c;
    c=getch();
    if(c==49){
        REPCARACTERES();
    }else if(c==50){
        //LOG
    }else if(c==51){
        //Pal Ordenadas
    }
}

void Editor(){
    Resolucion();
    string PALBR;
    initscr();
    MenuOpcionEditor();
    if(InicioLDC!=NULL){move(0,0);refresh();ImprimirLista(InicioLDC);}
    int c;
    string cadena;
    string BuscarRemplazar;
    do{
        c=getchar();
        if(c==8){//AL PRESIONAR BACKSPACE
            if(FinLDC->Caracter=="enter"){BorrarUltimo(InicioLDC,FinLDC);}
            int f=FinLDC->Fila;
            int c=FinLDC->Columna;
            BorrarUltimo(InicioLDC,FinLDC);
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            move(f,c);refresh();
        }else if(c==23){//AL PRECIONAR CTRL W
            FilaGeneral=wherey();ColumnaGeneral=wherex();
            move(0,0);refresh();
            clear();
            endwin();
            BuscarRempl(InicioLDC);
            AgregarCambio(PalB,PalR);
            initscr();
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
        }else if(c==13){//AL PRECIONAR enter
            InsertarALFinal(InicioLDC,FinLDC,"enter");
            int fila=wherey()+1;
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            move(fila,0);refresh();
        }else if(c==19){//AL PRESIONAR CTRL S
            string ruta;
            move(0,0);refresh();
            clear();
            printw("INGRESE LA RUTA DEL ARCHIVO\n");
            printw("\n");
            scanw("%s",ruta);refresh();
            GuardarArchivo(ruta);
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            c=0;
        }else if(c==9){//AL PRECIONAR TAB
            printw("%c",' ');
            cadena=" ";
            InsertarALFinal(InicioLDC,FinLDC,cadena);
        }else if(c==25){//AL PRECIONAR CTRL Y
            QuitarPilaRevertido();
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
        }else if(c==1){//AL PRECIONAR CTRL Z
            QuitarCambio();
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
        }else if(c==3){//AL PRECIONAR CTRL C
            clear();
            move(0,0);refresh();
            MenReportes();
            endwin();
            initscr();
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
        }else{
            printw("%c",c);
            cadena=char(c);
            InsertarALFinal(InicioLDC,FinLDC,cadena);
        }
        refresh();
    }while(c!=24);
    cout<<"FIN DEL PROGRAMA...";
    system("pause");
    Permiso=false;
    endwin();
}


string Menu(){
    string Opcion;
    cout<<"UNIVERSIDAD SAN CARLOS DE GUATEMALA\n";
    cout<<"FACULTAD DE INGENIERIA\n";
    cout<<"ESTRUCTURA DE DATOS\n";
    cout<<"PRACTICA 1\n";
    cout<<"RANDY ALEXANDER CAN AJUCHAN\n";
    cout<<"201801527\n";

    cout<<"     MENU:\n";
    cout<<"     1. CREAR ARCHIVO\n";
    cout<<"     2. ABRIR ARCHIVO\n";
    cout<<"     3. ARCHIVOS RECIENTES\n";
    cout<<"     4. SALIR\n";
    cin>>Opcion;
    return Opcion;
}

void abrir_archivo(string ruta){
    InicioLDC=NULL;
    FinLDC=NULL;
    char letra;
    ifstream fichero;
    fichero.open(ruta.c_str());
    if(!fichero.fail()){
        fichero.get(letra);
        while(!fichero.eof()){
            string s(1,letra);
            if(letra=='\n'){InsertarALFinal(InicioLDC,FinLDC,"enter");}
            else{InsertarALFinal(InicioLDC,FinLDC,s);}
            fichero.get(letra);
        }
        AgregarRuta(ruta);
        fichero.close();
        Permiso=true;
    Editor();
    }
}

int _tmain(){
    ALTENTER();
    string Opcion;
    do{
        Opcion=Menu();
        if(Opcion=="1"){
            InicioLDC=NULL;
            FinLDC=NULL;
            Editor();
        }else if(Opcion=="2"){
            string ruta;
            cout<<"INGRESE LA RUTA DEL ARCHIVO TXT \n";
            cin>>ruta;
            abrir_archivo(ruta);
        }else if(Opcion=="3"){
            mostrarArchivosRecientes();
        }
    }while(Opcion!="4");
    cout<<"PROGRAMA FINALIZADO";
    return 0;
}


