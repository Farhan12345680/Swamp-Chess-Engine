#include "../1. core/board.h"
#include <threads.h>

extern thread_local uint64_t GAME_STATE[24];
extern thread_local uint8_t _chessBoard[64];

uint64_t pseudoRandomNumberGenerator(uint64_t *seed)
{
    uint64_t _z = (*seed += 0x9E3779B97F4A7C15ULL);

    _z = (_z ^ (_z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    _z = (_z ^ (_z >> 27)) * 0x94D049BB133111EBULL;

    return _z ^ (_z >> 31);
}

ZorbistKeys generateZorbistNumbers()
{
    uint64_t seed = 0xCAFEBABEDEADBEEFULL;
    ZorbistKeys _curr;

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            _curr._zorbistPieces[i][j] = pseudoRandomNumberGenerator(&seed);
        }
    }

    _curr._zorbistSideToMove = pseudoRandomNumberGenerator(&seed);

    for (int i = 0; i < 16; i++)
    {
        _curr._zorbistCastlingNums[i] = pseudoRandomNumberGenerator(&seed);
    }

    for (int i = 0; i < 8; i++)
    {
        _curr._zobistFileNums[i] = pseudoRandomNumberGenerator(&seed);
    }
    return _curr;
}

uint64_t generateXORforPiece(Pieces PIECE, uint64_t PIECE_BIT_MAP)
{
    uint64_t _curr = 0;

    while (PIECE_BIT_MAP)
    {
        _curr ^= _globalZorbistHashing._zorbistPieces[PIECE][__builtin_ctzll(PIECE_BIT_MAP)];
        PIECE_BIT_MAP = PIECE_BIT_MAP & (PIECE_BIT_MAP - 1);
    }

    return _curr;
}

uint64_t generateZorbistHashFromAGameState()
{
    uint64_t _curr = 0;


    _curr ^= generateXORforPiece(WHITE_KING, GAME_STATE[WHITE_KING_OCCUPANCY]) ^
             generateXORforPiece(WHITE_PAWN, GAME_STATE[WHITE_PAWN_OCCUPANCY]) ^
             generateXORforPiece(WHITE_BISHOP, GAME_STATE[WHITE_BISHOP_OCCUPANCY]) ^
             generateXORforPiece(WHITE_QUEEN, GAME_STATE[WHITE_QUEEN_OCCUPANCY]) ^
             generateXORforPiece(WHITE_KNIGHT, GAME_STATE[WHITE_KNIGHT_OCCUPANCY]) ^
             generateXORforPiece(WHITE_ROOK, GAME_STATE[WHITE_ROOK_OCCUPANCY]) ^
             generateXORforPiece(BLACK_KING, GAME_STATE[BLACK_KING_OCCUPANCY]) ^
             generateXORforPiece(BLACK_BISHOP, GAME_STATE[BLACK_BISHOP_OCCUPANCY]) ^
             generateXORforPiece(BLACK_KNIGHT, GAME_STATE[BLACK_KNIGHT_OCCUPANCY]) ^
             generateXORforPiece(BLACK_QUEEN, GAME_STATE[BLACK_QUEEN_OCCUPANCY]) ^
             generateXORforPiece(BLACK_PAWN, GAME_STATE[BLACK_PAWN_OCCUPANCY]) ^
             generateXORforPiece(BLACK_ROOK ,GAME_STATE[BLACK_ROOK_OCCUPANCY]);

    _curr ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

    if (GAME_STATE[ENPASSANT_SQUARE] != NS)
    {
        _curr ^= _globalZorbistHashing._zobistFileNums[(GAME_STATE[ENPASSANT_SQUARE] % 8)];
    }

    return _curr;
}

void printPieceBitBoard(uint64_t bitboard, char piece)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if (bitboard & (1ULL << square))
                printf("%c ", piece);
            else
                printf(". ");
        }

        printf("\n");
    }
}


void printBoard()
{
    

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;

            switch (_chessBoard[sq])
            {
            case BLACK_ROOK:
                printf("r ");
                break;
            case BLACK_KNIGHT:
                printf("n ");
                break;
            case BLACK_BISHOP:
                printf("b ");
                break;
            case BLACK_QUEEN:
                printf("q ");
                break;
            case BLACK_KING:
                printf("k ");
                break;
            case BLACK_PAWN:
                printf("p ");
                break;

            case WHITE_ROOK:
                printf("R ");
                break;
            case WHITE_KNIGHT:
                printf("N ");
                break;
            case WHITE_BISHOP:
                printf("B ");
                break;
            case WHITE_QUEEN:
                printf("Q ");
                break;
            case WHITE_KING:
                printf("K ");
                break;
            case WHITE_PAWN:
                printf("P ");
                break;

            default:
                printf(". ");
                break;
            }
        }

        printf("\n");
    }

    printf("castling available %d%d%d%d\n" ,(GAME_STATE[CASTLING_AVAILABLE]&(1<<3)),
                                            (GAME_STATE[CASTLING_AVAILABLE]&(1<<2)),
                                            (GAME_STATE[CASTLING_AVAILABLE]&(1<<1)),
                                            (GAME_STATE[CASTLING_AVAILABLE]&(1<<0)));

    printf("Side to Move %c\n", (GAME_STATE[SIDE] == 6)?'b':'w');
    if(GAME_STATE[ENPASSANT_SQUARE]==ES){
        printf("Enpassant File ES\n");
    }else{
        printf("Enpassant File %c%c\n", ((char)(GAME_STATE[ENPASSANT_SQUARE]%8)+'a'), ((char)(GAME_STATE[ENPASSANT_SQUARE]/8)+'1'));
    }

    printf("Full Move %d\n",(int)GAME_STATE[NUMBER_FULL_MOVES]);
    printf("HALF Move %d\n",(int)GAME_STATE[NUMBER_HALF_MOVES]);

}

