// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "rules.hh"

#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"
#include "rules/client-messenger.hh"
#include "utils/log.hh"
#include "utils/sandbox.hh"

Rules::Rules(const rules::Options opt)
    : rules::Rules(opt)
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
    api_->actions()->register_action(
        ID_ACTION_REPONDRE_CHOIX_TROIS,
        [] { return std::make_unique<ActionRepondreChoixTrois>(); });
    api_->actions()->register_action(
        ID_ACTION_REPONDRE_CHOIX_PAQUETS,
        [] { return std::make_unique<ActionRepondreChoixPaquets>(); });
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

void Rules::player_loop(rules::ClientMessenger_sptr msgr)
{
    try
    {
        sandbox_.execute(champion_init_jeu_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("champion_init_jeu: timeout");
    }

    while (!is_finished())
    {
        uint32_t playing_id;

        DEBUG("Waiting for a turn...");
        /* Other players turns */
        if (msgr->wait_for_turn(opt_.player->id, &playing_id))
        {
            if (is_spectator(playing_id))
            {
                /* Actions of spectators are not published. */
                DEBUG("Turn for spectator %d, never mind...", playing_id);
                continue;
            }

            DEBUG("Turn for player %d (not me)", playing_id);
            api_->game_state().debut_tour();

            /* Get current player actions */
            rules::Actions* actions = get_actions();
            actions->clear();
            DEBUG("Getting actions...");
            msgr->pull_actions(actions);
            DEBUG("Got %u actions", actions->size());

            /* Apply them onto the gamestate */
            for (const auto& action : actions->all())
                apply_action(*action);
        }
        else /* Current player turn */
        {
            DEBUG("Turn for player %d (me!!!)", playing_id);
            auto turn_start = std::chrono::high_resolution_clock::now();

            api_->game_state().debut_tour();

            get_actions()->clear();

            // On joue le tour

            if (api_->game_state().attente_reponse())
            {
                action_jouee aj = api_->game_state().derniere_action();

                if (aj.act == CHOIX_TROIS)
                {
                    try
                    {
                        sandbox_.execute(champion_repondre_action_choix_trois_);
                    }
                    catch (utils::SandboxTimeout&)
                    {
                        FATAL("champion_repondre_action_choix_trois: timeout");
                    }
                }
                else if (aj.act == CHOIX_PAQUETS)
                {
                    try
                    {
                        sandbox_.execute(
                            champion_repondre_action_choix_paquets_);
                    }
                    catch (utils::SandboxTimeout&)
                    {
                        FATAL("champion_repondre_action_choix_paquets: "
                              "timeout");
                    }
                }
                else
                    FATAL("Réponse à cette action inconnue.");
            }
            else
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

            rules::Actions* actions = get_actions();
            DEBUG("Sending %u actions...", actions->size());
            msgr->send_actions(*actions);
            DEBUG("Waiting for acknowledgement...");
            msgr->wait_for_ack();
            DEBUG("Getting actions...");
            msgr->pull_actions(actions);
            DEBUG("Got %u actions", actions->size());
            actions->clear();

            // Record turn duration
            auto turn_end = std::chrono::high_resolution_clock::now();
            double millis =
                std::chrono::duration<double, std::milli>(turn_end - turn_start)
                    .count();
            players_[playing_id]->turn_duration_ms.push_back(millis);
        }
        api_->game_state().fin_tour();
    }

    write_stats();

    try
    {
        sandbox_.execute(champion_fin_jeu_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("champion_fin_jeu: timeout");
    }
}

void Rules::replay_loop(rules::ReplayMessenger_sptr msgr)
{
    auto actions = get_actions();
    while (!is_finished())
    {
        joueur joueur_courant = api_->game_state().joueur_courant();
        auto player =
            players_[api_->game_state().attente_reponse() ? ~joueur_courant
                                                          : joueur_courant];
        DEBUG("Turn for player: %d", player->id);

        api_->game_state().debut_tour();

        msgr->pull_actions(actions);
        DEBUG("Pulled %d actions", actions->size());
        for (const auto& action : actions->all())
            apply_action(*action);
        actions->clear();

        api_->game_state().fin_tour();

        for (const auto& spectator : spectators_)
        {
            DEBUG("Turn for spectator %d", spectator->id);

            msgr->pull_actions(actions);
            DEBUG("Pulled %d actions", actions->size());
            for (const auto& action : actions->all())
                apply_action(*action);
            actions->clear();
        }
    }

    write_stats();
}

void Rules::spectator_loop(rules::ClientMessenger_sptr msgr)
{
    FATAL("No spectator for now !");
    // bool last_round = false;
    // uint32_t last_player_id;
    // msgr->pull_id(&last_player_id);

    // at_start();
    // at_spectator_start(msgr);

    // start_of_round();
    // uint32_t previous_playing_id = static_cast<uint32_t>(-1);
    // /* `last_round` allows us to inspect the final state of the game,
    // when no
    //  * other player can play anymore. */
    // while (last_round || !is_finished())
    // {

    //     uint32_t playing_id;
    //     bool my_turn = false;

    //     DEBUG("Waiting for a turn...");
    //     /* Other players turns */
    //     if (msgr->wait_for_turn(opt_.player->id, &playing_id))
    //     {
    //         if (is_spectator(playing_id))
    //         {
    //             DEBUG("Turn for spectator %d, never mind...",
    //             playing_id); start_of_spectator_turn(playing_id);
    //             start_of_turn(playing_id);
    //             end_of_spectator_turn(playing_id);
    //             end_of_turn(playing_id);
    //             continue;
    //         }

    //         DEBUG("Turn for player %d", playing_id);
    //         previous_playing_id = playing_id;
    //         start_of_player_turn(playing_id);
    //         start_of_turn(playing_id);

    //         /* Get current player actions */
    //         Actions* actions = get_actions();
    //         actions->clear();
    //         DEBUG("Getting actions...");
    //         msgr->pull_actions(actions);
    //         DEBUG("Got %u actions", actions->size());

    //         /* Apply them onto the gamestate */
    //         for (const auto& action : actions->all())
    //             apply_action(*action);

    //         end_of_player_turn(playing_id);
    //     }
    //     else /* Current player turn */
    //     {
    //         DEBUG("Turn for spectator %d (me!!!)", playing_id);
    //         my_turn = true;
    //         start_of_spectator_turn(playing_id);
    //         start_of_turn(playing_id);

    //         get_actions()->clear();
    //         spectator_turn();
    //         /* The server is waiting for this spectator actions, so the
    //          * spectator can (on intend) hang some time so the user can
    //          inspect
    //          * the state of the game. */
    //         Actions* actions = get_actions();
    //         DEBUG("Sending %u actions...", actions->size());
    //         msgr->send_actions(*actions);
    //         DEBUG("Waiting for acknowledgement...");
    //         msgr->wait_for_ack();
    //         /* The server do not publish spectators' actions: do not try
    //         to
    //          * pull them.  */
    //         end_of_spectator_turn(playing_id);
    //     }

    //     end_of_turn(playing_id);

    //     /* End of each round */
    //     if (last_round)
    //     {
    //         /* If that was the last round, stop there. */
    //         DEBUG("That was the last turn, bye!");
    //         break;
    //     }
    //     if (last_player_id == previous_playing_id && my_turn)
    //     {
    //         DEBUG("End of round!");
    //         end_of_round();
    //         if (!is_finished())
    //             start_of_round();
    //         else
    //         {
    //             DEBUG("The next round will be the last one!");
    //             last_round = true;
    //         }
    //     }
    // }

    // at_end();
    // at_spectator_end(msgr);

    // write_stats();
}

void Rules::server_loop(rules::ServerMessenger_sptr msgr)
{
    api_->game_state().debut_manche();
    dump_state_stream();

    while (!is_finished())
    {
        joueur joueur_courant = api_->game_state().joueur_courant();
        auto player =
            players_[api_->game_state().attente_reponse() ? ~joueur_courant
                                                          : joueur_courant];

        auto turn_start = std::chrono::high_resolution_clock::now();

        api_->game_state().debut_tour();

        DEBUG("Turn for player %d", player->id);
        msgr->push_id(player->id);
        rules::Actions* actions = get_actions();
        actions->clear();

        if (player->nb_timeout < rules::max_consecutive_timeout)
        {
            if (!msgr->poll(timeout_ > 0 ? timeout_ : -1))
            {
                player->nb_timeout++;
                DEBUG("Timeout reached, never mind: %d", player->nb_timeout);
            }
            else
            {
                DEBUG("Getting actions...");
                msgr->recv_actions(actions);
                DEBUG("Got %u actions", actions->size());
                DEBUG("Acknowledging...");
                msgr->ack();

                for (const auto& action : actions->all())
                    apply_action(*action);
            }
        }

        metrics_["actions_total"] += actions->size();
        save_player_actions(actions);
        msgr->push_actions(*actions);

        api_->game_state().fin_tour();

        dump_state_stream();

        // Record turn duration
        auto turn_end = std::chrono::high_resolution_clock::now();
        double millis =
            std::chrono::duration<double, std::milli>(turn_end - turn_start)
                .count();
        player->turn_duration_ms.push_back(millis);

        /* Spectators must be able to see the state of the game between
         * after each player has finished its turn. */
        for (const auto& s : spectators_)
        {
            DEBUG("Turn for spectator %d", s->id);
            msgr->push_id(s->id);
            rules::Actions* actions = get_actions();
            actions->clear();
            DEBUG("Receiving its only Ack action...");
            msgr->recv_actions(actions);
            DEBUG("Acknowledging...");
            msgr->ack();

            save_player_actions(actions);
        }
    }

    write_stats();
}

bool Rules::is_finished()
{
    return api_->game_state().fini();
}

void Rules::dump_state(std::ostream& out)
{
    api_->game_state().dump_state(out);
}