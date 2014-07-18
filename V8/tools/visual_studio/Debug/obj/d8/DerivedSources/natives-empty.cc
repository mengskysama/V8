// Copyright 2008 Google Inc. All Rights Reserved.

// This file was generated from .js source files by SCons.  If you
// want to make changes to this file you should either change the
// javascript source files or the SConstruct script.

#include "v8.h"
#include "natives.h"

namespace v8 {
namespace internal {

  static const char d8[] = { 0 };

  template <>
  int NativesCollection<D8>::GetBuiltinsCount() {
    return 1;
  }

  template <>
  int NativesCollection<D8>::GetDelayCount() {
    return 0;
  }

  template <>
  int NativesCollection<D8>::GetIndex(const char* name) {
    if (strcmp(name, "d8") == 0) return 0;
    return -1;
  }

  template <>
  Vector<const char> NativesCollection<D8>::GetScriptSource(int index) {
    if (index == 0) return Vector<const char>(d8, 33430);
    return Vector<const char>("", 0);
  }

  template <>
  Vector<const char> NativesCollection<D8>::GetScriptName(int index) {
    if (index == 0) return Vector<const char>("native d8.js", 12);
    return Vector<const char>("", 0);
  }

}  // internal
}  // v8
