#include "../1. core/board.h"



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

uint64_t generateZorbistHashFromAGameState(GAME_STATE_STRUCT* gameState)
{
    uint64_t _curr = 0;
    GAME_STATE_STRUCT state = *(gameState);

    _curr ^= generateXORforPiece(WHITE_KING, state.GAME_STATE[WHITE_KING_OCCUPANCY]) ^
             generateXORforPiece(WHITE_PAWN, state.GAME_STATE[WHITE_PAWN_OCCUPANCY]) ^
             generateXORforPiece(WHITE_BISHOP, state.GAME_STATE[WHITE_BISHOP_OCCUPANCY]) ^
             generateXORforPiece(WHITE_QUEEN, state.GAME_STATE[WHITE_QUEEN_OCCUPANCY]) ^
             generateXORforPiece(WHITE_KNIGHT, state.GAME_STATE[WHITE_KNIGHT_OCCUPANCY]) ^
             generateXORforPiece(WHITE_ROOK, state.GAME_STATE[WHITE_ROOK_OCCUPANCY]) ^
             generateXORforPiece(BLACK_KING, state.GAME_STATE[BLACK_KING_OCCUPANCY]) ^
             generateXORforPiece(BLACK_BISHOP, state.GAME_STATE[BLACK_BISHOP_OCCUPANCY]) ^
             generateXORforPiece(BLACK_KNIGHT, state.GAME_STATE[BLACK_KNIGHT_OCCUPANCY]) ^
             generateXORforPiece(BLACK_QUEEN, state.GAME_STATE[BLACK_QUEEN_OCCUPANCY]) ^
             generateXORforPiece(BLACK_PAWN, state.GAME_STATE[BLACK_PAWN_OCCUPANCY]) ^
             generateXORforPiece(BLACK_ROOK ,state.GAME_STATE[BLACK_ROOK_OCCUPANCY]);

    _curr ^= _globalZorbistHashing._zorbistCastlingNums[state.GAME_STATE[CASTLING_AVAILABLE]];

    if (state.GAME_STATE[ENPASSANT_SQUARE] != NS)
    {
        _curr ^= _globalZorbistHashing._zobistFileNums[(state.GAME_STATE[ENPASSANT_SQUARE] % 8)];
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


void printBoard(GAME_STATE_STRUCT* state)
{
    GAME_STATE_STRUCT temp=*(state);

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;

            switch (temp._chessBoard[sq])
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

    printf("castling available %d%d%d%d\n" ,(temp.GAME_STATE[CASTLING_AVAILABLE]&(1<<3)),
                                            (temp.GAME_STATE[CASTLING_AVAILABLE]&(1<<2)),
                                            (temp.GAME_STATE[CASTLING_AVAILABLE]&(1<<1)),
                                            (temp.GAME_STATE[CASTLING_AVAILABLE]&(1<<0)));

    printf("Side to Move %c\n", (temp.GAME_STATE[SIDE] == 6)?'b':'w');
    if(temp.GAME_STATE[ENPASSANT_SQUARE]==ES){
        printf("Enpassant File ES\n");
    }else{
        printf("Enpassant File %c%c\n", ((char)(temp.GAME_STATE[ENPASSANT_SQUARE]%8)+'a'), ((char)(temp.GAME_STATE[ENPASSANT_SQUARE]/8)+'1'));
    }

    printf("Full Move %d\n",(int)temp.GAME_STATE[NUMBER_FULL_MOVES]);
    printf("HALF Move %d\n",(int)temp.GAME_STATE[NUMBER_HALF_MOVES]);

}

uint64_t perft(int depth ,GAME_STATE_STRUCT* state)
{
    if (depth == 0)
        return 1;

    GAME_STATE_STRUCT temp= *(state);

    MoveList moves = {0};
    generateMoveList(&moves,state);

    uint64_t nodes = 0;


    for (int i = 0; i < moves.index; i++)
    {
        GAME_STATE_STRUCT tempStruct={};

        memcpy((void *)&tempStruct ,state , sizeof(GAME_STATE_STRUCT));

        __uint64_t result =makeMove(moves.moves[i],&tempStruct);


        nodes += perft(depth - 1 , &(tempStruct));

    }

    return nodes;
}


__uint64_t perftBulk(int depth, GAME_STATE_STRUCT* state)
{

    if (depth == 0)
        return 1;

    MoveList moves = {0};

    GAME_STATE_STRUCT temp= *(state);

    generateMoveList(&moves,state);

    if (depth == 1)
        return moves.index;
    uint64_t nodes = 0;


    for (int i = 0; i < moves.index; i++)
    {
        GAME_STATE_STRUCT tempStruct={};

        memcpy((void *)&tempStruct ,state , sizeof(GAME_STATE_STRUCT));

        __uint64_t result =makeMove(moves.moves[i],&tempStruct);


        nodes += perft(depth - 1 , &(tempStruct));

    }


    return nodes;
}

uint64_t divide(int depth , GAME_STATE_STRUCT* state)
{
    if (depth <= 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves,state);

    uint64_t total = 0;



    for (int i = 0; i < moves.index; i++)
    {
        GAME_STATE_STRUCT tempStruct={};
        memcpy((void *)&tempStruct ,state , sizeof(GAME_STATE_STRUCT));
        __uint64_t result =makeMove(moves.moves[i], &tempStruct);
        total += perft(depth - 1 , &(tempStruct));
    }

    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);
    return total;
}

uint64_t divideBulk(int depth ,GAME_STATE_STRUCT* state)
{
    if (depth <= 0)
        return 1;
    MoveList moves = {0};
    generateMoveList(&moves,state);
    uint64_t total = 0;


    for (int i = 0; i < moves.index; i++)
    {
        GAME_STATE_STRUCT tempStruct={};
        memcpy((void *)&tempStruct ,state , sizeof(GAME_STATE_STRUCT));
        __uint64_t result =makeMove(moves.moves[i], &tempStruct);
        total += perft(depth - 1 , &(tempStruct));
    }


    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);
    return total;
}

