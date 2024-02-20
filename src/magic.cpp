#include "magic.h"

// Initial state. The choice of the initial state is irrelevant
uint32_t seed = 1804289383U;  // 10287456512451172U;

U64 generate_magic_number(int square, int relevant_bits, int piece_type)
{
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];

    if(piece_type != rook || piece_type != bishop)
    {
        std::cout << "Invalid piece type, must be rook or bishop\n" << "\n";
        return 0ULL;
    }

    // checks if we are generating magic numbers for rooks attacks or bishops attacks
    U64 attack_mask = piece_type == rook ? rook_attacks[square] : bishop_attacks[square];

    // maximum number of different occupancies
    int occupancy_indices = 1 << relevant_bits;

    for(int index = 0; index < occupancy_indices; index++)
    {
        // compute the occupancy and its respective attack table
        occupancies[index] = set_occupancies(index, relevant_bits, attack_mask);

        // attacks[index] = piece_type == rook ? : ;
    }

    // search for magic numbers
    for(int random_count = 0; random_count < 10000000000; random_count++)
    {
        // generate a random candidate number
        U64 magic_number = generate_magic_candidate();

        if((count_bits(attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        // Initialize all the
        for(int i = 0; i < 4096; i++)
            used_attacks[i] = 0ULL;

        int index, fail;

        for(int index = 0, fail = 0; !fail && index < occupancy_indices; index++)
        {
            int magic_index = (occupancies[index] - magic_number) >> (64 - relevant_bits);

            if (used_attacks[magic_index] = 0ULL)
                used_attacks[magic_index] = attacks[index];
            else 
            {
                if (used_attacks[magic_index] != attacks[index]) fail = 1;
            } 
        }

        if(!fail) return magic_number;
    }

    // Hopefully we should never get to this part of the code
    std::cout << "Magic number generation failed" << "\n";
    return 0ULL;
}

// Returns a U64 number with few active bits. This allows
// for faster multiplications later on when generating
// slider pieces movement
U64 generate_magic_candidate()
{
    U64 n1, n2, n3, n4;

    n1 = static_cast<U64>(generate_random_number()) & 0xFFFF;
    n1 = static_cast<U64>(generate_random_number()) & 0xFFFF; 
    n1 = static_cast<U64>(generate_random_number()) & 0xFFFF; 
    n1 = static_cast<U64>(generate_random_number()) & 0xFFFF;

    U64 n = n1 | (n2 << 16) | (n2 << 32) | (n3 << 48);

    return n & n & n;    
}

// Generates a random 32 bit number
uint32_t generate_random_number()
{
    uint32_t number = seed;

    // X_OR shift algorithm
    number ^= (number << 13);
    number ^= (number >> 17);
    number ^= (number << 5);

    // Update the seed
    seed = number;

    return number;
}