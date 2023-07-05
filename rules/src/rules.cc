// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "rules.hh"

#include <memory>
#include <sstream>
#include <utility>

#include "actions.hh"
#include "rules/client-messenger.hh"
#include "utils/sandbox.hh"

Rules::Rules(const rules::Options opt)
    : TurnBasedRules(opt)
    , sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = std::make_unique<utils::DLL>(opt.champion_lib);

        champion_init_jeu_ =
            champion_dll_->get<f_champion_init_jeu>("init_jeu");
        champion_jouer_tour_ =
            champion_dll_->get<f_champion_jouer_tour>("jouer_tour");
        champion_repondre_action_choix_trois_ =
            champion_dll_->get<f_champion_repondre_action_choix_trois>(
                "repondre_action_choix_trois");
        champion_repondre_action_choix_paquets_ =
            champion_dll_->get<f_champion_repondre_action_choix_paquets>(
                "repondre_action_choix_paquets");
        champion_fin_jeu_ = champion_dll_->get<f_champion_fin_jeu>("fin_jeu");
    }

    std::istringstream map_stream(opt.map_content);
    auto game_state = std::make_unique<GameState>(map_stream, opt.players);
    api_ = std::make_unique<Api>(std::move(game_state), opt.player);
    register_actions();
}

void Rules::register_actions()
{
    api_->actions()->register_action(
        ID_ACTION_ACTION_VALIDER,
        [] { return std::make_unique<ActionActionValider>(); });
    api_->actions()->register_action(
        ID_ACTION_ACTION_DEFAUSSER,
        [] { return std::make_unique<ActionActionDefausser>(); });
    api_->actions()->register_action(
        ID_ACTION_ACTION_CHOIX_TROIS,
        [] { return std::make_unique<ActionActionChoixTrois>(); });
    api_->actions()->register_action(
        ID_ACTION_ACTION_CHOIX_PAQUETS,
        [] { return std::make_unique<ActionActionChoixPaquets>(); });
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.

    int err = api_->game_state_check(action);
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action.id(), action.player_id(), err);
    api_->game_state_apply(action);
}

bool Rules::is_finished()
{
    // FIXME
    return api_->game_state().fini();
}

void Rules::start_of_player_turn(unsigned int)
{
    api_->game_state().debut_tour();
};
void Rules::end_of_player_turn(unsigned int)
{
    api_->game_state().fin_tour();
};

void Rules::player_turn()
{
    try
    {
        sandbox_.execute(champion_jouer_tour_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("champion_jouer_tour: timeout");
    }
}

void Rules::spectator_turn()
{
    champion_jouer_tour_();
}

void Rules::at_player_start(rules::ClientMessenger_sptr)
{
    try
    {
        sandbox_.execute(champion_init_jeu_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("champion_init_jeu_: timeout");
    }
}

void Rules::at_spectator_start(rules::ClientMessenger_sptr)
{
    champion_init_jeu_();
}

void Rules::at_player_end(rules::ClientMessenger_sptr)
{
    try
    {
        sandbox_.execute(champion_fin_jeu_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("champion_init_jeu_: timeout");
    }
}

void Rules::at_spectator_end(rules::ClientMessenger_sptr)
{
    champion_fin_jeu_();
}