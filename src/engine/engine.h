#pragma once

#include "engine_types.h"

namespace DrunkEngine
{
    class GameEngine
    {
    public:
        GameEngine();

        void SetGameState(const DrunkEngine::GameState state);
        void SetGameRule(const DrunkEngine::GameRule rule);
        void SetGameDifficulty(const DrunkEngine::GameDifficulty difficulty);

        bool IsInGameState(DrunkEngine::GameState checkingState) const;

        DrunkEngine::GameState GetGameState() const;
        DrunkEngine::GameRule GetGameRule() const;
        DrunkEngine::GameDifficulty GetGameDifficulty() const;

    private:
        DrunkEngine::GameState gameState;
        DrunkEngine::GameRule gameRule;
        DrunkEngine::GameDifficulty gameDifficulty;
    };
}