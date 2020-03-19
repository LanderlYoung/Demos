package io.github.landerlyoung.androidopengltest

import android.graphics.Color
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = stringFromJNI()
//        my_gl_surface_view.renderer = { w, h ->
////            GLES20.glClearColor(1f, 0f,0f,1f)
////            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)
//            render(w, h)
//        }
        my_gl_surface_view.setOnClickListener {
            val lp = my_gl_surface_view.layoutParams as ConstraintLayout.LayoutParams
            lp.matchConstraintPercentHeight -= 0.2f
            if (lp.matchConstraintPercentHeight < 0.2f) {
                lp.matchConstraintPercentHeight = 1f
            }
            my_gl_surface_view.requestLayout()
        }
        my_gl_surface_view.let {
            it.setBackgroundColor(Color.BLUE)
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun render(width: Int, height: Int)

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
