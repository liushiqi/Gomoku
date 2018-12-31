//
// Created by liu on 18-11-10.
//

#include "detector.h"

typedef struct sst {
  pos_t pos;
  int chess;
  void *param;
  int (*get_chess_func)(pos_t, void *);
} search_stack_t;

static pos_t dirs[8] = {{1, 0},
                        {1, 1},
                        {0, 1},
                        {-1, 1},
                        {-1, 0},
                        {-1, -1},
                        {0, -1},
                        {1, -1}};

int get_chess_from_stack(pos_t pos, void *param) {
  search_stack_t *stack = (search_stack_t *) param;
  if (!pos_cmp(pos, stack->pos) && stack->chess != 0) return stack->chess;
  else return stack->get_chess_func(pos, stack->param);
}

int check_available(pos_t point, int player, int (*get_chess_func)(pos_t, void *), void *param, int depth) {
  return get_chess_func(point, param) == 0 && !is_forbidden(point, player, get_chess_func, param, depth);
}

int search_chess_one_dir(pos_t point, pos_t direction, int (*get_chess_func)(pos_t, void *), void *param, int depth) {
  int current = get_chess_func(point, param);
  int direct_link = 1, impl_link = 0, space = 0, last_zero1 = 0;
  pos_t cur = point;
  while (1) {
    cur = pos_add(cur, direction);
    int chess = get_chess_func(cur, param);
    if (chess == current) {
      ++direct_link;
    } else if (chess == 0) {
      if (check_available(cur, current, get_chess_func, param, depth + 1) != 0) {
        ++last_zero1;
        while (1) {
          cur = pos_add(cur, direction);
          int chess_now = get_chess_func(cur, param);
          if (chess_now == current) {
            --last_zero1;
            ++impl_link;
            if (space == 0) ++space;
          } else if (chess_now == 0 && last_zero1 == 0) {
            ++last_zero1;
            break;
          } else {
            break;
          }
        }
        break;
      }
      break;
    } else break;
  }
  int last_zero2 = 0;
  cur = point;
  while (1) {
    cur = pos_sub(cur, direction);
    int chess = get_chess_func(cur, param);
    if (chess == current) {
      ++direct_link;
    } else if (chess == 0) {
      if (check_available(cur, current, get_chess_func, param, depth + 1) && impl_link == 0) {
        ++last_zero2;
        while (1) {
          cur = pos_sub(cur, direction);
          int chess_now = get_chess_func(cur, param);
          if (chess_now == current) {
            --last_zero2;
            ++impl_link;
            if (space == 0) ++space;
          } else if (chess_now == 0 && last_zero2 == 0) {
            ++last_zero2;
            break;
          } else break;
        }
        break;
      }
      break;
    } else break;
  }
  int last_zero = last_zero1 + last_zero2;
  if (direct_link >= 5) return direct_link;
  else if (direct_link == 4 && last_zero > 0) return 4;
  else if (direct_link == 4 && last_zero == 0) return 0;
  else if (direct_link == 3) {
    if (impl_link == 1) return 4;
    else if (last_zero == 2) return 3;
    else return 0;
  } else if (direct_link == 2) {
    if (impl_link >= 2) return 4;
    else if (impl_link == 1 && last_zero >= 1) return 3;
    else if (last_zero == 2) return 2;
    else return 0;
  } else if (impl_link >= 3) return 4;
  else if (impl_link == 2 && last_zero >= 1) return 3;
  else if (impl_link == 1 && last_zero == 2) return 2;
  else if (last_zero == 2) return 1;
  else return 0;
}

int is_win(pos_t pos, int (*get_chess_func)(pos_t, void *), void *param) {
  int t;
  if (get_chess_func(pos, param) == 1)
    for (int i = 0; i < 8; ++i) {
      if ((t = search_chess_one_dir(pos, dirs[i], get_chess_func, param, 0)) == 5 && t < 6)
        return 1;
      else if (t >= 6) return 0;
    }
  else if (get_chess_func(pos, param) == 2)
    for (int i = 0; i < 8; ++i) {
      if (search_chess_one_dir(pos, dirs[i], get_chess_func, param, 0) >= 5)
        return 2;
    }
  return 0;
}

int is_forbidden(pos_t pos, int player, int (*get_chess_func)(pos_t, void *), void *param, int depth) {
  if (player == 1 && depth < 3) {
    search_stack_t sst;
    sst.pos = pos;
    sst.get_chess_func = get_chess_func;
    sst.param = param;
    sst.chess = player;
    int double_three = 0, double_four = 0;
    for (int i = 0; i < 8; ++i) {
      switch (search_chess_one_dir(pos, dirs[i], get_chess_from_stack, &sst, depth)) {
        case 6:
        case 7:
        case 8:
        case 9:return 1;
        case 4: ++double_four;
          break;
        case 3: ++double_three;
          break;
        default:break;
      }
    }
    if (double_three == 4 || double_four == 4) return 1;
  }
  return 0;
}
