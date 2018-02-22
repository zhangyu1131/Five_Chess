//包含游戏状态和一些全局设置

#ifndef GAMESTATE_H
#define GAMESTATE_H
#include<vector>
#include<QPoint>
const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子边长
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
const int kAIDelay = 700; // AI下棋的思考时间

//游戏模式，默认情况下是普通PVP模式
enum GameModel{PVP,PVPONLINE,PVE};
//游戏状态
enum GameStatus{PLAYING,END,CLOSED};
//pve中的分值数组，从前往后依次是死一，活一，死二，活二，死三……五连。
std::vector<int> chessScore={4,20,90,400,800,6000,10000,20000,50000};

class GameState
{

public:
    GameState();
    void StartGame();
    bool isWin(int x,int y);
    //计算每个点的评分，电脑根据评分高低落子
    void calculateScore();
public:
    std::vector<std::vector<int>> gameMapVec; // 存储当前游戏棋盘和棋子的情况,空白为0，白子1，黑子-1
    std::vector<std::vector<int>> scoreMapVec; // 存储各个点位的评分情况，作为AI下棋依据
    bool playerFlag; // 标示下棋方
    GameModel gameModel; // 游戏模式
    GameStatus gameStatus; // 游戏状态
    std::vector<QPoint> maxPoints;
private:
    int calculateRowScore(int x,int y);
    int calculateColScore(int x,int y);
    int calculateMainDiagonalScore(int x,int y);
    int calculateViceDiagonalScore(int x,int y);
};
#endif // GAMESTATE_H
