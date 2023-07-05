// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include "cardset.hh"
#include "constant.hh"
#include <rules/game-state.hh>
#include <rules/player.hh>
#include <vector>

const int SIZE_PIOCHE = NB_CARTES_TOTAL * NB_MANCHES_MAX;

class GameState final : public rules::GameState
{
public:
    GameState(std::istream& map_stream, const rules::Players& players);
    GameState(const GameState& st);
    ~GameState();

    GameState* copy() const override;

    int get_score_joueur(joueur j) const;
    joueur joueur_from_id(uint32_t id) const;
    int tour() const;
    int manche() const;
    joueur joueur_courant() const;

    std::vector<int> cartes_en_main(joueur j) const;
    int nb_cartes(joueur j) const;
    int nb_cartes_validee(joueur j, int g) const;
    joueur possession_geisha(int g) const;
    bool est_jouee_action(joueur j, action a) const;
    bool est_action_deja_jouee() const;
    bool a_cartes(joueur j, cardset set) const;
    action_jouee derniere_action() const;

    void appliquer_act_valider(joueur j, int c);
    void appliquer_act_defausser(joueur j, int c1, int c2);
    void appliquer_act_choix_trois(joueur j, int c1, int c2, int c3);
    void appliquer_act_choix_paquets(joueur j, int p1c1, int p1c2, int p2c1,
                                     int p2c2);

    void appliquer_repondre_trois(joueur j, int c);
    void appliquer_repondre_paquet(joueur j, int p);

    void debut_tour();
    void debut_manche();

    void fin_tour();
    void fin_manche();

    bool fini() const;
    bool demarre() const;
    bool attente_reponse() const;
    joueur gagnant() const;

private:
    joueur m_geisha_owner[NB_GEISHA];
    cardset m_joueurs_main[NB_JOUEURS];
    cardset m_joueurs_validee[NB_JOUEURS];

    bool m_action_deja_jouee;
    bool m_actions_jouee[NB_JOUEURS][NB_ACTIONS];
    bool m_attente_reponse;
    bool m_demarre;

    // Les 3 manches se suivent
    // Pour chaque manche, la dernière est écarté, les 6 premières sont pour
    // le joueur 1, les 6 autre pour le 2, puis dans l'ordre des tours.
    int m_pioches[SIZE_PIOCHE];
    action_jouee m_derniere_action;

    int m_seed;
    int m_tour;
    int m_manche;
};
