#pragma once


#include <engine_types.h>
#include <deck.h>
#include <hand.h>

namespace DrunkEngine
{
    class DealerAI 
    {
    public:
        DealerAI(const DrunkEngine::GameDifficulty gDifficulty, const DrunkEngine::Hand *dHand, const DrunkEngine::Hand &pHand, const DrunkEngine::Deck &gDeck);
        bool choice();
    private:
        const DrunkEngine::GameDifficulty gameDifficulty;
        const DrunkEngine::Hand* dealerHand;
        const DrunkEngine::Hand playerHand;
        const DrunkEngine::Deck deck;
        constexpr double difficultyWeight() const;
        double playerHandWeight();
        double dealerHandWeight();
        double deckWeight();
        std::pair<double, uint8_t> unifiedCardSum(const DrunkEngine::Hand &handA, bool isBigSum);
    };
}