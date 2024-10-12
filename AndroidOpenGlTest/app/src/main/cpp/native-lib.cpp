#include <jni.h>
#include <string>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

extern "C" JNIEXPORT jstring JNICALL
Java_io_github_landerlyoung_androidopengltest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_io_github_landerlyoung_androidopengltest_MainActivity_render(
        JNIEnv *env,
        jobject /* this */) {

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
