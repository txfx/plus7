#pragma once

#include <memory>

#include "SdlGl/GlTexture.hpp"

namespace p7 {
namespace gfx {

using Texture    = GlTexture;
using TexturePtr = std::shared_ptr<Texture>;

} // namespace gfx
} // namespace p7