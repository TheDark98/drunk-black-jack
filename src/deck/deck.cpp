#include "deck.h"
#include <generator.h>

DrunkEngine::Deck::Deck(const uint64_t seed) 
    : deckIndex(0), deckSeed(seed) {
    using namespace DrunkEngine;

    Random deckGenerator(deckSeed);

    while (deckIndex < DECK_SIZE - 1) {
        Card newCard;
        newCard.seed = CardType::Seed(1 + deckGenerator.NextRandom() % 4);
        newCard.value = CardType::Value(1 + deckGenerator.NextRandom() % 13);
        bool cardInDeck = false;

        for (uint8_t i = 0; deckIndex > 0 && i < deckIndex; i++) {
            cardInDeck = cards[i] == newCard;
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

DrunkEngine::Card DrunkEngine::Deck::LastDraw() {
    return cards[deckIndex - 1];
}