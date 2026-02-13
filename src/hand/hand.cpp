#include "hand.h"
#include <cstdio>
DrunkEngine::Hand::Hand()
    : handValue(0), handIndex(0)
{
    const DrunkEngine::Card ghostCard = Card{CardType::Value::UNDEFINED, CardType::Seed::UNDEFINED};

    for (uint8_t i = 0; i < cards.size(); i++)
    {
        cards[i] = ghostCard;
    }
}

void DrunkEngine::Hand::AddCard(const DrunkEngine::Card card)
{
    cards[handIndex] = card;
    handIndex++;
    handValue = calcValue();
}

std::array<DrunkEngine::Card, HAND_SIZE> DrunkEngine::Hand::GetHand() const
{
    return cards;
}

uint8_t DrunkEngine::Hand::GetValue() const
{
    return handValue;
}

uint8_t DrunkEngine::Hand::calcValue()
{
    using namespace DrunkEngine;

    if (handIndex == 0)
        return 0;

    uint8_t totalValue = 0;
    uint8_t aceNumber = 0;

    for (uint8_t i = 0; i < handIndex; i++)
    {
        if (cards[i].value == CardType::Value::UNDEFINED)
            break;

        if (cards[i].value != CardType::Value::ACE)
            totalValue += DrunkEngine::FromEnumToInt(cards[i].value);
        else
            aceNumber++;
    }

    if (aceNumber > 0)
    {
        const uint8_t totalMaxAce = aceNumber + 10;
        if (totalValue + totalMaxAce < 22)
            totalValue += totalMaxAce;
        else
            totalValue += aceNumber;
    }

    return totalValue;
}