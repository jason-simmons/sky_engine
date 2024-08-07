// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_EMBEDDER_EMBEDDER_RENDER_TARGET_H_
#define FLUTTER_SHELL_PLATFORM_EMBEDDER_EMBEDDER_RENDER_TARGET_H_

#include <memory>
#include "fml/closure.h"
#include "fml/macros.h"
#include "shell/platform/embedder/embedder.h"
#include "third_party/skia/include/core/SkSize.h"
#include "third_party/skia/include/core/SkSurface.h"

namespace impeller {
class RenderTarget;
class AiksContext;
}  // namespace impeller

namespace flutter {

//------------------------------------------------------------------------------
/// @brief      Describes a surface whose backing store is managed by the
///             embedder. The type of surface depends on the client rendering
///             API used. The embedder is notified of the collection of this
///             render target via a callback.
///
class EmbedderRenderTarget {
 public:
  //----------------------------------------------------------------------------
  /// @brief      Destroys this instance of the render target and invokes the
  ///             callback for the embedder to release its resource associated
  ///             with the particular backing store.
  ///
  virtual ~EmbedderRenderTarget();

  //----------------------------------------------------------------------------
  /// @brief      A render surface the rasterizer can use to draw into the
  ///             backing store of this render target.
  ///
  /// @return     The render surface.
  ///
  virtual sk_sp<SkSurface> GetSkiaSurface() const = 0;

  //----------------------------------------------------------------------------
  /// @brief      An impeller render target the rasterizer can use to draw into
  ///             the backing store.
  ///
  /// @return     The Impeller render target.
  ///
  virtual impeller::RenderTarget* GetImpellerRenderTarget() const = 0;

  //----------------------------------------------------------------------------
  /// @brief      Returns the AiksContext that should be used for rendering, if
  ///             this render target is backed by Impeller.
  ///
  /// @return     The Impeller Aiks context.
  ///
  virtual std::shared_ptr<impeller::AiksContext> GetAiksContext() const = 0;

  //----------------------------------------------------------------------------
  /// @brief      Returns the size of the render target.
  ///
  /// @return     The size of the render target.
  ///
  virtual SkISize GetRenderTargetSize() const = 0;

  //----------------------------------------------------------------------------
  /// @brief      The embedder backing store descriptor. This is the descriptor
  ///             that was given to the engine by the embedder. This descriptor
  ///             may contain context the embedder can use to associate it
  ///             resources with the compositor layers when they are given back
  ///             to it in present callback. The engine does not use this in any
  ///             way.
  ///
  /// @return     The backing store.
  ///
  const FlutterBackingStore* GetBackingStore() const;

 protected:
  //----------------------------------------------------------------------------
  /// @brief      Creates a render target whose backing store is managed by the
  ///             embedder. The way this render target is exposed to the engine
  ///             is via an SkSurface and a callback that is invoked by this
  ///             object in its destructor.
  ///
  /// @param[in]  backing_store   The backing store describing this render
  ///                             target.
  /// @param[in]  on_release      The callback to invoke (eventually forwarded
  ///                             to the embedder) when the backing store is no
  ///                             longer required by the engine.
  ///
  EmbedderRenderTarget(FlutterBackingStore backing_store,
                       fml::closure on_release);

 private:
  FlutterBackingStore backing_store_;

  fml::closure on_release_;

  FML_DISALLOW_COPY_AND_ASSIGN(EmbedderRenderTarget);
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_EMBEDDER_EMBEDDER_RENDER_TARGET_H_
