#pragma once

#include "cards_type.h"

namespace DrunkEngine {
    struct Card {
        DrunkEngine::CardType::Value value = DrunkEngine::CardType::Value::UNDEFINED;
        DrunkEngine::CardType::Seed seed = DrunkEngine::CardType::Seed::UNDEFINED;

        bool operator==(const Card other) const {
            return this->value == other.value && seed == other.seed;
        }
    };
}