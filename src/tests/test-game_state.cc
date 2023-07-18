#include "test-helpers.hh"
#include <gtest/gtest.h>

TEST_F(GameStateTest, TestMancheTour)
{
    for (int m = 0; m < NB_MANCHES_MAX; m++)
    {
        joueur current_j = m == 1 ? JOUEUR_2 : JOUEUR_1;
        for (int t = 0; t < NB_ACTIONS * NB_JOUEURS; t++)
        {
            gs->debut_tour();
            ASSERT_EQ(current_j, gs->joueur_courant());
            ASSERT_EQ(m, gs->manche());
            ASSERT_EQ(t, gs->tour());
            gs->fin_tour();
            current_j = current_j == JOUEUR_1 ? JOUEUR_2 : JOUEUR_1;
        }
    }
}