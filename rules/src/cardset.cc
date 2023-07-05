#include "cardset.hh"
#include "constant.hh"
#include "utils/log.hh"
#include <vector>

const int OFFSETS[] = {0, 2, 4, 6, 9, 12, 16};

cardset operator|(cardset set1, cardset set2)
{
    return {set1.cards | set2.cards};
}

cardset operator&(cardset set1, cardset set2)
{
    return {set1.cards & set2.cards};
}

cardset operator+(cardset set, int c)
{
    assert(0 <= c && c < NB_GEISHA);
    switch ((GEISHA_MASK[c] & set).cards >> OFFSETS[c])
    {
    case 0:
        return {set.cards | (1 << OFFSETS[c])};
    case 1:
        return {set.cards | (1 << (OFFSETS[c] + 1))};
    case 3:
        return {set.cards | (1 << (OFFSETS[c] + 2))};
    case 7:
        return {set.cards | (1 << (OFFSETS[c] + 3))};
    default:
        return {set.cards | (1 << (OFFSETS[c] + 4))};
    }
}

cardset operator-(cardset set, int c)
{
    assert(0 <= c && c < NB_GEISHA);
    switch ((GEISHA_MASK[c] & set).cards >> OFFSETS[c])
    {
    case 0:
        return set;
    case 1:
        return {set.cards ^ (1 << (OFFSETS[c]))};
    case 3:
        return {set.cards ^ (1 << (OFFSETS[c] + 1))};
    case 7:
        return {set.cards ^ (1 << (OFFSETS[c] + 2))};
    case 15:
        return {set.cards ^ (1 << (OFFSETS[c] + 3))};
    default:
        return {set.cards ^ (1 << (OFFSETS[c] + 4))};
    }
}

cardset& operator+=(cardset& set, int c)
{
    set = set + c;
    return set;
}

cardset& operator-=(cardset& set, int c)
{
    set = set - c;
    return set;
}

bool operator==(cardset set1, cardset set2)
{
    return set1.cards == set2.cards;
}

bool operator<(cardset set1, cardset set2)
{
    return set1.cards < set2.cards;
}

bool operator>(cardset set1, cardset set2)
{
    return set1.cards > set2.cards;
}

bool operator<=(cardset set1, cardset set2)
{
    return set1.cards <= set2.cards;
}

bool operator>=(cardset set1, cardset set2)
{
    return set1.cards >= set2.cards;
}

joueur majorite_carte(cardset c[2], int g)
{
    const cardset cg1 = c[0] & GEISHA_MASK[g];
    const cardset cg2 = c[1] & GEISHA_MASK[g];
    if (cg1 == cg2)
        return EGALITE;
    else if (cg1 > cg2)
        return JOUEUR_1;
    else
        return JOUEUR_2;
}

bool contains_cardset(cardset small_set, cardset big_set)
{
    return (big_set & small_set) == small_set;
}

std::vector<int> cardset_to_vector(cardset set)
{
    std::vector<int> cards;
    int f = 1;
    for (int i = 0; i < NB_CARTES_TOTAL; i++)
    {
        if (set.cards & f)
            cards.push_back(card_to_geisha(i));
        f <<= 1;
    }
    return cards;
}

int cardset_count(cardset set)
{
    int count = 0;
    int f = 1;
    for (int i = 0; i < NB_CARTES_TOTAL; i++)
    {
        if (set.cards & f)
            count++;
        f <<= 1;
    }
    return count;
}

int card_to_geisha(int c)
{
    switch (c)
    {
    case 0:
    case 1:
        return 0;
    case 2:
    case 3:
        return 1;
    case 4:
    case 5:
        return 2;
    case 6:
    case 7:
    case 8:
        return 3;
    case 9:
    case 10:
    case 11:
        return 4;
    case 12:
    case 13:
    case 14:
    case 15:
        return 5;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 6;
    default:
        FATAL("Carte invalide");
    }
}