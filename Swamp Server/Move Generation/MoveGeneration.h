#pragma once
#include "../Board Representation/gameHistory.h"
#include "PieceMovement.h"


extern Pieces _chessBoard[64];


void _doMove(Square _src , Square _dest , int CASTLING_STATE){

    switch(_chessBoard[_src]){
        case WHITE_KING:
            __uint64_t mask = 1ULL<<_dest;
            _gloabl_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;

            break;

        case WHITE_BISHOP:
            __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case WHITE_PAWN:
            __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case WHITE_KNIGHT:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case WHITE_QUEEN:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case WHITE_ROOK:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case BLACK_BISHOP:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case BLACK_KING:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case BLACK_KNIGHT:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case BLACK_PAWN:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case BLACK_QUEEN:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        case BLACK_ROOK:
                        __uint64_t mask = 1ULL<<_dest;
            _state->_whiteOccupancy ^= _state->_whiteKing | mask;
            _state-> _occupancy ^= _state->_whiteKing | mask;

            _state->_whiteKing ^= (1ULL << _src)| mask;
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_dest];
            _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][_src];

            _state->_zobristHash ^= _globalZorbistHashing._zorbistSideToMove ;


            switch(_capturedPiece){
                case BLACK_BISHOP:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackBishops ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_BISHOP][_dest];
                    break;
                case BLACK_KNIGHT:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackKnights ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_KNIGHT][_dest];
                    break;
                case BLACK_PAWN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackPawns ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_PAWN][_dest];
                    break;
                case BLACK_ROOK:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackRooks ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][_dest];

                    break;
                case BLACK_QUEEN:
                    _state->_blackOccupancy ^= mask;
                    _state->_blackQueens ^= mask;
                    _state->_numberHalfMoves=0;
                    _state->_zobristHash ^= _globalZorbistHashing._zorbistPieces[BLACK_QUEEN][_dest];
                    break;
                default:
                    break;
            }
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[CASTLING_STATE];
            _state->_castlingAvailable=0;
            _state->_zobristHash ^=_globalZorbistHashing._zorbistCastlingNums[0];
            _state->_numberMoves++;
            
            break;
        default:
            break;
    }

}



void _undoMove(GameState* _state , Square _src , Square _dest , Pieces PIECE){

    switch(PIECE){
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







