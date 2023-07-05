// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionRepondreChoixTrois : public rules::Action<GameState>
{
public:
    ActionRepondreChoixTrois(int c, int player_id)
        : c_(c)
        , player_id_(player_id)
    {
    }
    ActionRepondreChoixTrois() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(c_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override
    {
        return player_id_;
    };
    uint32_t id() const override
    {
        return ID_ACTION_REPONDRE_CHOIX_TROIS;
    }

private:
    int c_;
    int player_id_;
};
