// Copyright 2008 Google Inc. All Rights Reserved.

// This file was generated from .js source files by SCons.  If you
// want to make changes to this file you should either change the
// javascript source files or the SConstruct script.

#include "v8.h"
#include "natives.h"

namespace v8 {
namespace internal {

  static const char runtime[] = { 0 };

  static const char v8natives[] = { 0 };

  static const char array[] = { 0 };

  static const char string[] = { 0 };

  static const char uri[] = { 0 };

  static const char math[] = { 0 };

  static const char messages[] = { 0 };

  static const char apinatives[] = { 0 };

  static const char debug[] = { 0 };

  static const char mirror[] = { 0 };

  static const char date[] = { 0 };

  static const char regexp[] = { 0 };

  static const char json[] = { 0 };

  template <>
  int NativesCollection<CORE>::GetBuiltinsCount() {
    return 13;
  }

  template <>
  int NativesCollection<CORE>::GetDelayCount() {
    return 5;
  }

  template <>
  int NativesCollection<CORE>::GetIndex(const char* name) {
    if (strcmp(name, "debug") == 0) return 0;
    if (strcmp(name, "mirror") == 0) return 1;
    if (strcmp(name, "date") == 0) return 2;
    if (strcmp(name, "regexp") == 0) return 3;
    if (strcmp(name, "json") == 0) return 4;
    if (strcmp(name, "runtime") == 0) return 5;
    if (strcmp(name, "v8natives") == 0) return 6;
    if (strcmp(name, "array") == 0) return 7;
    if (strcmp(name, "string") == 0) return 8;
    if (strcmp(name, "uri") == 0) return 9;
    if (strcmp(name, "math") == 0) return 10;
    if (strcmp(name, "messages") == 0) return 11;
    if (strcmp(name, "apinatives") == 0) return 12;
    return -1;
  }

  template <>
  Vector<const char> NativesCollection<CORE>::GetScriptSource(int index) {
    if (index == 0) return Vector<const char>(debug, 38113);
    if (index == 1) return Vector<const char>(mirror, 32509);
    if (index == 2) return Vector<const char>(date, 27058);
    if (index == 3) return Vector<const char>(regexp, 8981);
    if (index == 4) return Vector<const char>(json, 5840);
    if (index == 5) return Vector<const char>(runtime, 10443);
    if (index == 6) return Vector<const char>(v8natives, 10521);
    if (index == 7) return Vector<const char>(array, 22747);
    if (index == 8) return Vector<const char>(string, 17479);
    if (index == 9) return Vector<const char>(uri, 8571);
    if (index == 10) return Vector<const char>(math, 4041);
    if (index == 11) return Vector<const char>(messages, 19775);
    if (index == 12) return Vector<const char>(apinatives, 2137);
    return Vector<const char>("", 0);
  }

  template <>
  Vector<const char> NativesCollection<CORE>::GetScriptName(int index) {
    if (index == 0) return Vector<const char>("native debug.js", 15);
    if (index == 1) return Vector<const char>("native mirror.js", 16);
    if (index == 2) return Vector<const char>("native date.js", 14);
    if (index == 3) return Vector<const char>("native regexp.js", 16);
    if (index == 4) return Vector<const char>("native json.js", 14);
    if (index == 5) return Vector<const char>("native runtime.js", 17);
    if (index == 6) return Vector<const char>("native v8natives.js", 19);
    if (index == 7) return Vector<const char>("native array.js", 15);
    if (index == 8) return Vector<const char>("native string.js", 16);
    if (index == 9) return Vector<const char>("native uri.js", 13);
    if (index == 10) return Vector<const char>("native math.js", 14);
    if (index == 11) return Vector<const char>("native messages.js", 18);
    if (index == 12) return Vector<const char>("native apinatives.js", 20);
    return Vector<const char>("", 0);
  }

}  // internal
}  // v8
