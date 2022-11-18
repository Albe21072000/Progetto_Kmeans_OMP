#include "cluster.h"

struct points{
    vector<double>  x;
    vector<double> y;
    vector<int> cluster;
    vector<double> distcluster;
};

struct centroids{ //structure of array per contenere coordinate dei centroidi ed i lock per sovrascriverli
    vector<double> x;
    vector<double> y;
    vector<int> numpunti;
};

int main() {
    //srand(time(0));
    centroids centroidi{};
    points punti{};
#ifdef test
    printf("ciao\n");
   punti.x={3,4,5,6,7,6,7,8,3,2,3,2};
   punti.y={7,6,5,4,3,2,2,4,3,6,5,4};
   centroidi.x={4,4};
   centroidi.y={2,3};
#endif

#ifndef test
    int npunti=NUM_PUNTI;
    punti.x=*new vector<double>(npunti);
    punti.y=*new vector<double>(npunti);
    punti.distcluster=*new vector<double>(npunti);
    int numcentr=NUM_CENTROIDI;
    centroidi.x=*new vector<double>(numcentr);
    centroidi.y=*new vector<double>(numcentr);
    generate(punti.x.begin(), punti.x.end(), randgen);
    generate(punti.y.begin(), punti.y.end(), randgen);
    generate(centroidi.x.begin(), centroidi.x.end(), randgen);
    generate(centroidi.y.begin(), centroidi.y.end(), randgen);
#endif

    double start = omp_get_wtime();
    const unsigned long long n=punti.x.size();
    const unsigned long long k=centroidi.x.size();
    centroidi.numpunti=*new vector<int>(k);
    punti.distcluster=*new vector<double>(n);

    for(int i=0;i<k;i++){
        printf("(%f,%f) %d\n",centroidi.x[i],centroidi.y[i],i);
    }
    punti.cluster=*new vector<int>(n);
    std::fill_n(punti.cluster.begin(),n,-1);
    printf("ciao");
    auto cambi=-1;
    double sse=0.0;
    double distanza;
    int indmin=0;
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

            for (int i = 0; i < k; i++) { //azzero i centroidi precedenti
                centroidi.x[i] = 0;
                centroidi.y[i] = 0;
                centroidi.numpunti[i] = 0;
            }
            for (int i = 0; i < n; i++) { //calcolo i nuovi centroidi
                centroidi.x[punti.cluster[i]] += punti.x[i];
                centroidi.y[punti.cluster[i]] += punti.y[i];
                centroidi.numpunti[punti.cluster[i]] = centroidi.numpunti[punti.cluster[i]] + 1;
            }
            for (int i = 0; i < k; i++) {
                if(centroidi.numpunti[i]!=0) {
                    centroidi.x[i] = centroidi.x[i] / centroidi.numpunti[i];
                    centroidi.y[i] = centroidi.y[i] / centroidi.numpunti[i];
                } else{
                    unsigned long long j=(rand() / RAND_MAX) * n;
                    printf("entrato\n");
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

    cout<<sse<<" "<<cambi<<endl;
    std::cout << "Tempo necessario: " << end-start << " secondi "<< std::endl << std::endl;
    return 0;
}
