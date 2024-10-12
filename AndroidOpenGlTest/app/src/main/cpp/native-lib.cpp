#include <jni.h>
#include <string>
#include <pthread.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

extern "C" JNIEXPORT jstring JNICALL
Java_io_github_landerlyoung_androidopengltest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

static pthread_once_t kNativeInitEGL;
extern "C" JNIEXPORT void JNICALL
Java_io_github_landerlyoung_androidopengltest_MainActivity_render(
        JNIEnv *env,
        jobject /* this */,
        jint width,
        jint height) {

    pthread_once(&kNativeInitEGL, []() {
        auto display = eglGetCurrentDisplay();
        auto read = eglGetCurrentSurface(EGL_READ);
        auto draw = eglGetCurrentSurface(EGL_DRAW);

    });

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
