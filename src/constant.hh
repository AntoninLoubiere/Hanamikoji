// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

/// Les 7 Geisha (2, 2, 2, 3, 3, 4, 5)
#define NB_GEISHA 7

/// Le nombre total de cartes (2 + 2 + 2 + 3 + 3 + 4 + 5)
#define NB_CARTES_TOTALES 21

/// Le nombre de cartes que chaque personne a au début
#define NB_CARTES_DEBUT 6

/// Le nombre de cartes écartées au début du jeu
#define NB_CARTES_ECARTEES 1

/// Le nombre total d'actions que chaque joueur devra faire
#define NB_ACTIONS 4

/// Le nombre de joueurs qui joue
#define NB_JOUEURS 2

/// Le nombre total de manches avant la fin de la partie
#define NB_MANCHES_MAX 3

/// La valeur (et le nombre de cartes) de chaque Geisha séparée par des |
#define GEISHA_VALEUR "2|2|2|3|3|4|5"
const int GEISHA_VALEUR_INT[] = {2, 2, 2, 3, 3, 4, 5};

/// Les actions de jeu
typedef enum action
{
    VALIDER,        ///< Valide une unique carte
    DEFAUSSER,      ///< Défausse deux cartes
    CHOIX_TROIS,    ///< Donne le choix entre trois cartes
    CHOIX_PAQUETS,  ///< Donne le choix entre deux paquets de deux cartes
    PREMIER_JOUEUR, ///< Aucune action n'a été jouée (utilisé dans
                    ///< tour_precedent)
} action;

/// Enumeration contentant toutes les erreurs possibles
typedef enum error
{
    OK,                ///< pas d'erreur
    ACTION_DEJA_JOUEE, ///< l'action a déjà été jouée
    CARTES_INVALIDES,  ///< vous ne pouvez pas jouer ces cartes
    PAQUET_INVALIDE,   ///< ce paquet n'existe pas
    GEISHA_INVALIDES, ///< cette Geisha n'existe pas (doit être un entier entre
                      ///< 0 et NB_GEISHA - 1)
    JOUEUR_INVALIDE,  ///< ce joueur n'existe pas
    CHOIX_INVALIDE,   ///< vous ne pouvez pas répondre à ce choix
    ACTION_INVALIDE,  ///< vous ne pouvez pas jouer cette action maintenant
} error;

/// Enumeration représentant les différents joueurs
typedef enum joueur
{
    JOUEUR_1, ///< Le joueur 1
    JOUEUR_2, ///< Le joueur 2
    EGALITE,  ///< Égalité, utilisé uniquement dans possession_geisha
} joueur;

joueur operator~(joueur j);

int joueur_to_int(joueur j);

/// La description d'une action jouée
typedef struct action_jouee
{
    action act; ///< L'action jouée
    int c1; ///< Si act==VALIDER ou act==DEFAUSSER, -1 sinon la première carte
            ///< (du premier paquet)
    int c2; ///< Si act==V|D: -1 sinon la deuxième carte (du premier paquet)
    int c3; ///< Si act==V|D: -1 sinon la troisième carte (ou la première carte
            ///< du second paquet si act==choix paquet)
    int c4; ///< Si act!=choix paquet: -1 sinon la deuxième carte du second
            ///< paquet
} action_jouee;
