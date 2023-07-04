// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"
#include "cardset.hh"
#include "constant.hh"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>

GameState::GameState(std::istream& map_stream, const rules::Players& players)
    : rules::GameState(players)
    , m_tour(0)
    , m_manche(0)

{
    for (int i = 0; i < NB_GEISHA; i++)
        m_geisha_owner[i] = EGALITE;

    std::istream_iterator<int> map_it(map_stream);
    std::copy_n(map_it, SIZE_PIOCHE, m_pioches);
    std::fill_n(m_joueurs_main, NB_JOUEURS, EMPTY_CARDSET);
    std::fill_n(m_joueurs_validee, NB_JOUEURS, EMPTY_CARDSET);

    for (int manche = 0; manche < NB_MANCHES_MAX; manche++)
    {
        int cards[NB_CARTES_TOTAL];
        std::fill_n(cards, NB_CARTES_TOTAL, 0);
        for (int carte = NB_CARTES_TOTAL * manche;
             carte < NB_CARTES_TOTAL * (manche + 1); carte++)
        {
            if (m_pioches[carte] < 0 || m_pioches[carte] >= NB_GEISHA)
            {
                std::cerr << "Carte invalide n°" << carte << " - "
                          << m_pioches[carte] << std::endl;
                throw "Carte invalide";
            }
            cards[m_pioches[carte]] += 1;
        }

        for (int g = 0; g < NB_GEISHA; g++)
        {
            if (cards[g] != GEISHA_VALEUR_INT[g])
            {
                std::cerr << "Compte de cartes invalide manche " << manche
                          << " geisha " << g << " - " << cards[g] << std::endl;
                throw "Cartes invalides";
            }
        }
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
{
    std::copy(st.m_geisha_owner, st.m_geisha_owner + NB_GEISHA, m_geisha_owner);
    std::copy_n(st.m_joueurs_main, 2, m_joueurs_main);
    std::copy_n(st.m_joueurs_validee, 2, m_joueurs_validee);
    m_manche = st.m_manche;
    m_tour = st.m_tour;
    m_seed = st.m_seed;
    std::copy_n(st.m_pioches, SIZE_PIOCHE, m_pioches);
}

GameState::~GameState() {}

void GameState::debut_tour()
{
    if (m_tour == 0)
        debut_manche();

    int carte = m_pioches[NB_CARTES_TOTAL * m_manche +
                          NB_JOUEURS * NB_CARTES_DEBUT + m_tour];
    m_joueurs_main[joueur_courant()] += carte;
}

void GameState::debut_manche()
{
    // On distribue les cartes
    for (int j = 0; j < NB_JOUEURS; j++)
    {
        m_joueurs_main[j] = EMPTY_CARDSET;
        for (int c = 0; c < NB_CARTES_DEBUT; c++)
        {
            m_joueurs_main[j] +=
                m_pioches[NB_CARTES_TOTAL * m_manche + j * NB_CARTES_DEBUT + c];
        }
        m_joueurs_validee[j] = EMPTY_CARDSET;
    }
}

void GameState::fin_tour()
{
    m_tour++;
    if (m_tour >= NB_ACTIONS * NB_JOUEURS)
        fin_manche();
};

void GameState::fin_manche()
{
    m_tour = 0;
    m_manche++;
};

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
    return score;
}

joueur GameState::joueur_courant() const
{
    return (m_tour + m_manche) % 2 == 0 ? JOUEUR_1 : JOUEUR_2;
}

bool GameState::fini() const
{
    return m_manche >= NB_MANCHES_MAX || gagnant() != EGALITE;
}

joueur GameState::gagnant() const
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

    if (m_manche >= NB_MANCHES_MAX)
    {
        if (score1 > score2)
            return JOUEUR_1;
        if (score2 > score1)
            return JOUEUR_2;
    }

    return EGALITE;
}

std::vector<int> GameState::cartes_en_main(joueur j) const
{
    return cardset_to_vector(m_joueurs_main[j]);
}

int GameState::nb_cartes(joueur j) const
{
    return cardset_count(m_joueurs_main[j]);
}

int GameState::nb_cartes_validee(joueur j, int g) const
{
    // Un peu sous optimal car 21 comparaisons à la place des 5 au maximum…
    // ÇA PASSE
    return cardset_count(m_joueurs_validee[j] & GEISHA_MASK[g]);
}
int GameState::premier_joueur_id() const
{
    return players_[0]->id;
}

joueur GameState::possession_geisha(int g) const
{
    return m_geisha_owner[g];
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
