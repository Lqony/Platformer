package com.natywna.aplikacja.cplusplus;

/**
 * Created by Beniamin on 2017-05-07.
 */

public class Liczba {

    private int id;

    public Liczba() { id = Create(); }

    public void SetX(int n_x)  {SetX(n_x, id); }
    private native void SetX(int n_x, int n_id);

    public int X() { return X(id); }
    private native int X(int n_id);

    public native int Create();
}
