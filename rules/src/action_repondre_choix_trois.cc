// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "constant.hh"

int ActionRepondreChoixTrois::check(const GameState& st) const
{
    joueur j = st.joueur_from_id(player_id_);
    action_jouee act = st.derniere_action();

    if (st.joueur_courant() == j || !st.attente_reponse() ||
        act.act != CHOIX_TROIS)
        return CHOIX_INVALIDE;

    if (c_ < 0 || 3 <= c_)
        return CARTES_INVALIDES;

    return OK;
}

void ActionRepondreChoixTrois::apply_on(GameState* st) const
{
    joueur j = st->joueur_from_id(player_id_);
    st->appliquer_repondre_trois(j, c_);
}
