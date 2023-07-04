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
    int premier_joueur_id() const;
    int tour() const;
    int manche() const;
    joueur joueur_courant() const;

    std::vector<int> cartes(joueur j) const;

    void debut_tour();
    void debut_manche();

    void fin_tour();
    void fin_manche();

    bool fini() const;
    joueur gagnant() const;

private:
    joueur m_geisha_owner[NB_GEISHA];
    cardset m_joueur1_main;
    cardset m_joueur1_validee;
    cardset m_joueur2_main;
    cardset m_joueur2_validee;

    // Les 3 manches se suivent
    // Pour chaque manche, la dernière est écarté, les 6 premières sont pour le
    // joueur 1, les 6 autre pour le 2, puis dans l'ordre des tours.
    int m_pioches[SIZE_PIOCHE];

    int m_seed;
    int m_tour;
    int m_manche;
};
