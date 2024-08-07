// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FLOW_LAYERS_OFFSCREEN_SURFACE_H_
#define FLUTTER_FLOW_LAYERS_OFFSCREEN_SURFACE_H_

#if !SLIMPELLER

#include "fml/logging.h"
#include "fml/macros.h"

#include "display_list/dl_canvas.h"
#include "display_list/skia/dl_sk_canvas.h"
#include "third_party/skia/include/core/SkData.h"
#include "third_party/skia/include/core/SkRefCnt.h"
#include "third_party/skia/include/core/SkSize.h"
#include "third_party/skia/include/core/SkSurface.h"

class GrDirectContext;

namespace flutter {

class OffscreenSurface {
 public:
  explicit OffscreenSurface(GrDirectContext* surface_context,
                            const SkISize& size);

  ~OffscreenSurface() = default;

  sk_sp<SkData> GetRasterData(bool compressed) const;

  DlCanvas* GetCanvas();

  bool IsValid() const;

 private:
  sk_sp<SkSurface> offscreen_surface_;
  DlSkCanvasAdapter adapter_;

  FML_DISALLOW_COPY_AND_ASSIGN(OffscreenSurface);
};

}  // namespace flutter

#endif  //  !SLIMPELLER

#endif  // FLUTTER_FLOW_LAYERS_OFFSCREEN_SURFACE_H_
