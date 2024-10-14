#include <napi.h>
#include <unistd.h>

static Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

static Napi::Number Getpid(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  pid_t pid = getpid();
  return Napi::Number::New(env, (int32_t)pid);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Method));
  exports.Set(Napi::String::New(env, "getpid"), Napi::Function::New(env, Getpid));
  return exports;
}

NODE_API_MODULE(MyAddon, Init)
