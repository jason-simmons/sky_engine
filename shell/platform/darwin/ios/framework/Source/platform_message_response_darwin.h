// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_DARWIN_IOS_FRAMEWORK_SOURCE_PLATFORM_MESSAGE_RESPONSE_DARWIN_H_
#define FLUTTER_SHELL_PLATFORM_DARWIN_IOS_FRAMEWORK_SOURCE_PLATFORM_MESSAGE_RESPONSE_DARWIN_H_

#include <Foundation/Foundation.h>

#include "fml/macros.h"
#include "fml/make_copyable.h"
#include "fml/platform/darwin/scoped_block.h"
#include "fml/task_runner.h"
#include "lib/ui/window/platform_message_response.h"
#import "flutter/shell/platform/darwin/common/buffer_conversions.h"

typedef void (^PlatformMessageResponseCallback)(NSData*);

namespace flutter {

class PlatformMessageResponseDarwin : public flutter::PlatformMessageResponse {
 public:
  void Complete(std::unique_ptr<fml::Mapping> data) override;

  void CompleteEmpty() override;

 private:
  explicit PlatformMessageResponseDarwin(PlatformMessageResponseCallback callback,
                                         fml::RefPtr<fml::TaskRunner> platform_task_runner);

  ~PlatformMessageResponseDarwin() override;

  fml::ScopedBlock<PlatformMessageResponseCallback> callback_;
  fml::RefPtr<fml::TaskRunner> platform_task_runner_;

  FML_FRIEND_MAKE_REF_COUNTED(PlatformMessageResponseDarwin);
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_DARWIN_IOS_FRAMEWORK_SOURCE_PLATFORM_MESSAGE_RESPONSE_DARWIN_H_
