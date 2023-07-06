// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <memory>

#include <rules/actions.hh>
#include <rules/client-messenger.hh>
#include <rules/options.hh>
#include <rules/player.hh>
#include <rules/rules.hh>
#include <rules/server-messenger.hh>
#include <utils/dll.hh>
#include <utils/sandbox.hh>

#include "api.hh"

using f_champion_init_jeu = void (*)();
using f_champion_jouer_tour = void (*)();
using f_champion_repondre_action_choix_trois = void (*)();
using f_champion_repondre_action_choix_paquets = void (*)();
using f_champion_fin_jeu = void (*)();

class Rules : public rules::Rules
{
public:
    explicit Rules(const rules::Options opt);
    virtual ~Rules() {}

    rules::Actions* get_actions() override;
    void apply_action(const rules::IAction& action) override;
    bool is_finished() override;
    void player_loop(rules::ClientMessenger_sptr msgr) override;
    void replay_loop(rules::ReplayMessenger_sptr msgr) override;
    void spectator_loop(rules::ClientMessenger_sptr msgr) override;
    void server_loop(rules::ServerMessenger_sptr msgr) override;
    void dump_state(std::ostream&) override;

private:
    void register_actions();

    std::unique_ptr<utils::DLL> champion_dll_;
    std::unique_ptr<Api> api_;
    utils::Sandbox sandbox_;

    f_champion_init_jeu champion_init_jeu_;
    f_champion_jouer_tour champion_jouer_tour_;
    f_champion_repondre_action_choix_trois
        champion_repondre_action_choix_trois_;
    f_champion_repondre_action_choix_paquets
        champion_repondre_action_choix_paquets_;
    f_champion_fin_jeu champion_fin_jeu_;
};
