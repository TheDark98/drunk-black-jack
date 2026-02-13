#include "engine.h"

DrunkEngine::GameEngine::GameEngine()
    : gameState(DrunkEngine::GameState::MENU),
      gameDifficulty(DrunkEngine::GameDifficulty::EASY),
      gameRule(DrunkEngine::GameRule::CLASSIC)
{
}

void DrunkEngine::GameEngine::SetGameState(const DrunkEngine::GameState state)
{
    gameState = state;
}

void DrunkEngine::GameEngine::SetGameRule(const DrunkEngine::GameRule rule)
{
    gameRule = rule;
}

void DrunkEngine::GameEngine::SetGameDifficulty(const DrunkEngine::GameDifficulty difficulty)
{
    gameDifficulty = difficulty;
}

bool DrunkEngine::GameEngine::IsInGameState(DrunkEngine::GameState checkingState) const
{
    return checkingState == gameState;
}

DrunkEngine::GameState DrunkEngine::GameEngine::GetGameState() const
{
    return gameState;
}

DrunkEngine::GameRule DrunkEngine::GameEngine::GetGameRule() const
{
    return gameRule;
}

DrunkEngine::GameDifficulty DrunkEngine::GameEngine::GetGameDifficulty() const
{
    return gameDifficulty;
}
