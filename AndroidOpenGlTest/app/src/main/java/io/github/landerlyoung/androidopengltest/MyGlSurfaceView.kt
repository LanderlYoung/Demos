package io.github.landerlyoung.androidopengltest

import android.content.Context
import android.opengl.EGL14
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.util.AttributeSet
import android.view.Choreographer
import android.view.Choreographer.FrameCallback
import android.view.SurfaceHolder
import android.view.SurfaceView
import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.egl.EGLContext

/*
 * ```
 * Author: taylorcyang@tencent.com
 * Date:   2019-09-17
 * Time:   16:56
 * Life with Passion, Code with Creativity.
 * ```
 */

class MyGlSurfaceView(context: Context, attributeSet: AttributeSet) :
    SurfaceView(context, attributeSet) {

    private val glThread = HandlerThread("GlThread").also { it.start() }
    private val glHandler = Handler(glThread.looper)
    private val mainHandler = Handler(Looper.getMainLooper())

    var renderer: (() -> Unit)? = null

    init {
        holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder?) {
                glHandler.post(::createEglContext)
            }

            override fun surfaceChanged(
                holder: SurfaceHolder?,
                format: Int,
                width: Int,
                height: Int
            ) {
                glHandler.post { surfaceChanged(width, height) }
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                glThread.quit()
            }
        })
    }

    private fun createEglContext() {
        val egl = EGLContext.getEGL() as EGL10
        val display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY)
        require(display != EGL10.EGL_NO_DISPLAY)
        val version = IntArray(2)

        require(egl.eglInitialize(display, version))

        val config = intArrayOf(
            EGL10.EGL_RENDERABLE_TYPE, EGL14.EGL_OPENGL_ES2_BIT,
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_ALPHA_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 8,
            EGL10.EGL_STENCIL_SIZE, 8,
            EGL10.EGL_NONE
        )
        val outConfig = arrayOfNulls<EGLConfig?>(1)
        val outConfigSize = intArrayOf(0)
        egl.eglChooseConfig(
            display,
            config,
            outConfig,
            1,
            outConfigSize
        )

        val attrList = intArrayOf(EGL14.EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE)
        val context = egl.eglCreateContext(display, outConfig[0], EGL10.EGL_NO_CONTEXT, attrList)
        val surface = egl.eglCreateWindowSurface(display, outConfig[0], holder, null)
        egl.eglMakeCurrent(display, surface, surface, context)

        scheduleDraw {
            egl.eglSwapBuffers(display, surface)
        }
    }

    private fun scheduleDraw(swap: () -> Unit) {
        mainHandler.post {
            val frameCallback = object : FrameCallback {
                override fun doFrame(frameTimeNanos: Long) {
                    glHandler.post {
                        renderer?.invoke()
                        swap()
                    }
                    Choreographer.getInstance().postFrameCallback(this)
                }

            }
            Choreographer.getInstance().postFrameCallback(frameCallback)
        }
    }

    private fun surfaceChanged(width: Int, height: Int) {

    }
}