#include "cards.h"

constexpr unsigned int cards::handValue(const std::pair<cards::value, cards::seed> cardsHand[12]) {

    if (cardsHand[11].first != cards::value::Null) return 0;

    unsigned int handValue = 0;
    unsigned int numberOfAce = 0;

    for (unsigned int i = 0; i < 12; i++) {
        const value currentCard = cardsHand[i].first;

        if (currentCard != cards::value::Ace)
            handValue += currentCard;
        else
            numberOfAce++;

        if (handValue > 21) return 0;
    }

    if (numberOfAce > 0) {
        const unsigned int totalMaxAce = numberOfAce + 10;

        if (handValue + numberOfAce > 21) return 0;

        if (handValue + totalMaxAce > 21)
            handValue += numberOfAce;
        else
            handValue += totalMaxAce;
    }

    return handValue;
}
