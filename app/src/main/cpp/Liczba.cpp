//
// Created by Beniamin on 2017-05-07.
//

#include <string>
#include <jni.h>
#include "Liczba.h"
using namespace std;


extern "C"{
Liczba **Liczba::obiekty;
int Liczba::ilosc;


JNIEXPORT jint JNICALL
Java_com_natywna_aplikacja_cplusplus_Liczba_Create(JNIEnv *env, jobject instance) {

    if (Liczba::ilosc < 100) {
        Liczba::obiekty[Liczba::ilosc] = new Liczba;
        Liczba::ilosc++;
        return Liczba::ilosc - 1;
    } else return -1;

}


JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_Liczba_SetX__II(JNIEnv *env, jobject instance, jint n_x,
jint n_id) {

    Liczba::obiekty[n_id]->SetX(n_x);

}


JNIEXPORT jint JNICALL
        Java_com_natywna_aplikacja_cplusplus_Liczba_X__I(JNIEnv *env, jobject instance, jint n_id) {

return Liczba::obiekty[n_id]->X();

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MainActivity_Start(JNIEnv *env, jobject instance) {
        Liczba::obiekty = new Liczba *[100];
    Liczba::ilosc = 0;
}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MainActivity_Stop(JNIEnv *env, jobject instance) {

    for (int i = 0; i < Liczba::ilosc; i++) delete Liczba::obiekty[i];
    delete[] Liczba::obiekty;
}

}
