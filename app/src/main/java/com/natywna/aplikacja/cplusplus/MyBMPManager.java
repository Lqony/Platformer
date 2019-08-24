package com.natywna.aplikacja.cplusplus;

import android.content.Context;

/**
 * Created by Beniamin on 2017-05-13.
 */
//Probably useless
public class MyBMPManager {

    private int count;
    private int IDs[];
    private String pathes[];
    //x from table IDs corresponds to x from table patches

    MyBMPManager(Context context, int count, String names[]){
        this.count = count;
        pathes = new String[count];
        IDs = new int[count];

        for(int i = 0; i < count; i++){
            IDs[i] = context.getResources().getIdentifier(pathes[i], null, context.getPackageName());
            this.pathes[i] = pathes[i];
        }
    }

    public int ID(String patch) {
        for(int i = 0; i < count; i++) {
            if (pathes[i].equals(patch))
                return IDs[i];
        }
        return -1;
    }


}
