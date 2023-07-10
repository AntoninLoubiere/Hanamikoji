// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"
#include "constant.hh"

#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, error>(std::move(game_state), player)
{
    api = this;
}

joueur Api::id_joueur()
{
    return game_state_->joueur_from_id(player_->id);
}

joueur Api::id_adversaire()
{
    return ~game_state_->joueur_from_id(player_->id);
}
int Api::manche()
{
    return game_state_->manche();
}
int Api::tour()
{
    return game_state_->tour();
}
action_jouee Api::tour_precedent()
{
    return game_state_->derniere_action();
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
bool Api::est_jouee_action(joueur j, action a)
{
    if (a == PREMIER_JOUEUR)
        return false;

    return game_state_->est_jouee_action(j, a);
}
int Api::nb_cartes(joueur j)
{
    return game_state_->nb_cartes(j);
}
std::vector<int> Api::cartes_en_main()
{
    return game_state_->cartes_en_main(id_joueur());
}
void Api::afficher_action(action v)
{
    std::cout << "Action: ";
    print_action(std::cout, v) << "\n";
}

std::ostream& print_action(std::ostream& out, action v)
{
    switch (v)
    {
    case PREMIER_JOUEUR:
        out << "PREMIER_JOUEUR";
        break;
    case VALIDER:
        out << "VALIDER";
        break;
    case DEFAUSSER:
        out << "DEFAUSSER";
        break;
    case CHOIX_TROIS:
        out << "CHOIX_TROIS";
        break;
    case CHOIX_PAQUETS:
        out << "CHOIX_PAQUETS";
        break;
    }
    return out;
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
    case PAQUET_INVALIDE:
        std::cout << "ERREUR: Paquet invalide\n";
        break;
    case CHOIX_INVALIDE:
        std::cout << "ERREUR: Choix invalide\n";
        break;
    case ACTION_INVALIDE:
        std::cout << "ERREUR: Action invalide\n";
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
    std::cout << "Action jouée: \n";
    afficher_action(v.act);

    if (v.act == CHOIX_TROIS)
    {
        std::cout << v.c1 << ", " << v.c2 << ", " << v.c3 << "\n";
    }
    else if (v.act == CHOIX_PAQUETS)
    {
        std::cout << "(" << v.c1 << ", " << v.c2 << "), (" << v.c3 << ", "
                  << v.c4 << ")\n";
    }
}
