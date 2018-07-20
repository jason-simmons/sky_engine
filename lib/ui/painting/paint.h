// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_PAINT_H_
#define FLUTTER_LIB_UI_PAINTING_PAINT_H_

#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/tonic/converter/dart_converter.h"

namespace blink {

class Paint {
 public:
  Paint() = default;
  Paint(Dart_Handle paint_objects, Dart_Handle paint_data);

  const SkPaint* paint() const { return is_null_ ? nullptr : &paint_; }

 private:
  friend struct tonic::DartConverter<Paint>;

  SkPaint paint_;
  bool is_null_ = true;
};

// The PaintData argument is a placeholder to receive encoded data for Paint
// objects. The data is actually processed by DartConverter<Paint>, which reads
// both at the given index and at the next index (which it assumes is a byte
// data for a Paint object).
class PaintData {};

}  // namespace blink

namespace tonic {

template <>
struct DartConverter<blink::Paint> {
  static blink::Paint FromArguments(Dart_NativeArguments args,
                                    int index,
                                    Dart_Handle& exception);
};

template <>
struct DartConverter<blink::PaintData> {
  static blink::PaintData FromArguments(Dart_NativeArguments args,
                                        int index,
                                        Dart_Handle& exception);
};

}  // namespace tonic

#endif  // FLUTTER_LIB_UI_PAINTING_PAINT_H_
