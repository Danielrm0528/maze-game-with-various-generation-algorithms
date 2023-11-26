#ifndef ARCO_H
#define ARCO_H
#include <iostream>

using std::ostream;

class Arco {
private:
    int fila;
    int col;
    int peso;
public:
     Arco() {}

    Arco(int fila, int col, int peso) {
        this->fila = fila;
        this->col = col;
        this->peso = peso;
    }

    int getFila() const {
        return fila;
    }

    int getCol() const {
        return col;
    }

    int getPeso() const {
        return peso;
    }
};

ostream& operator <<(ostream &os, const Arco &arc) {
    os << "(" << arc.getFila() << ", " << arc.getCol() << ")";
    return os;
}

#endif // ARCO_H
