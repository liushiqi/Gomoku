//
// Created by liu on 18-10-20.
//

#include <wchar.h>
#include "values.h"

const wchar_t *numberOfPlayersRequest = L"=================五子棋=================\n\n\n\n"
                                        L"欢迎使用五子棋交互程序，请选择玩家数量。\n"
                                        L"1：单人                    2：多人\n";

const wchar_t *numberRequest = L"请输入编号：";

const wchar_t *typeOfChessRequest = L"清选择您要使用的棋子颜色。\n"
                                    L"1：黑色                    2：白色\n";

const wchar_t *header = L"=================五子棋=================\n\n\n\n";

const wchar_t *foot = L"    A B C D E F G H I J K L M N O\n";

const wchar_t *direct = L"使用字母+数字的形式输入，如A1或a1，quit或Ctrl+D以退出。\n";

const wchar_t *blackTerm = L"请黑棋下子：";

const wchar_t *blackWin = L"黑棋胜！！！";

const wchar_t *whiteTerm = L"请白棋下子：";

const wchar_t *whiteWin = L"白棋胜！！！";

const wchar_t *quit = L"即将退出。";

const wchar_t *wrongInput = L"无效输入，请重新输入。\n";

const wchar_t *forbidden = L"该位置为禁手位，请重新输入。\n";

const wchar_t *oldest = L"已经处于最老的状态,无法悔棋.\n";

const wchar_t black = L'●';
const wchar_t lastBlack = L'◉';
const wchar_t white = L'○';
const wchar_t lastWhite = L'◎';
const wchar_t leftTop = L'┏';
const wchar_t rightTop = L'┓';
const wchar_t leftGround = L'┗';
const wchar_t rightGround = L'┛';
const wchar_t top = L'┯';
const wchar_t ground = L'┷';
const wchar_t left = L'┠';
const wchar_t right = L'┨';
const wchar_t middle = L'┼';