void piecePuter(uint64_t PIECE_NUMBER, Pieces piece ,GAME_STATE_STRUCT* state)
{
    GAME_STATE_STRUCT temp= *(state);

    for (int i = 0; i < 64; i++)
    {
        if (PIECE_NUMBER & (1ULL << i))
        {
            temp._chessBoard[i] = piece;
        }
    }
}

void emptyInitializationHelper(GAME_STATE_STRUCT* state)
{
    GAME_STATE_STRUCT temp = *state;
    _globalZorbistHashing = generateZorbistNumbers();

    temp.GAME_STATE[EVALUATION] = 1000;

    temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] = 0b0000000011111111000000000000000000000000000000000000000000000000;
    temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000001111111100000000;
    temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] = 0b1000000100000000000000000000000000000000000000000000000000000000;
    temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] = 0b0100001000000000000000000000000000000000000000000000000000000000;
    temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] = 0b0010010000000000000000000000000000000000000000000000000000000000;
    temp.GAME_STATE[BLACK_KING_OCCUPANCY] = 0b0001000000000000000000000000000000000000000000000000000000000000;
    temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] = 0b0000100000000000000000000000000000000000000000000000000000000000;
    temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000010000001;
    temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000001000010;
    temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000100100;
    temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000001000;
    temp.GAME_STATE[WHITE_KING_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000010000;

    temp.GAME_STATE[TOTAL_OCCUPANCY] =
        temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] ^ temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^
        temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^ temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^
        temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^ temp.GAME_STATE[BLACK_KING_OCCUPANCY] ^
        temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] ^ temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^
        temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^ temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^
        temp.GAME_STATE[WHITE_KING_OCCUPANCY] ^ temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    temp.GAME_STATE[BLACK_OCCUPANCY] =
        temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] ^ temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^
        temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^ temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^
        temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^ temp.GAME_STATE[BLACK_KING_OCCUPANCY];

    temp.GAME_STATE[WHITE_OCCUPANCY] = temp.GAME_STATE[TOTAL_OCCUPANCY] ^ temp.GAME_STATE[BLACK_OCCUPANCY];

    temp.GAME_STATE[CASTLING_AVAILABLE] = 0b00001111;
    temp.GAME_STATE[SIDE] = 0;

    temp.GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState(state);
    temp.GAME_STATE[ENPASSANT_SQUARE] = NS;
    temp.GAME_STATE[NUMBER_FULL_MOVES] = 1;
    temp.GAME_STATE[NUMBER_HALF_MOVES] = 0;
}