uint64_t perft(int depth )
{
    if (depth == 0)
        return 1;



    MoveList moves = {0};
    generateMoveList(&moves);

    uint64_t nodes = 0;


    for (int i = 0; i < moves.index; i++)
    {
        uint64_t GAME_STATE_COPY[24];
        uint8_t  _chessBoard_COPY[64];

        memcpy(GAME_STATE_COPY ,GAME_STATE , 24*sizeof(uint64_t));
        memcpy(_chessBoard_COPY ,_chessBoard , 64*sizeof(uint8_t));

        __uint64_t result =makeMove(moves.moves[i]);


        nodes += perft(depth - 1 );


        memcpy(GAME_STATE ,GAME_STATE_COPY , 24*sizeof(uint64_t));
        memcpy(_chessBoard ,_chessBoard_COPY , 64*sizeof(uint8_t));
    }

    return nodes;
}


__uint64_t perftBulk(int depth)
{

    if (depth == 0)
        return 1;

    MoveList moves = {0};

    generateMoveList(&moves);

    if (depth == 1)
        return moves.index;
    uint64_t nodes = 0;


    for (int i = 0; i < moves.index; i++)
    {
        uint64_t GAME_STATE_COPY[24];
        uint8_t  _chessBoard_COPY[64];

        memcpy(GAME_STATE_COPY ,GAME_STATE , 24*sizeof(uint64_t));
        memcpy(_chessBoard_COPY ,_chessBoard , 64*sizeof(uint8_t));

        __uint64_t result =makeMove(moves.moves[i]);


        nodes += perftBulk(depth - 1 );


        memcpy(GAME_STATE ,GAME_STATE_COPY , 24*sizeof(uint64_t));
        memcpy(_chessBoard ,_chessBoard_COPY , 64*sizeof(uint8_t));
    }


    return nodes;
}

uint64_t divide(int depth)
{
    if (depth <= 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);

    uint64_t total = 0;



    for (int i = 0; i < moves.index; i++)
    {
        uint64_t GAME_STATE_COPY[24];
        uint8_t  _chessBoard_COPY[64];

        memcpy(GAME_STATE_COPY ,GAME_STATE , 24*sizeof(uint64_t));
        memcpy(_chessBoard_COPY ,_chessBoard , 64*sizeof(uint8_t));

        __uint64_t result =makeMove(moves.moves[i]);

        total += perft(depth - 1 );


        memcpy(GAME_STATE ,GAME_STATE_COPY , 24*sizeof(uint64_t));
        memcpy(_chessBoard ,_chessBoard_COPY , 64*sizeof(uint8_t));
    }

    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);
    return total;
}

uint64_t divideBulk(int depth)
{
    if (depth <= 0)
        return 1;
    MoveList moves = {0};
    generateMoveList(&moves);
    uint64_t total = 0;


    for (int i = 0; i < moves.index; i++)
    {
        uint64_t GAME_STATE_COPY[24];
        uint8_t  _chessBoard_COPY[64];

        memcpy(GAME_STATE_COPY ,GAME_STATE , 24*sizeof(uint64_t));
        memcpy(_chessBoard_COPY ,_chessBoard , 64*sizeof(uint8_t));

        __uint64_t result =makeMove(moves.moves[i]);
        total += perftBulk(depth - 1 );

        memcpy(GAME_STATE ,GAME_STATE_COPY , 24*sizeof(uint64_t));
        memcpy(_chessBoard,_chessBoard_COPY , 64*sizeof(uint8_t));


    }


    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);
    return total;
}

uint64_t divideBulkWithThread(int depth)
{
    if (depth==1)
    {
        return divideBulk(1); 
    }
    
    if (depth <= 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);
    uint64_t total = 0;

    


    memcpy(moveGeneratedArray, moves.moves , sizeof(uint16_t)*moves.index );
    moveGeneratedArrayIndex=0;
    moveGeneratedLastArrayIndex=moves.index;
    perftNumber=0;
    GAME_STATE_STRUCT tempState = copyState(); 
    GAME_STATE_STRUCT_WITH_DEPTH temp={depth-1,tempState};



    const int threads = THREAD_COUNT+4;
    pthread_t threadArrays[threads];


    for(int i =0 ;i<threads; i++){
        pthread_create(&threadArrays[i],NULL , doMultiThreadPerft , (void*)&temp);

    }

    for(int i =0;i<threads; i++){
        pthread_join(threadArrays[i],NULL);
    }



    printf("\nTotal Nodes: %llu\n", (unsigned long long)perftNumber);
    return perftNumber;
}

