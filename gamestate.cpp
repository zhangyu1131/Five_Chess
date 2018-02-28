#include"gamestate.h"
#include<qDebug>
#include<random>
#include<time.h>

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
       qDebug()<<"gameModel is pve.";
       scoreMap.clear();
       for (int i = 0; i < 15; i++)
       {
             std::vector<int> lineScores;
             for (int j = 0; j < 15; j++)
                    lineScores.push_back(0);
             scoreMap.push_back(lineScores);
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


QPoint GameState::putChess(QPoint point)
{
    if(!playerFlag)//playerFlag为false表示电脑下棋
    {
        calculateScore();
        int minD=INT_MAX;
        std::vector<QPoint> minp;
        qDebug()<<maxPoints.size();
        for(int i=0;i<maxPoints.size();i++)
        {
            int distance=(maxPoints[i].x()-point.x())*(maxPoints[i].x()-point.x())+(maxPoints[i].y()-point.y())*(maxPoints[i].y()-point.y());
            if(distance<minD)
            {
                minp.clear();
                minD=distance;
                minp.push_back(maxPoints[i]);
            }
            else if(distance==minD)
                minp.push_back(maxPoints[i]);
        }
        //当存在多个评分一样的点时，随机选择一个
        srand(time(0));
        int index=(int)(rand()%minp.size());
        gameMapVec[minp[index].x()][minp[index].y()]=-1;//白棋
        for(int j=0;j<minp.size();j++)
            qDebug()<<"max point: "<<minp[j].x()<<" "<<minp[j].y()<<"size: "<<minp.size();
        playerFlag=!playerFlag;
        return minp[index];
    }
}

//计算每个点评分，用于电脑下棋
void GameState::calculateScore()
{
    //先全部置0
    //清空上一局棋盘
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            scoreMap[i][j]=0;
        }
    }
    maxPoints.clear();

    //对局势进行评判
    int maxScore=0;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            if(gameMapVec[i][j]==0)
            {
                scoreMap[i][j]+=calculateRowScore(i,j);//横
                scoreMap[i][j]+=calculateColScore(i,j);//纵
                scoreMap[i][j]+=calculateMainDiagonalScore(i,j);//主对角线
                scoreMap[i][j]+=calculateViceDiagonalScore(i,j);//副对角线
                if(scoreMap[i][j]>maxScore)
                {
                    maxScore=scoreMap[i][j];
                    maxPoints.clear();
                    maxPoints.push_back(QPoint(i,j));
                }
                else if(scoreMap[i][j]==maxScore)
                    maxPoints.push_back(QPoint(i,j));
            }
        }
    }
}


