#include"gamestate.h"
#include<qDebug>


GameState::GameState()
{
    gameMapVec.clear();
    for (int i = 0; i < 15; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j < 15; j++)
            lineBoard.push_back(0);
        gameMapVec.push_back(lineBoard);
    }
    qDebug()<<"gameState inited";
}

void GameState::StartGame()
{
    //清空上一局棋盘
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
            gameMapVec[i][j]=0;
    }
    gameStatus=PLAYING;
    playerFlag=true;
    qDebug()<<"start game!";
}

//落子后判断是否胜利，即游戏结束
bool GameState::isWin(int x, int y)
{
    int cnt=0;
    //横向
    for(int i=1;i<=4;i++)
    {
        if(x-i<0||gameMapVec[x][y]!=gameMapVec[x-i][y])
            break;
        cnt++;
    }
    for(int i=1;i<=4;i++)
    {
        if(x+i>14||gameMapVec[x][y]!=gameMapVec[x+i][y])
            break;
        cnt++;
    }
    if(cnt+1>=5)
    {
        qDebug()<<"hengwin";
        return true;

    }
    //纵向
    cnt=0;//计数器清零
    for(int i=1;i<=4;i++)
    {
        if(y-i<0||gameMapVec[x][y]!=gameMapVec[x][y-i])
            break;
        cnt++;
    }
    for(int i=1;i<=4;i++)
    {
        if(y+i>14||gameMapVec[x][y]!=gameMapVec[x][y+i])
            break;
        cnt++;
    }
    if(cnt+1>=5)
    {
        qDebug()<<"shuwin";
        return true;
    }
    //左斜
    cnt=0;
    for(int i=1;i<=4;i++)
    {
        if(y-i<0||x-i<0||gameMapVec[x][y]!=gameMapVec[x-i][y-i])
            break;
        cnt++;
    }
    for(int i=1;i<=4;i++)
    {
        if(y+i>14||x+i>14||gameMapVec[x][y]!=gameMapVec[x+i][y+i])
            break;
        cnt++;
    }
    if(cnt+1>=5)
    {
        qDebug()<<"zuoxiewin";
        return true;
    }
    //右斜
    cnt=0;
    for(int i=1;i<=4;i++)
    {
        if(y+i>14||x-i<0||gameMapVec[x][y]!=gameMapVec[x-i][y+i])
            break;
        cnt++;
    }
    for(int i=1;i<=4;i++)
    {
        if(y-i<0||x+i>14||gameMapVec[x][y]!=gameMapVec[x+i][y-i])
            break;
        cnt++;
    }
    if(cnt+1>=5)
    {
        qDebug()<<"youxiewin";
        return true;
    }
    return false;

}
