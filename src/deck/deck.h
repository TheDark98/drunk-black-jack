#pragma once

#include <card.h>
#include <cstdint>
#include <array>

#define DECK_SIZE 52

namespace DrunkEngine {
    class Deck {
    public:
        Deck(const uint32_t seed);
        DrunkEngine::Card Draw();
    private:
        std::array<DrunkEngine::Card, DECK_SIZE> cards;
        uint8_t deckIndex;
    };
}