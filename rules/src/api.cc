// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"
#include "constant.hh"

#include <iostream>
#include <iterator>
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
    if (g < 0 || g >= NB_GEISHA)
        return -1;
    return game_state_->nb_cartes_validee(j, g);
}

joueur Api::possession_geisha(int g)
{
    if (g < 0 || g >= NB_GEISHA)
        return EGALITE;
    return game_state_->possession_geisha(g);
}
int Api::est_jouee_action(joueur j, action a)
{
    // TODO
    abort();
}
int Api::nb_cartes(joueur j)
{
    return game_state_->nb_cartes(m_joueur);
}
std::vector<int> Api::cartes_en_main()
{
    return game_state_->cartes_en_main(m_joueur);
}
void Api::afficher_action(action v)
{
    std::cout << "Action: ";
    switch (v)
    {
    case PREMIER_JOUEUR:
        std::cout << "Premier joueur\n";
        break;
    case VALIDER:
        std::cout << "Valider\n";
        break;
    case DEFAUSSER:
        std::cout << "Défausser\n";
        break;
    case CHOIX_TROIS:
        std::cout << "Choix trois\n";
        break;
    case CHOIX_PAQUETS:
        std::cout << "Choix paquets";
        break;
    }
}
void Api::afficher_error(error v)
{
    switch (v)
    {
    case OK:
        std::cout << "OK\n";
        break;
    case ACTION_DEJA_JOUEE:
        std::cout << "ERREUR: Action déjà jouée\n";
        break;
    case CARTES_INVALIDES:
        std::cout << "ERREUR: Cartes invalides\n";
        break;
    case GEISHA_INVALIDES:
        std::cout << "ERREUR: Geisha invalide\n";
        break;
    case JOUEUR_INVALIDE:
        std::cout << "ERREUR: Joueur invalide\n";
        break;
    }
}
void Api::afficher_joueur(joueur v)
{
    switch (v)
    {
    case JOUEUR_1:
        std::cout << "Joueur1\n";
        break;
    case JOUEUR_2:
        std::cout << "Joueur2\n";
        break;
    case EGALITE:
        std::cout << "Égalité\n";
        break;
    }
}
void Api::afficher_action_jouee(action_jouee v)
{
    std::cout << "Action jouée: ";
    afficher_action(v.act);

    if (!v.cartes.empty())
    {
        std::ostream_iterator<int> cout_it(std::cout, ", ");
        std::copy(v.cartes.begin(), v.cartes.end(), cout_it);
        std::cout << "\n";
    }
}