void initializeHelperFunc(GAME_STATE_STRUCT* state){
    GAME_STATE_STRUCT temp=*(state);
    pieceInitializer();

    piecePuter(temp.GAME_STATE[BLACK_PAWN_OCCUPANCY], BLACK_PAWN
        ,state);
    piecePuter(temp.GAME_STATE[BLACK_ROOK_OCCUPANCY], BLACK_ROOK,state);
    piecePuter(temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY], BLACK_BISHOP,state);
    piecePuter(temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY], BLACK_KNIGHT,state);
    piecePuter(temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY], BLACK_QUEEN,state);
    piecePuter(temp.GAME_STATE[BLACK_KING_OCCUPANCY], BLACK_KING,state);
    piecePuter(temp.GAME_STATE[WHITE_PAWN_OCCUPANCY], WHITE_PAWN,state);
    piecePuter(temp.GAME_STATE[WHITE_ROOK_OCCUPANCY], WHITE_ROOK,state);
    piecePuter(temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY], WHITE_BISHOP,state);
    piecePuter(temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY], WHITE_KNIGHT,state);
    piecePuter(temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY], WHITE_QUEEN,state);
    piecePuter(temp.GAME_STATE[WHITE_KING_OCCUPANCY], WHITE_KING,state);
    piecePuter(~(temp.GAME_STATE[TOTAL_OCCUPANCY]), ES,state);
}

void initializer(GAME_STATE_STRUCT* state)
{
    emptyInitializationHelper(state);
    initializeHelperFunc(state);
}

__uint64_t makeMove(uint16_t move,GAME_STATE_STRUCT* state)
{
    GAME_STATE_STRUCT temp=*(state);
    char promotionPie[5] = {'\0', 'q', 'r', 'b', 'n'};
    __uint64_t result = 0;

    bool isPromotion = (move >> 12) != 0;
    char promotion = promotionPie[move >> 12];

    Square src =(Square) (move & 0x3F);
    Square dest = (Square)((move >> 6) & 0x3F);

    bool isEnpassant = false;
    bool isCastle = false;

    Square oldEnpassantSquare = (Square)temp.GAME_STATE[ENPASSANT_SQUARE];

    Pieces movingPiece =(Pieces) temp._chessBoard[src];
    Pieces capturedPiece =(Pieces) temp._chessBoard[dest];

    if ((movingPiece == WHITE_KING || movingPiece == BLACK_KING) &&
        ((src == E1 && (dest == G1 || dest == C1)) ||
         (src == E8 && (dest == G8 || dest == C8))))
    {
        isCastle = true;
    }

   temp.GAME_STATE[ENPASSANT_SQUARE] = NS;

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
       temp.GAME_STATE[ENPASSANT_SQUARE] = (Square)(src - 8);
    }
    else if (movingPiece == WHITE_PAWN &&
             src >= A2 && src <= H2 &&
             dest >= A4 && dest <= H4)
    {
       temp.GAME_STATE[ENPASSANT_SQUARE] = (Square)(src + 8);
    }

    result |= ((__uint64_t)src) << 16;
    result |= ((__uint64_t)dest) << 8;
    result |= ((__uint64_t)(uint8_t)temp._chessBoard[src]) << 40;
    result |= ((__uint64_t)(uint8_t)temp._chessBoard[dest]) << 32;

    switch (movingPiece)
    {
        case WHITE_PAWN:
           temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_KNIGHT:
           temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_BISHOP:
           temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_ROOK:
           temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);

            if (src == A1)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1011;

            else if (src == H1)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b0111;
            break;

        case WHITE_QUEEN:
           temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_KING:
           temp.GAME_STATE[WHITE_KING_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b0011;
            break;

        case BLACK_PAWN:
           temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_KNIGHT:
           temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_BISHOP:
           temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_ROOK:
           temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);

            if (src == A8)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1110;

            if (src == H8)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1101;
            break;

        case BLACK_QUEEN:
           temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_KING:
           temp.GAME_STATE[BLACK_KING_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
           temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1100;
            break;

        default:
            break;
    }

    switch (capturedPiece)
    {
        case WHITE_PAWN:
           temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_KNIGHT:
           temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_BISHOP:
           temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_ROOK:
           temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);

            if (dest == A1)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1011;

            if (dest == H1)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b0111;
            break;

        case WHITE_QUEEN:
           temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_PAWN:
           temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_KNIGHT:
           temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_BISHOP:
           temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_ROOK:
           temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);

            if (dest == A8)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1110;

            if (dest == H8)
               temp.GAME_STATE[CASTLING_AVAILABLE] &= 0b1101;
            break;

        case BLACK_QUEEN:
           temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^= (1ULL << dest);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
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

           temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
        }
        else
        {
            capturedPawnSquare = (Square)(dest + 8);

            result |= (uint64_t)WHITE_PAWN << 24;

           temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
        }

        result |= (uint8_t)capturedPawnSquare;
        temp._chessBoard[capturedPawnSquare] = ES;
    }

    temp._chessBoard[src] = ES;
    temp._chessBoard[dest] = movingPiece;

    if (isPromotion)
    {
        if (movingPiece == WHITE_PAWN)
        {
           temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << dest);

            switch (promotion)
            {
                case 'q':
                    temp._chessBoard[dest] = WHITE_QUEEN;
                   temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'r':
                    temp._chessBoard[dest] = WHITE_ROOK;
                   temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'b':
                    temp._chessBoard[dest] = WHITE_BISHOP;
                   temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'n':
                    temp._chessBoard[dest] = WHITE_KNIGHT;
                   temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] |= (1ULL << dest);
                    break;
            }
        }
        else if (movingPiece == BLACK_PAWN)
        {
           temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << dest);

            switch (promotion)
            {
                case 'q':
                    temp._chessBoard[dest] = BLACK_QUEEN;
                   temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'r':
                    temp._chessBoard[dest] = BLACK_ROOK;
                   temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'b':
                    temp._chessBoard[dest] = BLACK_BISHOP;
                   temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'n':
                    temp._chessBoard[dest] = BLACK_KNIGHT;
                   temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] |= (1ULL << dest);
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

            temp._chessBoard[H1] = ES;
            temp._chessBoard[F1] = WHITE_ROOK;

           temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << H1);
           temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << F1);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << H1);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << F1);
        }
        else if (src == E1 && dest == C1)
        {
            result |= 0x02;

            temp._chessBoard[A1] = ES;
            temp._chessBoard[D1] = WHITE_ROOK;

           temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << A1);
           temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << D1);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << A1);
           temp.GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << D1);
        }
        else if (src == E8 && dest == G8)
        {
            result |= 0x04;

            temp._chessBoard[H8] = ES;
            temp._chessBoard[F8] = BLACK_ROOK;

           temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << H8);
           temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << F8);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << H8);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << F8);
        }
        else if (src == E8 && dest == C8)
        {
            result |= 0x08;

            temp._chessBoard[A8] = ES;
            temp._chessBoard[D8] = BLACK_ROOK;

           temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << A8);
           temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << D8);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << A8);
           temp.GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << D8);
        }
    }

   temp.GAME_STATE[SIDE] ^= 6;
   temp.GAME_STATE[TOTAL_OCCUPANCY] =
       temp.GAME_STATE[WHITE_OCCUPANCY] |
       temp.GAME_STATE[BLACK_OCCUPANCY];

    return result;
}


