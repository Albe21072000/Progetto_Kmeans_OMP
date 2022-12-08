#include "Kmeans.h"

int main() {
   //srand(time(NULL));
    centroids centroidi{};
    points punti{};
#ifdef test
   punti.x={3,4,5,6,7,6,7,8,3,2,3,2};
   punti.y={7,6,5,4,3,2,2,4,3,6,5,4};
   centroidi.x={4,4};
   centroidi.y={2,3};
#endif

#ifndef test
    int npunti=NUM_PUNTI;   //scelgo il numero di punti da generare casualmente
    punti.x.reserve(npunti);
    punti.y.reserve(npunti);
    punti.distcluster.reserve(npunti);
    int numcentr=NUM_CENTROIDI; //scelgo il numero di centroidi da generare casualmente (e quindi i cluster a cui assegnare i miei punti
    centroidi.x.reserve(numcentr);
    centroidi.y.reserve(numcentr);
    for(int i=0;i<npunti;i++){
        punti.x.push_back(randgen());
        punti.y.push_back(randgen());
    }
    for(int i=0;i<numcentr;i++){
        centroidi.x.push_back(randgen());
        centroidi.y.push_back(randgen());
    }
#endif
    const unsigned long long n=punti.x.size();  //mi ricavo il numero di punti
    const unsigned long long k=centroidi.x.size(); //mi ricavo il numero di cluster
    for(int i=0;i<k;i++){  //stampo i centroidi iniziali scelti casualmente
        printf("(%f,%f) %d\n",centroidi.x[i],centroidi.y[i],i);
    }
    double start = omp_get_wtime();  //faccio partire il contatore per il tempo dopo aver generato i miei dati su cui lavorare
    vector<double> newx(k);
    vector<double> newy(k);
    centroidi.numpunti.reserve(k);
    punti.distcluster.reserve(n);
    punti.cluster.reserve(n);
    std::fill_n(punti.cluster.begin(),n,-1); //Inserisco -1 come valore del cluster di appartenenza iniziale dei punti
    // per indicare che inizialmente non appartengono ad alcun gruppo
    int indmin;
    auto cambi=-1;
    double sse=0.0;
    double distanza;
    while(cambi!=0) {
        cambi=0;
        {
            for (int i = 0; i < n; i++) { //calcolo distanza tra un punto ed ogni centroide e scelgo quella minima per ogni punto
                punti.distcluster[i]=MAX_VAL*1.5;
                for (int j = 0; j < k; j++) {
                    distanza=distance(2, punti.x[i], punti.y[i], centroidi.x[j], centroidi.y[j]);
                    if(distanza<=punti.distcluster[i]){
                        punti.distcluster[i]=distanza;
                        indmin=j;
                    }
                }
                if(punti.cluster[i] != indmin) {
                    cambi += 1;
                }
                punti.cluster[i] = indmin;
            }
            std::fill_n(centroidi.x.begin(),k,0);
            std::fill_n(centroidi.y.begin(),k,0);
            std::fill_n(centroidi.numpunti.begin(),k,0);
            for (int i = 0; i < n; i++) { //calcolo i nuovi centroidi
                centroidi.x[punti.cluster[i]] += punti.x[i];
                centroidi.y[punti.cluster[i]] += punti.y[i];
                centroidi.numpunti[punti.cluster[i]]++;
            }
            for (int i = 0; i < k; i++) {
                if(centroidi.numpunti[i]!=0) {
                    centroidi.x[i] = centroidi.x[i] / centroidi.numpunti[i];
                    centroidi.y[i] = centroidi.y[i] / centroidi.numpunti[i];
                } else{
                    unsigned long long j=(rand() / RAND_MAX) * n;
                    centroidi.x[i]=punti.x[j];
                    centroidi.y[i]=punti.y[j];
                }
            }
        }
    }
    for (int i = 0; i < n; i++)  //calcolo la somma degli errori al quadrato
        sse = sse + pow(punti.distcluster[i], 2);
    double end = omp_get_wtime( );
    for(int i=0;i<k;i++){
        printf("(%f,%f) %d\n",centroidi.x[i],centroidi.y[i],i);
    }
    //stampa facoltativa per graficare i risultati
#ifdef output
    std::ofstream myfile;
    myfile.open("outputseq.csv");
    myfile << "x,y,c" << std::endl;
    for (int i=0; i<n;i++) {
        myfile << punti.x[i] << "," << punti.y[i] << "," << punti.cluster[i]<< std::endl;
    }
    for (int j=0; j<k;j++) {
        myfile << centroidi.x[j] << "," << centroidi.y[j] << "," << "Centroide "<< j<< std::endl;
    }

    myfile.close();
#endif
    cout<<"Somma degli errorei al quadrato (SSE): "<<sse<<endl;
    std::cout << "Tempo necessario: " << end-start << " secondi "<< std::endl << std::endl;
    return 0;
}
