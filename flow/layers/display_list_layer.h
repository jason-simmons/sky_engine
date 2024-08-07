// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FLOW_LAYERS_DISPLAY_LIST_LAYER_H_
#define FLUTTER_FLOW_LAYERS_DISPLAY_LIST_LAYER_H_

#include <memory>

#include "common/macros.h"
#include "display_list/display_list.h"
#include "flow/layers/display_list_raster_cache_item.h"
#include "flow/layers/layer.h"
#include "flow/raster_cache_item.h"

namespace flutter {

class DisplayListLayer : public Layer {
 public:
  static constexpr size_t kMaxBytesToCompare = 10000;

  DisplayListLayer(const SkPoint& offset,
                   sk_sp<DisplayList> display_list,
                   bool is_complex,
                   bool will_change);

  DisplayList* display_list() const { return display_list_.get(); }

  bool IsReplacing(DiffContext* context, const Layer* layer) const override;

  void Diff(DiffContext* context, const Layer* old_layer) override;

  const DisplayListLayer* as_display_list_layer() const override {
    return this;
  }

  void Preroll(PrerollContext* frame) override;

  void Paint(PaintContext& context) const override;

#if !SLIMPELLER
  const DisplayListRasterCacheItem* raster_cache_item() const {
    return display_list_raster_cache_item_.get();
  }

  RasterCacheKeyID caching_key_id() const override {
    return RasterCacheKeyID(display_list()->unique_id(),
                            RasterCacheKeyType::kDisplayList);
  }
#endif  //  !SLIMPELLER

 private:
  NOT_SLIMPELLER(std::unique_ptr<DisplayListRasterCacheItem>
                     display_list_raster_cache_item_);

  SkPoint offset_;
  SkRect bounds_;

  sk_sp<DisplayList> display_list_;

  static bool Compare(DiffContext::Statistics& statistics,
                      const DisplayListLayer* l1,
                      const DisplayListLayer* l2);

  FML_DISALLOW_COPY_AND_ASSIGN(DisplayListLayer);
};

}  // namespace flutter

#endif  // FLUTTER_FLOW_LAYERS_DISPLAY_LIST_LAYER_H_
