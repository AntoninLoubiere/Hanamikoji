#pragma once

#include "constant.hh"
#include <cassert>
#include <iomanip>
#include <ostream>

typedef struct
{
    int cards;
} cardset;

const cardset EMPTY_CARDSET = {0};
const cardset GEISHA_MASK[] = {
    {0b00000'0000'000'000'00'00'11}, {0b00000'0000'000'000'00'11'00},
    {0b00000'0000'000'000'11'00'00}, {0b00000'0000'000'111'00'00'00},
    {0b00000'0000'111'000'00'00'00}, {0b00000'1111'000'000'00'00'00},
    {0b1111'0000'000'000'00'00'00}};

cardset operator|(cardset set1, cardset set2);
cardset operator&(cardset set1, cardset set2);
cardset operator+(cardset set, int c);
cardset& operator+=(cardset& set, int c);
cardset operator-(cardset set, int c);
cardset& operator-=(cardset& set, int c);

bool operator==(cardset set1, cardset set2);
bool operator!=(cardset set1, cardset set2);
bool operator<(cardset set1, cardset set2);
bool operator>(cardset set1, cardset set2);
bool operator<=(cardset set1, cardset set2);
bool operator>=(cardset set1, cardset set2);

std::ostream& operator<<(std::ostream& out, cardset set);

std::vector<int> cardset_to_vector(cardset set);
int cardset_count(cardset set);

/* Renvoie une geisha */
int card_to_geisha(int c);

/* Renvoie le joueur ayant la majorité des cartes sur la geisha g*/
joueur majorite_carte(cardset c[2], int g);

/* Renvoie small_set inclus? dans big_set */
bool contains_cardset(cardset small_set, cardset big_set);