int initializeNewGameFromString(char *string , GAME_STATE_STRUCT* state)
{
    GAME_STATE_STRUCT temp =*(state);
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

    temp.GAME_STATE[EVALUATION] = 1000;

    temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] = 0;
    temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] = 0;
    temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] = 0;
    temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] = 0;
    temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] = 0;
    temp.GAME_STATE[BLACK_KING_OCCUPANCY] = 0;
    temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] = 0;
    temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] = 0;
    temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] = 0;
    temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY] = 0;
    temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] = 0;
    temp.GAME_STATE[WHITE_KING_OCCUPANCY] = 0;

    temp.GAME_STATE[CASTLING_ABILITY] = 0;
    temp.GAME_STATE[SIDE] = 0;
    temp.GAME_STATE[ZORBIST_HASH] = 0;
    temp.GAME_STATE[ENPASSANT_SQUARE] = NS;
    temp.GAME_STATE[NUMBER_FULL_MOVES] = 1;
    temp.GAME_STATE[NUMBER_HALF_MOVES] = 0;

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
            SET_BIT_PIECE(temp.GAME_STATE[BLACK_KING_OCCUPANCY], _rankCounter, _currPosition);
            break;
        case 'K':
            SET_BIT_PIECE(temp.GAME_STATE[WHITE_KING_OCCUPANCY], _rankCounter, _currPosition);
            break;
        case 'q':
            SET_BIT_PIECE(temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'Q':
            SET_BIT_PIECE(temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'p':
            SET_BIT_PIECE(temp.GAME_STATE[BLACK_PAWN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'P':
            SET_BIT_PIECE(temp.GAME_STATE[WHITE_PAWN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'b':
            SET_BIT_PIECE(temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'B':
            SET_BIT_PIECE(temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'n':
            SET_BIT_PIECE(temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'N':
            SET_BIT_PIECE(temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'r':
            SET_BIT_PIECE(temp.GAME_STATE[BLACK_ROOK_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'R':
            SET_BIT_PIECE(temp.GAME_STATE[WHITE_ROOK_OCCUPANCY], _rankCounter, _currPosition);
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
    temp.GAME_STATE[TOTAL_OCCUPANCY] =
        temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] | temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] |
        temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] | temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] | temp.GAME_STATE[BLACK_KING_OCCUPANCY] |
        temp.GAME_STATE[WHITE_PAWN_OCCUPANCY] | temp.GAME_STATE[WHITE_ROOK_OCCUPANCY] |
        temp.GAME_STATE[WHITE_KNIGHT_OCCUPANCY] | temp.GAME_STATE[WHITE_QUEEN_OCCUPANCY] |
        temp.GAME_STATE[WHITE_KING_OCCUPANCY] | temp.GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    temp.GAME_STATE[BLACK_OCCUPANCY] =
        temp.GAME_STATE[BLACK_PAWN_OCCUPANCY] | temp.GAME_STATE[BLACK_ROOK_OCCUPANCY] |
        temp.GAME_STATE[BLACK_KNIGHT_OCCUPANCY] | temp.GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        temp.GAME_STATE[BLACK_QUEEN_OCCUPANCY] | temp.GAME_STATE[BLACK_KING_OCCUPANCY];

    temp.GAME_STATE[WHITE_OCCUPANCY] = temp.GAME_STATE[TOTAL_OCCUPANCY] ^ temp.GAME_STATE[BLACK_OCCUPANCY];


    if(_string1.sideTOMove !='w' && _string1.sideTOMove !='b' )
    {
                    printf("problem 4");
        return -1;

    }


    temp.GAME_STATE[SIDE] = (_string1.sideTOMove == 'w') ? 0 : 6;



    if (strcmp(_string1.castlingAbility , "-")==0)
    {
        temp.GAME_STATE[CASTLING_AVAILABLE] = 0;
    }
    else
    {
            temp.GAME_STATE[CASTLING_AVAILABLE] = 0;

            if (strcmp(_string1.castlingAbility, "-") != 0)
            {
                for (int i = 0; _string1.castlingAbility[i] != '\0'; i++)
                {
                    switch (_string1.castlingAbility[i])
                    {
                        case 'K':
                            temp.GAME_STATE[CASTLING_AVAILABLE] |= 1 << 3;
                            break;

                        case 'Q':
                            temp.GAME_STATE[CASTLING_AVAILABLE] |= 1 << 2;
                            break;

                        case 'k':
                            temp.GAME_STATE[CASTLING_AVAILABLE] |= 1 << 1;
                            break;

                        case 'q':
                            temp.GAME_STATE[CASTLING_AVAILABLE] |= 1 << 0;
                            break;

                        default:
                            return -1;
                    }
                }
            }
    }


    if (strcmp(_string1.enpassantTargetSquare ,"-")==0)
    {
        temp.GAME_STATE[ENPASSANT_SQUARE] = NS;

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
        temp.GAME_STATE[ENPASSANT_SQUARE] = (Square)(file + rank * 8);
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

    temp.GAME_STATE[NUMBER_HALF_MOVES] = value;



    value = strtol(_string1.fullMoveClock, &end, 10);

    if (end == _string1.fullMoveClock) {
                printf("problem 7");

        return -1;
    }

    if (*end != '\0') {
                printf("problem 8");

        return -1;
    }
    if(!(__builtin_popcountll(temp.GAME_STATE[BLACK_KING_OCCUPANCY])==1)){
                printf("problem 9");

        return -1;
    }
    if(!(__builtin_popcountll(temp.GAME_STATE[WHITE_KING_OCCUPANCY])==1)){
                       printf("problem 10");

        return -1;
    }
    temp.GAME_STATE[NUMBER_FULL_MOVES] = value;
    temp.GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState(&temp);

    initializeHelperFunc(&temp);

    return 1;
}
