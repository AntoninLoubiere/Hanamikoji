// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionRepondreChoixPaquets : public rules::Action<GameState>
{
public:
    ActionRepondreChoixPaquets(int p, int player_id)
        : p_(p)
        , player_id_(player_id)
    {
    }
    ActionRepondreChoixPaquets() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(p_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_REPONDRE_CHOIX_PAQUETS; }

private:
    int p_;
    int player_id_;
};

