#include <iostream>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include "latticeview.h"

//#define N 13            //Lateral number of cells (deve ser ímpar!)
#define ImageWidth 90  //image width
#define ImageHeight 90 //image height
#define max_iter 16

using namespace std;


int main ( ) 
        {
    double prob;

    int N=9;
    int lat[N*N];
    int iter=0, cam_curto=0, inatividade=0, percolados=0, soma_caminhos=0, soma_queima_perc =0, soma_cam_quad =0;
    char filename[160];
    int img_no=1;
    int i, j, n, estat, percent_success;
    bool evaluate= true;
    srand48(time(0));
    
    for (i=0;i<pow(10,6);i++){drand48();}

    //for (; N>8; N-=2){



    for (n=0; n<N*N; n++){
        if(n%2==0){
            lat[n]=0;
        }
        else{
            lat[n]=3;
        }
            }

    
    int p_ini = (N/4);
    int X_1 = p_ini*N+p_ini; //1 é Pursuer move em 4 direçoes (média de passo: 1)
    lat[X_1]=1;

    int X_2 = (N-p_ini-1)*N+N-p_ini-1; //2 é Evader move em 8 direçoes (média de passo: (1+sqrt(2))/2=1.20711)
    

    lat[X_2]=2;

    int colisoes_1=0;
    int colisoes_2=0;
    double a_1, a_2;


            sprintf(filename,"test%d.ppm",img_no);
            Print_lattice (lat, N, N, ImageWidth, ImageHeight, filename);
            img_no++;

    int game = 2;
    switch(game){                                       
                                                            //game 1: random-walk4 Pursuer, random-walk8 Evader
                                                            //game 2: perfect knowledge Pursuer, r&&om-walk8 Evader
        case 1:{                                        

    while (iter<max_iter && evaluate==true){
        //ação do Pursuer
        if (X_1%2==0)
            {lat[X_1]=0;}
        else {lat[X_1]=3;}

        a_1 = drand48();
        if (a_1<0.25) {
            if (X_1%N==0) {X_1+=1; colisoes_1+=1;}
            else {X_1-=1;}
        }
        else if (a_1<0.5) {
            if ((X_1+N)>(N*N-1)) {X_1-=N; colisoes_1+=1;}
            else {X_1+=N;}
        }
        else if (a_1<0.75) {
            if ((X_1+1)%N==0) {X_1-=1; colisoes_1+=1;}
            else {X_1+=1;}
        }
        else {
            if ((X_1-N)<0) {X_1+=N; colisoes_1+=1;}
            else {X_1-=N;}
        }
        lat[X_1]=1;


        if (X_2%2==0)
            {lat[X_2]=0;}
        else {lat[X_2]=3;}

        //ação do Evader
        a_2 = drand48();
        if (a_2<0.125) {                    //passo para esquerda
            if (X_2%N==0) {X_2+=1;}            //se parede esquerda
            else {X_2-=1;}
        }
        else if (a_2<0.25) {                //passo para diagonal sup esq
            if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {}    //se canto superior direito, fica no lugar
            else if ((X_2+N)>(N*N-1)) {X_2-=1;}                   //se parede de cima, passo para esquerda
            else if (X_2%N==0) {X_2+=N;}                              //se parede esquerda, passo para cima
            else {X_2+=(N-1);}                                  
        }
        else if (a_2<0.375) {                   //passo para cima
            if ((X_2+N)>(N*N-1)) {}      //se parede cima, fica no lugar
            else {X_2+=N;}
        }
        else if (a_2<0.5) {                     //passo diagonal sup dir
            if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {} //se canto sup dir, fica no lugar
            else if ((X_2+N)>(N*N-1)) {X_2+=1;}             //se parede de cima, passo para a direita
            else if ((X_2+1)%N==0) {X_2+=N;}                //se parede direita, passo para cima
            else {X_2+=(N+1);}
        }
        else if (a_2<0.625) {                   //passo direita
            if ((X_2+1)%N==0) {}        //se parede direita fica no lugar
            else {X_2+=1;}
        }
        else if (a_2<0.75) {                    //passo diagonal inf dir
            if (((X_2-N)<0) && ((X_2+1)%N==0)) {} //se canto inf dir, fica no lugar
            else if ((X_2+1)%N==0) {X_2-=N;}            //se parede direita, passo para baixo
            else if ((X_2-N)<0) {X_2+=1;}               //se parede baixo, passo para direita
            else {X_2-=(N-1);}
        }
        else if (a_2<0.875) {                   //passo para baixo
            if ((X_2-N)<0) {}           //se parede de baixo, fica no lugar
            else {X_2-=N;}
        }
        else {                                  //passo diagonal inf esq
            if (((X_2-N)<0) && (X_2%N==0)) {} //se canto inf esq, fica no lugar
            else if ((X_2-N)<0) {X_2-=1;}           //se parede de baixo, passo para esquerda
            else if (X_2%N==0) {X_2-=N;}            //se parede esquerda, passo para baixo
            else {X_2-=(N+1);}
        }
        lat[X_2]=2;
 
            
            sprintf(filename,"test%d.ppm",img_no);
            Print_lattice (lat, N, N, ImageWidth, ImageHeight, filename);
            img_no++;

            iter++;

        if ((X_1==X_2) || ((X_1==X_2+1) && (X_1%N!=0)) || ((X_1==X_2-1) && (X_2%N!=0)) || (X_1==X_2+N) || (X_1==X_2-N)) {evaluate=false;}
        
    }
        break;}

        case 2:{                                                //Pursuer persigue, Evader R&&om-walk

        int p_E_para_P = 1;                                        //passos dados por Evader para cada passo dado por Pursuer
        int y_1, y_2, d_x, d_y, y_12, y_22, d_x2, d_y2;
        double dist_toma_consc_quad = 10.0;                            //distância à qual E2 passa a ter conhecimento da posição de P1 (ao quadrado)
        percent_success = 0;

        for (estat=0; estat<1; estat++){
            if (X_1%2==0)                               //apagar rastro
                {lat[X_1]=0;}
            else {lat[X_1]=3;}

            if (X_2%2==0)
                {lat[X_2]=0;}
            else {lat[X_2]=3;}

            X_1 = p_ini*N+p_ini;            //reinicializar X1
            lat[X_1]=1;

            X_2 = (N-p_ini-1)*N+N-p_ini-1; //reinicializar X2
            lat[X_2]=2;

            iter =0;
            evaluate=true;

            while (iter<max_iter && evaluate==true){
                                                                //ação do Pursuer
            if (X_1%2==0)                               //apagar rastro
                {lat[X_1]=0;}
            else {lat[X_1]=3;}
            
            y_2 = (X_2/N);
            y_1 = (X_1/N);
            if ((y_2!=0) && (y_1!=0)){d_x = X_2%(N*y_2)-X_1%(N*y_1);}
            else if ((y_2==0) && (y_1==0)){d_x = X_2-X_1;}
            else if (y_2==0){d_x = X_2-X_1%(N*y_1);}
            else {d_x = X_2%(N*y_2)-X_1;}
            d_y = y_2-y_1;


            if (abs(d_x) > abs(d_y)) {
                if (d_x<0) {
                    X_1-=1;
                    }
                else {
                    X_1+=1;
                }
                }
            else {
                if (d_y<0){
                    X_1-=N;
                }
                else {
                    X_1+=N;
                }
            }
            
            lat[X_1]=1;

            if (X_2%2==0)
                {lat[X_2]=0;}
            else {lat[X_2]=3;}

            

            if ((pow(d_x,2)+pow(d_y,2))<=dist_toma_consc_quad){       //opção em que E apercebe-se que P está perto
                if ((d_x>0) && (d_y>0)) {          //evaluate up, right, diagonal up right
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {     //se canto sup dir, vai para esq ou para baixo
                        if (abs(d_x)<abs(d_y)) {X_2-=1;}
                        else {X_2-=N;}
                        } 
                    else if ((X_2+N)>(N*N-1)) {X_2+=1;}             //se parede de cima, passo para a direita
                    else if ((X_2+1)%N==0) {X_2+=N;}                //se parede direita, passo para cima
                    else {X_2+=(N+1);}
                }
                else if ((d_x>0) && (d_y<0)) {     //evaluate down, right, diagonal
                    if (((X_2-N)<0) && ((X_2+1)%N==0)) {       //se canto inf dir, vai para cima ou para esq
                        if (abs(d_x)<abs(d_y)) {X_2-=1;}
                        else {X_2+=N;}
                        }
                    else if ((X_2+1)%N==0) {X_2-=N;}            //se parede direita, passo para baixo
                    else if ((X_2-N)<0) {X_2+=1;}               //se parede baixo, passo para direita
                    else {X_2-=(N-1);}
                }
                else if ((d_x<0) && (d_y<0)) {     //evaluate down, left, diagonal
                    if (((X_2-N)<0) && (X_2%N==0)) {           //se canto inf esq, vai para cima ou para dir
                        if (abs(d_x)<abs(d_y)) {X_2+=1;}
                        else {X_2+=N;} 
                        } 
                    else if ((X_2-N)<0) {X_2-=1;}               //se parede de baixo, passo para esquerda
                    else if (X_2%N==0) {X_2-=N;}                //se parede esquerda, passo para baixo
                    else {X_2-=(N+1);}
                }
                else if ((d_x==0) && (d_y<0)){                     //se alinhados na vertical com E em baixo
                    if (((X_2-N)<0) && (X_2%N==0)) {X_2+=1;}
                    else if (((X_2-N)<0) && ((X_2+1)%N==0)) {X_2-=1;}
                    else { 
                        if (X_2%N<N/2){
                            if ((X_2-N)<0) {X_2+=1;}
                            else {X_2-=(N-1);}
                        }
                        else {
                            if ((X_2-N)<0) {X_2-=1;}
                            else {X_2-=(N+1);}
                        }
                }
                }
                else if ((d_x==0) && (d_y>0)){                     //se alinhados na vertical com E em cima
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {X_2-=1;}
                    else if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {X_2+=1;}
                    else { 
                        if (X_2%N<N/2) {
                            if ((X_2+N)>(N*N-1)){X_2+=1;}
                            else {X_2+=N+1;}
                        }
                        else {
                            if ((X_2+N)>(N*N-1)){X_2-=1;} 
                            else {X_2+=(N-1);}
                        }
                }
                }
                else if ((d_x<0) && (d_y==0)){                     //se alinhados na horizontal com E à esquerda
                    if (((X_2-N)<0) && (X_2%N==0)) {X_2+=N;}
                    else if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {X_2-=N;}
                    else { 
                        if (X_2/N<N/2){
                            if (X_2%N==0) {X_2+=N;} 
                            else {X_2+=(N-1);}
                        }
                        else {
                            if (X_2%N==0) {X_2-=N;}
                            else {X_2-=(N-1);}
                        }
                }
                }
                else if ((d_x>0) && (d_y==0)){                     //se alinhados na horizontal com E à direita
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {X_2-=N;}
                    else if (((X_2-N)<0) && ((X_2+1)%N==0)) {X_2+=N;}
                    else {
                        if (X_2/N<N/2){
                            if ((X_2+1)%N==0) {X_2+=N;}
                            else {X_2+=(N+1);}
                        }
                        else {
                            if ((X_2+1)%N==0) {X_2-=N;}
                            else {X_2-=(N-1);}
                        }
                }
                }
                else {                          //evaluate up, left, diagonal
                if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {     //se canto superior esq, vaia para dir ou para baixo
                        if (abs(d_x)<abs(d_y)) {X_2+=1;}
                        else {X_2-=N;}}    
                    else if ((X_2+N)>(N*N-1)) {X_2-=1;}                   //se parede de cima, passo para esquerda
                    else if (X_2%N==0) {X_2+=N;}                          //se parede esquerda, passo para cima
                    else {X_2+=(N-1);}    
                }


            }

            else {

                //ação do Evader
                a_2 = drand48();
                if (a_2<0.125) {                    //passo para esquerda
                    if (X_2%N==0) {X_2+=1;}            //se parede esquerda
                    else {X_2-=1;}
                }
                else if (a_2<0.25) {                //passo para diagonal sup esq
                    if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {}    //se canto superior esq, fica no lugar
                    else if ((X_2+N)>(N*N-1)) {X_2-=1;}                   //se parede de cima, passo para esquerda
                    else if (X_2%N==0) {X_2+=N;}                              //se parede esquerda, passo para cima
                    else {X_2+=(N-1);}                                  
                }
                else if (a_2<0.375) {                   //passo para cima
                    if ((X_2+N)>(N*N-1)) {}      //se parede cima, fica no lugar
                    else {X_2+=N;}
                }
                else if (a_2<0.5) {                     //passo diagonal sup dir
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {} //se canto sup dir, fica no lugar
                    else if ((X_2+N)>(N*N-1)) {X_2+=1;}             //se parede de cima, passo para a direita
                    else if ((X_2+1)%N==0) {X_2+=N;}                //se parede direita, passo para cima
                    else {X_2+=(N+1);}
                }
                else if (a_2<0.625) {                   //passo direita
                    if ((X_2+1)%N==0) {}        //se parede direita fica no lugar
                    else {X_2+=1;}
                }
                else if (a_2<0.75) {                    //passo diagonal inf dir
                    if (((X_2-N)<0) && ((X_2+1)%N==0)) {} //se canto inf dir, fica no lugar
                    else if ((X_2+1)%N==0) {X_2-=N;}            //se parede direita, passo para baixo
                    else if ((X_2-N)<0) {X_2+=1;}               //se parede baixo, passo para direita
                    else {X_2-=(N-1);}
                }
                else if (a_2<0.875) {                   //passo para baixo
                    if ((X_2-N)<0) {}           //se parede de baixo, fica no lugar
                    else {X_2-=N;}
                }
                else {                                  //passo diagonal inf esq
                    if (((X_2-N)<0) && (X_2%N==0)) {} //se canto inf esq, fica no lugar
                    else if ((X_2-N)<0) {X_2-=1;}           //se parede de baixo, passo para esquerda
                    else if (X_2%N==0) {X_2-=N;}            //se parede esquerda, passo para baixo
                    else {X_2-=(N+1);}
                }
            }

            lat[X_2]=2;
    
                
            sprintf(filename,"test%d.ppm",img_no);
            Print_lattice (lat, N, N, ImageWidth, ImageHeight, filename);
            img_no++;

            iter++;

            y_22 = X_2/N;
            y_12 = X_1/N;
            if ((y_22!=0) && (y_12!=0)){d_x2 = X_2%(N*y_22)-X_1%(N*y_12);}
            else if ((y_22==0) && (y_12==0)){d_x2 = X_2-X_1;}
            else if (y_22==0){d_x2 = X_2-X_1%(N*y_12);}
            else {d_x2 = X_2%(N*y_22)-X_1;}
            d_y2 = y_22-y_12;

            if (((d_y==(-d_y2)) && (d_x==d_x2)) || ((d_x==(-d_x2)) && (d_y==d_y2))) {evaluate=false;}

            if ((X_1==X_2) or ((X_1==X_2+1) && (X_1%N!=0)) or ((X_1==X_2-1) && (X_2%N!=0)) or (X_1==X_2+N) or (X_1==X_2-N)) {evaluate=false;}
            
        i=0;
        while (i<(p_E_para_P-1) && evaluate==true){

            y_2 = X_2/N;
            y_1 = X_1/N;
            if ((y_2!=0) && (y_1!=0)){d_x = X_2%(N*y_2)-X_1%(N*y_1);}
            else if ((y_2==0) && (y_1==0)){d_x = X_2-X_1;}
            else if (y_2==0){d_x = X_2-X_1%(N*y_1);}
            else {d_x = X_2%(N*y_2)-X_1;}
            d_y = y_2-y_1;

            if (X_2%2==0)
                {lat[X_2]=0;}
            else {lat[X_2]=3;}

            if ((pow(d_x,2)+pow(d_y,2))<=dist_toma_consc_quad){       //opção em que E apercebe-se que P está perto
                if ((d_x>0) && (d_y>0)) {          //evaluate up, right, diagonal up right
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {     //se canto sup dir, vai para esq ou para baixo
                        if (abs(d_x)<abs(d_y)) {X_2-=1;}
                        else {X_2-=N;}} 
                    else if ((X_2+N)>(N*N-1)) {X_2+=1;}             //se parede de cima, passo para a direita
                    else if ((X_2+1)%N==0) {X_2+=N;}                //se parede direita, passo para cima
                    else {X_2+=(N+1);}
                }
                else if ((d_x>0) && (d_y<0)) {     //evaluate down, right, diagonal
                    if (((X_2-N)<0) && ((X_2+1)%N==0)) {       //se canto inf dir, vai para cima ou para esq
                        if (abs(d_x)<abs(d_y)) {X_2-=1;}
                        else {X_2+=N;} }
                    else if ((X_2+1)%N==0) {X_2-=N;}            //se parede direita, passo para baixo
                    else if ((X_2-N)<0) {X_2+=1;}               //se parede baixo, passo para direita
                    else {X_2-=(N-1);}
                }
                else if ((d_x<0) && (d_y<0)) {     //evaluate down, left, diagonal
                    if (((X_2-N)<0) && (X_2%N==0)) {           //se canto inf esq, vai para cima ou para dir
                        if (abs(d_x)<abs(d_y)) {X_2+=1;}
                        else {X_2+=N;} } 
                    else if ((X_2-N)<0) {X_2-=1;}               //se parede de baixo, passo para esquerda
                    else if (X_2%N==0) {X_2-=N;}                //se parede esquerda, passo para baixo
                    else {X_2-=(N+1);}
                }
                else if ((d_x==0) && (d_y<0)){                     //se alinhados na vertical com E em baixo
                    if (((X_2-N)<0) && (X_2%N==0)) {X_2+=1;}
                    else if (((X_2-N)<0) && ((X_2+1)%N==0)) {X_2-=1;}
                    else { 
                        if (X_2%N<N/2){
                            if ((X_2-N)<0) {X_2+=1;}
                            else {X_2-=(N-1);}
                        }
                        else {
                            if ((X_2-N)<0) {X_2-=1;}
                            else {X_2-=(N+1);}
                        }
                }
                }
                else if ((d_x==0) && (d_y>0)){                     //se alinhados na vertical com E em cima
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {X_2-=1;}
                    else if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {X_2+=1;}
                    else { 
                        if (X_2%N<N/2) {
                            if ((X_2+N)>(N*N-1)){X_2+=1;}
                            else {X_2+=N+1;}
                        }
                        else {
                            if ((X_2+N)>(N*N-1)){X_2-=1;} 
                            else {X_2+=(N-1);}
                        }
                }
                }
                else if ((d_x<0) && (d_y==0)){                     //se alinhados na horizontal com E à esquerda
                    if (((X_2-N)<0) && (X_2%N==0)) {X_2+=N;}
                    else if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {X_2-=N;}
                    else { 
                        if (X_2/N<N/2){
                            if (X_2%N==0) {X_2+=N;} 
                            else {X_2+=(N-1);}
                        }
                        else {
                            if (X_2%N==0) {X_2-=N;}
                            else {X_2-=(N-1);}
                        }
                }
                }
                else if ((d_x>0) && (d_y==0)){                     //se alinhados na horizontal com E à direita
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {X_2-=N;}
                    else if (((X_2-N)<0) && ((X_2+1)%N==0)) {X_2+=N;}
                    else {
                        if (X_2/N<N/2){
                            if ((X_2+1)%N==0) {X_2+=N;}
                            else {X_2+=(N+1);}
                        }
                        else {
                            if ((X_2+1)%N==0) {X_2-=N;}
                            else {X_2-=(N-1);}
                        }
                }
                }
                else {                          //evaluate up, left, diagonal
                if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {     //se canto superior esq, vaia para dir ou para baixo
                        if (abs(d_x)<abs(d_y)) {X_2+=1;}
                        else {X_2-=N;}}    
                    else if ((X_2+N)>(N*N-1)) {X_2-=1;}                   //se parede de cima, passo para esquerda
                    else if (X_2%N==0) {X_2+=N;}                          //se parede esquerda, passo para cima
                    else {X_2+=(N-1);}    
                }

            }

            else {

                //ação do Evader
                a_2 = drand48();
                if (a_2<0.125) {                    //passo para esquerda
                    if (X_2%N==0) {X_2+=1;}            //se parede esquerda
                    else {X_2-=1;}
                }
                else if (a_2<0.25) {                //passo para diagonal sup esq
                    if (((X_2+N)>(N*N-1)) && (X_2%N==0)) {}    //se canto superior direito, fica no lugar
                    else if ((X_2+N)>(N*N-1)) {X_2-=1;}                   //se parede de cima, passo para esquerda
                    else if (X_2%N==0) {X_2+=N;}                              //se parede esquerda, passo para cima
                    else {X_2+=(N-1);}                                  
                }
                else if (a_2<0.375) {                   //passo para cima
                    if ((X_2+N)>(N*N-1)) {}      //se parede cima, fica no lugar
                    else {X_2+=N;}
                }
                else if (a_2<0.5) {                     //passo diagonal sup dir
                    if (((X_2+1)%N==0) && ((X_2+N)>(N*N-1))) {} //se canto sup dir, fica no lugar
                    else if ((X_2+N)>(N*N-1)) {X_2+=1;}             //se parede de cima, passo para a direita
                    else if ((X_2+1)%N==0) {X_2+=N;}                //se parede direita, passo para cima
                    else {X_2+=(N+1);}
                }
                else if (a_2<0.625) {                   //passo direita
                    if ((X_2+1)%N==0) {}        //se parede direita fica no lugar
                    else {X_2+=1;}
                }
                else if (a_2<0.75) {                    //passo diagonal inf dir
                    if (((X_2-N)<0) && ((X_2+1)%N==0)) {} //se canto inf dir, fica no lugar
                    else if ((X_2+1)%N==0) {X_2-=N;}            //se parede direita, passo para baixo
                    else if ((X_2-N)<0) {X_2+=1;}               //se parede baixo, passo para direita
                    else {X_2-=(N-1);}
                }
                else if (a_2<0.875) {                   //passo para baixo
                    if ((X_2-N)<0) {}           //se parede de baixo, fica no lugar
                    else {X_2-=N;}
                }
                else {                                  //passo diagonal inf esq
                    if (((X_2-N)<0) && (X_2%N==0)) {} //se canto inf esq, fica no lugar
                    else if ((X_2-N)<0) {X_2-=1;}           //se parede de baixo, passo para esquerda
                    else if (X_2%N==0) {X_2-=N;}            //se parede esquerda, passo para baixo
                    else {X_2-=(N+1);}
                }
            }

            lat[X_2]=2;
    
                
            sprintf(filename,"test%d.ppm",img_no);
            Print_lattice (lat, N, N, ImageWidth, ImageHeight,filename);
            img_no++;
            i++;

            y_22 = X_2/N;
            y_12 = X_1/N;
            if ((y_22!=0) && (y_12!=0)){d_x2 = X_2%(N*y_22)-X_1%(N*y_12);}
            else if ((y_22==0) && (y_12==0)){d_x2 = X_2-X_1;}
            else if (y_22==0){d_x2 = X_2-X_1%(N*y_12);}
            else {d_x2 = X_2%(N*y_22)-X_1;}
            d_y2 = y_22-y_12;

            if (((d_y==(-d_y2)) && (d_x==d_x2)) || ((d_x==(-d_x2)) && (d_y==d_y2))) {evaluate=false;}


            if ((X_1==X_2) || ((X_1==X_2+1) && (X_1%N!=0)) || ((X_1==X_2-1) && (X_2%N!=0)) || (X_1==X_2+N) || (X_1==X_2-N)) {evaluate=false;}
            }
            }
            if (iter<(max_iter-1)) {percent_success+=1;}
    }
    cout << N << "\t" << (double)percent_success/(double)10 << endl;
        //}
        break;}
    }

    
    

        return 0;
        }