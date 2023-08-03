// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "cardset.hh"

int ActionActionChoixTrois::check(const GameState& st) const
{
    joueur j = st.joueur_from_id(player_id_);

    if (st.joueur_courant() != j)
        return ACTION_INVALIDE;

    if (st.est_action_deja_jouee() || st.est_jouee_action(j, CHOIX_TROIS))
        return ACTION_DEJA_JOUEE;

    if (!is_card_valid(c1_) || !is_card_valid(c2_) || !is_card_valid(c3_) ||
        !st.a_cartes(j, EMPTY_CARDSET + c1_ + c2_ + c3_))
        return CARTES_INVALIDES;

    return OK;
}

void ActionActionChoixTrois::apply_on(GameState* st) const
{
    joueur j = st->joueur_from_id(player_id_);
    st->appliquer_act_choix_trois(j, c1_, c2_, c3_);
}
