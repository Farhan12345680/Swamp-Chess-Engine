#pragma once
#include "../Board Representation/gameHistory.h"
#include "PieceMovement.h"

extern Pieces _chessBoard[64];

typedef struct{
    uint8_t _prevCastlingState;
    uint8_t _promotion;
    uint8_t _preHalfMove;
    uint8_t _preFullMove;
    char _preEnpassantFile;
    char _side;

    __uint8_t _src ;
    __uint8_t _dest;

    Pieces _destPiece;
    Pieces _srcPiece;
    __uint64_t _prevZorbistHash; 
    __uint64_t* _colorOccupancySRC;
    __uint64_t* _pieceOccupancySRC;
    __uint64_t* _colorOccupancyDEST;
    __uint64_t* _pieceOccupancyDEST;


} Move;

typedef struct{
    __uint8_t _prevCastle;
    __uint8_t _currState;
} CastleMove;


Move _doMove(Pieces _chessBoard[64], GameState* _state, 
            Square _src , Square _dest  , int _promotion,
            __uint64_t* restrict _colorOccupancySRC , __uint64_t* restrict _pieceOccupancySRC,
            __uint64_t* restrict _colorOccupancyDEST , __uint64_t* restrict _pieceOccupancyDEST )
{

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
    move._prevZorbistHash=_state->_zobristHash;
    move._colorOccupancySRC=_colorOccupancySRC;
    move._colorOccupancyDEST=_colorOccupancyDEST;
    move._pieceOccupancySRC=_pieceOccupancySRC;
    move._pieceOccupancyDEST=_pieceOccupancyDEST;

    __uint64_t mask = 1ULL<<_dest;
    __uint64_t _srcMask = 1ULL << _src;
    __uint64_t _gMask= mask | _srcMask;
    
    *(_colorOccupancySRC) ^= _gMask ;
    *(_pieceOccupancySRC) ^= _gMask;
    
    _state->_numberMoves+= (_state->_pieceToMove == 'w')?0:1;
    _state->_pieceToMove = (_state->_pieceToMove=='w')?'b':'w';
    _state-> _occupancy ^= _srcMask ;
    _state-> _occupancy |= mask;
    
    if(_pieceOccupancyDEST != NULL){
        *_colorOccupancyDEST ^= mask;
        *_pieceOccupancyDEST ^= mask;
    }

    switch(_chessBoard[_src]){
        case WHITE_KING:
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable= _state->_castlingAvailable & 0b00000011;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            break;

        case WHITE_ROOK:       
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            if(_src == A1)
            {
                _state->_castlingAvailable= _state->_castlingAvailable & 0b00001011;
            }
            else if(_src == H1)
            {
                _state->_castlingAvailable= _state->_castlingAvailable & 0b00000111;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            break;


        case BLACK_KING:
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable= _state->_castlingAvailable & 0b00001100;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            break;
        case BLACK_ROOK:
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            if(_src == A8)
            {
                _state->_castlingAvailable= _state->_castlingAvailable & 0b00001110;
            }
            else if(_src == H8)
            {
                _state->_castlingAvailable= _state->_castlingAvailable & 0b00001101;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
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
                case BLACK_ROOK:
                    _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

                    if(_dest == A8)
                    {
                        _state->_castlingAvailable= _state->_castlingAvailable & 0b00001110;
                    }
                    else if(_dest == H8)
                    {
                        _state->_castlingAvailable= _state->_castlingAvailable & 0b00001101;
                    }
                    _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

                    break;

                case WHITE_ROOK:
                    _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
                    if(_dest == A1)
                    {
                        _state->_castlingAvailable= _state->_castlingAvailable & 0b00001011;
                    }
                    else if(_dest == H1)
                    {
                        _state->_castlingAvailable= _state->_castlingAvailable & 0b00000111;
                    }
                    _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
                    break;

                default:
                    break;
            }

    }
    
    _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;
    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_src]][_dest];
    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_src]][_src];

    _chessBoard[_dest]= _chessBoard[_src];
    _chessBoard[_src]=ES;

    if(_promotion && move._side=='b')
    {
        _state->_blackPawns ^=mask;
        _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
        
        switch (_promotion)
        {
        case 1:
            _chessBoard[_dest]=BLACK_QUEEN;
            _state->_blackQueens ^= mask;
            break;
        case 2:
            _chessBoard[_dest]=BLACK_ROOK;
            _state->_blackRooks ^= mask;
            break;
        case 3:
            _chessBoard[_dest]=BLACK_BISHOP;
            _state->_blackBishops ^= mask;
            break;
        case 4:
            _chessBoard[_dest]=BLACK_KNIGHT;
            _state->_blackKnights ^= mask;
            break;
        default:
            break;
        }
        _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_dest]][_dest];
    }
    else if(_promotion && move._side=='w')
    {
        _state->_whitePawns ^=mask;
        _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_PAWN][_dest];

        switch (_promotion)
        {
        case 1:
            _chessBoard[_dest]=WHITE_QUEEN;
            _state->_whiteQueens ^= mask;
            break;
        case 2:
            _chessBoard[_dest]=WHITE_ROOK;
            _state->_whiteRooks ^= mask;
            break;
        case 3:
            _chessBoard[_dest]=WHITE_BISHOP;
            _state->_whiteBishops ^= mask;
            break;
        case 4:
            _chessBoard[_dest]=WHITE_KNIGHT;
            _state->_whiteKnights ^= mask;
            break;
        default:
            break;
        }
        _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_dest]][_dest];

    }
    
    return move;
}

 
CastleMove _doCastle(Pieces _chessBoard[64] ,GameState* _state , __uint8_t _castleSide){
    CastleMove move;
    move._prevCastle=_state->_castlingAvailable;
    _state->_numberHalfMoves+=1;

    switch(_castleSide)
    {
        case 0b00001000:
            _state->_whiteOccupancy ^= _state->_whiteKing | (1ULL<<H1);
            _state->_occupancy ^= _state->_whiteKing | (1ULL<<H1);

            _state->_whiteOccupancy |= (1ULL<<G1) | (1ULL<<F1);
            
            _state->_occupancy |= (1ULL<<G1) | (1ULL<<F1);
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][G1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][H1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][F1];
            
            _state->_whiteRooks ^= 0b10100000;
            _state->_whiteKing = (1ULL<<G1);
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable &=0b00000011 ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            _chessBoard[H1]=ES;
            _chessBoard[E1]=ES;
            _chessBoard[G1]=WHITE_KING;
            _chessBoard[F1]=WHITE_ROOK;
            _state->_pieceToMove='b';

            break;

        case 0b00000100:
            _state->_whiteOccupancy ^= _state->_whiteKing | (1ULL<<A1);
            _state->_occupancy ^= _state->_whiteKing | (1ULL<<A1);

            _state->_whiteOccupancy |= (1ULL<<C1) |(1ULL<<D1);
            
            _state->_occupancy |= (1ULL<<C1) |(1ULL<<D1);

            _state->_whiteRooks ^= 0b00001001;
            _state->_whiteKing = (1ULL<<C1);
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][C1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][A1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][D1];
            
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable &=0b00000011 ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            _chessBoard[A1]=ES;
            _chessBoard[E1]=ES;
            _chessBoard[C1]=WHITE_KING;
            _chessBoard[D1]=WHITE_ROOK;
            _state->_pieceToMove='b';

            break;

        case 0b00000010:
            _state->_blackOccupancy ^= _state->_blackKing | (1ULL<< H8 );
            _state->_occupancy ^= _state->_blackKing | (1ULL<< H8 );

            _state->_blackOccupancy |= (1ULL<<G8) | (1ULL<< F8 );
            
            _state->_occupancy |= (1ULL<<G8) |(1ULL<<F8);

            _state->_blackRooks ^= (1ULL<< H8) | (1ULL<<F8);
            _state->_blackKing = (1ULL<<G8);
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][G8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][H8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][F8];
            
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable &=0b00001100 ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            _chessBoard[E8]=ES;
            _chessBoard[H8]=ES;
            _chessBoard[G8]=BLACK_KING;
            _chessBoard[F8]=BLACK_ROOK;
            _state->_numberMoves+=1;
            _state->_pieceToMove='w';

            break;

        case 0b00000001:
            _state->_blackOccupancy ^= _state->_blackKing | (1ULL << A8 );
            _state->_occupancy ^= _state->_blackKing | (1ULL << A8 );

            _state->_blackOccupancy |= (1ULL <<C8) | (1ULL << D8 );
            
            _state->_occupancy |= (1ULL<<C8) |(1ULL<<D8);

            _state->_blackRooks ^= (1ULL << A8) | (1ULL<<D8);
            _state->_blackKing = (1ULL<<C8);
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][C8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][A8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][D8];
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable &=0b00001100 ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            
            _chessBoard[E8]=ES;
            _chessBoard[A8]=ES;
            _chessBoard[C8]=BLACK_KING;
            _chessBoard[D8]=BLACK_ROOK;
            _state->_numberMoves+=1;
            _state->_pieceToMove='w';

            break;
    }
    
    move._currState= _castleSide;

    return move;
}

