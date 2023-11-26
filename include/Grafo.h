#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <time.h>
#include "LinkedList.h"
#include "KVPair.h"

//clase de grafos
class Grafo {
private:
    LinkedList<int> *grafo;
    LinkedList<int> *laberinto;
    bool *visitados;
    int numNodos;
    int rows;
    int cols;
    int inicio = 0;
    int actual;
    int fin;
    LinkedList<int> objetivos;
public:
    Grafo(int alto, int largo) {
        if(alto <= 0 || largo <= 0)
            throw runtime_error("Number of rows and columns must be greater than zero.");
        rows = alto;
        cols = largo;
        numNodos = cols * rows;
        grafo = new LinkedList<int>[numNodos];
    }

    ~Grafo() {
            delete [] laberinto;
    }

    //inicializa un grafo como matriz (arcos en los vecinos)
    void initGrafo() {
        for(int i = 0; i < numNodos; i++) {
        if((i+cols) < numNodos) {
            grafo[i].append(i+cols);
        }
        if(i-cols >= 0) {
            grafo[i].append(i-cols);
        }
        if(i % cols != 0) {
            grafo[i].append(i-1);
        }
        if((i+1) % cols != 0) {
            grafo[i].append(i+1);
        }
        }
    }

    //inicializa el laberinto
    void initLaberinto() {
        laberinto = new LinkedList<int>[numNodos];
    }

    void printGrafo() {
        for(int i = 0; i < numNodos; i++) {
            grafo[i].print();
        }
    }

    void printLaberinto() {
        for(int i = 0; i < numNodos; i++) {
            if(i%cols==0) {
                cout << "\n";
            }
            laberinto[i].print();
        }
    }

    void printVisitados() {
        for(int i = 0; i < numNodos; i++) {
            cout << visitados[i] << endl;
        }
    }

    //genera el laberinto
    void generarLaberinto(int selec,int cantObj) {
        srand(time(0));
        iniciarVisitados();
        initLaberinto();
        initGrafo();
        if(selec == 0) {
            DFS();
            cout << "DFS" << endl;
        } else if(selec == 1) {
            Prim(rand() % numNodos);
            cout << "Prim" << endl;
        } else if (selec == 2) {
            Kruskal(rand() % numNodos);
            cout << "Kruskal" << endl;
        }
        agregarCiclos(numNodos * 0.05);
        generarObjetivos(cantObj);
        deleteVisitados();
        deleteGrafo();
        fin = rand()%numNodos;
        if(fin < numNodos/2) fin += numNodos/2;
        actual = inicio;
    }

    //Depth First search
    void DFS() {
        DFSAux(rand() % numNodos);
    }

    void DFSAux(int startNode) {
        visitados[startNode] = true;
        while(grafo[startNode].getSize() > 0) {
            int pos = rand() % grafo[startNode].getSize();
            grafo[startNode].goToPos(pos);
            int destino = grafo[startNode].remove();
            if(!visitados[destino]) {
                laberinto[startNode].append(destino);
                laberinto[destino].append(startNode);
                DFSAux(destino);
            }
        }
    }

    //Prim
    void Prim(int startNode) {
        LinkedList<KVPair<int,int>> arcos;
        visitados[startNode] = true;
        arcosPosibles(arcos,startNode);
        while(arcos.getSize()>0) {
            int indice = rand() % arcos.getSize();
            arcos.goToPos(indice);
            KVPair<int,int> arco = arcos.remove();
            int origen = arco.key;
            int destino = arco.value;
            if(!visitados[destino]) {
                visitados[destino] = true;
                laberinto[origen].append(destino);
                laberinto[destino].append(origen);
                arcosPosibles(arcos,destino);
            }
        }
    }

    //Kruskal
    void Kruskal(int startNode) {
        LinkedList<KVPair<int,int>> arcos;
        listaTodosArcos(arcos);
        int *conjuntos = new int[numNodos];
        for(int i = 0;i<numNodos;i++) {
            conjuntos[i] = i;
        }
        while(arcos.getSize()>0) {
            int indice = rand() % arcos.getSize();
            arcos.goToPos(indice);
            KVPair<int,int> arco = arcos.remove();
            int origen = arco.key;
            int destino = arco.value;
            if(conjuntos[origen] != conjuntos[destino]) {
                laberinto[origen].append(destino);
                laberinto[destino].append(origen);
                int conjunto = conjuntos[destino];
                for(int i = 0;i<numNodos;i++) {
                    if(conjuntos[i] == conjunto) conjuntos[i] = conjuntos[origen];
                }

            }
        }
    }

