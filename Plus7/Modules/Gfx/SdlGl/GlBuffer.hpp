#pragma once

#include <Modules/Gfx/BufferInterface.hpp>

namespace p7 {
namespace gfx {

struct GlBuffer final : public BufferInterface
{
    uint32_t GetSize() const override;

    void UpdateData(uint32_t _offset, uint32_t _size, void* _data) override;
};

} // namespace gfx
} // namespace p7