#include <jni.h>
#include <string>




using namespace std;

extern "C" {


JNIEXPORT jstring JNICALL
Java_com_natywna_aplikacja_cplusplus_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Yo";
    return env->NewStringUTF(hello.c_str());
}

}