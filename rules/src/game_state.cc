// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"
#include "cardset.hh"
#include "constant.hh"
#include "rules/player.hh"
#include "utils/log.hh"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <vector>

GameState::GameState(std::istream& map_stream, const rules::Players& players)
    : rules::GameState(players)
    , m_action_deja_jouee(false)
    , m_attente_reponse(false)
    , m_demarre(false)
    , m_tour(0)
    , m_manche(0)

{
    std::fill_n(m_geisha_owner, NB_GEISHA, EGALITE);

    std::istream_iterator<int> map_it(map_stream);
    std::copy_n(map_it, SIZE_PIOCHE, m_pioches);
    std::fill_n(m_joueurs_main, NB_JOUEURS, EMPTY_CARDSET);
    std::fill_n(m_joueurs_validee, NB_JOUEURS, EMPTY_CARDSET);
    for (int i = 0; i < NB_JOUEURS; i++)
        std::fill_n(m_actions_jouee[i], NB_ACTIONS, false);

    for (int manche = 0; manche < NB_MANCHES_MAX; manche++)
    {
        int cards[NB_CARTES_TOTAL];
        std::fill_n(cards, NB_CARTES_TOTAL, 0);
        for (int carte = NB_CARTES_TOTAL * manche;
             carte < NB_CARTES_TOTAL * (manche + 1); carte++)
        {
            if (m_pioches[carte] < 0 || m_pioches[carte] >= NB_GEISHA)
            {
                FATAL("Carte invalide n°%d (%d)", carte, m_pioches[carte]);
            }
            cards[m_pioches[carte]] += 1;
        }

        for (int g = 0; g < NB_GEISHA; g++)
        {
            if (cards[g] != GEISHA_VALEUR_INT[g])
            {
                FATAL("Compte de cartes invalide manche %d geisha %d (%d)",
                      manche, g, cards[g]);
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
    for (int i = 0; i < NB_JOUEURS; i++)
        std::copy_n(st.m_actions_jouee[i], NB_ACTIONS, m_actions_jouee[i]);

    m_manche = st.m_manche;
    m_tour = st.m_tour;
    m_seed = st.m_seed;
    m_action_deja_jouee = st.m_action_deja_jouee;
    m_attente_reponse = st.m_attente_reponse;
    m_demarre = st.m_demarre;
    std::copy_n(st.m_pioches, SIZE_PIOCHE, m_pioches);
}

GameState::~GameState() {}

void GameState::debut_tour()
{
    if (!m_attente_reponse)
    {
        if (m_tour == 0)
            debut_manche();

        int carte = m_pioches[NB_CARTES_TOTAL * m_manche +
                              NB_JOUEURS * NB_CARTES_DEBUT + m_tour];
        m_joueurs_main[joueur_courant()] += carte;
    }
    m_action_deja_jouee = false;
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
    // Reset des actions déjà faites
    for (int i = 0; i < NB_JOUEURS; i++)
        std::fill_n(m_actions_jouee[i], NB_ACTIONS, false);
}

void GameState::fin_tour()
{
    if (!m_action_deja_jouee)
    {
        joueur j = joueur_courant();
        if (m_attente_reponse)
        {
            WARN("Le joueur %d, n'a pas répondu à l'action de l'adversaire son "
                 "action (%d: %d)!",
                 ~j, m_manche, m_tour);

            if (m_derniere_action.act == CHOIX_TROIS)
                appliquer_repondre_trois(~j, 0);
            else if (m_derniere_action.act == CHOIX_PAQUETS)
                appliquer_repondre_paquet(~j, 0);
            else
                FATAL("On attends une réponse d'une action inconnue ??");
            m_attente_reponse = false;
        }
        else
        {
            WARN("Le joueur %d, n'a pas joué son action (%d: %d)!", j, m_manche,
                 m_tour);
            std::vector<int> cartes = cartes_en_main(j);

            if (!est_jouee_action(j, VALIDER))
                appliquer_act_valider(j, cartes[0]);
            else if (!est_jouee_action(j, DEFAUSSER))
                appliquer_act_defausser(j, cartes[0], cartes[1]);
            else if (!est_jouee_action(j, CHOIX_TROIS))
                appliquer_act_choix_trois(j, cartes[0], cartes[1], cartes[2]);
            else if (!est_jouee_action(j, VALIDER))
                appliquer_act_choix_paquets(j, cartes[0], cartes[1], cartes[2],
                                            cartes[3]);
            else
                FATAL("Il n'y avait plus d'action restante ???");
        }
    }

    if (!m_attente_reponse)
    {
        m_tour++;
        if (m_tour >= NB_ACTIONS * NB_JOUEURS)
            fin_manche();
    }
};

void GameState::fin_manche()
{
    m_tour = 0;
    m_manche++;

    for (int i = 0; i < NB_JOUEURS; i++)
    {
        players_[i]->score = 0;
        if (m_joueurs_main[i] != EMPTY_CARDSET)
            ERR("Il restait des cartes au joueur %d", i);
    }

    for (int g = 0; g < NB_GEISHA; g++)
    {
        joueur majoritaire = majorite_carte(m_joueurs_validee, g);
        if (majoritaire != EGALITE)
            m_geisha_owner[g] = majoritaire;

        if (m_geisha_owner[g] != EGALITE)
            players_[m_geisha_owner[g]]->score += GEISHA_VALEUR_INT[g];
    }
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
joueur GameState::joueur_from_id(uint32_t id) const
{
    return id == players_[0]->id ? JOUEUR_1 : JOUEUR_2;
}

joueur GameState::possession_geisha(int g) const
{
    return m_geisha_owner[g];
}

bool GameState::est_jouee_action(joueur j, action a) const
{
    return m_actions_jouee[j][a];
}

bool GameState::est_action_deja_jouee() const
{
    return m_action_deja_jouee;
}

bool GameState::a_cartes(joueur j, cardset set) const
{
    return contains_cardset(set, m_joueurs_main[j]);
}

void GameState::appliquer_act_valider(joueur j, int c)
{
    m_joueurs_main[j] -= c;
    m_joueurs_validee[j] += c;
    m_action_deja_jouee = true;
    m_actions_jouee[j][VALIDER] = true;

    m_derniere_action.act = VALIDER;
    m_derniere_action.c1 = -1;
    m_derniere_action.c2 = -1;
    m_derniere_action.c3 = -1;
    m_derniere_action.c4 = -1;
}

void GameState::appliquer_act_defausser(joueur j, int c1, int c2)
{
    m_joueurs_main[j] -= c1;
    m_joueurs_main[j] -= c2;
    m_action_deja_jouee = true;
    m_actions_jouee[j][DEFAUSSER] = true;

    m_derniere_action.act = DEFAUSSER;
    m_derniere_action.c1 = -1;
    m_derniere_action.c2 = -1;
    m_derniere_action.c3 = -1;
    m_derniere_action.c4 = -1;
}

void GameState::appliquer_act_choix_trois(joueur j, int c1, int c2, int c3)
{
    m_joueurs_main[j] -= c1;
    m_joueurs_main[j] -= c2;
    m_joueurs_main[j] -= c3;
    m_action_deja_jouee = true;
    m_attente_reponse = true;
    m_actions_jouee[j][CHOIX_TROIS] = true;

    m_derniere_action.act = CHOIX_TROIS;
    m_derniere_action.c1 = c1;
    m_derniere_action.c2 = c2;
    m_derniere_action.c3 = c3;
    m_derniere_action.c4 = -1;
}

void GameState::appliquer_act_choix_paquets(joueur j, int p1c1, int p1c2,
                                            int p2c1, int p2c2)
{
    m_joueurs_main[j] -= p1c1;
    m_joueurs_main[j] -= p1c2;
    m_joueurs_main[j] -= p2c1;
    m_joueurs_main[j] -= p2c2;
    m_action_deja_jouee = true;
    m_attente_reponse = true;

    m_actions_jouee[j][CHOIX_PAQUETS] = true;

    m_derniere_action.act = CHOIX_PAQUETS;
    m_derniere_action.c1 = p1c1;
    m_derniere_action.c2 = p1c2;
    m_derniere_action.c3 = p2c1;
    m_derniere_action.c4 = p2c2;
}

void GameState::appliquer_repondre_trois(joueur j, int c)
{
    m_action_deja_jouee = true;
    m_attente_reponse = false;

    switch (c)
    {
    case 0:
        m_joueurs_validee[j] += m_derniere_action.c1;
        m_joueurs_validee[~j] += m_derniere_action.c2;
        m_joueurs_validee[~j] += m_derniere_action.c3;
        break;

    case 1:
        m_joueurs_validee[~j] += m_derniere_action.c1;
        m_joueurs_validee[j] += m_derniere_action.c2;
        m_joueurs_validee[~j] += m_derniere_action.c3;
        break;

    case 2:
        m_joueurs_validee[~j] += m_derniere_action.c1;
        m_joueurs_validee[~j] += m_derniere_action.c2;
        m_joueurs_validee[j] += m_derniere_action.c3;
        break;

    default:
        FATAL("IMPOSSIBLE ??, c doit être compris entre 0 et 2 !");
    }
}

void GameState::appliquer_repondre_paquet(joueur j, int p)
{
    assert(0 <= p && p <= 1);
    m_action_deja_jouee = true;
    m_attente_reponse = false;

    joueur paquet0 = p == 0 ? j : ~j;

    m_joueurs_validee[paquet0] += m_derniere_action.c1;
    m_joueurs_validee[paquet0] += m_derniere_action.c2;
    m_joueurs_validee[~paquet0] += m_derniere_action.c3;
    m_joueurs_validee[~paquet0] += m_derniere_action.c4;
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

action_jouee GameState::derniere_action() const
{
    return m_derniere_action;
}

bool GameState::attente_reponse() const
{
    return m_attente_reponse;
}

bool GameState::demarre() const
{
    return m_demarre;
}