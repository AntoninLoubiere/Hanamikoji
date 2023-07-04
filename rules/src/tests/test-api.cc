#include "../api.hh"
#include "../game_state.hh"
#include "../rules.hh"
#include <gtest/gtest.h>
#include <memory>

class ApiTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        // Create two players (no spectator)
        rules::Players rules_players;
        rules_players.add(std::make_shared<rules::Player>(0, rules::PLAYER));
        rules_players.add(std::make_shared<rules::Player>(1, rules::PLAYER));

        apis[0] = std::make_unique<Api>(
            std::make_unique<GameState>(rules_players), rules_players[0]);
        apis[1] = std::make_unique<Api>(
            std::make_unique<GameState>(rules_players), rules_players[1]);
    }

    std::unique_ptr<GameState> gamestate_;
    std::array<std::unique_ptr<Api>, 2> apis;
};

TEST_F(ApiTest, TestIdPlayer)
{
    ASSERT_EQ(JOUEUR_1, apis[0]->id_joueur());
    ASSERT_EQ(JOUEUR_2, apis[1]->id_joueur());
    ASSERT_EQ(JOUEUR_2, apis[0]->id_adversaire());
    ASSERT_EQ(JOUEUR_1, apis[1]->id_adversaire());
}
