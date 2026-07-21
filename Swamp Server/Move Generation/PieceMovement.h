#pragma once

void generatePreCalculatedKnightAttack(__uint64_t knightTable[64])
{

    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            knightTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

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

void generatePreCalculatedWhitePawnAttack(__uint64_t whitePawnTable[64])
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            whitePawnTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if(i==1 ){
                continue;
            }
            
            if((i+1)<9 && (j-1)>0){
                whitePawnTable[_idx] |= (_curr<<7);
            }
            if((i+1)<9 && (j+1)<9){
                whitePawnTable[_idx] |= (_curr<<9);
            }
        }
    }
}

void generatePreCalculateWhitePawnMovement(__uint64_t whitePawnMovementTable[64])
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            whitePawnMovementTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if(i==1 ){
                continue;
            }

            if((i+1)<9){
                whitePawnMovementTable[_idx] |= (_curr<<8);
            }
        }
    }
}

void generatePreCalculateBlackPawnAttck(__uint64_t blackPawnTable[64])
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            blackPawnTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if(i==8 ){
                continue;
            }
            
            if((i-1)>0 && (j-1)>0){
                blackPawnTable[_idx] |= (_curr<<9);
            }
            if((i-1)>0 && (j+1)<9){
                blackPawnTable[_idx] |= (_curr<<7);
            }
        }
    }
}

void generatePreCalculateBlackPawnMovement(__uint64_t blackPawnMovementTable[64])
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            blackPawnMovementTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if( i==8){
                continue;
            }

            if((i-1)>0 ){
                blackPawnMovementTable[_idx] |= (_curr>>8);
            }
        }
    }
}

void generatePreCalculatedKingAttack(__uint64_t kingTable[64])
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));

            kingTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

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
