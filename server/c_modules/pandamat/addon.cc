#include "pandamat.h"

void PandaMat(const FunctionCallbackInfo<Value>& args) {
  PandaMat::NewInstance(args);
}

void InitAll(Local<Object> exports, Local<Object> module) {
  PandaMat::Init(exports->GetIsolate());

  NODE_SET_METHOD(module, "exports", PandaMat);
}

NODE_MODULE(pandamat, InitAll);
