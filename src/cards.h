#pragma once

#include <utility>

namespace cards {

    enum seed {
        Null,
        Heart,
        Diamond,
        Spade,
        Club
    };

    enum value {
        Null,
        Ace,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack = 10,
        Queen = 10,
        King = 10
    };

    constexpr unsigned int handValue(const std::pair<cards::value, cards::seed> cardType[12]);
}