#include <curses.h>
#include <iostream>
#include <string>
#include <windows.h>
#define _tmain main
#include <stdio.h>
#include <string.h>
using namespace std;
int FilaGeneral;
int ColumnaGeneral;
int Ancho;
int Alto;
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
    move(0,0);refresh();
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

void SplitBR(string PALABRA){
    string delimiter = ";";
    size_t pos = 0;
    string token;
    pos = PALABRA.find(delimiter);
    token = PALABRA.substr(0, pos);
    PalB=token;
    PALABRA.erase(0, pos + delimiter.size());
    PalR=PALABRA;
}


void RecargarEditor(){
    initscr();
    move(0,0);
    refresh();
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
void RemplazarNodo_Agregar(LCD &PrimerLetra,LCD &Espacio2){
    LCD Espacio=PrimerLetra->ant;
    for(int i=0;i<PalR.size();i++){
        LCD Nuevo=new (struct Caracteres);
        Nuevo->Caracter=PalR[i];
        Espacio->sig=Nuevo;
        Nuevo->ant=Espacio;
        Nuevo->sig=Espacio2;
        Espacio2->ant=Nuevo;
        Espacio=Nuevo;
    }
}
void BuscarRempl(){
    LCD Aux=InicioLDC;
    LCD PrimLetra;
    int ContarEspacios=0;
    string PalEncontrada="";
    while(true){
        if(Aux->Caracter==" "||Aux->Caracter=="enter"){
            if(PalEncontrada==PalB){
                RemplazarNodo_Agregar(PrimLetra,Aux);
            }
            PalEncontrada="";
        }else{
            if(PalEncontrada==""){PrimLetra=Aux;}
            PalEncontrada+=Aux->Caracter;
        }
        Aux=Aux->sig;
        if(Aux==NULL){break;}
    }
}


void Editor(){
    ALTENTER();
    Resolucion();
    string PALBR;
    initscr();
    MenuOpcionEditor();
    int c;
    int blanco=32;
    int columna;
    int fila;
    string cadena;
    string BuscarRemplazar;
    do{
        c=getchar();
        if(c==8){
            if(FinLDC->Caracter=="enter"){BorrarUltimo(InicioLDC,FinLDC);}
            int f=FinLDC->Fila;
            int c=FinLDC->Columna;
            BorrarUltimo(InicioLDC,FinLDC);
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            move(f,c);refresh();
        }else if(c==16){
            FilaGeneral=wherey();ColumnaGeneral=wherex();
            system("cls");
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            move(FilaGeneral,ColumnaGeneral);
            refresh();
        }
        else if(c==23){
            FilaGeneral=wherey();ColumnaGeneral=wherex();
            move(0,0);refresh();
            clear();
            printw("INGRESE LA PALABRA A REEMPLAZAR Y LUEGO LA PALABRA CON LA QUE SE REEMPLAZARA\n");
            printw("\n");
            scanw("%s ; %s",PalB,PalR);
            //SplitBR(PALBR);
            //METODO BUSCAR Y REEMPLAZAR
            BuscarRempl();
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
        }else if(c==13){
            InsertarALFinal(InicioLDC,FinLDC,"enter");
            int fila=wherey()+1;
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            move(fila,0);refresh();
        }else if(c==24){
            PalB="hola";
            PalR="adop";
            BuscarRempl();
        }else{
            printw("%c",c);
            cadena=char(c);
            InsertarALFinal(InicioLDC,FinLDC,cadena);
        }
        refresh();
    }while(c!=20);
    cout<<"FIN DEL PROGRAMA...";
    endwin();
}


int Menu(){
    int Opcion;
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

int _tmain(){
    int Opcion;
    do{
        Opcion=Menu();
        if(Opcion==1){
            Editor();
        }else if(Opcion==2){

        }else if(Opcion==3){
        }
    }while(Opcion!=4);

    return 0;
}


