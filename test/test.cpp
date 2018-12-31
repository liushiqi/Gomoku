//
// Created by liu on 18-12-31.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE gomoku

#include <boost/test/unit_test.hpp>
#include <interface/detector.h>

int search_board[8][8] = {{0, 2, 0, 2, 0, 0, 0, 2},
                          {0, 0, 1, 1, 1, 0, 2, 2},
                          {0, 0, 0, 1, 1, 1, 1, 2},
                          {0, 0, 1, 1, 1, 0, 0, 2},
                          {0, 1, 1, 1, 1, 1, 0, 2},
                          {2, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0},
                          {2, 0, 1, 1, 0, 0, 1, 0}};

int search_get_chess_func(pos_t pos, __attribute__((unused)) void *param) {
  if (pos.x > 7 || pos.y > 7 || pos.x < 0 || pos.y < 0)
    return -1;
  return search_board[pos.x][pos.y];
}

int forbidden_board[9][9] = {{0, 0, 2, 0, 0, 0, 2, 0, 0},
                             {0, 0, 2, 1, 0, 1, 1, 0, 0},
                             {0, 0, 0, 0, 1, 0, 1, 0, 0},
                             {0, 0, 0, 0, 0, 1, 1, 0, 0},
                             {0, 0, 0, 1, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 1, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0}};

int forbidden_get_chess_func(pos_t pos, __attribute__((unused)) void *param) {
  if (pos.x > 8 || pos.y > 8 || pos.x < 0 || pos.y < 0)
    return -1;
  return forbidden_board[pos.x][pos.y];
}

BOOST_AUTO_TEST_SUITE(forbidden)

BOOST_AUTO_TEST_CASE(search) {
  BOOST_CHECK_EQUAL(search_chess_one_dir({7, 6}, {0, 1}, search_get_chess_func, nullptr, 0), 1);
  BOOST_CHECK_EQUAL(search_chess_one_dir({7, 2}, {0, 1}, search_get_chess_func, nullptr, 0), 2);
  BOOST_CHECK_EQUAL(search_chess_one_dir({3, 2}, {0, 1}, search_get_chess_func, nullptr, 0), 3);
  BOOST_CHECK_EQUAL(search_chess_one_dir({3, 2}, {1, 0}, search_get_chess_func, nullptr, 0), 3);
  BOOST_CHECK_EQUAL(search_chess_one_dir({3, 2}, {-1, 0}, search_get_chess_func, nullptr, 0), 3);
  BOOST_CHECK_EQUAL(search_chess_one_dir({2, 3}, {0, 1}, search_get_chess_func, nullptr, 0), 4);
  BOOST_CHECK_EQUAL(search_chess_one_dir({2, 3}, {0, -1}, search_get_chess_func, nullptr, 0), 4);
  BOOST_CHECK_EQUAL(search_chess_one_dir({4, 3}, {0, -1}, search_get_chess_func, nullptr, 0), 5);
  BOOST_CHECK_EQUAL(is_win({4, 3}, search_get_chess_func, nullptr), 1);
  BOOST_CHECK_EQUAL(is_win({2, 7}, search_get_chess_func, nullptr), 2);
}

BOOST_AUTO_TEST_CASE(forbidden) {
  BOOST_CHECK_EQUAL(is_forbidden({3, 4}, 1, forbidden_get_chess_func, nullptr, 0), 1);
  BOOST_CHECK_EQUAL(is_forbidden({4, 6}, 1, forbidden_get_chess_func, nullptr, 0), 1);
  BOOST_CHECK_EQUAL(is_forbidden({3, 7}, 1, forbidden_get_chess_func, nullptr, 0), 1);
  BOOST_CHECK_EQUAL(is_forbidden({1, 7}, 1, forbidden_get_chess_func, nullptr, 0), 0);
}

BOOST_AUTO_TEST_SUITE_END()
