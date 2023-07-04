#include "../api.hh"
#include "../game_state.hh"
#include "../rules.hh"
#include "test-helpers.hh"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>

TEST_F(ApiTest, TestIdPlayer)
{
    ASSERT_EQ(JOUEUR_1, players[0].api->id_joueur());
    ASSERT_EQ(JOUEUR_2, players[1].api->id_joueur());
    ASSERT_EQ(JOUEUR_2, players[0].api->id_adversaire());
    ASSERT_EQ(JOUEUR_1, players[1].api->id_adversaire());
}

TEST_F(ApiTest, TestDistributionCartes)
{
    players[0].api->game_state().debut_manche();
    std::vector<int> cartes = players[0].api->cartes_en_main();
    ASSERT_EQ(6, (int)cartes.size());
    ASSERT_EQ(6, players[0].api->nb_cartes(JOUEUR_1));
    ASSERT_EQ(6, players[0].api->nb_cartes(JOUEUR_2));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 0));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 1));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 4));
    ASSERT_EQ(2, std::count(cartes.begin(), cartes.end(), 5));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 6));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 6));

    players[1].api->game_state().debut_manche();
    cartes = players[1].api->cartes_en_main();
    ASSERT_EQ(6, (int)cartes.size());
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 1));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 2));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 3));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 4));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 5));
    ASSERT_EQ(1, std::count(cartes.begin(), cartes.end(), 6));
}