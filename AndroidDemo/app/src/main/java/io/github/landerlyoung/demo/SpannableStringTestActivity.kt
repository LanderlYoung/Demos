package io.github.landerlyoung.demo

import android.graphics.Color
import android.os.Bundle
import android.text.Editable
import android.text.Layout
import android.text.SpannableStringBuilder
import android.text.Spanned
import android.text.style.AlignmentSpan
import android.text.style.ForegroundColorSpan
import android.text.style.ImageSpan
import android.view.View
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import io.github.landerlyoung.demo.databinding.ActivitySpannableStringTestBinding

class SpannableStringTestActivity : AppCompatActivity() {

    private val w by lazy { (resources.displayMetrics.density * 24).toInt() }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        setContentView(R.layout.activity_spannable_string_test)

        val binding = ActivitySpannableStringTestBinding.bind(findViewById<View>(R.id.main))

        ViewCompat.setOnApplyWindowInsetsListener(binding.main) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        static(binding)
        changeColor(binding)
        changeDrawable(binding)
    }

    private fun changeDrawable(binding: ActivitySpannableStringTestBinding) {
        val ss = SS()
        binding.text3.setText(ss.spannableString, TextView.BufferType.EDITABLE)
        binding.text3.postDelayed({
            val span = binding.text3.text as Editable
            span.removeSpan(
                span.getSpans(
                    0,
                    span.length,
                    ImageSpan::class.java
                )[0]
            )

            span.setSpan(
                ImageSpan(getDrawable(R.drawable.outline_adb_24)!!.also {
                    it.setBounds(0, 0, w, w)
                }),
                ss.imageSpanStart,
                ss.imageSpanEnd,
                Spanned.SPAN_INCLUSIVE_EXCLUSIVE
            )
        }, 1000L)
    }

    private fun changeColor(binding: ActivitySpannableStringTestBinding) {
        val ss = SS()
        binding.text2.setText(ss.spannableString, TextView.BufferType.EDITABLE)
        binding.text2.postDelayed({
            val span = binding.text2.text as Editable
            span.removeSpan(
                span.getSpans(
                    0,
                    span.length,
                    ForegroundColorSpan::class.java
                )[0]
            )

            span.setSpan(
                ForegroundColorSpan(Color.RED),
                ss.foregroundColorSpanStart,
                ss.foregroundColorSpanEnd,
                Spanned.SPAN_INCLUSIVE_EXCLUSIVE
            )
        }, 1000L)
    }

    private fun static(binding: ActivitySpannableStringTestBinding) {
        binding.text1.setText(SS().spannableString, TextView.BufferType.EDITABLE)
    }


    private inner class SS {
        val foregroundColorSpanStart: Int
        val foregroundColorSpanEnd: Int

        val imageSpanStart: Int
        val imageSpanEnd: Int

        val spannableString = SpannableStringBuilder()

        init {
            spannableString.append("Hello")
            foregroundColorSpanStart = spannableString.length
            spannableString.append(
                "Green",
                ForegroundColorSpan(Color.GREEN),
                Spanned.SPAN_INCLUSIVE_EXCLUSIVE
            )
            foregroundColorSpanEnd = spannableString.length
            spannableString.append("World")
            spannableString.appendLine()

            imageSpanStart = spannableString.length
            spannableString.append(
                "Image",
                ImageSpan(getDrawable(R.drawable.ic_android_black_24dp)!!.also {
                    it.setBounds(0, 0, w, w)
                }),
                Spanned.SPAN_INCLUSIVE_EXCLUSIVE
            )
            imageSpanEnd = spannableString.length

            spannableString.setSpan(
                AlignmentSpan.Standard(Layout.Alignment.ALIGN_CENTER),
                imageSpanStart, imageSpanEnd, Spanned.SPAN_INCLUSIVE_EXCLUSIVE
            )

            spannableString.appendLine()
            spannableString.append("NextLine")
        }
    }
}
