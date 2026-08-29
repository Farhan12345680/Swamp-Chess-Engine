#include "../1. core/core.h"
#include <string.h>
#include <threads.h>


#define KEEP_IF_BIT_SET(value, bit) \
    ((value) & -(((value) >> (bit)) & 1ULL))

ZorbistKeys _globalZorbistHashing={};
thread_local uint64_t GAME_STATE[24]={0};
thread_local uint8_t _chessBoard[64]={0};


uint64_t knightTable[64];
uint64_t whitePawnTable[64];
uint64_t whitePawnMovementTable[64];
uint64_t blackPawnTable[64];
uint64_t blackPawnMovementTable[64];
uint64_t kingTable[64];
uint64_t bishopAttacks[64][512];
uint64_t rookAttacks[64][4096];
uint64_t rookMask[64];
uint64_t bishopMask[64];
uint64_t fileMask[64];
uint64_t cornerfileMask[64];


const __uint64_t pawnEnpassantMask[2]={
                                 0b0000000000000000000000001111111100000000000000000000000000000000,
                                 0b0000000000000000000000000000000011111111000000000000000000000000};
int helperArr[13][2];

const uint64_t rookMagicNumbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL};

const uint64_t bishopMagicNumbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL

};

const int bishopRelevantBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6};

const int rookRelevantBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// --------------------------------------------
// ----------- basic PIECE attack generation --
// --------------------------------------------

static inline uint64_t getBishopAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    occupancy &= bishopMask[square];
    uint64_t _idx = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);
    return bishopAttacks[square][_idx] & (~sameSideOccupancy);
}

static inline uint64_t getRookAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    occupancy &= rookMask[square];
    uint64_t _idx = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);
    return rookAttacks[square][_idx] & (~sameSideOccupancy);
}


static inline uint64_t getQueenAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return getBishopAttack(square, occupancy, sameSideOccupancy) | getRookAttack(square, occupancy, sameSideOccupancy);
}

static inline uint64_t getKnightAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return knightTable[square] & ~(sameSideOccupancy);
}

static inline uint64_t getKingAttackAndMovement(Square square, uint64_t sameSideOccupancy)
{
    return kingTable[square] & ~(sameSideOccupancy) ;
}

static inline uint64_t getWhitePawnAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return (whitePawnTable[square] & ~sameSideOccupancy) & occupancy;
}

static inline uint64_t getBlackPawnAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return (blackPawnTable[square] & ~sameSideOccupancy) & occupancy;
}


// ---------------------------------------------------
// -------------- move getting -----------------------
// ---------------------------------------------------

//do move

static inline void doMove(
    uint8_t _src, uint8_t _dest, uint8_t _promotion,
    uint8_t _colorOccupancySRC, uint8_t _pieceOccupancySRC,
    uint8_t _colorOccupancyDEST, uint8_t _pieceOccupancyDEST,MoveList*    _moveList)
{
    uint16_t indiMove=0;
        indiMove |= _src;
        indiMove |=(_dest<<6);
        indiMove |=(_promotion<<12);

    _moveList->moves[_moveList->index++] = indiMove;

}

static inline void doSpecialMove(
    uint8_t _src, uint8_t _dest, uint8_t _promotion,
    uint8_t _colorOccupancySRC, uint8_t _pieceOccupancySRC,
    uint8_t _colorOccupancyDEST, uint8_t _pieceOccupancyDEST,MoveList*    _moveList)
{

    uint64_t mask = 1ULL << _dest;
    uint64_t _srcMask = 1ULL << _src;
    uint64_t _gMask = mask | _srcMask;

    GAME_STATE[_colorOccupancySRC] ^= _gMask;
    GAME_STATE[_pieceOccupancySRC] ^= _gMask;
    GAME_STATE[_colorOccupancyDEST] ^= mask;
    GAME_STATE[_pieceOccupancyDEST] ^= mask;
    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];


    checkKingInCheckAndAddMove(GAME_STATE[SIDE] , GAME_STATE[SIDE]^6 ,(Square)_src ,(Square) _dest ,_promotion,_moveList );


    GAME_STATE[_colorOccupancySRC] ^= _gMask;
    GAME_STATE[_pieceOccupancySRC] ^= _gMask;
    GAME_STATE[_colorOccupancyDEST] ^= mask;
    GAME_STATE[_pieceOccupancyDEST] ^= mask;
    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];

}



