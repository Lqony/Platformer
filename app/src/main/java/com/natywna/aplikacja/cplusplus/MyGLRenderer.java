package com.natywna.aplikacja.cplusplus;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.util.Log;
import android.view.MotionEvent;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static javax.microedition.khronos.opengles.GL10.GL_CLAMP_TO_EDGE;

/**
 * Created by Beniamin on 2017-05-07.
 */

public class MyGLRenderer implements GLSurfaceView.Renderer {

    int mPlayersAmount = 0;

    static final int GAME_SCENE = 0;
    static final int FRONT_GUI = 1;
    static final int PLATFORM_VERTICAL = 0;
    static final int PLATFORM_HORIZONTAL = 1;
    static final int PLAYER_FRAME_DATA_AMOUNT = 23;


    int mTextureNames[]; // create and delete localy in load textures
    int mBMPIds[];
    int mTextureAmount;
    float mGameBitmapW;
    float mGameBitmapH;

    private float mScreenWidth = 0;
    private float mScreenHeight = 0;
    private float ratio = 0;

    private final float mInputData[] = new float[5];

    private Context context = null;

    // mMVPMatrix is an abbreviation for "Model View Projection Matrix"
    private final float[] mMVPMatrix = new float[16];
    private final float[] mProjectionMatrix = new float[16];
    private final float[] mViewMatrix = new float[16];


    private FloatBuffer vertexBuffer;
    private ShortBuffer drawListBuffer;
    private FloatBuffer uvBuffer;

    private int mProgram;

    private final int COORDS_PER_VERTEX = 3;
    private float squareCoords[] = new float[12];
    private final float uvs[] = new float [8];

    private int mPositionHandle;
    private int mMVPMatrixHandle;
    private int mTexCoordLoc;
    private int mSamplerLoc;

    //private final int vertexCount = squareCoords.length / COORDS_PER_VERTEX;
    private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex

    MyGLRenderer(Context context){
        this.context = context;
    }

    public void onSurfaceCreated(GL10 unused, EGLConfig config) {

        InitBuffers();
        InitShaders();
        InitOpenGL();
        InitRender();
        ResetTextures();
        LoadFrontGUI();

        float l_xyzwh[] = new float[5];
        l_xyzwh[0] = -1.5f;
        l_xyzwh[1] = 0.5f;
        l_xyzwh[2] = 0.0f;
        l_xyzwh[3] = 0.4f;
        l_xyzwh[4] = 0.2f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = -1.1f;
        l_xyzwh[1] = 0.4f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = -0.7f;
        l_xyzwh[1] = 0.3f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = -0.3f;
        l_xyzwh[1] = 0.2f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = 0.1f;
        l_xyzwh[1] = 0.1f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = 0.5f;
        l_xyzwh[1] = 0.0f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = 0.9f;
        l_xyzwh[1] = -0.1f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);


