#pragma once

#include <Modules/Gfx/BufferInterface.hpp>

namespace p7 {
namespace gfx {

struct GlBuffer final : public BufferInterface
{
    GlBuffer(uint32_t _size);
    GlBuffer(uint32_t _size, const void* _data);

    uint32_t GetSize() const override;

    void UpdateData(uint32_t _offset, uint32_t _size, void* _data) override;
};

} // namespace gfx
} // namespace p7