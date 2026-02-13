#pragma once

#include <string_view>
#include <cstdint>

namespace DrunkEngine
{
    namespace CardType
    {
        constexpr std::string_view SeedName[5] = {
            "Undefined",
            "Heart",
            "Diamond",
            "Spade",
            "Club"};

        enum class Seed
        {
            UNDEFINED,
            HEART,
            DIAMOND,
            SPADE,
            CLUB
        };

        constexpr std::string_view ValueName[14] = {
            "Undefined",
            "Ace",
            "Two",
            "Three",
            "Four",
            "Five",
            "Six",
            "Seven",
            "Eight",
            "Nine",
            "Ten",
            "Jack",
            "Queen",
            "King"};

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
            JACK,
            QUEEN,
            KING
        };
    }
}