// only keeping this function because this function will be used in enpassant move making && castling
static inline bool isTheKingInCheck(int curr, int opp )
{

    uint64_t occ = GAME_STATE[TOTAL_OCCUPANCY];
    uint64_t ownOcc =GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    Square kingSq = (Square)__builtin_ctzll(GAME_STATE[curr]);

    if (knightTable[kingSq] & GAME_STATE[opp + 4])
        return true;

    if (opp == WHITE_KING_OCCUPANCY)
    {
        if (blackPawnTable[kingSq] & GAME_STATE[WHITE_PAWN_OCCUPANCY])
            return true;
    }
    else
    {

        if (whitePawnTable[kingSq] & GAME_STATE[BLACK_PAWN_OCCUPANCY])
            return true;
    }


    if (kingTable[kingSq] & GAME_STATE[opp])
        return true;


    if (getBishopAttack(kingSq, occ, ownOcc) &
        (GAME_STATE[opp + 3] | GAME_STATE[opp + 1]))
        return true;


    if (getRookAttack(kingSq, occ, ownOcc) &
        (GAME_STATE[opp + 2] | GAME_STATE[opp + 1]))
        return true;

    return false;
}

static inline int isKingInCheckCounter(int curr, int opp )
{

    int counter=0;
    uint64_t occ = GAME_STATE[TOTAL_OCCUPANCY];
    uint64_t ownOcc = GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    Square kingSq = (Square)__builtin_ctzll(GAME_STATE[curr]);


    counter+= __builtin_popcountl(knightTable[kingSq] & GAME_STATE[opp + 4]);

    if (opp == WHITE_KING_OCCUPANCY)
    {
        counter+= __builtin_popcountl(blackPawnTable[kingSq] & GAME_STATE[WHITE_PAWN_OCCUPANCY]);

    }
    else
    {
        counter+= __builtin_popcountl(whitePawnTable[kingSq] & GAME_STATE[BLACK_PAWN_OCCUPANCY]);

    }

    counter+= __builtin_popcountl(kingTable[kingSq] & GAME_STATE[opp]);



    counter += __builtin_popcountl(getBishopAttack(kingSq, occ, ownOcc) & (GAME_STATE[opp + 3] | GAME_STATE[opp + 1]));
    counter += __builtin_popcountl(getRookAttack(kingSq, occ, ownOcc) & (GAME_STATE[opp + 2] | GAME_STATE[opp + 1]));


    return counter;
}

static inline Square kingInCheckByWhichSquare(int curr , int opp)
{
    uint64_t occ = GAME_STATE[TOTAL_OCCUPANCY];
    uint64_t ownOcc =GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    Square kingSq = (Square)__builtin_ctzll(GAME_STATE[curr]);

    if (knightTable[kingSq] & GAME_STATE[opp + 4])
        return (Square) __builtin_ctzll(knightTable[kingSq] & GAME_STATE[opp + 4]);

    if (opp == WHITE_KING_OCCUPANCY)
    {
        unsigned long temp = blackPawnTable[kingSq] & GAME_STATE[WHITE_PAWN_OCCUPANCY];
        if (temp)
            return (Square)__builtin_ctzll(temp);
    }
    else
    {

        unsigned long temp = whitePawnTable[kingSq] & GAME_STATE[BLACK_PAWN_OCCUPANCY];
        if (temp)
            return (Square)__builtin_ctzll(temp);
    }

    unsigned long temp = kingTable[kingSq] & GAME_STATE[opp];
    if (temp)
        return (Square)__builtin_ctzll(temp);

    temp = getBishopAttack(kingSq, occ, ownOcc) & (GAME_STATE[opp + 3] | GAME_STATE[opp + 1]) ;
    if (temp)
        return (Square)__builtin_ctzll(temp);



    temp = getRookAttack(kingSq, occ, ownOcc) & (GAME_STATE[opp + 2] | GAME_STATE[opp + 1]) ;
    if (temp)
        return (Square)__builtin_ctzll(temp);



    return ES;
}


static inline void addMove(Square src,Square dest , int _promotion , MoveList* _moveList)
{
        uint16_t indiMove=0;
        indiMove |= src;
        indiMove |=(dest<<6);
        indiMove |=(_promotion<<12);

        _moveList->moves[_moveList->index++] = indiMove;
}


inline void  checkKingInCheckAndAddMove(int curr , int opp
                                    ,Square src , Square dest,int _promotion
                                    ,MoveList*    _moveList )
{

    if ( !isTheKingInCheck(curr , opp))
    {
        uint16_t indiMove=0;
        indiMove |= src;
        indiMove |=(dest<<6);
        indiMove |=(_promotion<<12);

        _moveList->moves[_moveList->index++] = indiMove;
    }

}


