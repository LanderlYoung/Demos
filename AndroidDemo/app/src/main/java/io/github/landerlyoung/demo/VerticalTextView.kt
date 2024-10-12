package io.github.landerlyoung.demo

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.util.TypedValue

/*
 * ```
 * Author: yangchao12
 * Date:   2023-08-24
 * Time:   19:16
 * Life with Passion, Code with Creativity.
 * ```
 */

class VerticalTextView : androidx.appcompat.widget.AppCompatTextView {
  constructor(context: Context) : super(context)
  constructor(context: Context, attrs: AttributeSet) : super(context, attrs)
  constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int) : super(context,
    attrs,
    defStyleAttr)


  override fun onDraw(canvas: Canvas?) {
    stroke(canvas)
  }

  private fun stroke(canvas: Canvas?) {
    // draw stroke
    val color = currentTextColor
    paint.strokeWidth = 5f
    // paint.strokeCap = Paint.Cap.BUTT
    // paint.strokeJoin = Paint.Join.BEVEL
    // paint.strokeMiter = 0f
    setTextColor(Color.WHITE)
    paint.style = Paint.Style.STROKE
    // layout.draw(canvas)
    super.onDraw(canvas)

    // restore
    setTextColor(color)
    paint.style = Paint.Style.FILL

    // draw text
    super.onDraw(canvas)
  }
}