void piecePuter(uint64_t PIECE_NUMBER, Pieces piece )
{
    for (int i = 0; i < 64; i++)
    {
        if (PIECE_NUMBER & (1ULL << i))
        {
            _chessBoard[i] = piece;
        }
    }
}

void piecePuterWithBoard(uint64_t PIECE_NUMBER , Pieces piece,uint8_t _chessBoard[64])
{
    for (int i = 0; i < 64; i++)
    {
        if (PIECE_NUMBER & (1ULL << i))
        {
            _chessBoard[i] = piece;
        }
    }
}

void emptyInitializationHelper()
{

    _globalZorbistHashing = generateZorbistNumbers();

    GAME_STATE[EVALUATION] = 1000;

    GAME_STATE[BLACK_PAWN_OCCUPANCY] = 0b0000000011111111000000000000000000000000000000000000000000000000;
    GAME_STATE[WHITE_PAWN_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000001111111100000000;
    GAME_STATE[BLACK_ROOK_OCCUPANCY] = 0b1000000100000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_KNIGHT_OCCUPANCY] = 0b0100001000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_BISHOP_OCCUPANCY] = 0b0010010000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_KING_OCCUPANCY] = 0b0001000000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_QUEEN_OCCUPANCY] = 0b0000100000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[WHITE_ROOK_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000010000001;
    GAME_STATE[WHITE_KNIGHT_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000001000010;
    GAME_STATE[WHITE_BISHOP_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000100100;
    GAME_STATE[WHITE_QUEEN_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000001000;
    GAME_STATE[WHITE_KING_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000010000;

    GAME_STATE[TOTAL_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] ^ GAME_STATE[BLACK_ROOK_OCCUPANCY] ^
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^ GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^ GAME_STATE[BLACK_KING_OCCUPANCY] ^
        GAME_STATE[WHITE_PAWN_OCCUPANCY] ^ GAME_STATE[WHITE_ROOK_OCCUPANCY] ^
        GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^ GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^
        GAME_STATE[WHITE_KING_OCCUPANCY] ^ GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    GAME_STATE[BLACK_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] ^ GAME_STATE[BLACK_ROOK_OCCUPANCY] ^
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^ GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^ GAME_STATE[BLACK_KING_OCCUPANCY];

    GAME_STATE[WHITE_OCCUPANCY] = GAME_STATE[TOTAL_OCCUPANCY] ^ GAME_STATE[BLACK_OCCUPANCY];

    GAME_STATE[CASTLING_AVAILABLE] = 0b00001111;
    GAME_STATE[SIDE] = 0;

    GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState();
    GAME_STATE[ENPASSANT_SQUARE] = NS;
    GAME_STATE[NUMBER_FULL_MOVES] = 1;
    GAME_STATE[NUMBER_HALF_MOVES] = 0;
}

void initializeHelperFunc(){

    pieceInitializer();

    piecePuter(GAME_STATE[BLACK_PAWN_OCCUPANCY], BLACK_PAWN
        );
    piecePuter(GAME_STATE[BLACK_ROOK_OCCUPANCY], BLACK_ROOK);
    piecePuter(GAME_STATE[BLACK_BISHOP_OCCUPANCY], BLACK_BISHOP);
    piecePuter(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], BLACK_KNIGHT);
    piecePuter(GAME_STATE[BLACK_QUEEN_OCCUPANCY], BLACK_QUEEN);
    piecePuter(GAME_STATE[BLACK_KING_OCCUPANCY], BLACK_KING);
    piecePuter(GAME_STATE[WHITE_PAWN_OCCUPANCY], WHITE_PAWN);
    piecePuter(GAME_STATE[WHITE_ROOK_OCCUPANCY], WHITE_ROOK);
    piecePuter(GAME_STATE[WHITE_BISHOP_OCCUPANCY], WHITE_BISHOP);
    piecePuter(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], WHITE_KNIGHT);
    piecePuter(GAME_STATE[WHITE_QUEEN_OCCUPANCY], WHITE_QUEEN);
    piecePuter(GAME_STATE[WHITE_KING_OCCUPANCY], WHITE_KING);
    piecePuter(~(GAME_STATE[TOTAL_OCCUPANCY]), ES);
}

void initializer()
{
    emptyInitializationHelper();
    initializeHelperFunc();
}

__uint64_t makeMove(uint16_t move)
{

    
    char promotionPie[5] = {'\0', 'q', 'r', 'b', 'n'};
    __uint64_t result = 0;

    bool isPromotion = (move >> 12) != 0;
    char promotion = promotionPie[move >> 12];

    Square src =(Square) (move & 0x3F);
    Square dest = (Square)((move >> 6) & 0x3F);

    bool isEnpassant = false;
    bool isCastle = false;

    Square oldEnpassantSquare = (Square)GAME_STATE[ENPASSANT_SQUARE];

    Pieces movingPiece =(Pieces) _chessBoard[src];
    Pieces capturedPiece =(Pieces) _chessBoard[dest];

    if ((movingPiece == WHITE_KING || movingPiece == BLACK_KING) &&
        ((src == E1 && (dest == G1 || dest == C1)) ||
         (src == E8 && (dest == G8 || dest == C8))))
    {
        isCastle = true;
    }

   GAME_STATE[ENPASSANT_SQUARE] = NS;

    if (movingPiece == WHITE_PAWN &&
        dest == oldEnpassantSquare &&
        capturedPiece == ES &&
        src >= A5 && src <= H5)
    {
        isEnpassant = true;
    }

    if (movingPiece == BLACK_PAWN &&
        dest == oldEnpassantSquare &&
        capturedPiece == ES &&
        src >= A4 && src <= H4)
    {
        isEnpassant = true;
    }

    if (movingPiece == BLACK_PAWN &&
        src >= A7 && src <= H7 &&
        dest >= A5 && dest <= H5)
    {
       GAME_STATE[ENPASSANT_SQUARE] = (Square)(src - 8);
    }
    else if (movingPiece == WHITE_PAWN &&
             src >= A2 && src <= H2 &&
             dest >= A4 && dest <= H4)
    {
       GAME_STATE[ENPASSANT_SQUARE] = (Square)(src + 8);
    }

    result |= ((__uint64_t)src) << 16;
    result |= ((__uint64_t)dest) << 8;
    result |= ((__uint64_t)(uint8_t)_chessBoard[src]) << 40;
    result |= ((__uint64_t)(uint8_t)_chessBoard[dest]) << 32;

    switch (movingPiece)
    {
        case WHITE_PAWN:
           GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_KNIGHT:
           GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_BISHOP:
           GAME_STATE[WHITE_BISHOP_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_ROOK:
           GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);

            if (src == A1)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b1011;

            else if (src == H1)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b0111;
            break;

        case WHITE_QUEEN:
           GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_KING:
           GAME_STATE[WHITE_KING_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[CASTLING_AVAILABLE] &= 0b0011;
            break;

        case BLACK_PAWN:
           GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_KNIGHT:
           GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_BISHOP:
           GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_ROOK:
           GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);

            if (src == A8)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b1110;

            if (src == H8)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b1101;
            break;

        case BLACK_QUEEN:
           GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_KING:
           GAME_STATE[BLACK_KING_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           GAME_STATE[CASTLING_AVAILABLE] &= 0b1100;
            break;

        default:
            break;
    }

    switch (capturedPiece)
    {
        case WHITE_PAWN:
           GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_KNIGHT:
           GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_BISHOP:
           GAME_STATE[WHITE_BISHOP_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_ROOK:
           GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);

            if (dest == A1)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b1011;

            if (dest == H1)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b0111;
            break;

        case WHITE_QUEEN:
           GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_PAWN:
           GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_KNIGHT:
           GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_BISHOP:
           GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_ROOK:
           GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);

            if (dest == A8)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b1110;

            if (dest == H8)
               GAME_STATE[CASTLING_AVAILABLE] &= 0b1101;
            break;

        case BLACK_QUEEN:
           GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^= (1ULL << dest);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_KING:
        case BLACK_KING:
            break;

        default:
            break;
    }

    if (isEnpassant)
    {
        Square capturedPawnSquare;

        if (movingPiece == WHITE_PAWN)
        {
            capturedPawnSquare = (Square)(dest - 8);

            result |= (uint64_t)BLACK_PAWN << 24;

           GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
        }
        else
        {
            capturedPawnSquare = (Square)(dest + 8);

            result |= (uint64_t)WHITE_PAWN << 24;

           GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
        }

        result |= (uint8_t)capturedPawnSquare;
        _chessBoard[capturedPawnSquare] = ES;
    }

    _chessBoard[src] = ES;
    _chessBoard[dest] = movingPiece;

    if (isPromotion)
    {
        if (movingPiece == WHITE_PAWN)
        {
           GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << dest);

            switch (promotion)
            {
                case 'q':
                    _chessBoard[dest] = WHITE_QUEEN;
                   GAME_STATE[WHITE_QUEEN_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'r':
                    _chessBoard[dest] = WHITE_ROOK;
                   GAME_STATE[WHITE_ROOK_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'b':
                    _chessBoard[dest] = WHITE_BISHOP;
                   GAME_STATE[WHITE_BISHOP_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'n':
                    _chessBoard[dest] = WHITE_KNIGHT;
                   GAME_STATE[WHITE_KNIGHT_OCCUPANCY] |= (1ULL << dest);
                    break;
            }
        }
        else if (movingPiece == BLACK_PAWN)
        {
           GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << dest);

            switch (promotion)
            {
                case 'q':
                    _chessBoard[dest] = BLACK_QUEEN;
                   GAME_STATE[BLACK_QUEEN_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'r':
                    _chessBoard[dest] = BLACK_ROOK;
                   GAME_STATE[BLACK_ROOK_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'b':
                    _chessBoard[dest] = BLACK_BISHOP;
                   GAME_STATE[BLACK_BISHOP_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'n':
                    _chessBoard[dest] = BLACK_KNIGHT;
                   GAME_STATE[BLACK_KNIGHT_OCCUPANCY] |= (1ULL << dest);
                    break;
            }
        }
    }

    if (isCastle)
    {
        result = 0x0001000000000000ULL;

        if (src == E1 && dest == G1)
        {
            result |= 0x01;

            _chessBoard[H1] = ES;
            _chessBoard[F1] = WHITE_ROOK;

           GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << H1);
           GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << F1);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << H1);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << F1);
        }
        else if (src == E1 && dest == C1)
        {
            result |= 0x02;

            _chessBoard[A1] = ES;
            _chessBoard[D1] = WHITE_ROOK;

           GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << A1);
           GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << D1);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << A1);
           GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << D1);
        }
        else if (src == E8 && dest == G8)
        {
            result |= 0x04;

            _chessBoard[H8] = ES;
            _chessBoard[F8] = BLACK_ROOK;

           GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << H8);
           GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << F8);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << H8);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << F8);
        }
        else if (src == E8 && dest == C8)
        {
            result |= 0x08;

            _chessBoard[A8] = ES;
            _chessBoard[D8] = BLACK_ROOK;

           GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << A8);
           GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << D8);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << A8);
           GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << D8);
        }
    }

   GAME_STATE[SIDE] ^= 6;
   GAME_STATE[TOTAL_OCCUPANCY] =
       GAME_STATE[WHITE_OCCUPANCY] |
       GAME_STATE[BLACK_OCCUPANCY];

    return result;
}


