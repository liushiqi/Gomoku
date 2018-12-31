//
// Created by liu on 18-10-20.
//

#include <wchar.h>
#include "values.h"

const wchar_t *const REQUEST_PLAYER_NUMBER = L"=================五子棋=================\n\n\n\n"
                                        L"欢迎使用五子棋交互程序，请选择玩家数量。\n"
                                        L"1：单人                    2：多人\n";

const wchar_t *const REQUEST_NUMBER = L"请输入编号：";

const wchar_t *const REQUEST_CHESS_TYPE = L"清选择您要使用的棋子颜色。\n"
                                    L"1：黑色                    2：白色\n";

const wchar_t *const BOARD_HEADER_LINES = L"=================五子棋=================\n\n\n\n";

const wchar_t *const BOARD_FOOT_LINES = L"    A B C D E F G H I J K L M N O\n";

const wchar_t *const INPUT_DIRECTION = L"使用字母+数字的形式输入，如A1或a1，quit或Ctrl+D以退出。\n";

const wchar_t *const BLACK_TERM_MESSAGE = L"请黑棋下子：";

const wchar_t *const BLACK_WIN_MESSAGE = L"黑棋胜！！！";

const wchar_t *const WHITE_TERM_MESSAGE = L"请白棋下子：";

const wchar_t *const WHITE_WIN_MESSAGE = L"白棋胜！！！";

const wchar_t *const QUIT_MESSAGE = L"即将退出。";

const wchar_t *const WRONG_INPUT_ERROR = L"无效输入，请重新输入。\n";

const wchar_t *const FORBIDDEN_POS_ERROR = L"该位置为禁手位，请重新输入。\n";

const wchar_t *const OLDEST_STATE_WARNING = L"已经处于最老的状态,无法悔棋.\n";

const char *RED_COLOR = "\033[31;1m";
const char *DEFAULT_COLOR = "\033[0m";
const wchar_t BLACK_CHESS = L'●';
const wchar_t WHITE_CHESS = L'○';
const wchar_t LEFT_TOP = L'┏';
const wchar_t RIGHT_TOP = L'┓';
const wchar_t LEFT_GROUND = L'┗';
const wchar_t RIGHT_GROUND = L'┛';
const wchar_t TOP_EDGE = L'┯';
const wchar_t GROUND_EDGE = L'┷';
const wchar_t LEFT_EDGE = L'┠';
const wchar_t RIGHT_EDGE = L'┨';
const wchar_t MIDDLE = L'┼';
