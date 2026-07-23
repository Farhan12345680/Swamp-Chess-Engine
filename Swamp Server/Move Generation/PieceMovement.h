#pragma once
#include <math.h>

__uint64_t knightTable[64];
__uint64_t rookMask[64];
__uint64_t bishopMask[64];
__uint64_t whitePawnTable[64];
__uint64_t whitePawnMovementTable[64];
__uint64_t blackPawnTable[64];
__uint64_t blackPawnMovementTable[64];
__uint64_t kingTable[64];

// -----------------------------
// *****************************
// **********  MAGIC  **********
// *****************************
// -----------------------------

__uint64_t rookMagicNumbers[64] = {
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

__uint64_t bishopMagicNumbers[64] = {
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
    0x4010011029020020ULL};

__uint64_t bishopAttacks[64][512];

__uint64_t rookAttacks[64][4096];

__uint64_t rookMask[64];

__uint64_t bishopMask[64];

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
    12, 11, 11, 11, 11, 11, 11, 12};

static void generateRookMask()
{
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            int _idx = (i - 1) * 8 + (j - 1);
            __uint64_t _point = 1ULL << _idx;
            __uint64_t _mask = 0;

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

static void generateBishopMask()
{
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            int _idx = (i - 1) * 8 + (j - 1);
            __uint64_t _point = 1ULL << _idx;
            __uint64_t _mask = 0;

            for (int k = i + 1, l = j - 1; k < 8 && l > 1; k++, l--)
            {
                _mask |= _point << (7 * (abs(k - i)));
            }
            for (int k = i + 1, l = j + 1; k < 8 && l < 8; k++, l++)
            {
                _mask |= _point << (9 * (abs(k - i)));
            }
            for (int k = i - 1, l = j - 1; k > 1 && l > 1; k--, l--)
            {
                _mask |= _point << (9 * (abs(k - i)));
            }
            for (int k = i - 1, l = j + 1; k > 1 && l < 8; k--, l++)
            {
                _mask |= _point << (7 * (abs(k - i)));
            }

            bishopMask[_idx] = _mask;
        }
    }
}

static void initMasks()
{

    generateRookMask();
    generateBishopMask();
}

static __uint64_t bishop_attacks_on_the_fly(int square, __uint64_t block)
{
    __uint64_t attacks = 0ULL;

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

static __uint64_t rook_attacks_on_the_fly(int square, __uint64_t block)
{
    __uint64_t attacks = 0ULL;

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

static __uint64_t set_occupancy(int index, int bits_in_mask, __uint64_t attack_mask)
{
    __uint64_t occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++)
    {
        int square = __builtin_ctzll(attack_mask);

        attack_mask &= (attack_mask - 1);

        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }

    return occupancy;
}

static void initSliderAttack()
{
    initMasks();

    // first for bishop
    {
        for (int square = 0; square < 64; square++)
        {

            __uint64_t attack_mask = bishopMask[square];

            int relevant_bits_count = __builtin_popcount(attack_mask);

            int occupancy_indicies = (1 << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {

                __uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                int magic_index = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);

                bishopAttacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
        }
    }

    // then for rooks
    {
        for (int square = 0; square < 64; square++)
        {

            __uint64_t attack_mask = rookMask[square];

            int relevant_bits_count = __builtin_popcount(attack_mask);

            int occupancy_indicies = (1 << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {

                {
                    __uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                    int magic_index = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);

                    rookAttacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
                }
            }
        }
    }
}

// -------------------------------
// *******************************
// **********MAGIC ENDS***********
// *******************************
// -------------------------------

static void generatePreCalculatedKnightAttack()
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

static void generatePreCalculatedWhitePawnAttack()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            whitePawnTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if (i == 1)
            {
                continue;
            }

            if ((i + 1) < 9 && (j - 1) > 0)
            {
                whitePawnTable[_idx] |= (_curr << 7);
            }
            if ((i + 1) < 9 && (j + 1) < 9)
            {
                whitePawnTable[_idx] |= (_curr << 9);
            }
        }
    }
}

static void generatePreCalculateWhitePawnMovement()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            whitePawnMovementTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if (i == 1)
            {
                continue;
            }

            if ((i + 1) < 9)
            {
                whitePawnMovementTable[_idx] |= (_curr << 8);
            }
        }
    }
}

static void generatePreCalculateBlackPawnAttck()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            blackPawnTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if (i == 8)
            {
                continue;
            }

            if ((i - 1) > 0 && (j - 1) > 0)
            {
                blackPawnTable[_idx] |= (_curr << 9);
            }
            if ((i - 1) > 0 && (j + 1) < 9)
            {
                blackPawnTable[_idx] |= (_curr << 7);
            }
        }
    }
}

static void generatePreCalculateBlackPawnMovement()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            blackPawnMovementTable[_idx] = 0;
            __uint64_t _curr = 1ULL << _idx;

            if (i == 8)
            {
                continue;
            }

            if ((i - 1) > 0)
            {
                blackPawnMovementTable[_idx] |= (_curr >> 8);
            }
        }
    }
}

static void generatePreCalculatedKingAttack()
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

/////////////////////////////////////////////////////////////
// ----------------------------------------------------------
// -------------------- PIECE GENERATION --------------------
// ----------------------------------------------------------
/////////////////////////////////////////////////////////////

void initializer()
{
    generateRookMask();
    generateBishopMask();

    initSliderAttack();
    generatePreCalculatedKnightAttack();
    generatePreCalculatedWhitePawnAttack();
    generatePreCalculateWhitePawnMovement();
    generatePreCalculateBlackPawnAttck();
    generatePreCalculatedKingAttack();
}
