// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "constant.hh"

int ActionActionDefausser::check(const GameState& st) const
{
    joueur j = st.joueur_from_id(player_id_);

    if (st.joueur_courant() != j)
        return ACTION_INVALIDE;

    if (st.est_action_deja_jouee() || st.est_jouee_action(j, DEFAUSSER))
        return ACTION_DEJA_JOUEE;

    if (!st.a_cartes(j, EMPTY_CARDSET + c1_ + c2_))
        return CARTES_INVALIDES;

    return OK;
}

void ActionActionDefausser::apply_on(GameState* st) const
{
    joueur j = st->joueur_from_id(player_id_);
    st->appliquer_act_defausser(j, c1_, c2_);
}