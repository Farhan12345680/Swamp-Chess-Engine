#pragma once
#include "Common.h";
#include "bitboards.h";
#include "FEN_parser.h";

#define nullptr NULL

typedef struct GameHistoryNode
{
    GameState _state;

    GameHistoryNode *_prev;
    GameHistoryNode *_next;

} GameHistoryNode;

typedef struct
{
    int _length;
    GameHistoryNode *_latest;

} GameStack;


GameHistoryNode* createHistoryNode(GameState* GAME_STATE)
{
    GameHistoryNode* _g=(GameHistoryNode*)malloc(sizeof(GameHistoryNode)*1);
    
    _g->_state=*GAME_STATE;
    _g->_next=nullptr;
    _g->_prev=nullptr;

    return _g;
}

GameStack createGameStack(){
    GameStack _g;
    _g._length=0;
    _g._latest=nullptr;

    return _g;

}


void pushinStack(GameStack* _historyStack, GameHistoryNode* HISTORY_NODE)
{
    _historyStack->_length+=1;
    if(_historyStack->_latest!=nullptr){

        HISTORY_NODE->_prev=_historyStack->_latest;
        _historyStack->_latest->_next=HISTORY_NODE;

        _historyStack->_latest=HISTORY_NODE;

    }else{
        _historyStack->_latest=HISTORY_NODE;
    }
    
}

void popfromStack(GameStack* _historyStack)
{
    if(_historyStack->_length==0){
        return;
    }
    _historyStack->_length-=1;
    GameHistoryNode* curr = _historyStack->_latest;
    
    _historyStack->_latest=_historyStack->_latest->_prev;
        free(curr);

    if(_historyStack->_latest ==nullptr){
        return;
    }
    _historyStack->_latest->_next = nullptr;
}


void emptyStack(GameStack* _history){
    GameStack _historyStack= *_history;

    while(_historyStack._latest!=nullptr){
        _historyStack._length-=1;
        GameHistoryNode* curr = _historyStack._latest;

        _historyStack._latest= _historyStack._latest->_prev;
        free(curr);
    }
} 