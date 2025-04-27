package io.github.landerlyoung.demo

import android.content.Context
import android.util.AttributeSet
import android.util.Log
import androidx.appcompat.widget.AppCompatTextView

/*
 * ```
 * Author: taylorcyang@tencent.com
 * Date:   2025-04-18
 * Time:   15:15
 * Life with Passion, Code with Creativity.
 * ```
 */

class CustomTextView : AppCompatTextView {
    constructor(context: Context) : super(context)
    constructor(context: Context, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    init {
        setText("CustomTextView ClickToTest")

        setOnClickListener {
            requestLayout()
            parent?.requestLayout()
        }
    }

    private var measureCount = 0

    override fun requestLayout() {
        super.requestLayout()
        measureCount = 0
    }


    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        measureCount++

        Log.v(javaClass.simpleName, "measureCount:$measureCount $this", Throwable())
    }

    override fun onLayout(changed: Boolean, left: Int, top: Int, right: Int, bottom: Int) {
        super.onLayout(changed, left, top, right, bottom)

        Log.i(javaClass.simpleName, "measureCount:$measureCount $this")
    }
}