static uint64_t getAttackBitBoard(int curr ,int  opp)
{
    uint64_t attacks = 0;
    uint64_t temp;
    uint64_t sameSideOccupancy = GAME_STATE[(opp >> 2) + WHITE_OCCUPANCY];
    uint64_t oppositeOccupancy = GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    temp =GAME_STATE[opp + 3];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getBishopAttack(sq,GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 2];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getRookAttack(sq,GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp =GAME_STATE[opp + 1];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getQueenAttack(sq,GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp =GAME_STATE[opp + 4];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getKnightAttack(sq,GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp =GAME_STATE[opp + 5];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        if (opp == 0)
        {
            attacks |= whitePawnTable[sq] & ~sameSideOccupancy;
        }
        else
        {
            attacks |= blackPawnTable[sq] & ~sameSideOccupancy;
        }
    }
    temp =  GAME_STATE[opp];
    Square sq = __builtin_ctzll(temp);

    attacks |= kingTable[sq] & ~sameSideOccupancy;

    return attacks;
}




static inline Square findEnpassantSquare(Square PAWN_SQUARE, Square enPassantSquare)
{
    if (enPassantSquare == NS) return NS;


    switch (GAME_STATE[SIDE])
    {
    case 0:
        if ((PAWN_SQUARE % 8) != 0 && enPassantSquare == PAWN_SQUARE + 7)
            return (Square)(PAWN_SQUARE - 1);
        if ((PAWN_SQUARE % 8) != 7 && enPassantSquare == PAWN_SQUARE + 9)
            return (Square)(PAWN_SQUARE + 1);
        break;

    case 6:
        if ((PAWN_SQUARE % 8) != 7 && enPassantSquare == PAWN_SQUARE - 7)
            return (Square)(PAWN_SQUARE + 1);
        if ((PAWN_SQUARE % 8) != 0 && enPassantSquare == PAWN_SQUARE - 9)
            return (Square)(PAWN_SQUARE - 1);
        break;
    }
    return NS;
}

static inline void doEnpassant( Square _pawnSrc, Square _pawnDest, Square _originDist ,MoveList*    _moveList )
{
    uint64_t mask = 1ULL << _pawnDest;
    uint64_t _srcMask = 1ULL << _pawnSrc;
    uint64_t _originDestMask = 1ULL << _originDist;
    uint64_t _gMask = mask | _srcMask;

    int srcPiece = helperArr[_chessBoard[_pawnSrc]][0];
    int srcColor = helperArr[_chessBoard[_pawnSrc]][1];
    int destPiece=helperArr[_chessBoard[_pawnDest]][0];
    int destColor= helperArr[_chessBoard[_pawnDest]][1];
    int enDist =  helperArr[_chessBoard[_originDist]][0];
    int enColorDist =  helperArr[_chessBoard[_originDist]][1];

    GAME_STATE[srcPiece] ^= _gMask;
    GAME_STATE[srcColor] ^= _gMask;
    GAME_STATE[destPiece] ^= mask;
    GAME_STATE[destColor] ^= mask;
    GAME_STATE[enColorDist] ^= _originDestMask;
    GAME_STATE[enDist] ^= _originDestMask;

    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];


    checkKingInCheckAndAddMove(GAME_STATE[SIDE] , GAME_STATE[SIDE]^6 ,_pawnSrc ,_pawnDest ,0,_moveList);


    GAME_STATE[srcPiece] ^= _gMask;
    GAME_STATE[srcColor] ^= _gMask;
    GAME_STATE[destPiece] ^= mask;
    GAME_STATE[destColor] ^= mask;
    GAME_STATE[enColorDist] ^= _originDestMask;
    GAME_STATE[enDist] ^= _originDestMask;

   GAME_STATE[TOTAL_OCCUPANCY] =GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];
}

static inline void generateCastlingList(MoveList *    _moveList)
{
    if (GAME_STATE[SIDE] == 0)
    {
        uint64_t attack = getAttackBitBoard(0, 6);


        if (!(attack &  GAME_STATE[WHITE_KING_OCCUPANCY]) &&
            !(attack & (1ULL << F1)) &&
            !(attack & (1ULL << G1)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00001000))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << F1) | (1ULL << G1))))
            {


                addMove(E1 , G1 ,0 ,_moveList);

            }
        }


        if (!(attack &GAME_STATE[WHITE_KING_OCCUPANCY]) &&
            !(attack & (1ULL << C1)) &&
            !(attack & (1ULL << D1)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00000100))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))))
            {


                addMove(E1 , C1 , 0 ,_moveList);

            }
        }
    }
    else
    {
        uint64_t attack = getAttackBitBoard(6, 0);


        if (!(attack &GAME_STATE[BLACK_KING_OCCUPANCY]) &&
            !(attack & (1ULL << F8)) &&
            !(attack & (1ULL << G8)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00000010))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << F8) | (1ULL << G8))))
            {

                addMove(E8 , G8 , 0 , _moveList);

            }
        }


        if (!(attack &GAME_STATE[BLACK_KING_OCCUPANCY]) &&
            !(attack & (1ULL << C8)) &&
            !(attack & (1ULL << D8)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00000001))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))))
            {

                addMove(E8 , C8 , 0 , _moveList);



            }
        }
    }
}

