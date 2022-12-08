#include <iostream>
#include<algorithm>
#include <cmath>
#include<omp.h>
#include <cstdlib>
#include <vector>
#define output
#define MAX_VAL 1000000
#ifdef output
#include <fstream>   // Per poter scrivere i risultati
#endif
#ifndef test
#define NUM_PUNTI 1000000
#define NUM_CENTROIDI 10
#endif
using namespace std;
struct points{       //structure of array per contenere le coordinate dei punti, il cluster di appartenenza e la distanza dal centroide di tale gruppo
    vector<double>  x;
    vector<double> y;
    vector<int> cluster;
    vector<double> distcluster;
};
struct centroids{ //structure of array per contenere le coordinate dei centroidi e il numero dei punti assegnati al rispettivo cluster
    vector<double> x;
    vector<double> y;
    vector<int> numpunti;
};
double inline distance(double d,double x1, double y1, double x2, double y2) { //Metodo per il calcolo della distanza di Minkowski di grado d
    double xdist= abs(x1-x2);
    double ydist= abs(y1-y2);
    return pow(pow(xdist,d)+pow(ydist,d),1/d);
}

double randgen(){ //funzione che restituisce un numero casuale nell'intervallo da me desiderato
    return(((double) rand() / RAND_MAX) * MAX_VAL);
}

