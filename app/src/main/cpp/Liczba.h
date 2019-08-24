//
// Created by Beniamin on 2017-05-07.
//

#ifndef CPLUSPLUS_LICZBA_H
#define CPLUSPLUS_LICZBA_H

extern "C" {

class Liczba {

private:
    int x;

public:

    static Liczba **obiekty;
    static int ilosc;

    Liczba() { x = 10; }

    void SetX(int n_x) { x = n_x; }

    int X() { return x; }

};

};


#endif //CPLUSPLUS_LICZBA_H