static inline void generateBishopMoveList(MoveList *    _moveList  , uint64_t pieces , uint64_t attackVector)
{

    const int side =GAME_STATE[SIDE];
    __uint64_t _temp =GAME_STATE[side+3] & (~pieces);
    __uint64_t _srcPieceOccupancy = side+3;
    __uint64_t totalOccupancy =GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = (side>>2) + WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getBishopAttack(src, totalOccupancy,GAME_STATE[_srcColorOccupancy]) & attackVector;
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];

            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}


static inline void generateRookMoveList(MoveList *    _moveList ,uint64_t pinnedPieces , uint64_t attackVector)
{

    const int side =GAME_STATE[SIDE];
    __uint64_t _temp =GAME_STATE[side+2] & ~pinnedPieces;
    __uint64_t _srcPieceOccupancy = side+2;
    __uint64_t totalOccupancy =GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = (side>>2) + WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getRookAttack(src, totalOccupancy,GAME_STATE[_srcColorOccupancy]) & attackVector;
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}

static inline void generateQueenMoveList(MoveList *    _moveList, uint64_t pinnedPieces , uint64_t attackVector)
{
    const int side =GAME_STATE[SIDE];
    __uint64_t _temp =GAME_STATE[side+1] & ~pinnedPieces;
    __uint64_t _srcPieceOccupancy = side+1;
    __uint64_t totalOccupancy =GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = (side>>2) + WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getQueenAttack(src, totalOccupancy,GAME_STATE[_srcColorOccupancy]) & attackVector;
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}

static inline void generateKnightMoveList(MoveList *    _moveList , uint64_t pinnedPieces , uint64_t attackVector)
{

    const int side =GAME_STATE[SIDE];
    __uint64_t _temp =GAME_STATE[side+4] & (~pinnedPieces);
    __uint64_t _srcPieceOccupancy = side+4;
    __uint64_t totalOccupancy =GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = (side>>2) + WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getKnightAttack(src, totalOccupancy,GAME_STATE[_srcColorOccupancy]) & attackVector;
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}

static inline void generateKingMoveList(MoveList *   _moveList )
{
    int side =GAME_STATE[SIDE];

        Square src = (Square)__builtin_ctzll(GAME_STATE[side]);


        uint64_t attacks =
            getKingAttackAndMovement(src,
                               GAME_STATE[ (side>>2)+WHITE_OCCUPANCY]) ;

        while (attacks)
        {
            Square dest = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];

            doMove(
                src,
                dest,
                0,
                (side>>2) +WHITE_OCCUPANCY,
                side,
                capturedColor,
                capturedPiece ,_moveList);
        }


    generateCastlingList(_moveList);
}

static inline void generateKingMoveListSpecial(MoveList *   _moveList )
{
    int side =GAME_STATE[SIDE];

        Square src = (Square)__builtin_ctzll(GAME_STATE[side]);


        uint64_t attacks =
            getKingAttackAndMovement(src,
                               GAME_STATE[ (side>>2)+WHITE_OCCUPANCY])  ;

        while (attacks)
        {
            Square dest = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];

            doSpecialMove(
                src,
                dest,
                0,
                (side>>2) +WHITE_OCCUPANCY,
                side,
                capturedColor,
                capturedPiece ,_moveList);
        }


    generateCastlingList(_moveList);
}

static inline void generateWhitePawnMoveList(MoveList *   _moveList)
{

    __uint64_t _temp =GAME_STATE[WHITE_PAWN];
    __uint64_t _srcPieceOccupancy = WHITE_PAWN;
    __uint64_t totalOccupancy =GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getWhitePawnAttack(src, totalOccupancy,GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }


}


static inline void generateEnpassantMoves(MoveList *   _moveList)
{
    Square enPassant = (Square)GAME_STATE[ENPASSANT_SQUARE];
    if (enPassant == NS) return;

    int curr =GAME_STATE[SIDE] == 0 ? 0 : 6;
    int opp =GAME_STATE[SIDE] == 0 ? 6 : 0;
    uint64_t pawns =GAME_STATE[curr + 5];
    pawns &= pawnEnpassantMask[GAME_STATE[SIDE]>>2] ;
    uint64_t dest1= 1ULL << enPassant;

    pawns &= ((dest1>>7) |(dest1 >>9) |(dest1 <<7)|(dest1<<9));

    while (pawns)
    {
        int index = __builtin_ctzll(pawns);
        pawns &= pawns - 1;

        Square epSquare = findEnpassantSquare((Square)index, enPassant);
        if (epSquare != NS)
        {
            doEnpassant((Square)index, enPassant, epSquare , _moveList);
        }
    }
}


