#pragma once
#include "../Board Representation/gameHistory.h"
#include "PieceMovement.h"


extern Pieces _chessBoard[64];

typedef struct{
    Square _src ;
    Square _dest;
    Pieces _destPiece;
    Pieces _srcPiece;
    uint8_t _prevCastlingState;
    uint8_t _promotion;
    uint8_t _preHalfMove;
    uint8_t _preFullMove;
    char _preEnpassantFile;
    char _side;
} Move;

Move _doMove(Pieces _chessBoard[64], GameState* _state, Square _src , Square _dest  , int _promotion){
    Move move;
    move._promotion=_promotion;
    move._dest=_dest;
    move._src=_src;
    move._destPiece=_chessBoard[_dest];
    move._srcPiece=_chessBoard[_src];
    move._prevCastlingState= _state->_castlingAvailable;
    move._side= _state -> _pieceToMove;
    move._preHalfMove=_state->_numberHalfMoves;
    move._preFullMove=_state->_numberMoves;
    move._preEnpassantFile=_state->_enpassantFile;
    

    __uint64_t mask = 1ULL<<_dest;
    __uint64_t _srcMask = 1ULL << _src;
    __uint64_t _gMask= mask | _srcMask;

    switch(_chessBoard[_src]){
        case WHITE_KING:
            _state->_whiteOccupancy ^= _srcMask | mask;
            _state-> _occupancy ^= _srcMask ;
            _state-> _occupancy |= mask;
            _state->_whiteKing = mask;
            _state->_pieceToMove= 'b';

            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable= _state->_castlingAvailable & 0b00000011;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            break;
        case WHITE_BISHOP:
            _state-> _whiteOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_whiteBishops ^= _gMask;
            _state->_pieceToMove= 'b';

            break;
        case WHITE_PAWN:
            _state-> _whiteOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_whitePawns ^=_gMask;
            _state->_pieceToMove= 'b';

            break;
        case WHITE_KNIGHT:
            _state-> _whiteOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_whiteKnights ^=_gMask;
            _state->_pieceToMove= 'b';

            break;
        case WHITE_QUEEN:
            _state-> _whiteOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_whiteQueens ^=_gMask;
            _state->_pieceToMove= 'b';

            break;

        case WHITE_ROOK:
            _state-> _whiteOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_whiteRooks ^=_gMask;
            _state->_pieceToMove= 'b';

            break;

        case BLACK_BISHOP:
            _state-> _blackOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_blackBishops ^=_gMask;
            _state->_pieceToMove= 'w';

            break;
        case BLACK_KING:
            _state->_blackOccupancy ^= _gMask;
            _state-> _occupancy ^= _srcMask;
            _state-> _occupancy |= mask;
            _state->_blackKing = mask;
            _state->_pieceToMove= 'w';

            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable= _state->_castlingAvailable & 0b00001100;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            
            break;

        case BLACK_KNIGHT:
            _state-> _blackOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_blackKnights ^=_gMask;
            _state->_pieceToMove= 'w';

            break;
        case BLACK_PAWN:
            _state-> _blackOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_blackPawns ^=_gMask;
            _state->_pieceToMove= 'w';

            break;
        case BLACK_QUEEN:
            _state-> _blackOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_blackQueens ^=_gMask;
            _state->_pieceToMove= 'w';

            break;

        case BLACK_ROOK:
            _state-> _blackOccupancy ^= _gMask;
            _state-> _occupancy ^=  _srcMask;
            _state-> _occupancy |= mask;
            _state->_blackRooks ^=_gMask;
            _state->_pieceToMove= 'w';

            break;
        default:
            break;
    }
    
                
    switch(_chessBoard[_dest]){
        case ES:
            _state->_numberHalfMoves+=1;
            if(_chessBoard[_src] == WHITE_PAWN || _chessBoard[_src]== BLACK_PAWN){
                _state->_numberHalfMoves=0;
            }
            break;
        default:
            _state->_numberHalfMoves=0;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_dest]][_dest];

            switch (_chessBoard[_dest])
            {
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    break;
                case WHITE_BISHOP:
                    _state-> _whiteOccupancy ^= mask;
                    _state->_whiteBishops ^= mask;
                    break;
                case WHITE_KNIGHT:
                    _state-> _whiteOccupancy ^= mask;
                    _state->_whiteKnights ^= mask;
                    break;
                case WHITE_PAWN:
                    _state-> _whiteOccupancy ^= mask;
                    _state->_whitePawns ^= mask;
                    break;
                case WHITE_ROOK:
                    _state-> _whiteOccupancy ^= mask;
                    _state->_whiteRooks ^= mask;
                    break;
                case WHITE_QUEEN:
                    _state-> _whiteOccupancy ^= mask;
                    _state->_whiteQueens ^= mask;
                    break;
                default:
                    break;
            }

    }
    
    _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;
    _state->_numberMoves++;
    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_src]][_dest];
    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_src]][_src];

    _chessBoard[_dest]= _chessBoard[_src];
    _chessBoard[_src]=ES;


    if(_promotion && move._side=='b')
    {
        _state->_blackPawns ^=mask;
        switch (_promotion)
        {
        case 1:
            _chessBoard[_dest]=BLACK_QUEEN;
            break;
        case 2:
            _chessBoard[_dest]=BLACK_ROOK;
            break;
        case 3:
            _chessBoard[_dest]=BLACK_BISHOP;
            break;
        case 4:
            _chessBoard[_dest]=BLACK_KNIGHT;

            break;
        default:
            break;
        }
    }
    else if(_promotion && move._side=='w')
    {
        _state->_whitePawns ^=mask;

        switch (_promotion)
        {
        case 1:
            _chessBoard[_dest]=WHITE_QUEEN;
            break;
        case 2:
            _chessBoard[_dest]=WHITE_ROOK;
            break;
        case 3:
            _chessBoard[_dest]=WHITE_BISHOP;
            break;
        case 4:
            _chessBoard[_dest]=WHITE_KNIGHT;
            break;
        default:
            break;
        }
    }
    
    
    
    return move;
}