void GameState::accumulate(int i, int j, int blackNum, int whiteNum, int emptyNum,int &score)
{
    //pve中的分值数组，从前往后依次是死一，活一，死二，活二，死三……五连。
    //const std::vector<int> Scores={4,20,90,400,800,6000,10000,20000,50000};
    if(i<=loopSize||j<=loopSize)//死
    {
        if(blackNum==0) score+=2;//死一
        else if(blackNum==1)
            score+=60;//死二
        else if(blackNum==2)//死三
        {
            if(emptyNum==0)
                score+=500;
            else
                score+=300;
        }
        else if(blackNum==3)//死四
        {
            if(emptyNum==0)
                score+=8000;
            else
                score+=6000;
        }
        else if(blackNum>=4)//五连
        {
            if(emptyNum==0)
                score+=40000;
            else
                score+=24000;
        }

        if(whiteNum==0) score+=4;//死一
        else if(whiteNum==1)
            score+=90;//死二
        else if(whiteNum==2)//死三
        {
            if(emptyNum==0)
                score+=800;
            else
                score+=600;
        }
        else if(whiteNum==3)//死四
        {
            if(emptyNum==0)
                score+=10000;
            else
                score+=6000;
        }
        else if(whiteNum>=4)//五连
        {
                score+=50000;
        }
    }
    else//活
    {
        if(blackNum==0) score+=10;//活一
        else if(blackNum==1) score+=200;//活二
        else if(blackNum==2) //活三
        {
            if(emptyNum==0)
                score+=2000;
            else
                score+=1300;
        }
        else if(blackNum==3) //活四
        {
            if(emptyNum==0)//没有空格
                score+=16000;
            else
                score+=12000;
        }
        else if(blackNum>=4) //五连
        {
            if(emptyNum==0)
                score+=40000;
            else
                score+=24000;
        }

        if(whiteNum==0) score+=20;//活一
        else if(whiteNum==1) score+=400;//活二
        else if(whiteNum==2) //活三
        {
            if(emptyNum==0)
                score+=6000;
            else
                score+=4000;
        }
        else if(whiteNum==3) //活四
        {
            if(emptyNum==0)
                score+=20000;
            else
                score+=12000;
        }
        else if(whiteNum>=4) score+=50000;//五连
    }
}
//计算横向得分
int GameState::calculateRowScore(int x, int y)
{
    int score=0;
    int whiteNum=0,blackNum=0,emptyNum=0;
    int i=0,j=0;
    int colorLeft=0,colorRight=0;//棋色
    for(i=1;i<=loopSize;i++)//先往左检查
    {
        if(x-i<0)
            break;
        //首先将该点左侧的棋子颜色存入color
        if(colorLeft==0&&gameMapVec[x-i][y]!=0)
        {
            colorLeft=gameMapVec[x-i][y];
            emptyNum+=i-1;
        }
        if(colorLeft!=0&&(colorLeft+gameMapVec[x-i][y]==0))//若颜色不为空，且与当前检查棋子颜色不同，说明出现了阻拦，再往左已经没有意义
            break;
        if(gameMapVec[x-i][y]==-1)//白
            whiteNum++;
        else if(gameMapVec[x-i][y]==1)//黑
            blackNum++;
    }
    //往右检查
    for(j=1;j<=loopSize;j++)
    {
        if(x+j>14)
            break;
        //首先将该点右侧的棋子颜色存入color
        if(colorRight==0&&gameMapVec[x+j][y]!=0)
        {
            colorRight=gameMapVec[x+j][y];
            emptyNum+=j-1;
        }
        if(colorRight!=0&&(colorRight+gameMapVec[x+j][y]==0))//若颜色不为空，且与当前检查棋子颜色不同，说明出现了阻拦，再往左已经没有意义
            break;
        if(gameMapVec[x+j][y]==-1)//白
            whiteNum++;
        else if(gameMapVec[x+j][y]==1)//黑
            blackNum++;
    }

    accumulate(i,j,blackNum,whiteNum,emptyNum,score);
    return score;
}
//计算纵向得分
int GameState::calculateColScore(int x, int y)
{
    int score=0;
    int whiteNum=0,blackNum=0,emptyNum=0;
    int i=0,j=0;
    int colorLeft=0,colorRight=0;//棋色
    for(i=1;i<=loopSize;i++)
    {
        if(y-i<0)
            break;
        if(colorLeft==0&&gameMapVec[x][y-i]!=0)
        {
            colorLeft=gameMapVec[x][y-i];
            emptyNum+=i-1;
        }
        if(colorLeft!=0&&(colorLeft+gameMapVec[x][y-i]==0))
            break;
        if(gameMapVec[x][y-i]==-1)//白
            whiteNum++;
        else if(gameMapVec[x][y-i]==1)//黑
            blackNum++;
    }
    for(j=1;j<=loopSize;j++)
    {
        if(y+j>14)
            break;
        if(colorRight==0&&gameMapVec[x][y+j]!=0)
        {
            colorRight=gameMapVec[x][y+j];
            emptyNum+=j-1;
        }
        if(colorRight!=0&&(colorRight+gameMapVec[x][y+j]==0))
            break;
        if(gameMapVec[x][y+j]==-1)//白
            whiteNum++;
        else if(gameMapVec[x][y+j]==1)//黑
            blackNum++;
    }

    accumulate(i,j,blackNum,whiteNum,emptyNum,score);
    return score;
}
//计算主对角线方向得分
int GameState::calculateMainDiagonalScore(int x, int y)
{
    int score=0;
    int whiteNum=0,blackNum=0,emptyNum=0;
    int i=0,j=0;
    int colorLeft=0,colorRight=0;//棋色
    for(i=1;i<=loopSize;i++)
    {
        if(y-i<0||x-i<0)
            break;
        if(colorLeft==0&&gameMapVec[x-i][y-i]!=0)
        {
            colorLeft=gameMapVec[x-i][y-i];
            emptyNum+=i-1;
        }
        if(colorLeft!=0&&(colorLeft+gameMapVec[x-i][y-i]==0))
            break;
        if(gameMapVec[x-i][y-i]==-1)//白
            whiteNum++;
        else if(gameMapVec[x-i][y-i]==1)//黑
            blackNum++;
    }
    for(j=1;j<=loopSize;j++)
    {
        if(y+j>14||x+j>14)
            break;
        if(colorRight==0&&gameMapVec[x+j][y+j]!=0)
        {
            colorRight=gameMapVec[x+j][y+j];
            emptyNum+=j-1;
        }
        if(colorRight!=0&&(colorRight+gameMapVec[x+j][y+j]==0))
            break;
        if(gameMapVec[x+j][y+j]==-1)//白
            whiteNum++;
        else if(gameMapVec[x+j][y+j]==1)//黑
            blackNum++;
    }

    accumulate(i,j,blackNum,whiteNum,emptyNum,score);
    return score;
}
//计算副对角线得分
int GameState::calculateViceDiagonalScore(int x, int y)
{
    int score=0;
    int whiteNum=0,blackNum=0,emptyNum=0;
    int i=0,j=0;
    int colorLeft=0,colorRight=0;//棋色
    for(i=1;i<=loopSize;i++)
    {
        if(y+i>14||x-i<0)
            break;
        if(colorLeft==0&&gameMapVec[x-i][y+i]!=0)
        {
            colorLeft=gameMapVec[x-i][y+i];
            emptyNum+=i-1;
        }
        if(colorLeft!=0&&(colorLeft+gameMapVec[x-i][y+i]==0))
            break;
        if(gameMapVec[x-i][y+i]==-1)//白
            whiteNum++;
        else if(gameMapVec[x-i][y+i]==1)//黑
            blackNum++;
    }
    for(j=1;j<=loopSize;j++)
    {
        if(y-j<0||x+j>14)
            break;
        if(colorRight==0&&gameMapVec[x+j][y-j]!=0)
        {
            colorRight=gameMapVec[x+j][y-j];
            emptyNum+=j-1;
        }
        if(colorRight!=0&&(colorRight+gameMapVec[x+j][y-j]==0))
            break;
        if(gameMapVec[x+j][y-j]==-1)//白
            whiteNum++;
        else if(gameMapVec[x+j][y-j]==1)//黑
            blackNum++;
    }

    accumulate(i,j,blackNum,whiteNum,emptyNum,score);
    return score;
}