static inline void generatePawnMovements(MoveList *_moveList , uint64_t pinnedPieces,uint64_t attackVector)
{

    generateEnpassantMoves(_moveList);

    int side =GAME_STATE[SIDE];

    const uint64_t FILE_A = 0x0101010101010101ULL;
    const uint64_t FILE_H = 0x8080808080808080ULL;

    const uint64_t RANK_1 = 0x00000000000000FFULL;
    const uint64_t RANK_2 = 0x000000000000FF00ULL;
    const uint64_t RANK_7 = 0x00FF000000000000ULL;
    const uint64_t RANK_8 = 0xFF00000000000000ULL;

    const uint64_t occupied =GAME_STATE[TOTAL_OCCUPANCY];
    const uint64_t empty = ~occupied;

    if (side == 0)
    {
        const uint64_t pawns =GAME_STATE[WHITE_PAWN_OCCUPANCY] & ~pinnedPieces;
        const uint64_t enemy =GAME_STATE[BLACK_OCCUPANCY];
        const uint64_t own =GAME_STATE[WHITE_OCCUPANCY];

        uint64_t singlePush =
            (pawns << 8) & empty & attackVector;

        uint64_t doublePush =
            (((pawns & RANK_2) << 8) & empty) << 8 & empty & attackVector;

        uint64_t leftCapture =
            ((pawns & ~FILE_A) << 7) & enemy & attackVector;

        uint64_t rightCapture =
            ((pawns & ~FILE_H) << 9) & enemy & attackVector;

        uint64_t moves = singlePush & ~RANK_8 ;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest - 8, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = doublePush;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest - 16, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = leftCapture & ~RANK_8;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(dest - 7, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = rightCapture & ~RANK_8;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(dest - 9, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = singlePush & RANK_8;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;


                uint16_t move =
                    (uint16_t)(dest - 8) |
                    (uint16_t)(dest << 6);

                for (int prom = 1; prom <= 4; prom++)
                {
                    _moveList->moves[_moveList->index++] =
                        move | (uint16_t)(prom << 12);
                }

        }

        moves = leftCapture & RANK_8;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 7;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];




                uint16_t move =
                    (uint16_t)src |
                    (uint16_t)(dest << 6);

                for (int prom = 1; prom <= 4; prom++)
                {
                    _moveList->moves[_moveList->index++] =
                        move | (uint16_t)(prom << 12);
                }

        }

        moves = rightCapture & RANK_8;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 9;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];



                uint16_t move =
                    (uint16_t)src |
                    (uint16_t)(dest << 6);

                for (int prom = 1; prom <= 4; prom++)
                {
                    _moveList->moves[_moveList->index++] =
                        move | (uint16_t)(prom << 12);
                }

        }
    }
    else
    {
        const uint64_t pawns =GAME_STATE[BLACK_PAWN_OCCUPANCY] & ~pinnedPieces;
        const uint64_t enemy =GAME_STATE[WHITE_OCCUPANCY];
        const uint64_t own =GAME_STATE[BLACK_OCCUPANCY];

        uint64_t singlePush =
            (pawns >> 8) & empty & attackVector;

        uint64_t doublePush =
            (((pawns & RANK_7) >> 8) & empty) >> 8 & empty & attackVector;

        uint64_t leftCapture =
            ((pawns & ~FILE_A) >> 9) & enemy & attackVector;

        uint64_t rightCapture =
            ((pawns & ~FILE_H) >> 7) & enemy & attackVector;

        uint64_t moves = singlePush & ~RANK_1 & attackVector;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest + 8, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = doublePush;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest + 16, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = leftCapture & ~RANK_1;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(dest + 9, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = rightCapture & ~RANK_1;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(dest + 7, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = singlePush & RANK_1;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;



                uint16_t move =
                    (uint16_t)(dest + 8) |
                    (uint16_t)(dest << 6);

                for (int prom = 1; prom <= 4; prom++)
                {
                    _moveList->moves[_moveList->index++] =
                        move | (uint16_t)(prom << 12);
                }


        }

        moves = leftCapture & RANK_1;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 9;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];



                uint16_t move =
                    (uint16_t)src |
                    (uint16_t)(dest << 6);

                for (int prom = 1; prom <= 4; prom++)
                {
                    _moveList->moves[_moveList->index++] =
                        move | (uint16_t)(prom << 12);
                }

        }

        moves = rightCapture & RANK_1;

        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 7;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];


                uint16_t move =
                    (uint16_t)src |
                    (uint16_t)(dest << 6);

                for (int prom = 1; prom <= 4; prom++)
                {
                    _moveList->moves[_moveList->index++] =
                        move | (uint16_t)(prom << 12);
                }


        }
    }
}



