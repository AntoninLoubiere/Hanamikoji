// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "constant.hh"

int ActionActionChoixPaquets::check(const GameState& st) const
{
    joueur j = st.joueur_from_id(player_id_);

    if (st.joueur_courant() != j)
        return ACTION_INVALIDE;

    if (st.est_action_deja_jouee() || st.est_jouee_action(j, CHOIX_PAQUETS))
        return ACTION_DEJA_JOUEE;

    if (!st.a_cartes(j, EMPTY_CARDSET + p1c1_ + p1c2_ + p2c1_ + p2c2_))
        return CARTES_INVALIDES;

    return OK;
}

void ActionActionChoixPaquets::apply_on(GameState* st) const
{
    joueur j = st->joueur_from_id(player_id_);
    st->appliquer_act_choix_paquets(j, p1c1_, p1c2_, p2c1_, p2c2_);
}