void _undoCastle(Pieces _chessBoard[64] , GameState* _state ,CastleMove* move)
{
    _state->_numberHalfMoves-=1;
    _state->_castlingAvailable= move->_prevCastle;

    switch (move->_currState)
    {
        case 0b00001000:
            _state->_whiteOccupancy ^= (1ULL<<G1) | (1ULL<<F1);
            _state->_occupancy ^= (1ULL<<G1) | (1ULL<<F1);

            _state->_whiteOccupancy |= (1ULL<<H1) | (1ULL<<E1);
            _state->_occupancy |= (1ULL<<H1) | (1ULL<<E1);

            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][G1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][H1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][F1];
            
            _state->_whiteRooks ^= 0b10100000;
            _state->_whiteKing = (1ULL<<E1);
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable =move->_prevCastle ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_pieceToMove='w';

            _chessBoard[H1]=WHITE_ROOK;
            _chessBoard[E1]=WHITE_KING;
            _chessBoard[G1]=ES;
            _chessBoard[F1]=ES;
            break;

        case 0b00000100:
            _state->_whiteOccupancy ^= (1ULL<<C1) | (1ULL<<D1);
            _state->_occupancy ^= (1ULL<<C1) | (1ULL<<D1);

            _state->_whiteOccupancy |= (1ULL<<A1) |(1ULL<<E1);
            
            _state->_occupancy |= (1ULL<<A1) |(1ULL<<E1);

            _state->_whiteRooks ^= 0b00001001;
            _state->_whiteKing = (1ULL<<E1);
            _state->_pieceToMove='w';

            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][C1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][A1];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][D1];
            
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable =move->_prevCastle ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];

            _chessBoard[A1]=WHITE_ROOK;
            _chessBoard[E1]=WHITE_KING;
            _chessBoard[C1]=ES;
            _chessBoard[D1]=ES;
            break;
        case 0b00000010:
            _state->_numberMoves-=1;
            _state->_blackOccupancy ^= (1ULL<<G8) | (1ULL<< F8 );
            _state->_occupancy ^= (1ULL<<G8) | (1ULL<< F8 );
            _state->_blackOccupancy |= (1ULL<<E8) | (1ULL<< H8 );
            
            _state->_occupancy |= (1ULL<<E8) | (1ULL<< H8 );

            _state->_blackRooks ^= (1ULL<< H8) | (1ULL<<F8);
            _state->_blackKing = (1ULL<<E8);
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][G8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][H8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][F8];
            
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable =move->_prevCastle ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_pieceToMove='b';

            _chessBoard[E8]=BLACK_KING;
            _chessBoard[H8]=BLACK_ROOK;
            _chessBoard[G8]=ES;
            _chessBoard[F8]=ES;
            break;
        case 0b00000001:
            _state->_numberMoves-=1;
            _state->_blackOccupancy ^= (1ULL<<C8) |(1ULL<<D8);
            _state->_occupancy ^= (1ULL<<C8) |(1ULL<<D8);

            _state->_blackOccupancy |= (1ULL <<A8) | (1ULL << E8 );
            _state->_pieceToMove='b';

            _state->_occupancy |= (1ULL <<A8) | (1ULL << E8 );

            _state->_blackRooks ^= (1ULL << A8) | (1ULL<<D8);
            _state->_blackKing = (1ULL<<E8);
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][C8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][A8];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][D8];
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            _state->_castlingAvailable =move->_prevCastle ;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[_state->_castlingAvailable];
            
            _chessBoard[E8]=BLACK_KING;
            _chessBoard[A8]=BLACK_ROOK;
            _chessBoard[C8]=ES;
            _chessBoard[D8]=ES;
            break;
    
    default:
        break;
    }
}

