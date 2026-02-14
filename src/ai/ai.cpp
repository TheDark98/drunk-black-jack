#include "ai.h"
#include <vector>
#include <numeric>

DrunkEngine::DealerAI::DealerAI(DrunkEngine::GameDifficulty gDifficulty, const DrunkEngine::Hand *dHand, const DrunkEngine::Hand &pHand, const DrunkEngine::Deck &gDeck)
    : gameDifficulty(gDifficulty), dealerHand(dHand), playerHand(pHand), deck(gDeck)
{
}

bool DrunkEngine::DealerAI::choice()
{
    const double difficultyImpact = difficultyWeight();
    const double playerImpact = playerHandWeight();
    const double dealerImpact = dealerHandWeight();
    double deckImpact = deckWeight();

    const double errorMultuplier = 0.5 + (1.0 - difficultyImpact + 1.0) / 2.0;

    if (gameDifficulty != GameDifficulty::CHEAT)
        deckImpact = 1.0;

    const double finalScore = deckImpact * ((playerImpact + dealerImpact) / 3.0) * errorMultuplier;
    return finalScore > 0.5;
}

constexpr double DrunkEngine::DealerAI::difficultyWeight() const
{
    switch (gameDifficulty)
    {
    case GameDifficulty::EASY:
        return 0.3;
    case GameDifficulty::NORMAL:
        return 0.6;
    case GameDifficulty::HARD:
        return 0.9;
    case GameDifficulty::CHEAT:
        return 1.0;
    default:
        return 0.3;
    }
}

double DrunkEngine::DealerAI::playerHandWeight()
{
    std::pair<double, uint8_t> bigCardsSum = unifiedCardSum(playerHand, true);
    std::pair<double, uint8_t> smallCardsSum = unifiedCardSum(playerHand, true);

    const double perfectScore = playerHand.GetValue() / 21;
    const double totalBigCard = bigCardsSum.first / bigCardsSum.second;
    const double totalSmallCard = smallCardsSum.first / smallCardsSum.second;

    const double handWeight = bigCardsSum.second / (playerHand.GetSize() + smallCardsSum.second);
    const double cardWeight = totalBigCard / totalSmallCard;

    return perfectScore * handWeight * cardWeight;
}

double DrunkEngine::DealerAI::dealerHandWeight()
{
    std::pair<double, uint8_t> bigCardsSum = unifiedCardSum(*dealerHand, true);
    std::pair<double, uint8_t> smallCardsSum = unifiedCardSum(*dealerHand, true);

    const double perfectScore = dealerHand->GetValue() / 21;
    const double cardWeight = (bigCardsSum.first * bigCardsSum.second) / (smallCardsSum.first * smallCardsSum.second) / DrunkEngine::FromEnumToInt(deck.LastDraw().value);

    return (1.0 - perfectScore) * cardWeight;
}

double DrunkEngine::DealerAI::deckWeight()
{
    const double nextCardValue = DrunkEngine::FromEnumToInt(deck.PickCard(deck.GetIndex() + 1).value);
    return nextCardValue + dealerHand->GetValue() > 21 ? 0.0 : 1.0;
}

std::pair<double, uint8_t> DrunkEngine::DealerAI::unifiedCardSum(const DrunkEngine::Hand &handA, bool isBigSum)
{
    std::vector<double> cards(6);

    for (std::pair<uint8_t, bool> i = {0, true}; i.first < playerHand.GetSize(); i.first++)
    {
        const double elementValue = DrunkEngine::FromEnumToInt(playerHand.GetHand()[i.first].value);

        const bool isValueSmall = i.first < 5;

        if (isValueSmall && isBigSum)
            continue;
        if (!isValueSmall && !isBigSum)
            continue;

        if (!isValueSmall)
        {
            cards.push_back(elementValue);
            continue;
        }

        if (isBigSum && i.first == 1 && i.second && playerHand.GetRawValue() < 21 - 10)
        {
            cards.push_back(elementValue + 10);
            i.second = false;
        }
        else if (isValueSmall)
        {
            cards.push_back(elementValue);
        }
    }
    return std::make_pair(std::reduce(cards.begin(), cards.end()), cards.size());
}
