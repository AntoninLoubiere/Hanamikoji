// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "constant.hh"

int ActionRepondreChoixPaquets::check(const GameState& st) const
{
    joueur j = st.joueur_from_id(player_id_);
    action_jouee act = st.derniere_action();

    if (st.joueur_courant() == j || !st.attente_reponse() ||
        act.act != CHOIX_PAQUETS)
        return CHOIX_INVALIDE;

    if (p_ < 0 || 1 <= p_)
        return PAQUET_INVALIDE;

    return OK;
}

void ActionRepondreChoixPaquets::apply_on(GameState* st) const
{
    joueur j = st->joueur_from_id(player_id_);
    st->appliquer_repondre_paquet(j, p_);
}
