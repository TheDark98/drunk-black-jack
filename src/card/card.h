#pragma once

#include "cards_type.h"
#include <string>
#include <array>

namespace DrunkEngine {

    constexpr uint8_t FromEnumToInt(const DrunkEngine::CardType::Value value);
    constexpr uint8_t FromEnumToInt(const DrunkEngine::CardType::Seed seed);
    constexpr std::string_view FromEnumToString(const DrunkEngine::CardType::Value value);
    constexpr std::string_view FromEnumToString(const DrunkEngine::CardType::Seed seed);

    struct Card {
        DrunkEngine::CardType::Value value = DrunkEngine::CardType::Value::UNDEFINED;
        DrunkEngine::CardType::Seed seed = DrunkEngine::CardType::Seed::UNDEFINED;

        bool operator==(const Card other) const {
            return this->value == other.value && seed == other.seed;
        }

        uint8_t operator+(const Card other) const {
            return FromEnumToInt(this->value) + FromEnumToInt(other.value);
        }
    };

    constexpr uint8_t FromEnumToInt(const DrunkEngine::CardType::Value value) {
        if (value == DrunkEngine::CardType::Value::UNDEFINED) return 0;
        if (value >= DrunkEngine::CardType::Value::JACK) return 10;
        return static_cast<uint8_t>(value);
    }

    constexpr uint8_t FromEnumToInt(const DrunkEngine::CardType::Seed seed) {
        if (seed == DrunkEngine::CardType::Seed::UNDEFINED) return 0;
        return static_cast<uint8_t>(seed);
    }

    constexpr std::string_view FromEnumToString(const DrunkEngine::CardType::Value value) {
        return DrunkEngine::CardType::ValueName[FromEnumToInt(value)];
    }

    constexpr std::string_view FromEnumToString(const DrunkEngine::CardType::Seed seed) {
        return DrunkEngine::CardType::SeedName[FromEnumToInt(seed)];
    }

    constexpr std::string FromCardToString(const DrunkEngine::Card card) {
        return std::string(FromEnumToString(card.value)) + " | " + std::string(FromEnumToString(card.seed).data());
    }

    constexpr std::string FromCardToString(const std::array<DrunkEngine::Card, 11> &cards) {
        std::string handString;
        handString.reserve(256);

        for (uint8_t i = 0; i < cards.size() && cards[i].value != DrunkEngine::CardType::Value::UNDEFINED; i++) {
            if (i > 0) handString += "\n";
            handString += FromCardToString(cards[i]);
        }
        return handString;
    }
}