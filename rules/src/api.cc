// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"
#include "constant.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, error>(std::move(game_state), player)
{
    api = this;
    m_joueur =
        player_->id == game_state_->premier_joueur_id() ? JOUEUR_1 : JOUEUR_2;
}

joueur Api::id_joueur()
{
    return m_joueur;
}

joueur Api::id_adversaire()
{
    return ~m_joueur;
}
int Api::manche()
{
    return game_state_->manche();
}
int Api::tour()
{
    return game_state_->tour();
}
action_jouee Api::tour_precedant()
{
    // TODO
    abort();
}
int Api::nb_carte_validee(joueur j, int g)
{
    // TODO
    abort();
}
joueur Api::possession_geisha(int g)
{
    // TODO
    abort();
}
int Api::est_jouee_action(joueur j, action a)
{
    // TODO
    abort();
}
int Api::nb_cartes(joueur j)
{
    // TODO
    abort();
}
std::vector<int> Api::cartes_en_main()
{
    return game_state_->cartes(m_joueur);
}
void Api::afficher_action(action v)
{
    // TODO
    abort();
}
void Api::afficher_error(error v)
{
    // TODO
    abort();
}
void Api::afficher_joueur(joueur v)
{
    // TODO
    abort();
}
void Api::afficher_action_jouee(action_jouee v)
{
    // TODO
    abort();
}
