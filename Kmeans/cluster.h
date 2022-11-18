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
#define NUM_CENTROIDI 40
#endif
using namespace std;
double inline distance(double d,double x1, double y1, double x2, double y2) { //calcolo la distanza di Minkowski di grado d
    double xdist= abs(x1-x2);
    double ydist= abs(y1-y2);
    return pow(pow(xdist,d)+pow(ydist,d),1/d);
}

double randgen(){ //funzione che restituisce un numero casuale nell'intervallo da me desiderato
    return(((double) rand() / RAND_MAX) * MAX_VAL);
}
