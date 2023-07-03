// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once


/// Les 7 geisha (2, 2, 2, 3, 3, 4, 5)
#define NB_GEISHA 7

/// Le nombre total de cartes (2 + 2 + 2 + 3 + 3 + 4 + 5)
#define NB_CARTES_TOTAL 21

/// Le nombre de cartes que chaque personne a au début
#define NB_CARTES_DEBUT 6

/// Le nombre de cartes écartées au début du jeu
#define NB_CARTES_ECARTEES 1

/// La valeur (et le nombre de cartes) de chaque geisha séparée par des |
#define GEISHA_VALEUR "2|2|2|3|4|5"


/// Les actions de jeu
typedef enum action
{
    VALIDER, ///< Valide une unique carte
    DEFAUSSER, ///< Defausse deux cartes
    CHOIX_TROIS, ///< Donne le choix entre trois cartes
    CHOIX_PAQUETS, ///< Donne le choix entre deux paquets de deux cartes
    PREMIER_JOUEUR, ///< Aucune action n'a été jouée (utilisé dans tour_precedant)
} action;

/// Enumeration contentant toutes les erreurs possibles
typedef enum error
{
    OK, ///< pas d'erreur
    ACTION_DEJA_JOUEE, ///< l'action a déjà été jouée
    CARTES_INVALIDES, ///< vous ne pouvez pas jouer ces cartes
    GEISHA_INVALIDES, ///< cette geisha n'existe pas (doit être un entier entre 0 et NB_GEISHA)
    JOUEUR_INVALIDE, ///< ce joueur n'existe pas
} error;

/// Enumeration représentant les différents joueurs
typedef enum joueur
{
    JOUEUR_1, ///< Le joueur 1
    JOUEUR_2, ///< Le joueur 2
    EGALITE, ///< Égalité, utilisé uniquement dans possession_geisha
} joueur;


/// La description d'une action jouée
typedef struct action_jouee
{
    action act; ///< L'action jouée
    std::vector<int> cartes; ///< Les cartes jouées
} action_jouee;