    //anade los arcos posibles de un nodo a una lista
    void arcosPosibles(LinkedList<KVPair<int,int>> &arcos, int nodo) {
        if (nodo - cols >= 0) arcos.append(KVPair(nodo,nodo-cols));
        if (nodo + cols < numNodos) arcos.append(KVPair(nodo,nodo + cols));
        if ((nodo + 1) % cols != 0) arcos.append(KVPair(nodo,nodo + 1));
        if (nodo % cols != 0) arcos.append(KVPair(nodo,nodo - 1));
    }

    //anade todos los arcos del grafo inicial a una lista
    void listaTodosArcos(LinkedList<KVPair<int,int>> &arcos) {
        for(int i = 0;i<numNodos;i++) {
            while(grafo[i].getSize() > 0) {
                arcos.append(KVPair(i,grafo[i].remove()));
            }
        }
    }



    void iniciarVisitados() {
        visitados = new  bool[numNodos];
        for (int i = 0; i < numNodos; i++) {
            visitados[i] = false;
        }
    }

    //agrega ciclos al laberinto
    void agregarCiclos(int cant) {
        while(cant > 0) {
            int nodoRandom = rand()%numNodos;
            LinkedList<KVPair<int,int>> posibles;
            arcosPosibles(posibles,nodoRandom);
            int indice = rand()%posibles.getSize();
            posibles.goToPos(indice);
            KVPair<int,int> arco = posibles.remove();
            int destino = arco.value;
            if(!arcoPresente(nodoRandom,destino)) {
              laberinto[nodoRandom].append(destino);
              laberinto[destino].append(nodoRandom);
              cant--;
            }
        }
    }

    bool arcoPresente(int origen, int destino) {
        return laberinto[origen].contains(destino);
    }

    void deleteVisitados() {
        delete [] visitados;
    }

    void deleteGrafo() {
        delete [] grafo;
    }

    int getRows() {
        return rows;
    }

    int getCols() {
        return cols;
    }

    int getnumNodos() {
        return numNodos;
    }

    int getActual() {
        return actual;
    }

    int getFin() {
        return fin;
    }

    LinkedList<int>* getListaAdy() {
        return laberinto;
    }

    void moverArriba() {
        if(actual-cols >= 0 && laberinto[actual].contains(actual-cols)) {
            actual = actual - cols;
        }
    }

     void moverAbajo() {
        if(actual+cols < numNodos && laberinto[actual].contains(actual+cols)) {
            actual = actual + cols;
        }
    }

     void moverDerecha() {
        if((actual+1) % cols != 0 && laberinto[actual].contains(actual+1)) {
            actual = actual + 1;
        }
    }

    void moverIzquierda() {
        if(actual % cols != 0 && laberinto[actual].contains(actual-1)) {
            actual = actual - 1;
        }
    }

    bool checkFin() {
        return inicio == fin;
    }

    //genera objetivos
    void generarObjetivos(int cant) {
        while(cant > 0) {
            int nodoObj = rand()%numNodos;
            if(!objetivos.contains(nodoObj)) {
               objetivos.append(nodoObj);
               cant--;
            }
        }
    }

    LinkedList<int>* getObjetivos() {
        return &objetivos;
    }

    //algoritmo de busqueda de dijkstra
    LinkedList<int>* dijkstra(int inicio) {
       int *distancias = new int[numNodos];
       int *rutas = new int[numNodos];
       bool *visited = new bool[numNodos];

       int cur = inicio;
       for(int i = 0;i<numNodos;i++) {
            visited[i] = false;
            distancias[i] = 99999999;
            rutas[i] = 99999999;
       }
       visited[cur] = true;
       distancias[cur] = 0;
       while(!visited[fin]) {
            for(laberinto[cur].goToStart();!laberinto[cur].atEnd();laberinto[cur].next()) {
                int destino = laberinto[cur].getElement();
                if((distancias[cur] + 1 < distancias[destino]) && (!visited[destino])) {
                    distancias[destino] = distancias[cur] + 1;
                    rutas[destino] = cur;
                }
            }
            int lower = fin;
            for(int i = 0;i<numNodos;i++) {
                if(!visited[i]) {
                    if(distancias[i] < distancias[lower]) {
                        lower = i;
                    }
                }
            }
            visited[lower] = true;
            cur = lower;
       }

       LinkedList<int>* rutaFinal = new LinkedList<int>();
       rutaFinal->insert(fin);
       int nodoAnterior = rutas[fin];
       while(nodoAnterior != inicio) {
           rutaFinal->goToStart();
           rutaFinal->insert(nodoAnterior);
           nodoAnterior = rutas[nodoAnterior];
       }
       rutaFinal->insert(inicio);
       delete [] visited;
       delete [] rutas;
       delete [] distancias;
       return rutaFinal;
    }









};

#endif // GRAFO_H
