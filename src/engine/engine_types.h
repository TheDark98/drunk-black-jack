#pragma once

namespace DrunkEngine
{
    enum class GameState
    {
        MENU, // starting point
        LOADING,
        PLAYING,
        PAUSED,
        LOSE,
        WIN,
        EXIT
    };

    enum class GameDifficulty
    {
        EASY,
        NORMAL
    };

    enum class GameRule
    {
        CLASSIC,
        DRUNK
    };
}