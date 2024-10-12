package io.github.landerlyoung.demo

/*
 * ```
 * Author: yangchao12
 * Date:   2023-03-30
 * Time:   16:47
 * Life with Passion, Code with Creativity.
 * ```
 */

enum class ViewType {
  GIFT
}

sealed class MeasureType {
  /**
   * 业务自行决定要多高，有多少内容，就展示多高
   */
  object Unspecified : MeasureType()

  /**
   * 业务展示的高度最多是这么高
   * 最终高度 = min(业务自己可展示的高度，noMoreThanHeight)
   */
  data class NoMoreThan(val heightPx: Int) : MeasureType()
}

interface DisplayStatus {
  /**
   * 测量后的高度，无内容就是0
   */
  val heightPx: Int

  /**
   * 可以继续携带，业务的高度相关的数据，框架透传，最终给业务自己消费
   * 比如
   * 1. heightPx = 48, state = 1个礼物槽位
   * 2. heightPx = 96, state = 2个礼物槽位
   */
}

interface FlexibleHeightLayoutStrategy<State : DisplayStatus> {

  /**
   * 测量业务高度
   */
  fun measureHeight(measureType: MeasureType): State

  /**
   * 最终确定业务高度
   */
  fun setDisplayState(measureResult: State)
}

abstract class AbsFlexibleHeightLayoutStrategy<State : DisplayStatus> :
  FlexibleHeightLayoutStrategy<State> {

  final override fun measureHeight(measureType: MeasureType): State {
    val desired = getDesiredDisplayStatus()
    return when (measureType) {
      MeasureType.Unspecified -> desired
      is MeasureType.NoMoreThan -> {
        if (measureType.heightPx >= desired.heightPx) {
          desired
        } else {
          // shrink
          shrinkToFit(desired, measureType.heightPx)
        }
      }
    }
  }

  final override fun setDisplayState(measureResult: State) {
    setDisplayState(getCurrentDisplayStatus(), measureResult)
  }

  /**
   * 获取当前业务的**希望**展示状态，（比如礼物槽位：有几个槽位的数据就展示几个；没数据则不展示）
   */
  protected abstract fun getDesiredDisplayStatus(): State

  /**
   * 获取当前业务的**实际**的展示状态，（比如礼物槽位：数据够展示2个槽位，但是上次被压了1个；这里返回的就是展示1个）
   */
  protected abstract fun getCurrentDisplayStatus(): State

  /**
   * 当框架最终确定显示状态时，调用这个方法
   * @param hasChanged 上一次的状态
   * @param displayStatus 最终确定的展示状态
   */
  protected abstract fun setDisplayState(
    from: State,
    to: State,
  )

  /**
   * 当框架给的高度不够用时，调用这个方法，看看业务能压缩多少来适应
   */
  protected abstract fun shrinkToFit(
    desiredDisplay: State,
    toFitHeightPx: Int,
  ): State
}

interface LM {

  fun registerLayoutStrategy(viewType: ViewType, strategy: FlexibleHeightLayoutStrategy<*>)

  /**
   * 重新执行一次布局压缩逻辑
   */
  fun requestLayout()

  /**
   * 重新执行一次布局压缩逻辑
   *
   * 允许业务临时传入一个strategy，这个strategy会(且仅会)在这一次layout中被使用
   * 1. 临时替换掉 [registerLayoutStrategy] 注册的那个
   *
   * @param viewType
   */
  fun requestLayout(viewType: ViewType, strategyDuringThisLayout: FlexibleHeightLayoutStrategy<*>)

}


/// =========== 礼物业务接入实例 ======

class Gift(val lm: LM) {
  private data class GiftSlotStatus(
    val hasTopUnit: Boolean,
    val hasBottomUnit: Boolean,
  ) : DisplayStatus {
    val displaySlotCount: Int
      get() = TODO()

    override val heightPx: Int
      get() = TODO()
  }

  private val passiveStrategy = object : AbsFlexibleHeightLayoutStrategy<GiftSlotStatus>() {
    override fun getDesiredDisplayStatus(): GiftSlotStatus {
      TODO("not implemented")
    }

    override fun getCurrentDisplayStatus(): GiftSlotStatus {
      TODO("not implemented")
    }

    override fun setDisplayState(from: GiftSlotStatus, to: GiftSlotStatus) {
      TODO("not implemented")
    }

    override fun shrinkToFit(desiredDisplay: GiftSlotStatus, toFitHeightPx: Int): GiftSlotStatus {
      TODO("not implemented")
    }
  }
  init {
    lm.registerLayoutStrategy(ViewType.GIFT, passiveStrategy)
  }
  fun addMessage() {
    lm.requestLayout(ViewType.GIFT, object : AbsFlexibleHeightLayoutStrategy<GiftSlotStatus>() {
      override fun getDesiredDisplayStatus(): GiftSlotStatus {
        TODO("not implemented")
      }

      override fun getCurrentDisplayStatus(): GiftSlotStatus {
        TODO("not implemented")
      }

      override fun setDisplayState(from: GiftSlotStatus, to: GiftSlotStatus) {
        TODO("not implemented")
      }

      override fun shrinkToFit(desiredDisplay: GiftSlotStatus, toFitHeightPx: Int): GiftSlotStatus {
        TODO("not implemented")
      }
    })
  }
}
