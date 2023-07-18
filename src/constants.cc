#include "constant.hh"
#include <cassert>

joueur operator~(joueur j)
{
    switch (j)
    {
    case JOUEUR_1:
        return JOUEUR_2;
    case JOUEUR_2:
        return JOUEUR_1;
    default:
        return EGALITE;
    }
}

int joueur_to_int(joueur j)
{
    assert(j != EGALITE);
    return (int)j;
}