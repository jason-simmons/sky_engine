// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flow/layers/texture_layer.h"

#include "flow/testing/diff_context_test.h"
#include "flow/testing/layer_test.h"
#include "flow/testing/mock_layer.h"
#include "flow/testing/mock_texture.h"
#include "fml/macros.h"
#include "testing/display_list_testing.h"

namespace flutter {
namespace testing {

using TextureLayerTest = LayerTest;

TEST_F(TextureLayerTest, InvalidTexture) {
  const SkPoint layer_offset = SkPoint::Make(0.0f, 0.0f);
  const SkSize layer_size = SkSize::Make(8.0f, 8.0f);
  auto layer = std::make_shared<TextureLayer>(
      layer_offset, layer_size, 0, false, DlImageSampling::kNearestNeighbor);

  layer->Preroll(preroll_context());
  EXPECT_EQ(layer->paint_bounds(),
            (SkRect::MakeSize(layer_size)
                 .makeOffset(layer_offset.fX, layer_offset.fY)));
  EXPECT_TRUE(layer->needs_painting(paint_context()));

  layer->Paint(display_list_paint_context());
  EXPECT_TRUE(display_list()->Equals(DisplayList()));
}

#ifndef NDEBUG
TEST_F(TextureLayerTest, PaintingEmptyLayerDies) {
  const SkPoint layer_offset = SkPoint::Make(0.0f, 0.0f);
  const SkSize layer_size = SkSize::Make(0.0f, 0.0f);
  const int64_t texture_id = 0;
  auto mock_texture = std::make_shared<MockTexture>(texture_id);
  auto layer =
      std::make_shared<TextureLayer>(layer_offset, layer_size, texture_id,
                                     false, DlImageSampling::kNearestNeighbor);

  // Ensure the texture is located by the Layer.
  preroll_context()->texture_registry->RegisterTexture(mock_texture);

  layer->Preroll(preroll_context());
  EXPECT_EQ(layer->paint_bounds(), kEmptyRect);
  EXPECT_FALSE(layer->needs_painting(paint_context()));

  EXPECT_DEATH_IF_SUPPORTED(layer->Paint(paint_context()),
                            "needs_painting\\(context\\)");
}

TEST_F(TextureLayerTest, PaintBeforePrerollDies) {
  const SkPoint layer_offset = SkPoint::Make(0.0f, 0.0f);
  const SkSize layer_size = SkSize::Make(8.0f, 8.0f);
  const int64_t texture_id = 0;
  auto mock_texture = std::make_shared<MockTexture>(texture_id);
  auto layer = std::make_shared<TextureLayer>(
      layer_offset, layer_size, texture_id, false, DlImageSampling::kLinear);

  // Ensure the texture is located by the Layer.
  preroll_context()->texture_registry->RegisterTexture(mock_texture);

  EXPECT_DEATH_IF_SUPPORTED(layer->Paint(paint_context()),
                            "needs_painting\\(context\\)");
}
#endif

TEST_F(TextureLayerTest, PaintingWithLinearSampling) {
  const SkPoint layer_offset = SkPoint::Make(0.0f, 0.0f);
  const SkSize layer_size = SkSize::Make(8.0f, 8.0f);
  const SkRect layer_bounds =
      SkRect::MakeSize(layer_size).makeOffset(layer_offset.fX, layer_offset.fY);
  const int64_t texture_id = 0;
  const auto texture_image = MockTexture::MakeTestTexture(20, 20, 5);
  auto mock_texture = std::make_shared<MockTexture>(texture_id, texture_image);
  auto layer = std::make_shared<TextureLayer>(
      layer_offset, layer_size, texture_id, false, DlImageSampling::kLinear);

  // Ensure the texture is located by the Layer.
  preroll_context()->texture_registry->RegisterTexture(mock_texture);

  layer->Preroll(preroll_context());
  EXPECT_EQ(layer->paint_bounds(), layer_bounds);
  EXPECT_TRUE(layer->needs_painting(paint_context()));

  layer->Paint(display_list_paint_context());
  DisplayListBuilder expected_builder;
  /* (Texture)layer::Paint */ {
    expected_builder.DrawImageRect(texture_image, layer_bounds,
                                   DlImageSampling::kLinear);
  }
  EXPECT_TRUE(DisplayListsEQ_Verbose(display_list(), expected_builder.Build()));
}

using TextureLayerDiffTest = DiffContextTest;

TEST_F(TextureLayerDiffTest, TextureInRetainedLayer) {
  MockLayerTree tree1;
  auto container = std::make_shared<ContainerLayer>();
  tree1.root()->Add(container);
  auto layer = std::make_shared<TextureLayer>(SkPoint::Make(0, 0),
                                              SkSize::Make(100, 100), 0, false,
                                              DlImageSampling::kLinear);
  container->Add(layer);

  MockLayerTree tree2;
  tree2.root()->Add(container);  // retained layer

  auto damage = DiffLayerTree(tree1, MockLayerTree());
  EXPECT_EQ(damage.frame_damage, SkIRect::MakeLTRB(0, 0, 100, 100));

  damage = DiffLayerTree(tree2, tree1);
  EXPECT_EQ(damage.frame_damage, SkIRect::MakeLTRB(0, 0, 100, 100));
}

TEST_F(TextureLayerTest, OpacityInheritance) {
  const SkPoint layer_offset = SkPoint::Make(0.0f, 0.0f);
  const SkSize layer_size = SkSize::Make(8.0f, 8.0f);
  const SkRect layer_bounds =
      SkRect::MakeSize(layer_size).makeOffset(layer_offset.fX, layer_offset.fY);
  const int64_t texture_id = 0;
  const auto texture_image = MockTexture::MakeTestTexture(20, 20, 5);
  auto mock_texture = std::make_shared<MockTexture>(texture_id, texture_image);
  SkAlpha alpha = 0x7f;
  auto texture_layer = std::make_shared<TextureLayer>(
      layer_offset, layer_size, texture_id, false, DlImageSampling::kLinear);
  auto layer = std::make_shared<OpacityLayer>(alpha, SkPoint::Make(0.0f, 0.0f));
  layer->Add(texture_layer);

  // Ensure the texture is located by the Layer.
  PrerollContext* context = preroll_context();
  context->texture_registry->RegisterTexture(mock_texture);

  // The texture layer always reports opacity compatibility.
  texture_layer->Preroll(context);
  EXPECT_EQ(context->renderable_state_flags,
            LayerStateStack::kCallerCanApplyOpacity);

  // Reset has_texture_layer since it is not supposed to be sent as we
  // descend a tree in Preroll, but it was set by the previous test.
  context->has_texture_layer = false;
  layer->Preroll(context);
  EXPECT_EQ(context->renderable_state_flags,
            LayerStateStack::kCallerCanApplyOpacity);

  DlPaint texture_paint;
  texture_paint.setAlpha(alpha);

  layer->Paint(display_list_paint_context());
  DisplayListBuilder expected_builder;
  /* (Opacity)layer::Paint */ {
    expected_builder.Save();
    {
      /* texture_layer::Paint */ {
        expected_builder.DrawImageRect(texture_image, layer_bounds,
                                       DlImageSampling::kLinear,
                                       &texture_paint);
      }
    }
    expected_builder.Restore();
  }
  EXPECT_TRUE(DisplayListsEQ_Verbose(display_list(), expected_builder.Build()));
}

}  // namespace testing
}  // namespace flutter
