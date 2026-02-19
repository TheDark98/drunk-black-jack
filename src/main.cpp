#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>

#include <engine.h>
#include <deck_handler.h>
#include <player.h>
#include <ai_logic.h>
#include <colors.h>
#include <special_characters.h>

#define MAX_ROUNDS 9
#define VISUAL_SPACER "————————————————————————————————————————\n"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

const std::string SuitName[5] = {
    "Undefined",
    SUIT_HEART,
    SUIT_DIAMOND,
    SUIT_SPADE,
    SUIT_CLUB
};

const std::string SuitColor[5] = {
    ERROR_COLOR,
    HEART_COLOR,
    DIAMOND_COLOR,
    SPADE_COLOR,
    CLUB_COLOR
};

const std::string RankName[14] = {
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
    "King"
};

const std::string RankColor[14] = {
    ERROR_COLOR,
    ACE_COLOR,
    TWO_COLOR,
    THREE_COLOR,
    FOUR_COLOR,
    FIVE_COLOR,
    SIX_COLOR,
    SEVEN_COLOR,
    EIGHT_COLOR,
    NINE_COLOR,
    TEN_COLOR,
    JACK_COLOR,
    QUEEN_COLOR,
    KING_COLOR
};

std::string InputLine();
bool InputIsYes(const std::string_view response);
uint64_t askPlayerSeed();
bool isPlayerWinner(const uint8_t playerScore, const uint8_t dealerScore);
void formattedPrint(const std::string_view style, const std::string_view color, const std::string_view message);
void formattedPrint(const std::string_view message);
void printHand(DrunkEngine::Hand &hand);
void printSecretHand(DrunkEngine::Hand &hand);
std::string FromCardToString(const DrunkEngine::Card card);
std::string FromHandToString(DrunkEngine::Hand &hand);
std::string FromSecretHandToString(DrunkEngine::Hand &hand);