int initializeNewGameFromString(char *string)
{
    FEN_STRING _string1;

    char copy[UCI_LINE_SIZE];
    memset(copy , 0 , UCI_LINE_SIZE);
    memcpy(copy , string , strlen(string));
    int length[]={100 , 1 , 4 , 2 , 10 , 10 };
    copy[strcspn(copy , "\r\n")]='\0';
    char* token = strtok(copy , " \t\r\n");

    for(int i=0;i<6 ; i++){

        if(token==NULL){
            printf("problem 1");
            printf("%s\n",string);
            return -1;
        }
        switch (i)
        {
        case 0:
            memcpy(_string1.piecePlacement,token , strlen(token)+1);
            break;
        case 1:
            memcpy(&(_string1.sideTOMove) , token , strlen(token));
            break;
        case 2:
            memcpy(_string1.castlingAbility , token , strlen(token)+1);
            break;
        case 3:
            memcpy(_string1.enpassantTargetSquare , token , strlen(token)+1);
            break;
        case 4:
            memcpy(_string1.halfmoveClock , token , strlen(token)+1);
            break;
        case 5:
            memcpy(_string1.fullMoveClock , token , strlen(token)+1);
            break;
        default:
            break;
        }
        token=strtok(NULL , " \t\r\n" );
    }

    // printf("%s\n%c\n%s\n%s\n%s\n%s\n",_string1.piecePlacement ,_string1.sideTOMove,_string1.castlingAbility ,_string1.enpassantTargetSquare,_string1.halfmoveClock ,_string1.fullMoveClock);


    _globalZorbistHashing = generateZorbistNumbers();

    GAME_STATE[EVALUATION] = 1000;

    GAME_STATE[BLACK_PAWN_OCCUPANCY] = 0;
    GAME_STATE[WHITE_PAWN_OCCUPANCY] = 0;
    GAME_STATE[BLACK_ROOK_OCCUPANCY] = 0;
    GAME_STATE[BLACK_KNIGHT_OCCUPANCY] = 0;
    GAME_STATE[BLACK_BISHOP_OCCUPANCY] = 0;
    GAME_STATE[BLACK_KING_OCCUPANCY] = 0;
    GAME_STATE[BLACK_QUEEN_OCCUPANCY] = 0;
    GAME_STATE[WHITE_ROOK_OCCUPANCY] = 0;
    GAME_STATE[WHITE_KNIGHT_OCCUPANCY] = 0;
    GAME_STATE[WHITE_BISHOP_OCCUPANCY] = 0;
    GAME_STATE[WHITE_QUEEN_OCCUPANCY] = 0;
    GAME_STATE[WHITE_KING_OCCUPANCY] = 0;

    GAME_STATE[CASTLING_ABILITY] = 0;
    GAME_STATE[SIDE] = 0;
    GAME_STATE[ZORBIST_HASH] = 0;
    GAME_STATE[ENPASSANT_SQUARE] = NS;
    GAME_STATE[NUMBER_FULL_MOVES] = 1;
    GAME_STATE[NUMBER_HALF_MOVES] = 0;

    int _rankCounter = 8;
    int _currPosition = 0;
    int pos = 0;

    while (pos < strlen(_string1.piecePlacement))
    {


        switch (_string1.piecePlacement[pos])
        {
        case '/':
            _rankCounter--;
            if(_currPosition!=8){
                printf("problem 2");

                return -1;
            }
            _currPosition = 0;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            _currPosition += (_string1.piecePlacement[pos] - '0');

            break;
        case 'k':
            SET_BIT_PIECE(GAME_STATE[BLACK_KING_OCCUPANCY], _rankCounter, _currPosition);
            break;
        case 'K':
            SET_BIT_PIECE(GAME_STATE[WHITE_KING_OCCUPANCY], _rankCounter, _currPosition);
            break;
        case 'q':
            SET_BIT_PIECE(GAME_STATE[BLACK_QUEEN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'Q':
            SET_BIT_PIECE(GAME_STATE[WHITE_QUEEN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'p':
            SET_BIT_PIECE(GAME_STATE[BLACK_PAWN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'P':
            SET_BIT_PIECE(GAME_STATE[WHITE_PAWN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'b':
            SET_BIT_PIECE(GAME_STATE[BLACK_BISHOP_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'B':
            SET_BIT_PIECE(GAME_STATE[WHITE_BISHOP_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'n':
            SET_BIT_PIECE(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'N':
            SET_BIT_PIECE(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'r':
            SET_BIT_PIECE(GAME_STATE[BLACK_ROOK_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'R':
            SET_BIT_PIECE(GAME_STATE[WHITE_ROOK_OCCUPANCY], _rankCounter, _currPosition);
            break;
        default:
            return -1;
        }

        pos++;

    }
    if(_currPosition!=8 && _rankCounter!=8){
                    printf("problem 3");

        return -1;
    }
    GAME_STATE[TOTAL_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] | GAME_STATE[BLACK_ROOK_OCCUPANCY] |
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] | GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] | GAME_STATE[BLACK_KING_OCCUPANCY] |
        GAME_STATE[WHITE_PAWN_OCCUPANCY] | GAME_STATE[WHITE_ROOK_OCCUPANCY] |
        GAME_STATE[WHITE_KNIGHT_OCCUPANCY] | GAME_STATE[WHITE_QUEEN_OCCUPANCY] |
        GAME_STATE[WHITE_KING_OCCUPANCY] | GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    GAME_STATE[BLACK_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] | GAME_STATE[BLACK_ROOK_OCCUPANCY] |
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] | GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] | GAME_STATE[BLACK_KING_OCCUPANCY];

    GAME_STATE[WHITE_OCCUPANCY] = GAME_STATE[TOTAL_OCCUPANCY] ^ GAME_STATE[BLACK_OCCUPANCY];


    if(_string1.sideTOMove !='w' && _string1.sideTOMove !='b' )
    {
                    printf("problem 4");
        return -1;

    }


    GAME_STATE[SIDE] = (_string1.sideTOMove == 'w') ? 0 : 6;



    if (strcmp(_string1.castlingAbility , "-")==0)
    {
        GAME_STATE[CASTLING_AVAILABLE] = 0;
    }
    else
    {
            GAME_STATE[CASTLING_AVAILABLE] = 0;

            if (strcmp(_string1.castlingAbility, "-") != 0)
            {
                for (int i = 0; _string1.castlingAbility[i] != '\0'; i++)
                {
                    switch (_string1.castlingAbility[i])
                    {
                        case 'K':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 3;
                            break;

                        case 'Q':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 2;
                            break;

                        case 'k':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 1;
                            break;

                        case 'q':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 0;
                            break;

                        default:
                            return -1;
                    }
                }
            }
    }


    if (strcmp(_string1.enpassantTargetSquare ,"-")==0)
    {
        GAME_STATE[ENPASSANT_SQUARE] = NS;

    }
    else
    {
        if(_string1.enpassantTargetSquare[0]<='a' && _string1.enpassantTargetSquare[0]>='h'){
            return -1;
        }
        if(_string1.enpassantTargetSquare[1]<='1' || _string1.enpassantTargetSquare[1]>='8'){
            return -1;
        }
        int file = _string1.enpassantTargetSquare[0] - 'a';
        int rank = _string1.enpassantTargetSquare[1] - '1';
        GAME_STATE[ENPASSANT_SQUARE] = (Square)(file + rank * 8);
    }

    char *end;
    long value = strtol(_string1.halfmoveClock, &end, 10);

    if (end == _string1.halfmoveClock) {
        printf("problem 5");
        return -1;
    }

    if (*end != '\0') {
                printf("problem 6");

        return -1;
    }

    GAME_STATE[NUMBER_HALF_MOVES] = value;



    value = strtol(_string1.fullMoveClock, &end, 10);

    if (end == _string1.fullMoveClock) {
                printf("problem 7");

        return -1;
    }

    if (*end != '\0') {
                printf("problem 8");

        return -1;
    }
    if(!(__builtin_popcountll(GAME_STATE[BLACK_KING_OCCUPANCY])==1)){
                printf("problem 9");

        return -1;
    }
    if(!(__builtin_popcountll(GAME_STATE[WHITE_KING_OCCUPANCY])==1)){
                       printf("problem 10");

        return -1;
    }
    GAME_STATE[NUMBER_FULL_MOVES] = value;
    GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState();

    initializeHelperFunc();

    return 1;
}

void initializeNewGameFromStruct(GAME_STATE_STRUCT* state)
{
    uint64_t* GAME_STATE= state->GAME_STATE;
    uint8_t* _chessBoard= state->_chessBoard;

    pieceInitializer();

    piecePuterWithBoard(GAME_STATE[BLACK_PAWN_OCCUPANCY], BLACK_PAWN ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[BLACK_ROOK_OCCUPANCY], BLACK_ROOK ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[BLACK_BISHOP_OCCUPANCY], BLACK_BISHOP ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], BLACK_KNIGHT ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[BLACK_QUEEN_OCCUPANCY], BLACK_QUEEN ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[BLACK_KING_OCCUPANCY], BLACK_KING ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[WHITE_PAWN_OCCUPANCY], WHITE_PAWN ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[WHITE_ROOK_OCCUPANCY], WHITE_ROOK ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[WHITE_BISHOP_OCCUPANCY], WHITE_BISHOP ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], WHITE_KNIGHT ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[WHITE_QUEEN_OCCUPANCY], WHITE_QUEEN ,_chessBoard);
    piecePuterWithBoard(GAME_STATE[WHITE_KING_OCCUPANCY], WHITE_KING ,_chessBoard);
    piecePuterWithBoard(~(GAME_STATE[TOTAL_OCCUPANCY]), ES ,_chessBoard);
}

GAME_STATE_STRUCT copyState()
{
    GAME_STATE_STRUCT newStruct;
    memcpy(newStruct.GAME_STATE,GAME_STATE, 24*sizeof(uint64_t) );
    memcpy(newStruct._chessBoard,_chessBoard ,64*sizeof(uint8_t) );
    
    return newStruct;
}


void doMultiThreadPerft(void* address )
{
    GAME_STATE_STRUCT_WITH_DEPTH data = *((GAME_STATE_STRUCT_WITH_DEPTH*)address);
    memcpy(GAME_STATE ,data.state.GAME_STATE, 24*sizeof(uint64_t));
    memcpy(_chessBoard, data.state._chessBoard , 64*sizeof(uint8_t));

    while(1){
        int index;
        pthread_mutex_lock(&moveArrayReadingLock);
        if(moveGeneratedArrayIndex == moveGeneratedLastArrayIndex){
            pthread_mutex_unlock(&moveArrayReadingLock);
            return;
        }
        index = (moveGeneratedArrayIndex++);
        pthread_mutex_unlock(&moveArrayReadingLock);

        uint64_t GAME_STATE_COPY[24];
        uint8_t  _chessBoard_COPY[64];

        memcpy(GAME_STATE_COPY ,GAME_STATE , 24*sizeof(uint64_t));
        memcpy(_chessBoard_COPY ,_chessBoard , 64*sizeof(uint8_t));

        makeMove(moveGeneratedArray[index]);




        uint64_t total = perftBulk(data.depth);
        memcpy(GAME_STATE ,GAME_STATE_COPY, 24*sizeof(uint64_t));
        memcpy(_chessBoard,_chessBoard_COPY , 64*sizeof(uint8_t));

        pthread_mutex_lock(&updatePerftCondLock);
        perftNumber += total;
        pthread_mutex_unlock(&updatePerftCondLock);

    }
}






void generateRookMask()
{
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            int _idx = (i - 1) * 8 + (j - 1);
            uint64_t _point = 1ULL << _idx;
            uint64_t _mask = 0;

            for (int k = i + 1; k < 8; k++)
            {
                _mask |= _point << (8 * (abs(k - i)));
            }
            for (int k = i - 1; k > 1; k--)
            {
                _mask |= _point >> (8 * (abs(k - i)));
            }
            for (int k = j + 1; k < 8; k++)
            {
                _mask |= _point << ((abs(k - j)));
            }
            for (int k = j - 1; k > 1; k--)
            {
                _mask |= _point >> ((abs(k - j)));
            }

            rookMask[_idx] = _mask;
        }
    }
}

void generateBishopMask()
{
    for (int row = 1; row < 9; row++)
    {
        for (int column = 1; column < 9; column++)
        {
            int _idx = (row - 1) * 8 + (column - 1);
            uint64_t _point = 1ULL << _idx;
            uint64_t _mask = 0;

            for (int k = row + 1, l = column - 1; k < 8 && l > 1; k++, l--)
            {
                _mask |= _point << (7 * (abs(k - row)));
            }
            for (int k = row + 1, l = column + 1; k < 8 && l < 8; k++, l++)
            {
                _mask |= _point << (9 * (abs(k - row)));
            }
            for (int k = row - 1, l = column - 1; k > 1 && l > 1; k--, l--)
            {
                _mask |= _point >> (9 * (abs(k - row)));
            }
            for (int k = row - 1, l = column + 1; k > 1 && l < 8; k--, l++)
            {
                _mask |= _point >> (7 * (abs(k - row)));
            }

            bishopMask[_idx] = _mask;
        }
    }
}

void initMasks()
{
    generateRookMask();
    generateBishopMask();
}


uint64_t bishop_attacks_on_the_fly(int square, uint64_t block)
{
    uint64_t attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    return attacks;
}

uint64_t rook_attacks_on_the_fly(int square, uint64_t block)
{
    uint64_t attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    return attacks;
}

uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask)
{
    uint64_t occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++)
    {
        int square = __builtin_ctzll(attack_mask);

        attack_mask &= (attack_mask - 1);

        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }

    return occupancy;
}

void initSliderAttack()
{
    initMasks();
    {
        for (int square = 0; square < 64; square++)
        {

            uint64_t attack_mask = bishopMask[square];

            int relevant_bits_count = __builtin_popcountll(attack_mask);

            int occupancy_indicies = (1 << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {

                uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                int magic_index = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);

                bishopAttacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
        }
    }

    {
        for (int square = 0; square < 64; square++)
        {

            uint64_t attack_mask = rookMask[square];

            int relevant_bits_count = __builtin_popcountll(attack_mask);

            int occupancy_indicies = (1 << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {

                {
                    uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                    int magic_index = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);

                    rookAttacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
                }
            }
        }
    }
}

void generatePreCalculatedKnightAttack()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            knightTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if ((i - 2) > 0 && (j - 1) > 0)
            {
                knightTable[_idx] |= (_curr >> 17);
            }
            if ((i - 2) > 0 && (j + 1) < 9)
            {
                knightTable[_idx] |= (_curr >> 15);
            }
            if ((i + 2) < 9 && (j - 1) > 0)
            {
                knightTable[_idx] |= (_curr << 15);
            }
            if ((i + 2) < 9 && (j + 1) < 9)
            {
                knightTable[_idx] |= (_curr << 17);
            }
            if ((j - 2) > 0 && (i - 1) > 0)
            {
                knightTable[_idx] |= (_curr >> 10);
            }
            if ((j - 2) > 0 && (i + 1) < 9)
            {
                knightTable[_idx] |= (_curr << 6);
            }
            if ((j + 2) < 9 && (i - 1) > 0)
            {
                knightTable[_idx] |= (_curr >> 6);
            }
            if ((j + 2) < 9 && (i + 1) < 9)
            {
                knightTable[_idx] |= (_curr << 10);
            }
        }
    }
}

void generatePreCalculatedWhitePawnAttack()
{
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
        {
            int _idx = (i) * 8 + ((j));
            whitePawnTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if ((j) > 0)
            {
                whitePawnTable[_idx] |= (_curr << 7);
            }
            if ((j) < 7)
            {
                whitePawnTable[_idx] |= (_curr << 9);
            }
        }
    }
}

void generatePreCalculateBlackPawnAttck()
{
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
        {
            int _idx = (i) * 8 + ((j));
            blackPawnTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if (j > 0)
                blackPawnTable[_idx] |= _curr >> 9;

            if (j < 7)
                blackPawnTable[_idx] |= _curr >> 7;
        }
    }
}

void generatePreCalculatedKingAttack()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));

            kingTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if ((i - 1) > 0 && (j - 1) > 0)
            {
                kingTable[_idx] |= (_curr >> 9);
            }
            if ((i - 1) > 0 && (j + 1) < 9)
            {
                kingTable[_idx] |= (_curr >> 7);
            }
            if ((i - 1) > 0)
            {
                kingTable[_idx] |= (_curr >> 8);
            }
            if ((i + 1) < 9 && (j - 1) > 0)
            {
                kingTable[_idx] |= (_curr << 7);
            }
            if ((i + 1) < 9 && (j + 1) < 9)
            {
                kingTable[_idx] |= (_curr << 9);
            }
            if ((i + 1) < 9)
            {
                kingTable[_idx] |= (_curr << 8);
            }
            if ((j - 1) > 0)
            {
                kingTable[_idx] |= (_curr >> 1);
            }
            if ((j + 1) < 9)
            {
                kingTable[_idx] |= (_curr << 1);
            }
        }
    }
}

