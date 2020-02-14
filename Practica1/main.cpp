
#include <curses.h>
#include <iostream>
#include <string>
#include <windows.h>
#define _tmain main
using namespace std;

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

//          Estructuras
struct Caracteres{
	  string Caracter;
	  struct Caracteres *sig=NULL;
	  struct Caracteres *ant=NULL;};
typedef struct Caracteres *LCD;

//                      LISTA DOBLE CARACTERES

void InsertarALFinal(LCD &Inicio, LCD &Fin, string Caracter){
    LCD Nuevo=new (struct Caracteres);
    Nuevo->Caracter=Caracter;
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
        if(Aux->Caracter=="enter"){cout<<"\n";}
        else{cout<<Aux->Caracter;}
        Aux=Aux->sig;
        if(Aux==NULL){break;}
    }
}

LCD InicioLDC=NULL;
LCD FinLDC=NULL;
int _tmain(){
    initscr();
    int c;
    int blanco=32;
    string cadena;
    string BuscarRemplazar;
    do{
        c=getchar();
        if(c==8){
            cout<<" ";
        }else if(c==13){
            InsertarALFinal(InicioLDC,FinLDC,"enter");
            clear();
            int fila=wherey()+1;
            ImprimirLista(InicioLDC);
            move(fila,0);
            refresh();
        }else{
            cadena=char(c);
            InsertarALFinal(InicioLDC,FinLDC,cadena);
        }
    }while(c!=20);
    cout<<"FIN DEL PROGRAMA...";
    system("pause");
    endwin();
    return 0;
}


