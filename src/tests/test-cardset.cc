#include "../cardset.hh"
#include <gtest/gtest.h>
#include <vector>

void assert_cardset(cardset v, std::vector<int> els)
{
    ASSERT_EQ(cardset_to_vector(v), els);
}

using namespace testing;
TEST(CardSetTest, AddCards)
{
    assert_cardset(EMPTY_CARDSET, {});
    assert_cardset(EMPTY_CARDSET + 0, {0});
    assert_cardset(EMPTY_CARDSET + 0 + 0, {0, 0});
    assert_cardset(EMPTY_CARDSET + 1 + 1, {1, 1});
    assert_cardset(EMPTY_CARDSET + 2 + 2, {2, 2});
    assert_cardset(EMPTY_CARDSET + 3 + 3 + 3, {3, 3, 3});
    assert_cardset(EMPTY_CARDSET + 3 + 2 + 3 + 2 + 3, {2, 2, 3, 3, 3});
    assert_cardset(EMPTY_CARDSET + 4 + 4 + 4, {4, 4, 4});
    assert_cardset(EMPTY_CARDSET + 5 + 5 + 5 + 5, {5, 5, 5, 5});
    assert_cardset(EMPTY_CARDSET + 6 + 6 + 6 + 6 + 6, {6, 6, 6, 6, 6});
    ASSERT_TRUE(
        contains_cardset(EMPTY_CARDSET + 6 + 6 + 6 + 3,
                         EMPTY_CARDSET + 6 + 6 + 6 + 6 + 6 + 3 + 4 + 2));
}