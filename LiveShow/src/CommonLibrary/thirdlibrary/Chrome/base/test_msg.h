// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This header is meant to be included in multiple passes, hence no traditional
// header guard.
// See ipc_message_macros.h for explanation of the macros and passes.

#include <string>
#include <vector>

#include "chrome/common/ipc_message_macros.h"
IPC_BEGIN_MESSAGES(HelloStruct, 1)
IPC_MESSAGE_CONTROL1(test_msg2,std::string)
IPC_END_MESSAGES(HelloStruct)
