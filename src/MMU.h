#pragma once

#include <array>
#include "utils.h"

struct MemorySegment{

    Pointer start;
    Pointer end;
    inline constexpr Pointer size() const { return (end-start)+1; }

};

constexpr struct MemorySegment SEGMENT_ROM                  {0x0000,0x3fff};
constexpr struct MemorySegment SEGMENT_SWITCHABLE_ROM_BANK  {0x4000,0x7fff};
constexpr struct MemorySegment SEGMENT_VIDEO_RAM            {0x8000,0x9fff};
constexpr struct MemorySegment SEGMENT_SWITCHABLE_RAM_BANK  {0xa000,0xbfff};
constexpr struct MemorySegment SEGMENT_INTERNAL_RAM         {0xc000,0xdfff};
constexpr struct MemorySegment SEGMENT_ECHO_INTERNAL_RAM    {0xe000,0xfdff};
constexpr struct MemorySegment SEGMENT_SPRITE_ATTRIBUTES    {0xfe00,0xfe9f};
constexpr struct MemorySegment SEGMENT_UNUSABLE2            {0xfea0,0xfeff};
constexpr struct MemorySegment SEGMENT_IO                   {0xff00,0xff4b};
constexpr struct MemorySegment SEGMENT_UNUSABLE3            {0xff4c,0xff7f};
constexpr struct MemorySegment SEGMENT_HIGH_RAM             {0xff80,0xfffe};
constexpr struct MemorySegment SEGMENT_INTERRUPT_REGISTER   {0xffff,0xffff};

class MMU{

public:

    MMU();
    Byte read( Pointer address );
    void write( Pointer address, Byte value);

private:

    std::array< Byte, SEGMENT_VIDEO_RAM.size() > video_ram = {0};
    std::array< Byte, SEGMENT_INTERNAL_RAM.size() > internal_ram = {0};

};