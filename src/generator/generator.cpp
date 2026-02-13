#include "generator.h"

DrunkEngine::Random::Random(const uint64_t seed)
    : generator(seed) {}

uint64_t DrunkEngine::Random::NextRandom() const
{
    return generator();
}
