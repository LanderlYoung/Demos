// https://github.com/nodejs/node-addon-examples
// https://nodejs.org/api/n-api.html

#define USE_N_API 1

#include <unistd.h>

#if USE_N_API
#include <assert.h>
#include <node_api.h>

static napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value world;
  status = napi_create_string_utf8(env, "world", 5, &world);
  assert(status == napi_ok);
  return world;
}

static napi_value Getpid(napi_env env, napi_callback_info info) {
  pid_t pid = getpid();
  napi_value ret;
  napi_status status = napi_create_int32(env, (int32_t)pid, &ret);
  assert(status == napi_ok);

  return ret;
}

#define DECLARE_NAPI_METHOD(name, func)          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc[] = {
      DECLARE_NAPI_METHOD("hello", Method),
      DECLARE_NAPI_METHOD("getpid", Getpid),
  };
  status = napi_define_properties(env, exports, 2, desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(MyAddon, Init)

#else

#include <napi.h>
static Napi::String Method(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

static Napi::Number Getpid(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  pid_t pid = getpid();
  return Napi::Number::New(env, (int32_t)pid);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  exports.Set(Napi::String::New(env, "getpid"),
              Napi::Function::New(env, Getpid));
  return exports;
}

NODE_API_MODULE(MyAddon, Init)

#endif