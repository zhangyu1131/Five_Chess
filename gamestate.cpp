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
    gameStatus=PLAYING;
    playerFlag=true;
    qDebug()<<"start game!";
}
