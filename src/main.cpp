#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <random>

#include <engine.h>
#include <card.h>
#include <deck.h>
#include <hand.h>
#include "colors.h"
#include <thread>
#include "ai.h"

#define MAX_ROUNDS 9
#define SPACER "————————————————————————————————————————\n"
#ifdef _WIN32
#define BACKGROUND_COLOR
#else
#define BACKGROUND_COLOR
#endif

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string InputLine();
bool InputIsYes(const std::string_view response);
uint64_t askPlayerSeed();
bool isPlayerWinner(const DrunkEngine::Hand &playerHand, const DrunkEngine::Hand &dealerHand);
void formattedPrint(const std::string_view style, const std::string_view color, const std::string_view message);
void formattedPrint(const std::string_view message);
void printPlayerHand(const DrunkEngine::Hand &hand);
void printDealerHand(const DrunkEngine::Hand &hand);
void printDealerSecretHand(const DrunkEngine::Card card);

int main()
{
    clearScreen();
    DrunkEngine::GameEngine engine;
    while (!engine.IsInGameState(DrunkEngine::GameState::EXIT))
    {
        const uint64_t seed = askPlayerSeed();
        engine.SetGameState(DrunkEngine::GameState::PLAYING);
        clearScreen();

        while (engine.IsInGameState(DrunkEngine::GameState::PLAYING) || engine.IsInGameState(DrunkEngine::GameState::PAUSED))
        {
            DrunkEngine::Deck deck(seed);

            DrunkEngine::Hand playerHand;
            DrunkEngine::Hand dealerHand;

            playerHand.AddCard(deck.Draw());
            printPlayerHand(playerHand);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            dealerHand.AddCard(deck.Draw());
            printPlayerHand(playerHand);
            printDealerHand(dealerHand);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            playerHand.AddCard(deck.Draw());
            printPlayerHand(playerHand);
            printDealerSecretHand(dealerHand.GetHand()[0]);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            dealerHand.AddCard(deck.Draw());

            // Player Draw Phase
            for (uint8_t i = 0; i < MAX_ROUNDS; i++)
            {
                printPlayerHand(playerHand);
                printDealerSecretHand(dealerHand.GetHand()[0]);
                formattedPrint("Draw a card? (Y/n): ");
                if (!InputIsYes(InputLine()) || playerHand.GetValue() > 21)
                    break;
                clearScreen();

                playerHand.AddCard(deck.Draw());
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            clearScreen();

            DrunkEngine::DealerAI dealerAI(engine.GetGameDifficulty(), &dealerHand, playerHand, deck);

            // Dealer Draw Phase
            for (uint8_t i = 0; i < MAX_ROUNDS; i++)
            {
                if (playerHand.GetValue() > 21)
                    break;
                if (dealerHand.GetValue() >= 21)
                    break;

                printPlayerHand(playerHand);
                printDealerHand(dealerHand);
                formattedPrint("Dealer is thinking...\n");

                if (dealerAI.choice())
                    break;

                dealerHand.AddCard(deck.Draw());
                std::this_thread::sleep_for(std::chrono::milliseconds(((playerHand.GetValue() + dealerHand.GetValue()) / 10) * 1000));
                clearScreen();
            }
            clearScreen();

            printPlayerHand(playerHand);
            printDealerHand(dealerHand);
            formattedPrint("Press Enter to Continue\n");
            std::cin.get();
            clearScreen();

            bool playerWon = isPlayerWinner(playerHand, dealerHand);

            if (playerWon)
                engine.SetGameState(DrunkEngine::GameState::WIN);
            else
                engine.SetGameState(DrunkEngine::GameState::LOSE);
        }

        const bool playerWon = engine.IsInGameState(DrunkEngine::GameState::WIN);
        std::string resoultString = playerWon ? "WON!" : "LOSE!";

        formattedPrint(BOLD, playerWon ? WIN_COLOR : LOSE_COLOR, std::string("YOU ") + resoultString + "\n");
        formattedPrint(BOLD, WHITE, SPACER);
        formattedPrint("Play Again? (Y/n): ");
        if (!InputIsYes(InputLine()))
            engine.SetGameState(DrunkEngine::GameState::EXIT);
        clearScreen();
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
    formattedPrint("Seed (0 is random): ");
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

void formattedPrint(const std::string_view style, const std::string_view color, const std::string_view message)
{
    printf("%s%s%s%s", style.data(), color.data(), message.data(), RESET);
}

void formattedPrint(const std::string_view message)
{
    printf("%s%s%s", WHITE, message.data(), RESET);
}

void printPlayerHand(const DrunkEngine::Hand &hand)
{
    formattedPrint("Your Hand\n");
    formattedPrint(DrunkEngine::FromCardToString(hand.GetHand()) + "\n");
    formattedPrint(BOLD, WHITE, std::string("Value: ") + std::to_string(hand.GetValue()) + "\n");
    formattedPrint(BOLD, WHITE, SPACER);
}

void printDealerHand(const DrunkEngine::Hand &hand)
{
    formattedPrint("Dealer Hand\n");
    formattedPrint(DrunkEngine::FromCardToString(hand.GetHand()) + "\n");
    formattedPrint(BOLD, WHITE, std::string("Value: ") + std::to_string(hand.GetValue()) + "\n");
    formattedPrint(BOLD, WHITE, SPACER);
}

void printDealerSecretHand(const DrunkEngine::Card card)
{
    formattedPrint("Dealer Hand\n");
    formattedPrint(DrunkEngine::FromCardToString(card) + "\n");
    formattedPrint("Secret Card\n");
    formattedPrint(BOLD, WHITE, std::string("Value: ") + "?" + "\n");
    formattedPrint(BOLD, WHITE, SPACER);
}