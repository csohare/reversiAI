#include "myAI.hpp"
#include <ics46/factory/DynamicFactory.hpp>
#include <iostream>



ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, csohare::ohareAI, "OHareAI (Required)");
   
std::pair<int, int> csohare::ohareAI::chooseMove(const OthelloGameState& state)
{      
   std::unique_ptr<OthelloGameState> boardCpy;
   int player = state.isBlackTurn() ? 1 : 2;
   int score;
   int maxScore = -9999;
   std::pair<int, int> nxtMove;

   std::vector<std::pair<int, int>> moves = options( state );
   for(auto move : moves)
   {
        boardCpy = state.clone();
        boardCpy->makeMove(move.first, move.second);
        score = search(boardCpy, 3, player);
        if(score > maxScore)
        {
            nxtMove.first = move.first;
            nxtMove.second = move.second;
            maxScore = score;
        }
   }
   return nxtMove;
   
}


int csohare::ohareAI::search(std::unique_ptr<OthelloGameState>& state, int depth, int player)
{
    
    std::vector<std::pair<int, int>> nextMoves = options( *state );
    int score;
    int maxScore = -999;
    int minScore = 999;

    if(depth == 0)
    {
        return evalState(*state);
    }
    if((state->isWhiteTurn() && player == 2) || (state->isBlackTurn() && player == 1))
    {
        for(auto move : nextMoves)
        {
            std::unique_ptr<OthelloGameState> boardCpy = state->clone();
            boardCpy->makeMove(move.first, move.second);
            score = search(boardCpy, depth -  1, player);
            maxScore = (score > maxScore) ? score : maxScore;
        }
        return maxScore;
    }
    else
    {
        for(auto move : nextMoves)
        {
            std::unique_ptr<OthelloGameState> boardCpy = state->clone();
            boardCpy->makeMove(move.first, move.second);
            score = search(boardCpy, depth -  1, player);
            minScore = (score < minScore) ? score : minScore;
        }
        return minScore;
        
    }
}

std::vector<std::pair<int, int>> csohare::ohareAI::options(const OthelloGameState& state)
{
    std::vector<std::pair<int, int>> posMoves;
    for(int x = 0; x < state.board().width(); ++x)
    {
        for(int y = 0; y < state.board().height(); ++y)
        {
            if(state.isValidMove(x, y)) {posMoves.push_back(std::pair<int, int>(x, y));}
        }
    }
    return posMoves;
}

int csohare::ohareAI::evalState(const OthelloGameState& state)
{
    int whiteCorners = 0;
    int blackCorners = 0;
    OthelloCell topL = state.board().cellAt(0, 0);
    OthelloCell topR = state.board().cellAt(state.board().width() - 1, 0);
    OthelloCell botL = state.board().cellAt(0, state.board().height() - 1);
    OthelloCell botR = state.board().cellAt(state.board().width() - 1, state.board().height() - 1);
    if(topL == OthelloCell::black)  {++blackCorners;}
    if(topL == OthelloCell::white)  {++whiteCorners;}
    if(topR == OthelloCell::black)  {++blackCorners;}
    if(topR == OthelloCell::black)  {++blackCorners;}
    if(botL == OthelloCell::white)  {++whiteCorners;}
    if(botL == OthelloCell::white)  {++whiteCorners;}
    if(botR == OthelloCell::black)  {++blackCorners;}
    if(botR == OthelloCell::white)  {++whiteCorners;}
    return state.isWhiteTurn() ? state.whiteScore() - state.blackScore() + 5* whiteCorners: state.blackScore() - state.whiteScore() + 5 * blackCorners;
    
}
