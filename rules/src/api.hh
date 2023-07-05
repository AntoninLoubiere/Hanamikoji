// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <rules/api.hh>
#include <rules/player.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

/**
 * The methods of this class are exported through 'interface.cc'
 * to be called by the clients
 */
class Api final : public rules::Api<GameState, error>
{
public:
    Api(std::unique_ptr<GameState> game_state,
        std::shared_ptr<rules::Player> player);
    ~Api() {}

    /// Renvoie l'identifiant du joueur
    joueur id_joueur();

    /// Renvoie l'identifiant de l'adversaire
    joueur id_adversaire();

    /// Renvoie le numéro de la manche
    int manche();

    /// Renvoie le numéro de la manche
    int tour();

    /// Renvoie l'action jouée par l'adversaire
    action_jouee tour_precedant();

    /// Renvoie le nombre de carte validée par le joueur pour la geisha
    int nb_carte_validee(joueur j, int g);

    /// Renvoie qui possède la geisha
    joueur possession_geisha(int g);

    /// Renvoie si l'action a déjà été jouée par le joueur
    bool est_jouee_action(joueur j, action a);

    /// Renvoie le nombre de carte que chaque joueur a
    int nb_cartes(joueur j);

    /// Renvoie les cartes que vous avez
    std::vector<int> cartes_en_main();

    /// Jouer l'action valider une carte
    ApiActionFunc<ActionActionValider> action_valider{this};

    /// Jouer l'action défausser deux cartes
    ApiActionFunc<ActionActionDefausser> action_defausser{this};

    /// Jouer l'action choisir entre trois cartes
    ApiActionFunc<ActionActionChoixTrois> action_choix_trois{this};

    /// Jouer l'action choisir entre deux paquets de deux cartes
    ApiActionFunc<ActionActionChoixPaquets> action_choix_paquets{this};

    /// Choisir une des trois cartes proposées.
    ApiActionFunc<ActionRepondreChoixTrois> repondre_choix_trois{this};

    /// Choisir un des deux paquets proposés.
    ApiActionFunc<ActionRepondreChoixPaquets> repondre_choix_paquets{this};

    /// Affiche le contenu d'une valeur de type action
    void afficher_action(action v);

    /// Affiche le contenu d'une valeur de type error
    void afficher_error(error v);

    /// Affiche le contenu d'une valeur de type joueur
    void afficher_joueur(joueur v);

    /// Affiche le contenu d'une valeur de type action_jouee
    void afficher_action_jouee(action_jouee v);
};
