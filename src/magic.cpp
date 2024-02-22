#include "magic.h"

// Initial state. The choice of the initial state is irrelevant
uint32_t seed = 1804289383U;

// Computes the magic numbers for all possible squares
void initialize_magic_numbers(Attacks & atks)
{
    std::cout << "\nRook magic numbers (a8 --> h1):\n";

    // Computes magic numbers for the rook
    for(int square = 0; square < SQUARES; square++)
    {
        std::cout << generate_magic_number(square, atks.bits_in_rook_mask[square], rook, atks)
                  << "ULL,\n";
    }

    std::cout << "\nBishop magic numbers (a8 --> h1):\n";

    // Computes magic numbers for the bishop
    for(int square = 0; square < SQUARES; square++)
    {
        std::cout << generate_magic_number(square, atks.bits_in_bishop_mask[square], bishop, atks)
                  << "ULL,\n";
    }
}

// Computes the magic number for a particular square and piece
U64 generate_magic_number(int square, int relevant_bits, int piece_type, Attacks & atks)
{
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 hashed_attacks[4096];

    if(piece_type != rook && piece_type != bishop)
    {
        std::cout << "Invalid piece type, must be rook or bishop\n" << "\n";
        return 0ULL;
    }

    // checks if we are generating magic numbers for rooks attacks or bishops attacks
    U64 attack_mask = piece_type == rook ? atks.rook_mask[square]
                                         : atks.bishop_mask[square];

    // maximum number of different occupancies
    int occupancy_indices = 1 << relevant_bits;

    for(int index = 0; index < occupancy_indices; index++)
    {
        // compute the occupancy for this particular index
        occupancies[index] = atks.generate_occupancy(index, relevant_bits, attack_mask);

        // compute the attack given the occupancy
        attacks[index] = piece_type == rook ? 
                        atks.generate_rook_attacks_with_blockers(square, occupancies[index]) :
                        atks.generate_bishop_attacks_with_blockers(square, occupancies[index]);
    }

    // search for magic numbers
    for(int random_count = 0; random_count < 10000000000; random_count++)
    {
        // generate a random candidate number
        U64 magic_number = generate_magic_candidate();

        // skips bad candidates
        if(count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        // Initialize the hashed attack table
        for(int i = 0; i < 4096; i++)
            hashed_attacks[i] = 0ULL;

        int index, fail;

        // test if the magic number produces no collisions
        for(index = 0, fail = 0; !fail && index < occupancy_indices; index++)
        {
            // generate the key for a particular occupancy using the magic number
            int magic_index = static_cast<int>((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if the key maps the current occupancy to an empty bitboard, link that 
            // bitboard to the corresponding attack
            if (hashed_attacks[magic_index] == 0ULL)
                hashed_attacks[magic_index] = attacks[index];
            else 
            {
                // if the key maps the current occupancy to a non-empty board and the two bitboards do
                // not coincide --> collision. We must change the magic number
                if (hashed_attacks[magic_index] != attacks[index]) fail = 1;
            }
        }

        // if there are no collisions then we have found the magic number
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
    U64 n = generate_random_64bit_number() &
            generate_random_64bit_number() &
            generate_random_64bit_number();
    return n;
}

// Generate a random 64 bits number using the 32 bits RNG
U64 generate_random_64bit_number()
{
    U64 n1, n2, n3, n4;

    n1 = static_cast<U64>(generate_random_number() & 0xFFFF);
    n2 = static_cast<U64>(generate_random_number() & 0xFFFF); 
    n3 = static_cast<U64>(generate_random_number() & 0xFFFF); 
    n4 = static_cast<U64>(generate_random_number() & 0xFFFF);

    U64 n = n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);

    return n;    
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