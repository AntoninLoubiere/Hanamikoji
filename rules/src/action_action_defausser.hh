// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionActionDefausser : public rules::Action<GameState>
{
public:
    ActionActionDefausser(int c1, int c2, int player_id)
        : c1_(c1)
        , c2_(c2)
        , player_id_(player_id)
    {
    }
    ActionActionDefausser() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(c1_);
        buf.handle(c2_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_ACTION_DEFAUSSER; }

private:
    int c1_;
    int c2_;
    int player_id_;
};