void _undoMove(Pieces _chessBoard[64],Move* move){

    switch(_chessBoard[move->_src]){
        case WHITE_KING:
            break;
        case WHITE_BISHOP:
            break;
        case WHITE_PAWN:
            break;
        case WHITE_KNIGHT:
            break;
        case WHITE_QUEEN:
            break;
        case WHITE_ROOK:
            break;
        case BLACK_BISHOP:
            break;
        case BLACK_KING:
            break;
        case BLACK_KNIGHT:
            break;
        case BLACK_PAWN:
            break;
        case BLACK_QUEEN:
            break;
        case BLACK_ROOK:

            break;
        default:
            break;
    }
}


__uint64_t* givePositionCountUntilDepth(GameState* _state , int _depth){

    __uint64_t* arr = (__uint64_t *)calloc(sizeof(__uint64_t)*(_depth+1) , 0);
    arr[0]=1;
    
    if(_state->_pieceToMove=='w')
    {
        __uint64_t _attack=0;
        
        __uint64_t _temp=_state->_blackBishops;
        
        while(_temp){
            _attack |= getBishopAttackPseudo((Square)__builtin_ctzll(_temp),_state->_blackOccupancy | _state->_whiteOccupancy);
            _temp &= (_temp-1);
        }

        _temp = _state->_blackRooks;
        while(_temp){
            _attack |= getRookAttackPseudo((Square)__builtin_ctzll(_temp),_state->_blackOccupancy | _state->_whiteOccupancy);
            _temp &= (_temp-1);
        }

        _temp = _state->_blackQueens;
        while(_temp){
            _attack |= getQueenAttackPseudo((Square)__builtin_ctzll(_temp),_state->_blackOccupancy | _state->_whiteOccupancy);
            _temp &= (_temp-1);
        }

        _temp = _state->_blackPawns;
        while(_temp){
            _attack |= getBlackPawnAttackPseudo((Square)__builtin_ctzll(_temp));
            _temp &= (_temp-1);
        }

        _temp= _state-> _blackKing;
        while(_temp){
            _attack |= getKingAttackAndMovementPseudo((Square)__builtin_ctzll(_temp) );
            _temp &= (_temp-1);        
        }

        

    }
    else
    {

    }




    return arr;


}