void _undoMove(Pieces _chessBoard[64],Move* move ,GameState* _state){

    __uint64_t mask = 1ULL<<move->_dest;
    __uint64_t _srcMask = 1ULL << move->_src;
    __uint64_t _gMask= mask | _srcMask;
    _chessBoard[move->_dest] = move->_destPiece;
    _chessBoard[move->_src]=move->_srcPiece;
    *(move->_colorOccupancySRC) ^= 1<<(move->_src)|1<<(move->_dest);
    *(move->_pieceOccupancySRC) ^= 1<<(move->_src)|1<<(move->_dest);

    if(move->_colorOccupancyDEST != NULL){
        *(move->_colorOccupancyDEST) ^=  1<<(move->_dest);
        *(move->_pieceOccupancySRC) ^= 1<<(move->_dest);
        _state->_occupancy |= 1<<(move->_src)|1<<(move->_dest);
    }else{

        _state->_occupancy |= 1<<(move->_src);
    }
    
    _state->_numberMoves=move->_preFullMove;
    _state->_numberHalfMoves=move->_preHalfMove ;
    _state->_castlingAvailable=move->_prevCastlingState;
    
    _state->_zobristHash=move->_prevZorbistHash;
    _state->_pieceToMove=move->_side;


    if(move->_promotion && move->_side=='b')
    {
        _state->_blackPawns ^=mask;

        switch (move->_promotion)
        {
        case 1:
            _state->_blackQueens^=mask;
            break;
        case 2:
            _state->_blackRooks^=mask;
            break;
        case 3:
            _state->_blackBishops^=mask;
            break;
        case 4:
            _state->_blackKnights^=mask;
            break;

        default:
            break;
        }
    }
    else if(move->_promotion && move->_side=='w')
    {
        _state->_whitePawns ^=mask;

        switch (move->_promotion)
        {
        case 1:
            _state->_whiteQueens^=mask;
            break;
        case 2:
            _state->_whiteRooks^=mask;
            break;
        case 3:
            _state->_whiteBishops^=mask;
            break;
        case 4:
            _state->_whiteKnights^=mask;
            break;

        default:
            break;
        }

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







