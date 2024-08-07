// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "testing/test_vulkan_image.h"

#include "testing/test_vulkan_context.h"

namespace flutter {
namespace testing {

TestVulkanImage::TestVulkanImage() = default;

TestVulkanImage::TestVulkanImage(TestVulkanImage&& other) = default;
TestVulkanImage& TestVulkanImage::operator=(TestVulkanImage&& other) = default;

TestVulkanImage::~TestVulkanImage() = default;

VkImage TestVulkanImage::GetImage() {
  return image_;
}

}  // namespace testing
}  // namespace flutter