typedef struct pinnedPieces
{
    uint64_t north;
    uint64_t south;
    uint64_t west;
    uint64_t east;
    uint64_t north_west;
    uint64_t south_west;
    uint64_t south_east;
    uint64_t north_east;

    uint64_t northMask;
    uint64_t southMask;
    uint64_t westMask;
    uint64_t eastMask;
    uint64_t north_westMask;
    uint64_t south_westMask;
    uint64_t south_eastMask;
    uint64_t north_eastMask;

    uint64_t northMask1;
    uint64_t southMask1;
    uint64_t westMask1;
    uint64_t eastMask1;
    uint64_t north_westMask1;
    uint64_t south_westMask1;
    uint64_t south_eastMask1;
    uint64_t north_eastMask1;

} PinnedPieces;


PinnedPieces findPinnedPiecesBitMap(int curr)
{
    int opp = curr ^ 6;

    PinnedPieces pieces;
    memset(&pieces, 0, sizeof(PinnedPieces));

    Square kingSq = __builtin_ctzl(GAME_STATE[curr]);

    int file = kingSq % 8;
    int row  = kingSq / 8;




    for (int r = row + 1; r < 8; r++)
    {
        int temp = r * 8 + file;


        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5  )
        {
            if (!pieces.north && r!=7)
            {
                pieces.north |= 1ULL << temp;
            }
            else
            {
                pieces.north = 0;
                pieces.northMask = 0;
                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] >= opp + 3 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.north = 0;
            pieces.northMask = 0;
            break;
        }


        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 2)
        {
            pieces.northMask |= 1ULL << temp;
            break;
        }

        pieces.northMask |= 1ULL << temp;


        if (r == 7)
        {
            pieces.north = 0;
            pieces.northMask = 0;
            break;
        }
    }




    for (int r = row - 1; r >= 0; r--)
    {
        int temp = r * 8 + file;

        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5 )
        {
            if (!pieces.south && r != 0)
            {
                pieces.south |= 1ULL << temp;

            }
            else
            {
                pieces.south = 0;
                pieces.southMask = 0;

                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] >= opp + 3 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.south = 0;
            pieces.southMask = 0;
            break;
        }

        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 2)
        {
            pieces.southMask |= 1ULL << temp;
            pieces.southMask1 |= 1ULL << temp;
            break;
        }

        pieces.southMask |= 1ULL << temp;

        if (r == 0)
        {
            pieces.south = 0;
            pieces.southMask = 0;
            break;
        }
    }




    for (int c = file + 1; c < 8; c++)
    {
        int temp = row * 8 + c;

        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5 )
        {
            if (!pieces.east && c!=7)
            {
                pieces.east |= 1ULL << temp;

            }
            else
            {
                pieces.east = 0;
                pieces.eastMask = 0;
                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] >= opp + 3 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.east = 0;
            pieces.eastMask = 0;

            break;
        }


        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 2)
        {
            pieces.eastMask |= 1ULL << temp;

            break;
        }

        // Empty
        pieces.eastMask |= 1ULL << temp;


        if (c == 7)
        {
            pieces.east = 0;
            pieces.eastMask = 0;
            break;
        }
    }




    for (int c = file - 1; c >= 0; c--)
    {
        int temp = row * 8 + c;

        // Friendly piece
        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5 )
        {
            if (!pieces.west &&
            c!=0)
            {
                pieces.west |= 1ULL << temp;

            }
            else
            {
                pieces.west = 0;
                pieces.westMask = 0;

                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] >= opp + 3 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.west = 0;
            pieces.westMask = 0;

            break;
        }

        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 2)
        {
            pieces.westMask |= 1ULL << temp;
            break;
        }

        pieces.westMask |= 1ULL << temp;

        if (c == 0)
        {
            pieces.west = 0;
            pieces.westMask = 0;
            break;
        }
    }



    for (int r = row + 1, c = file - 1;
         r < 8 && c >= 0;
         r++, c--)
    {
        int temp = r * 8 + c;


        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5 )
        {
            if (!pieces.north_west && r!=7 && c!=0)
            {
                pieces.north_west |= 1ULL << temp;
            }
            else
            {
                pieces.north_west = 0;
                pieces.north_westMask = 0;
                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] == opp + 2) ||
                 (_chessBoard[temp] >= opp + 4 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.north_west = 0;
            pieces.north_westMask = 0;

            break;
        }

        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 3)
        {
            pieces.north_westMask |= 1ULL << temp;
            break;
        }


        pieces.north_westMask |= 1ULL << temp;
        pieces.north_westMask1 |= 1ULL << temp;

        if (r == 7 || c == 0)
        {
            pieces.north_west = 0;
            pieces.north_westMask = 0;
            break;
        }
    }




    for (int r = row + 1, c = file + 1;
         r < 8 && c < 8;
         r++, c++)
    {
        int temp = r * 8 + c;

        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5
            )
        {
            if (!pieces.north_east && r!=7 && c!=7)
            {
                pieces.north_east |= 1ULL << temp;
            }
            else
            {
                pieces.north_east = 0;
                pieces.north_eastMask = 0;
                break;
            }

            continue;
        }


        else if ((_chessBoard[temp] == opp + 2) ||
                 (_chessBoard[temp] >= opp + 4 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.north_east = 0;
            pieces.north_eastMask = 0;
            break;
        }


        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 3)
        {
            pieces.north_eastMask |= 1ULL << temp;
            break;
        }


        pieces.north_eastMask |= 1ULL << temp;


        if (r == 7 || c == 7)
        {
            pieces.north_east = 0;
            pieces.north_eastMask = 0;
            break;
        }
    }




    for (int r = row - 1, c = file - 1;
         r >= 0 && c >= 0;
         r--, c--)
    {
        int temp = r * 8 + c;

        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5
            )
        {
            if (!pieces.south_west && r!=0 && c!=0)
            {
                pieces.south_west |= 1ULL << temp;
            }
            else
            {
                pieces.south_west = 0;
                pieces.south_westMask = 0;

                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] == opp + 2) ||
                 (_chessBoard[temp] >= opp + 4 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.south_west = 0;
            pieces.south_westMask = 0;

            break;
        }

        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 3)
        {
            pieces.south_westMask |= 1ULL << temp;

            break;
        }

        pieces.south_westMask |= 1ULL << temp;


        if (r == 0 || c == 0)
        {
            pieces.south_west = 0;
            pieces.south_westMask = 0;
            break;
        }
    }



    for (int r = row - 1, c = file + 1;
         r >= 0 && c < 8;
         r--, c++)
    {
        int temp = r * 8 + c;


        if (_chessBoard[temp] >= curr + 1 &&
            _chessBoard[temp] <= curr + 5
            )
        {
            if (!pieces.south_east && r!=0 && c!=7)
            {
                pieces.south_east |= 1ULL << temp;


            }
            else
            {
                pieces.south_east = 0;
                pieces.south_eastMask = 0;
                break;
            }

            continue;
        }

        else if ((_chessBoard[temp] == opp + 2) ||
                 (_chessBoard[temp] >= opp + 4 &&
                  _chessBoard[temp] <= opp + 5) ||
                 _chessBoard[temp] == opp)
        {
            pieces.south_east = 0;
            pieces.south_eastMask = 0;

            break;
        }


        else if (_chessBoard[temp] == opp + 1 ||
                 _chessBoard[temp] == opp + 3)
        {
            pieces.south_eastMask |= 1ULL << temp;

            break;
        }

        pieces.south_eastMask |= 1ULL << temp;

        if (r == 0 || c == 7)
        {
            pieces.south_east = 0;
            pieces.south_eastMask = 0;
            break;
        }
    }





    return pieces;
}

