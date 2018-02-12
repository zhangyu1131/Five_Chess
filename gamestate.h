#ifndef GAMESTATE_H
#define GAMESTATE_H
#include<vector>

class GameState
{

public:
    GameState();
public:
    std::vector<std::vector<int>> gameMapVec; // 存储当前游戏棋盘和棋子的情况,空白为0，白子1，黑子-1

};
#endif // GAMESTATE_H
