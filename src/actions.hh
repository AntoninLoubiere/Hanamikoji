// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

enum action_id
{
    ID_ACTION_ACTION_VALIDER,
    ID_ACTION_ACTION_DEFAUSSER,
    ID_ACTION_ACTION_CHOIX_TROIS,
    ID_ACTION_ACTION_CHOIX_PAQUETS,
    ID_ACTION_REPONDRE_CHOIX_TROIS,
    ID_ACTION_REPONDRE_CHOIX_PAQUETS,
};

#include "action_action_valider.hh"
#include "action_action_defausser.hh"
#include "action_action_choix_trois.hh"
#include "action_action_choix_paquets.hh"
#include "action_repondre_choix_trois.hh"
#include "action_repondre_choix_paquets.hh"
