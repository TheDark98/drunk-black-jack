#include "deck.h"
#include <generator.h>

DrunkEngine::Deck::Deck(const uint32_t seed) 
    : deckIndex(0) {
    using namespace DrunkEngine;

    Random deckSeed(seed);

    while (deckIndex < DECK_SIZE - 1) {
        Card newCard;
        newCard.seed = CardType::Seed(1 + deckSeed.NextRandom() % 4);
        newCard.value = CardType::Value(1 + deckSeed.NextRandom() % 13);
        bool cardInDeck = false;

        for (ushort j = 0; j < deckIndex; j++) {
            cardInDeck = cards[j] == newCard;
            if (cardInDeck) break;
        }

        if (!cardInDeck) {
            cards[deckIndex] = newCard;
            deckIndex++;
        }
    }
    deckIndex = 0;
}

DrunkEngine::Card DrunkEngine::Deck::Draw() {
    deckIndex++;
    return cards[deckIndex - 1];
}