int main()
{
    clearScreen();
    DrunkEngine::GameEngine engine;
    while (!(engine == DrunkEngine::GameState::QUITTING))
    {
        const uint64_t seed = askPlayerSeed();
        engine.SetGameState(DrunkEngine::GameState::IN_PROGRESS);
        clearScreen();

        while (engine == DrunkEngine::GameState::IN_PROGRESS || engine == DrunkEngine::GameState::PAUSED)
        {
            std::vector<DrunkEngine::Player> players;
            players.reserve(2);
            players.emplace_back(0);
            players.emplace_back(1);
            
            DrunkEngine::DeckHandler deck(players.size(), seed);

            for (auto &player : players)
                player.AssignHand(&deck.GetPlayerHand(player.GetPlayerID()));

            players[1].GetHand() += deck.Draw();
            formattedPrint("Your Hand\n");
            printHand(players[1].GetHand());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            players[0].GetHand() += deck.Draw();
            formattedPrint("Your Hand\n");
            printHand(players[1].GetHand());
            formattedPrint("Dealer Hand\n");
            printHand(players[0].GetHand());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            players[1].GetHand() += deck.Draw();
            formattedPrint("Your Hand\n");
            printHand(players[1].GetHand());
            formattedPrint("Dealer Hand\n");
            printSecretHand(players[0].GetHand());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            players[0].GetHand() += deck.Draw();
            formattedPrint("Your Hand\n");
            printHand(players[1].GetHand());
            formattedPrint("Dealer Hand\n");
            printSecretHand(players[0].GetHand());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            clearScreen();

            // Player Draw Phase
            for (uint8_t i = 0; i < MAX_ROUNDS; i++)
            {
                formattedPrint("Your Hand\n");
                printHand(players[1].GetHand());
                formattedPrint("Dealer Hand\n");
                printSecretHand(players[0].GetHand());
                formattedPrint("Draw a card? (Y/n): ");
                if (!InputIsYes(InputLine()) || players[1].GetHand() > 21)
                    break;
                clearScreen();

                players[1].GetHand() += deck.Draw();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            clearScreen();

            // Dealer Draw Phase
            for (uint8_t i = 0; i < MAX_ROUNDS; i++)
            {
                if (players[1].GetHand() > 21)
                    break;
                if (players[0].GetHand() >= players[1].GetHand().GetValue(false))
                    break;

                formattedPrint("Dealer is thinking...\n");
                if (DrunkEngine::AILogic::Choose(engine.GetDifficulty(), players[0].GetPlayerID(), &players)) break;

                players[0].GetHand() += deck.Draw();
                formattedPrint("Your Hand\n");
                printHand(players[1].GetHand());
                formattedPrint("Dealer Hand\n");
                printHand(players[0].GetHand());
                std::this_thread::sleep_for(std::chrono::milliseconds((players[0].GetPlayerID() / 10) * 1000));
                clearScreen();
            }
            clearScreen();

            formattedPrint("Your Hand\n");
            printHand(players[1].GetHand());
            formattedPrint("Dealer Hand\n");
            printHand(players[0].GetHand());
            formattedPrint("Press Enter to Continue\n");
            InputLine();
            clearScreen();

            engine.SetGameState(DrunkEngine::GameState::ENDED);

            const bool playerWon = isPlayerWinner(players[1].GetHand().GetValue(false), players[0].GetHand().GetValue(false));

            if (playerWon)
                engine.SetResult(DrunkEngine::Result::VICTORY);
            else
                engine.SetResult(DrunkEngine::Result::DEFEAT);
        }

        const bool playerWon = engine == DrunkEngine::Result::VICTORY;
        std::string resoultString = playerWon ? "WON!" : "LOSE!";

        formattedPrint(BOLD, playerWon ? WIN_COLOR : LOSE_COLOR, std::string("YOU ") + resoultString + "\n");
        formattedPrint(BOLD, WHITE, VISUAL_SPACER);
        formattedPrint("Play Again? (Y/n): ");
        if (!InputIsYes(InputLine()))
            engine.SetGameState(DrunkEngine::GameState::QUITTING);
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

bool isPlayerWinner(const uint8_t playerScore, const uint8_t dealerScore)
{
    bool playerIsGreater = playerScore > dealerScore;
    bool playerHandIsValid = playerScore < 22;
    bool dealerHandIsValid = dealerScore < 22;

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

void printHand(DrunkEngine::Hand &hand)
{
    formattedPrint(FromHandToString(hand) + "\n");
    formattedPrint(BOLD, WHITE, std::string("Value: ") + std::to_string(hand.GetValue(false)) + "\n");
    formattedPrint(BOLD, WHITE, VISUAL_SPACER);
}

void printSecretHand(DrunkEngine::Hand &hand)
{
    formattedPrint(FromSecretHandToString(hand) + "\n");
    formattedPrint(BOLD, WHITE, std::string("Value: ") + "?" + "\n");
    formattedPrint(BOLD, WHITE, VISUAL_SPACER);
}

std::string FromCardToString(const DrunkEngine::Card card)
{
    if (card == DrunkEngine::CardTrait::Rank::ACE)
    return std::string(SuitColor[card.GetSuit()].data()) + std::string(SuitName[card.GetSuit()]) + " " + std::string(RankColor[card.GetRank()].data()) + std::string(RankName[card.GetRank()].data()) + RESET;
    else
    return std::string(SuitColor[card.GetSuit()].data()) + std::string(SuitName[card.GetSuit()]) + " " + std::string(RankName[card.GetRank()].data()) + RESET;
}

std::string FromHandToString(DrunkEngine::Hand &hand)
{
    std::string handString;
    handString.reserve(256);

    for (auto card : hand.cards)
    {
        if (card == DrunkEngine::CardTrait::Rank::UNDEFINED) break;

        handString += FromCardToString(card) + "\n";
    }
    return handString;
}

std::string FromSecretHandToString(DrunkEngine::Hand &hand)
{
    std::string handString;
    handString.reserve(256);

    uint8_t i = 0;
    for (auto card : hand.cards)
    {
        if (card == DrunkEngine::CardTrait::Rank::UNDEFINED) break;
        if (i == 1)
            handString += "Hidden Card\n";
        else
            handString += FromCardToString(hand.cards[i]) + "\n";
        i++;
    }
    return handString;
}