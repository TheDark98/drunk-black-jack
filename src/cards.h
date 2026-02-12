#pragma once

namespace cards {

    enum seed {
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

    constexpr unsigned int handValue(const value cardType[]);
}