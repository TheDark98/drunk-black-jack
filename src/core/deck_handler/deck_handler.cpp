#include "deck_handler.h"

#include <generator.h>

DrunkEngine::DeckHandler::DeckHandler(const uint8_t playersCount)
    : m_deckIndex(0), m_deckSeed(0)
{
    m_playersHands.resize(playersCount);
}

DrunkEngine::DeckHandler::DeckHandler(const uint8_t playersCount, const uint64_t seed)
    : m_deckIndex(0), m_deckSeed(seed)
{
    m_playersHands.resize(playersCount);
    generateDeck();
}

void DrunkEngine::DeckHandler::SetDeckSeed(const uint64_t seed)
{
    m_deckSeed = seed;
    generateDeck();
}

uint64_t DrunkEngine::DeckHandler::GetDeckSeed() const
{
    return m_deckSeed;
}

DrunkEngine::Card DrunkEngine::DeckHandler::Draw(const uint8_t playerIndex)
{
    if (m_deckIndex >= DECK_SIZE)
        return Card();

    const Card drawnCard = m_deck[m_deckIndex++];

    m_playersHands[playerIndex] += drawnCard;
    return drawnCard;
}

DrunkEngine::Card DrunkEngine::DeckHandler::Draw()
{
    if (m_deckIndex >= DECK_SIZE)
        return Card();
    return m_deck[m_deckIndex++];
}

DrunkEngine::Card DrunkEngine::DeckHandler::PeekCard(const uint8_t index) const
{
    return m_deckIndex < DECK_SIZE ? m_deck[m_deckIndex] : Card();
}

uint8_t DrunkEngine::DeckHandler::GetDrawIndex() const
{
    return m_deckIndex;
}

DrunkEngine::Hand &DrunkEngine::DeckHandler::GetPlayerHand(const uint8_t playerIndex)
{
    return m_playersHands[playerIndex];
}

void DrunkEngine::DeckHandler::generateDeck()
{
    using namespace DrunkEngine;

    RandomGenerator numberRandomizer(m_deckSeed);
    uint8_t addedCards = 0;
    Card newCard;

    while (addedCards < DECK_SIZE)
    {
        const uint64_t randomValue = numberRandomizer.Generate();
        bool cardInDeck = false; // First card will always be added

        newCard.SetTraits(CardTrait::Rank(1 + randomValue % RANKS), CardTrait::Suit(1 + randomValue % SUITS));

        for (uint8_t i = 0; !cardInDeck && addedCards != 0 && i < addedCards; i++)
            cardInDeck = m_deck[i] == newCard;

        if (!cardInDeck)
        {
            m_deck[addedCards] = newCard;
            addedCards++;
        }
    }
}