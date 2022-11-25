#include "Kmeans.h"

int main() {
    //srand(time(NULL)); //imposto un seed per la generazione dei numeri casuali
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
    punti.x=*new vector<double>(npunti);
    punti.y=*new vector<double>(npunti);
    punti.distcluster=*new vector<double>(npunti);
    int numcentr=NUM_CENTROIDI; //scelgo il numero di centroidi da generare casualmente (e quindi i cluster a cui assegnare i miei punti
    centroidi.x=*new vector<double>(numcentr);
    centroidi.y=*new vector<double>(numcentr);
    generate(punti.x.begin(), punti.x.end(), randgen);   //riempio i miei vettori delle coordinate con valori casuali
    generate(punti.y.begin(), punti.y.end(), randgen);
    generate(centroidi.x.begin(), centroidi.x.end(), randgen);
    generate(centroidi.y.begin(), centroidi.y.end(), randgen);
#endif
    const unsigned long long n=punti.x.size();  //mi ricavo il numero di punti
    const unsigned long long k=centroidi.x.size(); //mi ricavo il numero di cluster
    for(int i=0;i<k;i++){  //stampo i centroidi iniziali scelti casualmente
        printf("(%f,%f) %d\n",centroidi.x[i],centroidi.y[i],i);
    }
    double start = omp_get_wtime();  //faccio partire il contatore per il tempo dopo aver generato i miei dati su cui lavorare
    vector<double> newx=*new vector<double>(k);
    vector<double> newy=*new vector<double>(k);
    centroidi.numpunti=*new vector<int>(k);
    punti.distcluster=*new vector<double>(n);
    punti.cluster=*new vector<int>(n);
    std::fill_n(punti.cluster.begin(),n,-1); //Inserisco -1 come valore del cluster di appartenenza iniziale dei punti
    // per indicare che inizialmente non appartengono ad alcun gruppo
    int indmin;
    auto cambi=-1;
    double sse=0.0;
    double distanza;
    while(cambi!=0) {
        cambi=0;
#pragma omp parallel default(none) shared(newx,newy,punti, centroidi, n, k, cambi,sse) private(indmin, distanza)
        {
#pragma omp for
            for (int i = 0; i < k; i++) { //azzero i centroidi precedenti
                newx[i] = 0;
                newy[i] = 0;
                centroidi.numpunti[i] = 0;
            }
#pragma omp for reduction(+:cambi)
            for (int i = 0; i < n; i++) { //calcolo distanza tra un punto e ogni centroide e scelgo quella minima per ogni punto
                punti.distcluster[i]=MAX_VAL*1.5;      //inserisco la massima distanza possibile tra due punti generati casualmente come valore iniziale
#pragma omp simd
                for (int j = 0; j < k; j++) {
                    distanza=distance(2, punti.x[i], punti.y[i], centroidi.x[j], centroidi.y[j]); //calcolo la distanza del punto i dal centroide j
                    if(distanza<=punti.distcluster[i]){  //se la distanza calcolata è minore rispetto
                        punti.distcluster[i]=distanza;
                        indmin=j;
                    }
                }
                if(punti.cluster[i] != indmin){
                    cambi=1;  //registro il cambio di gruppo da parte del punto
                }
#pragma omp atomic
                newx[indmin]+=punti.x[i];
#pragma omp atomic
                newy[indmin]+=punti.y[i];
#pragma omp atomic
                centroidi.numpunti[indmin]++;
                punti.cluster[i] = indmin;
            }
#pragma omp for
            for (int i = 0; i < k; i++) { //ricalcolo la posizione dei centroidi dei cluster stabiliti in precedenza
                // come media aritmetica delle coordinate di tutti i punti appartenenti al cluster
                if(centroidi.numpunti[i]!=0) {
                    centroidi.x[i] =newx[i] / centroidi.numpunti[i];
                    centroidi.y[i] = newy[i]  / centroidi.numpunti[i];
                } else{ //nel caso un cluster fosse rimasto senza punti assegnati, il centroide lo assegno casualmente a uno qualsiasi dei punti in input
                    unsigned long long j;
                    j = (rand() / RAND_MAX) * n;
                    centroidi.x[i]=punti.x[j];
                    centroidi.y[i]=punti.y[j];
                }
            }
            if(cambi==0) {
#pragma omp for reduction(+:sse)  //calcolo la somma degli errori al quadrato (SSE)
                for (int i = 0; i < n; i++) {
                    sse = sse + pow(punti.distcluster[i], 2);
                }
            }
        }
    }
    double end = omp_get_wtime( ); //Prendo il tempo di fine dell'esecuzione del mio algoritmo
    for(int i=0;i<k;i++){
        printf("(%f,%f) %d\n",centroidi.x[i],centroidi.y[i],i);
    }
    //stampa facoltativa per graficare i risultati

#ifdef output
    std::ofstream myfile;
    myfile.open("outputpar.csv");
    myfile << "x,y,c" << std::endl;
    for (int i=0; i<n;i++) {
        myfile << punti.x[i] << "," << punti.y[i] << "," << punti.cluster[i]<< std::endl;
    }
    for (int j=0; j<k;j++) {
        myfile << centroidi.x[j] << "," << centroidi.y[j] << "," << "Centroide "<< j<< std::endl;
    }

    myfile.close();
#endif

    cout<<sse<<endl;
    std::cout << "Tempo necessario: " << end-start << " secondi "<< std::endl;
    return 0;
}
