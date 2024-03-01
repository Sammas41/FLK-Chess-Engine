#include "evaluation.h"

int evaluate(Game& game)
{
    int score = 0;

    // Compute the total score for the position
    score += flk::lazy_evaluation(game);

    return score; 
}

namespace flk {
  
    int lazy_evaluation(Game& game)
    {
        int lazy_score = 0;
        
        // Material evaluation
        lazy_score += eval_material(game);
        lazy_score += eval_position(game);

        return lazy_score;
    }

    int eval_material(Game& game)
    {
        int material_score = 0, square = no_sq;
        U64 bitboard = 0ULL;

        for(int piece = P; piece <= k; piece++)
        {
            bitboard = game.get_bitboard(piece);

            while(bitboard)
            {
                square = get_ls1b_index(bitboard);

                material_score += piece_score[piece];

                pop_bit(bitboard, square);
            }
        }

        return (game.get_side() == white) ? material_score : -material_score;
    }

    int eval_position(Game& game)
    {
        int square = no_sq, pos_score = 0;

        for(int piece = P; piece <= k; piece++)
        {
            if(piece == q || piece == Q) continue;

            U64 bitboard = game.get_bitboard(piece);

            while(bitboard)
            {
                square = get_ls1b_index(bitboard);

                switch (piece)
                {
                case P:
                    pos_score += pawn_positional_score[square];
                    break;
                case N:
                    pos_score += knight_positional_score[square];
                    break;
                case B:
                    pos_score += bishop_positional_score[square];
                    break;
                case R:
                    pos_score += rook_positional_score[square];
                    break;
                case K:
                    pos_score += king_positional_score[square];
                    break;
                case p:
                    pos_score -= pawn_positional_score[mirror_squares[square]];
                    break;
                case n:
                    pos_score -= knight_positional_score[mirror_squares[square]];
                    break;
                case b:
                    pos_score -= bishop_positional_score[mirror_squares[square]];
                    break;
                case r:
                    pos_score -= rook_positional_score[mirror_squares[square]];
                    break;
                case k:
                    pos_score -= king_positional_score[mirror_squares[square]];
                default:
                    break;
                }

                pop_bit(bitboard, square);
            }
        }

        return pos_score;
    }
}