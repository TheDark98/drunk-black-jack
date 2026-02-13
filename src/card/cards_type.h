#pragma once

namespace DrunkEngine
{
    namespace CardType
    {

        enum class Seed
        {
            UNDEFINED,
            HEART,
            DIAMOND,
            SPADE,
            CLUB
        };

        enum class Value
        {
            UNDEFINED,
            ACE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            EIGHT,
            NINE,
            TEN,
            JACK = 10,
            QUEEN = 10,
            KING = 10
        };
    }
}