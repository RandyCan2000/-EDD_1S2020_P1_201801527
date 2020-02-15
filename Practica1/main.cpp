
#include <curses.h>

#include <iostream>
#include <string>
#include <windows.h>
#define _tmain main
using namespace std;
int FilaGeneral;
int ColumnaGeneral;
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
	  int Fila;
	  int Columna;
	  struct Caracteres *sig=NULL;
	  struct Caracteres *ant=NULL;};
typedef struct Caracteres *LCD;

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
        if(Aux->Caracter=="enter"){cout<<"\n";}
        else{cout<<Aux->Caracter;}
        Aux->Fila=wherey();
        Aux->Columna=wherex();
        Aux=Aux->sig;
        if(Aux==NULL){
                move(FilaGeneral,ColumnaGeneral);refresh();
                break;}
    }
}

LCD InicioLDC=NULL;
LCD FinLDC=NULL;

void MenuOpcionEditor(){
    move(0,0);
    refresh();
    cout<<"^W BUSCAR Y REEMPLAZAR   ";
    cout<<"^C REPORTES  ";
    cout<<"^S GUARDAR   ";
    move(0,0);
    refresh();
}
void BuscarRemplMenu(){
    string PALBR;
    system("cls");
    printw("\n");
    printw("INGRESE LA PALABRA A REEMPLAZAR \n");
    printw("EJEMPLO(HOLA;HOLA2): ");
    scanw("%s",PALBR);
    system("pause");
    system("cls");
    MenuOpcionEditor();
    ImprimirLista(InicioLDC);
}
void gotoxy(int x, int y){
		   COORD coord;
		   coord.X = x;
		   coord.Y = y;
		   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);}
void Editor(){
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
            BorrarUltimo(InicioLDC,FinLDC);
            columna=wherex()-1;
            fila=wherey();
            if(columna==-1){
                LCD aux=FinLDC;
                fila=aux->Fila;
                columna=aux->Columna;
                if(aux->Caracter=="enter"){BorrarUltimo(InicioLDC,FinLDC);}
            }
            move(fila,columna);refresh();
            cout<<" ";refresh();
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
            BuscarRemplMenu();
        }else if(c==13){
            InsertarALFinal(InicioLDC,FinLDC,"enter");
            int fila=wherey()+1;
            clear();
            MenuOpcionEditor();
            ImprimirLista(InicioLDC);
            move(fila,0);refresh();
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


