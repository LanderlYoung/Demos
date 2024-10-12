plugins {
  kotlin("jvm") version "2.0.20"
  kotlin("plugin.serialization") version "2.0.20"
  id("idea")
}

group = "io.github.landerlyoung.demo"
version = "1.0-SNAPSHOT"

repositories {
  mavenCentral()
}

dependencies {
  implementation("org.jetbrains.kotlinx:kotlinx-coroutines-core:1.9.0")
  implementation("org.jetbrains.kotlinx:kotlinx-serialization-json:1.7.1")

  val ktor_version = "2.3.12"
  implementation("io.ktor:ktor-client-core:$ktor_version")
  implementation("io.ktor:ktor-client-java:$ktor_version")
  implementation("io.ktor:ktor-client-content-negotiation:$ktor_version")
  implementation("io.ktor:ktor-serialization-kotlinx-json:$ktor_version")
  // implementation("io.ktor:ktor-serialization-kotlinx-protobuf:$ktor_version")

  testImplementation(kotlin("test"))
}

tasks.test {
  useJUnitPlatform()
}
kotlin {
  jvmToolchain(11)
}

idea {
  module {
    isDownloadJavadoc = true
  }
}