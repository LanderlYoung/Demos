package io.github.landerlyoung.demo

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.LinearGradient
import android.graphics.Paint
import android.graphics.PorterDuff
import android.graphics.PorterDuffXfermode
import android.graphics.Shader
import android.os.Build
import android.os.Bundle
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.core.graphics.withSave
import kotlin.math.abs
import kotlin.math.min

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // testVerticalTextView(findViewById(R.id.vertical_text_view))

    }

    fun testVerticalTextView(view: VerticalTextView) {
        // view.setShadowLayer(
        //     10f,
        //     5f,
        //     5f,
        //     Color.WHITE
        // )
        //
        // view.paint.style = Paint.Style.STROKE
        // view.paint.strokeWidth = 1.5f
        // view.paint.color = Color.WHITE
        // // view.paint.strokeCap = Paint.Cap.BUTT
        // view.paint.strokeJoin = Paint.Join.MITER
        // view.setTextColor(Color.BLACK)

        val text = "你好世界Hello World你好😁世😂界"
        val new = buildString {
            text.forEach { char ->
                append(char)
                // new line
                if (char.isLowSurrogate() || !char.isHighSurrogate()) {
                    append('\n')
                }
            }
        }
        view.setText(new)
    }
}

class AlphaGradientFrameLayout : FrameLayout {

    constructor(context: Context) : this(context, null)
    constructor(context: Context, attributeSet: AttributeSet?) : super(context, attributeSet)

    private val helper = AlphaGradientHelper(this)

    init {
        helper.setAlphaGradientParam(0.5f, 1f, 100f, 300f)
    }

    override fun draw(canvas: Canvas) {
        helper.drawWithGradient(canvas) {
            super.draw(canvas)
        }
    }
}

class AlphaGradientHelper(thisView: View) {

    companion object {
        private const val GRADIENT_DEFAULT_HEIGHT = 100f
    }

    private val view = thisView

    private var startAlpha = 0f
    private var endAlpha = 1f
    private var startY = 0f
    private var endY = 0f

    private val paint: Paint? by lazy {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            view.setLayerType(View.LAYER_TYPE_HARDWARE, null)

            val paint = Paint()
            val shader = LinearGradient(
                0f, 0f,
                0f, GRADIENT_DEFAULT_HEIGHT,
                Color.BLACK,
                // 0xFF_00_00_00_00L.toInt(),
                Color.TRANSPARENT,
                Shader.TileMode.CLAMP)
            paint.shader = shader
            paint.xfermode = PorterDuffXfermode(PorterDuff.Mode.DST_OUT)
            paint
        } else {
            null
        }
    }

    fun setAlphaGradientParam(startAlpha: Float, endAlpha: Float, startY: Float, endY: Float) {
        check(startAlpha in (0f..1f) && endAlpha in (0f..1f)) {
            "startAlpha:$startAlpha endAlpha:$endAlpha"
        }

        this.startAlpha = startAlpha
        this.endAlpha = endAlpha
        this.startY = startY
        this.endY = endY
    }

    fun drawGradient(canvas: Canvas) {
        if (startY >= endY) {
            // invalid
            return
        }

        paint?.let {
            val viewFullHeight = (endY - startY) / abs(endAlpha - startAlpha)
            val originY = startY - viewFullHeight * min(startAlpha, endAlpha)

            canvas.withSave {
                // canvas in range of startY to endY
                canvas.clipRect(
                    0f, startY,
                    canvas.width.toFloat(), endY
                )
                canvas.translate(0f, originY)

                // inverse upside down
                if (endAlpha < startAlpha) {
                    canvas.rotate(180f, canvas.width.toFloat() / 2, viewFullHeight / 2)
                }

                canvas.scale(1f, viewFullHeight / GRADIENT_DEFAULT_HEIGHT)

                // draw the gradient
                canvas.drawRect(0f, 0f, canvas.width.toFloat(), GRADIENT_DEFAULT_HEIGHT, it)
            }
        }
    }

    inline fun drawWithGradient(canvas: Canvas, drawBlock: () -> Unit) {
        canvas.withSave {
            drawBlock()
            drawGradient(canvas)
        }
    }
}
