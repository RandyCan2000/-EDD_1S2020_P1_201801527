#include <curses.h>
#include <iostream>
#include <string>
#include <windows.h>
#define _tmain main
#include <stdio.h>
#include <string.h>
#include <fstream>
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
    string Reporte1;
    string Reporte2;
    string Reporte3;
struct Rutas *sig=NULL;};
typedef struct Rutas *R;
R InicioRutasLCS=NULL;
R FinRutasLCS=NULL;
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
    LCD Aux=Inicio;
    LCD PRIMERCARACTER;
    string PB,PR;
    cout<<"INGRESE LA PALABRA A REEMPLAZAR Y LUEGO LA PALABRA CON LA QUE SE REEMPLAZARA\n";
    cout<<"EJEMPLO:(HOLA HOLA2)";
    cin>>PB>>PR;
    string PalEncontrada="";
    while(true){
        if(Aux->Caracter==" "||Aux->Caracter=="enter"){
            if(PalEncontrada==PB){
                RemplazarNodo_Agregar(PRIMERCARACTER,Aux,PR);
            }
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
        }else if(c==24){//AL PRESIONAR CTRL X
            PalB="hola";
            PalR="Resp";
            //BuscarRempl(InicioLDC);
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
        }else if(c==9){//AL PRECIONAR TAB
            printw("%c",' ');
            cadena=" ";
            InsertarALFinal(InicioLDC,FinLDC,cadena);
        }else{
            printw("%c",c);
            cadena=char(c);
            InsertarALFinal(InicioLDC,FinLDC,cadena);
        }
        refresh();
    }while(c!=20);
    cout<<"FIN DEL PROGRAMA...";
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

        }
    }while(Opcion!="4");
    cout<<"PROGRAMA FINALIZADO";
    return 0;
}


