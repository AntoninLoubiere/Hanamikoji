// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "cardset.hh"
#include "constant.hh"

int ActionActionValider::check(const GameState& st) const
{
    joueur j = st.joueur_from_id(player_id_);
    if (st.est_action_deja_jouee() || st.est_jouee_action(j, VALIDER))
        return ACTION_DEJA_JOUEE;

    if (!st.a_cartes(j, EMPTY_CARDSET + c_))
        return CARTES_INVALIDES;

    return OK;
}

void ActionActionValider::apply_on(GameState* st) const
{
    joueur j = st->joueur_from_id(player_id_);
    st->enlever_cartes_main(j, c_);
    st->valider_cartes(j, c_);
    st->faire_action(j, VALIDER);
}
