#pragma once

#include <cstdint>
#include <array>
#include <card.h>

#define HAND_SIZE 11

namespace DrunkEngine
{
    class Hand
    {
    public:
        Hand();
        void AddCard(const DrunkEngine::Card card);
        std::array<DrunkEngine::Card, HAND_SIZE> GetHand() const;
        uint8_t GetValue() const;

    private:
        uint8_t calcValue();
        std::array<DrunkEngine::Card, HAND_SIZE> cards;
        uint8_t handIndex;
        uint8_t handValue;
    };
}