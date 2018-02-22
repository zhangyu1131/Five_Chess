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
   if(gameModel==PVE)//如果是PVE模式，则需要初始化评分数组
   {
       scoreMapVec.clear();
       for (int i = 0; i < 15; i++)
       {
             std::vector<int> lineScores;
             for (int j = 0; j < 15; j++)
                    lineScores.push_back(0);
             scoreMapVec.push_back(lineScores);
       }
   }
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

//计算每个点评分，用于电脑下期
void GameState::calculateScore()
{
    //先全部置0
    //清空上一局棋盘
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
            scoreMapVec[i][j]=0;
    }
    maxPoints.clear();
    int maxScore=0;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            scoreMapVec[i][j]+=calculateRowScore(i,j);//横
            scoreMapVec[i][j]+=calculateColScore(i,j);//纵
            scoreMapVec[i][j]+=calculateMainDiagonalScore(i,j);//主对角线
            scoreMapVec[i][j]+=calculateViceDiagonalScore(i,j);//副对角线
            if(scoreMapVec[i][j]>maxScore)
            {
                maxPoints.clear();
                maxPoints.push_back(QPoint(i,j));
            }
            else if(scoreMapVec[i][j]==maxScore)
                maxPoints.push_back(QPoint(i,j));
        }
    }
}
//计算横向得分
int GameState::calculateRowScore(int x, int y)
{
    int score=0;
    int i=0;
    for(i=1;i<=4;i++)
    {
        if(x-i<0||gameMapVec[x-i][y]!=-1)
            break;
    }

    return score;
}
//计算纵向得分
int GameState::calculateColScore(int x, int y)
{
    return 0;
}
//计算主对角线方向得分
int GameState::calculateMainDiagonalScore(int x, int y)
{
    return 0;
}
//计算副对角线得分
int GameState::calculateViceDiagonalScore(int x, int y)
{
    return 0;
}
