// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"
#include "cardset.hh"
#include "constant.hh"

GameState::GameState(const rules::Players& players)
    : rules::GameState(players)
    , m_joueur1_main(EMPTY_CARDSET)
    , m_joueur1_validee(EMPTY_CARDSET)
    , m_joueur2_main(EMPTY_CARDSET)
    , m_joueur2_validee(EMPTY_CARDSET)
    , m_manche(0)
    , m_tour(0)

{
    for (int i = 0; i < NB_GEISHA; i++)
    {
        m_geisha_owner[i] = EGALITE;
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
{
    for (int i = 0; i < NB_GEISHA; i++)
    {
        m_geisha_owner[i] = st.m_geisha_owner[i];
    }
    m_joueur1_main = st.m_joueur1_main;
    m_joueur1_validee = st.m_joueur1_validee;
    m_joueur2_main = st.m_joueur2_main;
    m_joueur2_validee = st.m_joueur2_validee;
    m_manche = st.m_manche;
    m_tour = st.m_tour;
}

GameState::~GameState()
{
    // FIXME
}

int GameState::get_score_joueur(joueur j) const
{
    int score = 0;
    for (int i = 0; i < NB_GEISHA; i++)
    {
        if (m_geisha_owner[i] == j)
        {
            score += GEISHA_VALEUR_INT[i];
        }
    }
}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

int GameState::manche() const
{
    return m_manche;
}

int GameState::tour() const
{
    return m_tour;
}

bool GameState::finished() const
{
    return m_manche >= 2 || winner() != EGALITE;
}

joueur GameState::winner() const
{
    int score1 = 0;
    int nb_c1 = 0;
    int score2 = 0;
    int nb_c2 = 0;
    for (int i = 0; i < NB_GEISHA; i++)
    {
        if (m_geisha_owner[i] == JOUEUR_1)
        {
            score1 += GEISHA_VALEUR_INT[i];
            nb_c1 += 1;
        }
        else if (m_geisha_owner[i] == JOUEUR_2)
        {
            score2 += GEISHA_VALEUR_INT[i];
            nb_c2 += 1;
        }
    }

    if (score1 >= 11)
        return JOUEUR_1;

    if (score2 >= 11)
        return JOUEUR_2;

    if (nb_c1 >= 4)
        return JOUEUR_1;

    if (nb_c2 >= 4)
        return JOUEUR_2;

    if (m_manche >= 2)
    {
        if (score1 > score2)
            return JOUEUR_1;
        if (score2 > score1)
            return JOUEUR_2;
    }

    return EGALITE;
}