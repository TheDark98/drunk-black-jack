#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>
#include <random>

#include <engine.h>
#include <card.h>
#include <deck.h>
#include <hand.h>

#define MAX_ROUNDS 9

std::string InputLine();
bool InputIsYes(const std::string_view response);
uint64_t askPlayerSeed();
bool isPlayerWinner(const DrunkEngine::Hand &playerHand, const DrunkEngine::Hand &dealerHand);

int main()
{
    DrunkEngine::GameEngine engine;
    while (!engine.IsInGameState(DrunkEngine::GameState::EXIT))
    {
        const uint64_t seed = askPlayerSeed();
        engine.SetGameState(DrunkEngine::GameState::PLAYING);

        while (engine.IsInGameState(DrunkEngine::GameState::PLAYING) || engine.IsInGameState(DrunkEngine::GameState::PAUSED))
        {
            DrunkEngine::Deck deck(seed);

            DrunkEngine::Hand playerHand;
            DrunkEngine::Hand dealerHand;

            playerHand.AddCard(deck.Draw());
            printf("You got: %s\n", DrunkEngine::FromCardToString(deck.LastDraw()).data());
            dealerHand.AddCard(deck.Draw());
            printf("Dealer got: %s\n", DrunkEngine::FromCardToString(deck.LastDraw()).data());

            playerHand.AddCard(deck.Draw());
            printf("You got: %s\n", DrunkEngine::FromCardToString(deck.LastDraw()).data());
            dealerHand.AddCard(deck.Draw());
            printf("Dealer got a card\n");

            printf("Your Hand Value: %d\n", playerHand.GetValue());

            // Player Draw Phase
            for (uint8_t i = 0; i < MAX_ROUNDS; i++)
            {
                printf("Draw a card? (Y/n): ");
                if (!InputIsYes(InputLine()) || playerHand.GetValue() > 21)
                    break;

                playerHand.AddCard(deck.Draw());
                printf("You got: %s\n", DrunkEngine::FromCardToString(deck.LastDraw()).data());
                printf("Your Hand Value: %d\n", playerHand.GetValue());
            }

            // Dealer Draw Phase
            for (uint8_t i = 0; i < MAX_ROUNDS; i++)
            {
                if (playerHand.GetValue() > 21)
                    break;
                if (dealerHand.GetValue() > 21)
                    break;

                std::random_device rd;

                bool botStopDrawing = (rd() % 2);

                if (botStopDrawing)
                    break;

                dealerHand.AddCard(deck.Draw());
                printf("Dealer got a card\n");
            }

            if (playerHand.GetValue() < 22)
            {
                printf("Your Hand\n");
                printf("%s\n", DrunkEngine::FromCardToString(playerHand.GetHand()).data());
                printf("Value of Your Hand: %d\n", playerHand.GetValue());

                printf("Dealer Hand\n");
                printf("%s\n", DrunkEngine::FromCardToString(dealerHand.GetHand()).data());
                printf("Value of Dealer Hand: %d\n", dealerHand.GetValue());
            }
            else
                printf("Oof... You broke the barrier");

            bool playerWon = isPlayerWinner(playerHand, dealerHand);

            if (playerWon)
                engine.SetGameState(DrunkEngine::GameState::WIN);
            else
                engine.SetGameState(DrunkEngine::GameState::LOSE);
        }
        printf("You %s\n", engine.IsInGameState(DrunkEngine::GameState::WIN) ? "WON!" : "LOSE!");
        printf("Play Again? (Y/n): ");
        if (!InputIsYes(InputLine()))
            engine.SetGameState(DrunkEngine::GameState::EXIT);
    }
    return 0;
}

std::string InputLine()
{
    std::string response;
    std::getline(std::cin, response);
    response.erase(std::remove_if(response.begin(), response.end(), isspace), response.end());
    return response;
}

bool InputIsYes(const std::string_view response)
{
    const char lowerChar = response.empty() ? 'f' : std::tolower(response[0]);

    if (lowerChar == 'n' || lowerChar == 'm' || lowerChar == 'b')
        return false;
    else if (lowerChar == 'y' || lowerChar == 't' || lowerChar == 'u')
        return true;

    return true;
}

uint64_t askPlayerSeed()
{
    printf("Seed (0 is random): ");
    std::string userResponse = InputLine();
    if (userResponse.empty())
        userResponse = "0";
    uint64_t response = std::stoull(userResponse);

    if (response != 0)
        return response;
    std::random_device rd;
    uint64_t doublemized = static_cast<uint64_t>(rd()) << 32;
    doublemized += static_cast<uint64_t>(rd());
    return doublemized;
}

bool isPlayerWinner(const DrunkEngine::Hand &playerHand, const DrunkEngine::Hand &dealerHand)
{
    bool playerIsGreater = playerHand.GetValue() > dealerHand.GetValue();
    bool playerHandIsValid = playerHand.GetValue() < 22;
    bool dealerHandIsValid = dealerHand.GetValue() < 22;

    if (playerHandIsValid && dealerHandIsValid)
        return playerIsGreater;
    if (!playerHandIsValid)
        return false;
    return true;
}