package io.github.landerlyoung.kmm.proto

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

/**
 * SpaceX
 */
@Serializable
data class RocketLaunch (
    @SerialName("flight_number")
    val flightNumber: Int,
    @SerialName("name")
    val missionName: String,
    @SerialName("date_utc")
    val launchDateUTC: String,
    @SerialName("success")
    val launchSuccess: Boolean?,
)