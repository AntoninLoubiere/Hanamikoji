// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <iostream>
#include <sstream>
#include <vector>

#include "api.hh"
#include "constant.hh"

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
    os << "[";
    typename std::vector<T>::const_iterator it;
    for (it = arr.begin(); it != arr.end(); ++it)
    {
        if (it != arr.begin())
            os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}


extern "C" joueur api_id_joueur()
{
    return api->id_joueur();
}

extern "C" joueur api_id_adversaire()
{
    return api->id_adversaire();
}

extern "C" int api_manche()
{
    return api->manche();
}

extern "C" int api_tour()
{
    return api->tour();
}

extern "C" action_jouee api_tour_precedent()
{
    return api->tour_precedent();
}

extern "C" int api_nb_carte_validee(joueur j, int g)
{
    return api->nb_carte_validee(j, g);
}

extern "C" joueur api_possession_geisha(int g)
{
    return api->possession_geisha(g);
}

extern "C" bool api_est_jouee_action(joueur j, action a)
{
    return api->est_jouee_action(j, a);
}

extern "C" int api_nb_cartes(joueur j)
{
    return api->nb_cartes(j);
}

extern "C" std::vector<int> api_cartes_en_main()
{
    return api->cartes_en_main();
}

extern "C" int api_carte_pioche()
{
    return api->carte_pioche();
}

extern "C" void api_action_valider(int c)
{
    api->action_valider(c);
}

extern "C" void api_action_defausser(int c1, int c2)
{
    api->action_defausser(c1, c2);
}

extern "C" void api_action_choix_trois(int c1, int c2, int c3)
{
    api->action_choix_trois(c1, c2, c3);
}

extern "C" void api_action_choix_paquets(int p1c1, int p1c2, int p2c1, int p2c2)
{
    api->action_choix_paquets(p1c1, p1c2, p2c1, p2c2);
}

extern "C" error api_repondre_choix_trois(int p)
{
    return api->repondre_choix_trois(p);
}

extern "C" error api_repondre_choix_paquets(int p)
{
    return api->repondre_choix_paquets(p);
}

extern "C" void api_afficher_action(action v)
{
    api->afficher_action(v);
}

extern "C" void api_afficher_error(error v)
{
    api->afficher_error(v);
}

extern "C" void api_afficher_joueur(joueur v)
{
    api->afficher_joueur(v);
}

extern "C" void api_afficher_action_jouee(action_jouee v)
{
    api->afficher_action_jouee(v);
}

std::ostream& operator<<(std::ostream& os, action v)
{
    switch (v)
    {
    case VALIDER:
        os << "VALIDER";
        break;
    case DEFAUSSER:
        os << "DEFAUSSER";
        break;
    case CHOIX_TROIS:
        os << "CHOIX_TROIS";
        break;
    case CHOIX_PAQUETS:
        os << "CHOIX_PAQUETS";
        break;
    case PREMIER_JOUEUR:
        os << "PREMIER_JOUEUR";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, error v)
{
    switch (v)
    {
    case OK:
        os << "OK";
        break;
    case ACTION_DEJA_JOUEE:
        os << "ACTION_DEJA_JOUEE";
        break;
    case CARTES_INVALIDES:
        os << "CARTES_INVALIDES";
        break;
    case PAQUET_INVALIDE:
        os << "PAQUET_INVALIDE";
        break;
    case GEISHA_INVALIDES:
        os << "GEISHA_INVALIDES";
        break;
    case JOUEUR_INVALIDE:
        os << "JOUEUR_INVALIDE";
        break;
    case CHOIX_INVALIDE:
        os << "CHOIX_INVALIDE";
        break;
    case ACTION_INVALIDE:
        os << "ACTION_INVALIDE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, joueur v)
{
    switch (v)
    {
    case JOUEUR_1:
        os << "JOUEUR_1";
        break;
    case JOUEUR_2:
        os << "JOUEUR_2";
        break;
    case EGALITE:
        os << "EGALITE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, action_jouee v)
{
    os << "{ ";
    os << "act"
       << "=" << v.act;
    os << ", ";
    os << "c1"
       << "=" << v.c1;
    os << ", ";
    os << "c2"
       << "=" << v.c2;
    os << ", ";
    os << "c3"
       << "=" << v.c3;
    os << ", ";
    os << "c4"
       << "=" << v.c4;
    os << " }";
    return os;
}