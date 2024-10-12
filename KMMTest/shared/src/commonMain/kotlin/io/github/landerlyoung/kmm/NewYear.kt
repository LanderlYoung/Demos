package io.github.landerlyoung.kmm

import kotlinx.datetime.Clock
import kotlinx.datetime.LocalDate
import kotlinx.datetime.TimeZone
import kotlinx.datetime.daysUntil
import kotlinx.datetime.todayIn

/*
 * ```
 * Author: yangchao12
 * Date:   2023-01-24
 * Time:   13:47
 * Life with Passion, Code with Creativity.
 * ```
 */


fun daysUntilNewYear(): Int {
  val today = Clock.System.todayIn(TimeZone.currentSystemDefault())
  val closestNewYear = LocalDate(today.year + 1, 1, 1)
  return today.daysUntil(closestNewYear)
}