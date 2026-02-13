#pragma once

#include <cstdint>
#include <random>

namespace DrunkEngine
{
    class Random
    {
    public:
        Random(const uint64_t seed);
        uint64_t NextRandom() const;

    private:
        mutable std::mt19937_64 generator;
    };
}