static inline void printBitboard(uint64_t bb)
{
    printf("+-----------------+\n");

    for (int rank = 7; rank >= 0; rank--)
    {
        printf("| ");

        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            printf("%d ", (int)((bb >> sq) & 1ULL));
        }

        printf("|\n");
    }

    printf("+-----------------+\n");
}

void generateMoveList(MoveList * _moves )
{
    int counter = isKingInCheckCounter(GAME_STATE[SIDE],GAME_STATE[SIDE]^6);


    if(__builtin_expect(counter>1 , 0))
    {
        Square sq =kingInCheckByWhichSquare(GAME_STATE[SIDE] , GAME_STATE[SIDE]^6);

        generateKingMoveListSpecial(_moves  );
        return;
    }
    else if(counter ==1)
    {
        Square sq =kingInCheckByWhichSquare(GAME_STATE[SIDE] , GAME_STATE[SIDE]^6);
        PinnedPieces pieces=findPinnedPiecesBitMap(GAME_STATE[SIDE]);

        uint64_t pieceBitmap= pieces.east | pieces.west | pieces.north | pieces.south |
                              pieces.north_east | pieces.north_west | pieces.south_east | pieces.south_west;
        if(_chessBoard[sq] == WHITE_KNIGHT || _chessBoard[sq] == BLACK_KNIGHT){
            uint64_t sq1=  1ULL<<sq;
            generatePawnMovements(_moves , pieceBitmap ,sq1 );
            generateKingMoveListSpecial(_moves );
            generateRookMoveList(_moves , pieceBitmap , sq1);
            generateBishopMoveList(_moves , pieceBitmap , sq1);
            generateQueenMoveList(_moves , pieceBitmap , sq1);
            generateKnightMoveList(_moves , pieceBitmap , sq1);
            return;
        }
        uint64_t curr =GAME_STATE[SIDE];
        uint64_t opp = GAME_STATE[SIDE]^6;
        int oppBishop = (opp) +3;
        int oppRook = (opp) +2;
        int oppQueen = (opp) +1;
            uint64_t oppPawn = (opp)+5;
                uint64_t pieceMask =
                    KEEP_IF_BIT_SET(pieces.eastMask, sq) |
                    KEEP_IF_BIT_SET(pieces.westMask, sq) |
                    KEEP_IF_BIT_SET(pieces.northMask, sq) |
                    KEEP_IF_BIT_SET(pieces.southMask, sq) |
                    KEEP_IF_BIT_SET(pieces.north_eastMask, sq) |
                    KEEP_IF_BIT_SET(pieces.north_westMask, sq) |
                    KEEP_IF_BIT_SET(pieces.south_eastMask, sq) |
                    KEEP_IF_BIT_SET(pieces.south_westMask, sq) |
                    (1ULL << sq);

            generatePawnMovements(_moves , pieceBitmap , pieceMask );
            generateKingMoveListSpecial(_moves );
            generateRookMoveList(_moves , pieceBitmap , pieceMask);
            generateBishopMoveList(_moves , pieceBitmap , pieceMask);
            generateQueenMoveList(_moves , pieceBitmap , pieceMask);
            generateKnightMoveList(_moves , pieceBitmap , pieceMask);

            return;
    }
            PinnedPieces pieces=findPinnedPiecesBitMap(GAME_STATE[SIDE]);



            uint64_t pieceBitmap= pieces.east | pieces.west | pieces.north | pieces.south | pieces.north_east | pieces.north_west | pieces.south_east | pieces.south_west;


            generatePawnMovements(_moves , pieceBitmap ,~(0ULL) );
            generateKingMoveListSpecial(_moves );
            generateRookMoveList(_moves , pieceBitmap , ~(0ULL));
            generateBishopMoveList(_moves , pieceBitmap , ~(0ULL));
            generateQueenMoveList(_moves , pieceBitmap ,~(0ULL));
            generateKnightMoveList(_moves , pieceBitmap ,~(0ULL));



            uint64_t curr =GAME_STATE[SIDE];
            uint64_t opp = GAME_STATE[SIDE]^6;
            uint64_t copy[24];
            memcpy(copy, GAME_STATE, sizeof(uint64_t)*24);


            GAME_STATE[curr+5] &= pieceBitmap;
            GAME_STATE[curr+2] &= pieceBitmap;
            GAME_STATE[curr+1] &= pieceBitmap;
            GAME_STATE[curr+3] &= pieceBitmap;
            GAME_STATE[curr+4] &= pieceBitmap;

            GAME_STATE[curr>>2 + WHITE_OCCUPANCY] =  GAME_STATE[curr+5] | GAME_STATE[curr+2] |
            GAME_STATE[curr+1] |
            GAME_STATE[curr+3] |
            GAME_STATE[curr+4] | GAME_STATE[curr];

            GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[WHITE_OCCUPANCY] | GAME_STATE[BLACK_OCCUPANCY];
            uint64_t pinPoint[8] ={pieces.north , pieces.south , pieces.east , pieces.west , pieces.north_east , pieces.north_west , pieces.south_east , pieces.south_west };
            uint64_t rayPoint[8] ={pieces.northMask , pieces.southMask , pieces.eastMask , pieces.westMask , pieces.north_eastMask , pieces.north_westMask , pieces.south_eastMask , pieces.south_westMask };
            for(int pin =0 ;pin<8; pin++){
                if(pinPoint[pin]){
                    // printf("this is raypoint -> %lld\n", rayPoint[pin]);
                    generatePawnMovements(_moves , ~pinPoint[pin] , rayPoint[pin] );
                    generateRookMoveList(_moves , ~pinPoint[pin], rayPoint[pin]);
                    generateBishopMoveList(_moves , ~pinPoint[pin] , rayPoint[pin]);
                    generateQueenMoveList(_moves , ~pinPoint[pin] , rayPoint[pin]);
                    generateKnightMoveList(_moves , ~pinPoint[pin] , rayPoint[pin]);
                }

            }
            memcpy(GAME_STATE,copy, sizeof(uint64_t)*24);
}