void pieceInitializer()
{
    helperArr[ES][0]=NO_PIECE;
    helperArr[ES][1]=NO_PIECE;
    helperArr[BLACK_PAWN][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_PAWN][1]=BLACK_PAWN_OCCUPANCY;
    helperArr[BLACK_KNIGHT][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_KNIGHT][1]=BLACK_KNIGHT_OCCUPANCY;
    helperArr[BLACK_BISHOP][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_BISHOP][1]=BLACK_BISHOP_OCCUPANCY;
    helperArr[BLACK_ROOK][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_ROOK][1]=BLACK_ROOK_OCCUPANCY;
    helperArr[BLACK_QUEEN][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_QUEEN][1]=BLACK_QUEEN_OCCUPANCY;
    helperArr[BLACK_KING][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_KING][1]=BLACK_KING_OCCUPANCY;
    helperArr[WHITE_PAWN][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_PAWN][1]=WHITE_PAWN_OCCUPANCY;
    helperArr[WHITE_KNIGHT][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_KNIGHT][1]=WHITE_KNIGHT_OCCUPANCY;
    helperArr[WHITE_BISHOP][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_BISHOP][1]=WHITE_BISHOP_OCCUPANCY;
    helperArr[WHITE_ROOK][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_ROOK][1]=WHITE_ROOK_OCCUPANCY;
    helperArr[WHITE_QUEEN][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_QUEEN][1]=WHITE_QUEEN_OCCUPANCY;
    helperArr[WHITE_KING][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_KING][1]=WHITE_KING_OCCUPANCY;
    generateRookMask();
    generateBishopMask();

    initSliderAttack();
    generatePreCalculatedKnightAttack();
    generatePreCalculatedWhitePawnAttack();
    generatePreCalculateBlackPawnAttck();
    generatePreCalculatedKingAttack();
}