        l_xyzwh[0] = 0.9f;
        l_xyzwh[1] = 0.5f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = 0.5f;
        l_xyzwh[1] = 0.4f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = 0.1f;
        l_xyzwh[1] = 0.3f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = -0.7f;
        l_xyzwh[1] = 0.1f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = -1.1f;
        l_xyzwh[1] = 0.0f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);
        l_xyzwh[0] = -1.5f;
        l_xyzwh[1] = -0.1f;
        AddPlatform("betaplatform", PLATFORM_HORIZONTAL, l_xyzwh);

    }

    native short[] DrawOrder();
    native void InitRender();
    native void DeleteRender();

    private void InitShaders(){
        mProgram = GLES20.glCreateProgram();

        GLES20.glAttachShader(mProgram, MyGLRenderer.loadShader(GLES20.GL_VERTEX_SHADER,
                context.getString(R.string.VertexShader)));

        GLES20.glAttachShader(mProgram, MyGLRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER,
                context.getString(R.string.FragmentShader)));

        GLES20.glLinkProgram(mProgram);
    }

    private void InitBuffers(){

        ByteBuffer bb = ByteBuffer.allocateDirect(48 /*12 elemts * 4 (size of float*/);
        bb.order(ByteOrder.nativeOrder());
        vertexBuffer = bb.asFloatBuffer();


        ByteBuffer dlb = ByteBuffer.allocateDirect(12 /*6 elemets * 2 (size of short) */);
        dlb.order(ByteOrder.nativeOrder());
        drawListBuffer = dlb.asShortBuffer();
        drawListBuffer.put(DrawOrder());
        drawListBuffer.position(0);


        ByteBuffer uvb = ByteBuffer.allocateDirect(32 /*8 elements * 4 (size of float) */);
        uvb.order(ByteOrder.nativeOrder());
        uvBuffer = uvb.asFloatBuffer();
    }

    private void InitOpenGL() {
        // Set the background frame color
        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

       // GLES20.glEnable(GL10.GL_CULL_FACE);
        //GLES20.glCullFace(GL10.GL_BACK);
        //GLES20.glEnable(GLES20.GL_DEPTH_TEST);
        GLES20.glEnable(GLES20.GL_BLEND);
        GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
    }

    private native boolean AddThatSquare();
    private native float[] NewBorn();

    public void onDrawFrame(GL10 unused) {

        if(AddThatSquare() && mPlayersAmount < 2){
            mPlayersAmount += 1;
            float coords[] = NewBorn();
            AddPlayer("betamain", coords[0], coords[1], 0.0f);
            //String l_name[] = new String[1];
            //l_name[0] = "ghoststand";
            //int l_id = AddSquare(GAME_SCENE, 0.0f, 0.0f, 0.0f);
            //LoadSquareAnimations(l_id, 1, l_name);
        }
        HandleInput();
        Physic();// - Game scene physisc platform, gravity etc.
        Logic(); //Update Player Frames, Create Hitboxes, Move
        UpdateSquares(); //Updates Animations, and squares according to their objects positions
        Draw();

    }

    private native void SetScreen(float w, float h);

    private native void HandleInput();

    private native void Logic();

    private native void Physic();

    public void onSurfaceChanged(GL10 unused, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        mScreenWidth = width;
        mScreenHeight = height;
        SetScreen(mScreenWidth, mScreenHeight);
        ratio = (float)  width / height;

         Matrix.orthoM(mProjectionMatrix, 0, -ratio, ratio, -1, 1, -1, 1);
         Matrix.setLookAtM(mViewMatrix, 0, 0, 0, 1.0f, 0f, 0f, 0f, 0f, 1.0f, 0f);
       // ratio = (float)  width / height;
        //Matrix.frustumM(mProjectionMatrix, 0, -ratio, ratio, -1, 1, 3, 7);
       // Matrix.setLookAtM(mViewMatrix, 0, 0, 0, -3.0f, 0f, 0f, 0f, 0f, 1.0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        //Matrix.translateM(mMVPMatrix, 0, 0, 0, 0.0f);

    }

    private static int loadShader(int type, String shaderCode){

        // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
        // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
        int shader = GLES20.glCreateShader(type);

        // add the source code to the shader and compile it
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }

    private int AddSquare(int drawed_scene,  float x, float y, float z){
        float l_data[] = {drawed_scene, x, y, z};

        return AddSquares(1, l_data);
    }

    private native int AddSquares(int amount, float data[]);

    private void AddPlayer(String player_name, float l_x, float l_y, float l_z){
        int l_data_id;
        InputStream data;
        String main_name;
        InputStreamReader inputreader;
        BufferedReader bufferReader;

        int l_frame_data_id;
        InputStream frame_data;
        String frame_name;
        InputStreamReader frame_inputreader;
        BufferedReader frame_bufferReader;

        int l_animation_data_id;
        InputStream animation_data;
        String animation_name;
        InputStreamReader animation_inputreader;
        BufferedReader animation_bufferReader;


        int l_frames_amount = 0;
        int l_frame_data[]  = null;;

        int l_animations_amount = 0;
        String l_animation_names[] = null;

        float player_data[] = new float[9];

        int l_square_id;

        main_name = "raw/"+ player_name;
        l_data_id = context.getResources().getIdentifier(main_name, null, context.getPackageName());
        data = context.getResources().openRawResource(l_data_id);
        inputreader = new InputStreamReader(data);
        bufferReader = new BufferedReader(inputreader);

        try{
            //ReadValueName
            bufferReader.readLine();
            //LOAD FRAMES AMOUNT
            l_frames_amount = Integer.parseInt(bufferReader.readLine());
            l_frame_data = new int[l_frames_amount * PLAYER_FRAME_DATA_AMOUNT];

            //ReadValueName
            bufferReader.readLine();
            //OPEN FRAME FILE
            frame_name = "raw/" + bufferReader.readLine();
            l_frame_data_id = context.getResources().getIdentifier(frame_name, null, context.getPackageName());
            frame_data = context.getResources().openRawResource(l_frame_data_id);
            frame_inputreader = new InputStreamReader(frame_data);
            frame_bufferReader = new BufferedReader(frame_inputreader);

            //LOAD EVERY FRAME DATA
            for(int i = 0; i < l_frames_amount; i++){
                //Read Frame Name And ID
                frame_bufferReader.readLine();
                for(int j = 0; j < PLAYER_FRAME_DATA_AMOUNT; j++){
                    //ReadValueName
                    frame_bufferReader.readLine();
                    l_frame_data[(i*PLAYER_FRAME_DATA_AMOUNT)+j] = Integer.parseInt(frame_bufferReader.readLine());
                }
            }
            //CLOSE FRAME BUFFER
            frame_data.close();
            frame_inputreader.close();
            frame_bufferReader.close();

            //Load Animations Amount
            l_animations_amount = Integer.parseInt(bufferReader.readLine());
            l_animation_names = new String[l_animations_amount];

            //OPEN ANIMATIONS FILE
            animation_name = "raw/" + bufferReader.readLine();
            l_animation_data_id = context.getResources().getIdentifier(animation_name, null, context.getPackageName());
            animation_data = context.getResources().openRawResource(l_animation_data_id);
            animation_inputreader = new InputStreamReader(animation_data);
            animation_bufferReader = new BufferedReader(animation_inputreader);

            //LOAD EVERY ANIMATION NAME
            for(int i = 0; i < l_animations_amount; i++){
                l_animation_names[i] = animation_bufferReader.readLine();
            }
            //CLOSE FRAME BUFFER
            animation_data.close();
            animation_inputreader.close();
            animation_bufferReader.close();


            //LOAD PLAYER DATA
            //movement_velocity
            player_data[0] = Integer.parseInt(bufferReader.readLine());
            //jump_power
            player_data[1] = Integer.parseInt(bufferReader.readLine());
            //air jump count
            player_data[2] = Integer.parseInt(bufferReader.readLine());
            //air_vertical_velocioty
            player_data[3] = Integer.parseInt(bufferReader.readLine());
            //fall_velocity
            player_data[4] = Integer.parseInt(bufferReader.readLine());

            //CLOSE MAIN BUFFER
            data.close();
            inputreader.close();
            bufferReader.close();

        }catch (IOException e){

        }

        l_square_id = AddSquare(GAME_SCENE, l_x, l_y, l_z);
        LoadSquareAnimations(l_square_id, l_animations_amount, l_animation_names);
        player_data[5] = l_x;
        player_data[6] = l_y;
        player_data[7] = l_z;
        player_data[8] = l_square_id;
        CreatePlayer(player_data, l_frames_amount, l_frame_data);

    }

    native void CreatePlayer(float n_player_data[], int n_frames_amount, int n_frames[]);

    private void AddPlatform(String n_animation, int n_type, float n_xyz[]){
        int l_square_id = AddSquare(GAME_SCENE, n_xyz[0], n_xyz[1], n_xyz[2]);
        String l_animation[] = new String[1];
        l_animation[0] = n_animation;
        LoadSquareAnimations(l_square_id, 1, l_animation);
        CreatePlatform(l_square_id, n_type, n_xyz);
    }

    private native void CreatePlatform(int square_id, int type, float rect[]);

    private void LoadSquareAnimations(int id, int amount, String[] animations){
        int l_data_id[] = new int[amount];
        InputStreamReader inputreader;
        BufferedReader bufferReader;
        float my_data[] = new float[amount*12];
        InputStream data;
        for(int i = 0; i < amount; i++) {

            l_data_id[i] = context.getResources().getIdentifier("raw/" + animations[i], null, context.getPackageName());
            data = context.getResources().openRawResource(l_data_id[i]);

            inputreader = new InputStreamReader(data);
            bufferReader = new BufferedReader(inputreader);


            try {

                bufferReader.readLine();
                my_data[(i*12)+0] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+0] /= mGameBitmapW;

                bufferReader.readLine();
                my_data[(i*12)+1] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+1] /= mGameBitmapH;

                bufferReader.readLine();
                my_data[(i*12)+2] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+2] /= mGameBitmapW;

                bufferReader.readLine();
                my_data[(i*12)+3] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+3] /= mGameBitmapH;

                bufferReader.readLine();
                my_data[(i*12)+4] = Float.parseFloat(bufferReader.readLine());

                bufferReader.readLine();
                my_data[(i*12)+5] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+5] /= 1000.0f;

                bufferReader.readLine();
                my_data[(i*12)+6] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+6] /= 1000.0f;

                bufferReader.readLine();
                my_data[(i*12)+7] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+7] /= 1000.0f;

                bufferReader.readLine();
                my_data[(i*12)+8] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+8] /= 1000.0f;

                bufferReader.readLine();
                my_data[(i*12)+9] = Float.parseFloat(bufferReader.readLine());

                bufferReader.readLine();
                my_data[(i*12)+10] = Float.parseFloat(bufferReader.readLine());

                bufferReader.readLine();
                my_data[(i*12)+11] = Float.parseFloat(bufferReader.readLine());
                my_data[(i*12)+11] /= 1000.0f;

            } catch (IOException e) {
                //log no data loaded
            }
        }
        LoadSquareAnimations(id, amount, my_data);
    }
    private native void LoadSquareAnimations(int id, int amount, float data[]);

    private native boolean ToDraw(int drawed_scene);
    private native void Pause(int mss);
    private void DrawScene(int drawed_scene){
       GLES20.glUseProgram(mProgram);

        mMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVPMatrix, 0);

        mPositionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
        GLES20.glEnableVertexAttribArray(mPositionHandle);

        mTexCoordLoc = GLES20.glGetAttribLocation(mProgram, "a_texCoord");
        GLES20.glEnableVertexAttribArray(mTexCoordLoc);

        mSamplerLoc = GLES20.glGetUniformLocation(mProgram, "s_texture");

        while(ToDraw(drawed_scene)){
            /*The right order
            squareCoords[0] = -0.1f;     squareCoords[1] = 0.0f;     squareCoords[2] = 0.0f;
            squareCoords[3] = -0.1f;     squareCoords[4] = 0.2f;     squareCoords[5] = 0.0f;
            squareCoords[6] = 0.1f;     squareCoords[7] = 0.2f;     squareCoords[8] = 0.0f;
            squareCoords[9] = 0.1f;     squareCoords[10] = 0.0f;     squareCoords[11] = 0.0f;
            */
            vertexBuffer.put(SquareCoords());
            vertexBuffer.position(0);
            //Posiotion of square
            GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false,
                    vertexStride, vertexBuffer);

            //Here Goes the uvs from current animations(FrameRect)
            /*uvs[1] = 0.2f;  uvs[0] = 0.2f;
            uvs[2] = 0.2f;  uvs[3] = 0.0f;
            uvs[4] = 0.0f;  uvs[5] = 0.0f;
            uvs[6] = 0.0f;  uvs[7] = 0.2f; this order gives you normal draw, draw backward to change side*/

            uvBuffer.put(TextureCoords());
            uvBuffer.position(0);
            //Sprite coords
            GLES20.glVertexAttribPointer(mTexCoordLoc, 2, GLES20.GL_FLOAT, false, 0, uvBuffer);
            //Texture ID
            GLES20.glUniform1i(mSamplerLoc, 0);

            //DRAW
            GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT,
                    drawListBuffer);
        }
        GLES20.glDisableVertexAttribArray(mPositionHandle);
        GLES20.glDisableVertexAttribArray(mTexCoordLoc);
    }

    private void LoadFrontGUI(){
        String l_name[] = new String[1];
        float l_data[] = new float[12];

        int l_id = AddSquare(FRONT_GUI, -1.25f, -0.5f, 0);
        l_name[0] = "analogoutside";
        LoadSquareAnimations(l_id, 1, l_name);
        l_name[0] = "analoginside";
        l_id = AddSquare(FRONT_GUI, -1.25f, -0.5f, 0);
        LoadSquareAnimations(l_id, 1, l_name);
        l_data[0] = l_id;
        l_data[1] = -1.25f;
        l_data[2] = -0.5f;
        l_data[3] = 0.5f;
        l_data[4] = mScreenWidth;
        l_data[5] = mScreenHeight;

        l_id = AddSquare(FRONT_GUI, 1.25f, -0.5f, 0);
        l_name[0] = "analogoutside";
        LoadSquareAnimations(l_id, 1, l_name);
        l_name[0] = "analoginside";
        l_id = AddSquare(FRONT_GUI, 1.25f, -0.5f, 0);
        LoadSquareAnimations(l_id, 1, l_name);
        l_data[6] = l_id;
        l_data[7] = 1.25f;
        l_data[8] = -0.5f;
        l_data[9] = 0.5f;
        l_data[10] = mScreenWidth;
        l_data[11] = mScreenHeight;

        CreateAnalogs(2, l_data);

    }

    private native void CreateAnalogs(int amount, float[] data);

    private void Draw(){
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        Matrix.frustumM(mProjectionMatrix, 0, -ratio, ratio, -1, 1, 3, 7);
        Matrix.setLookAtM(mViewMatrix, 0, 0, 0, -3.0f, 0f, 0f, 0f, 0f, 1.0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        DrawScene(GAME_SCENE);

        Matrix.orthoM(mProjectionMatrix, 0, -ratio, ratio, -1, 1, -1, 1);
        Matrix.setLookAtM(mViewMatrix, 0, 0, 0, 1.0f, 0f, 0f, 0f, 0f, 1.0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        DrawScene(FRONT_GUI);
        //Draw Back GUI orthoM
        //Draw scene frustuM
        //Draw Front Gui orthoM

    }

    private float[] SquareCoords(){
        //Counter clockwise

        return DrawSquareCoords();
    }

    private float[] TextureCoords(){

        return DrawTextureCoords();
    }

    private native float[] DrawTextureCoords();

    private native float[] DrawSquareCoords();

    private void ResetTextures(){

        //Load one main game bitmap into texture
        int l_bmp_id;
        mTextureNames = new int[1];
        GLES20.glGenTextures(1, mTextureNames, 0);
        l_bmp_id =  context.getResources().getIdentifier("drawable/game_bitmap", null, context.getPackageName());
        LoadTexture(l_bmp_id);

    }

    private void LoadTexture(int id){
        //(POZNIEJ ZROBIC OBSLUGE KILKU TEXTUR NA WSZELKKI WYPADEK)
        Bitmap l_bmp = null;
        l_bmp = BitmapFactory.decodeResource(context.getResources(), id);
        mGameBitmapW = l_bmp.getWidth();
        mGameBitmapH = l_bmp.getHeight();
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureNames[0]);

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,
                GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER,
               GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, l_bmp, 0);
        if(l_bmp != null) l_bmp.recycle();

        /*Bitmap l_bmp = null;
        boolean already_loaded = false;
        if(mTextureAmount < MAX_TEXTURE_AMOUNT){
            for(int i = 0; i < mTextureAmount; i++){
                if(mBMPIds[i] == id) already_loaded = true;
            }

            if(!already_loaded){

                mBMPIds[mTextureAmount] = id;
                l_bmp = BitmapFactory.decodeResource(context.getResources(), mBMPIds[mTextureAmount]);
                GLES20.glActiveTexture(GLES20.GL_TEXTURE0+mTextureAmount);
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureNames[mTextureAmount]);

                GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,
                        GLES20.GL_LINEAR);
                GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER,
                        GLES20.GL_LINEAR);
                GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
                GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);

                GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, l_bmp, 0);


                mTextureAmount++;
            }
        }
        if(l_bmp != null)
            l_bmp.recycle();*/
    }

    private native void UpdateSquares();

    public void ProcesTouchEvent(MotionEvent event){
       mInputData[0] = event.getActionMasked();
       if(event.getPointerCount() == 1){
           mInputData[1] = event.getX(0);
           mInputData[2] = event.getY(0);
           mInputData[3] = -1;
           mInputData[4] = -1;
       }else if(event.getPointerCount() > 1){
           mInputData[1] = event.getX(0);
           mInputData[2] = event.getY(0);
           mInputData[3] = event.getX(1);
           mInputData[4] = event.getY(1);
       }
       else{
           mInputData[1] = -1;
           mInputData[2] = -1;
           mInputData[3] = -1;
           mInputData[4] = -1;
       }

       ProcessInput(mInputData);
    }

    private native void ProcessInput(float data[]);
}
