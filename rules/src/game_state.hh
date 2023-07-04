// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include "cardset.hh"
#include "constant.hh"
#include <rules/game-state.hh>
#include <rules/player.hh>

class GameState final : public rules::GameState
{
public:
    // FIXME
    // additional parameters? for instance map
    GameState(const rules::Players& players);
    GameState(const GameState& st);
    ~GameState();

    GameState* copy() const override;

    int get_score_joueur(joueur j) const;
    int tour() const;
    int manche() const;

    bool finished() const;
    joueur winner() const;

private:
    joueur m_geisha_owner[NB_GEISHA];
    cardset m_joueur1_main;
    cardset m_joueur1_validee;
    cardset m_joueur2_main;
    cardset m_joueur2_validee;

    int m_tour;
    int m_manche;
};
