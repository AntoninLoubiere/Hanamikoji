// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionActionChoixPaquets : public rules::Action<GameState>
{
public:
    ActionActionChoixPaquets(int p1c1, int p1c2, int p2c1, int p2c2, int player_id)
        : p1c1_(p1c1)
        , p1c2_(p1c2)
        , p2c1_(p2c1)
        , p2c2_(p2c2)
        , player_id_(player_id)
    {
    }
    ActionActionChoixPaquets() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(p1c1_);
        buf.handle(p1c2_);
        buf.handle(p2c1_);
        buf.handle(p2c2_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_ACTION_CHOIX_PAQUETS; }

private:
    int p1c1_;
    int p1c2_;
    int p2c1_;
    int p2c2_;
    int player_id_;
};

