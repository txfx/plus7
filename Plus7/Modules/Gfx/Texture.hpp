#pragma once

#include <memory>

#include <Modules/Gfx/Dummy/DummyTexture.hpp>

namespace p7 {
namespace gfx {
using Texture    = DummyTexture;
using TexturePtr = std::shared_ptr<Texture>;
} // namespace gfx
} // namespace p7