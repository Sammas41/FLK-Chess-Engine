#include "evaluation.h"

int evaluate(Game& game)
{
    int score = 0;

    // Compute the total score for the position
    score += flk::lazy_evaluation(game);

    return score; 
}

namespace flk {

    U64 file_masks[64];
    U64 rank_masks[64];
    U64 isolated_masks[64];
    U64 black_passed_masks[64];
    U64 white_passed_masks[64];

    U64 set_file_rank_mask(int file_number, int rank_number)
    {
        U64 mask = 0ULL;
        for(int rank = 0; rank < 8; rank++)
        {
            for(int file = 0; file < 8; file++)
            {
                int square = rank * 8 + file;


                if (file_number != -1){

                    if (file == file_number){
                        set_bit(mask, square);
                    }
                    
                }else if (rank_number != -1){

                    if (rank == rank_number){
                        set_bit(mask, square);
                    }
                }        
            }
        }
        return mask;       
    }


    void init_evaluation_masks(){
        

        for (int rank = 0; rank < 8; rank++){
            for (int file = 0; file < 8; file++){

                int square = rank * 8 + file;
                
                file_masks[square] = set_file_rank_mask(file, -1);
                rank_masks[square] = set_file_rank_mask(-1 , rank);

                isolated_masks[square] = set_file_rank_mask(file -1, -1);
                isolated_masks[square] |= set_file_rank_mask(file +1, -1);
            }
        }

        for (int rank = 0; rank < 8; rank++){
            for (int file = 0; file < 8; file++){

                int square = rank * 8 + file;

                // white passed pawn masks
                white_passed_masks[square] = set_file_rank_mask(file - 1, -1);
                white_passed_masks[square] |= set_file_rank_mask(file, -1);
                white_passed_masks[square] |= set_file_rank_mask(file + 1, -1);

                //loop over reduntant ranks for passed pawns masks
                for (int i = 0; i < (8 - rank); i++){
                    white_passed_masks[square] &=  ~rank_masks[(7 - i) * 8 + file];
                }

                // black passed pawn masks
                black_passed_masks[square] = set_file_rank_mask(file - 1, -1);
                black_passed_masks[square] |= set_file_rank_mask(file, -1);
                black_passed_masks[square] |= set_file_rank_mask(file + 1, -1);


                //loop over reduntant ranks for passed pawns masks
                for (int i = 0; i < rank + 1; i++){
                    black_passed_masks[square] &=  ~rank_masks[i * 8 + file];
                }
            }
        }
    };


    int lazy_evaluation(Game& game)
    {
        int lazy_score = 0;

        // Material and positional evaluation
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
        int double_pawns = 0;

        for(int piece = P; piece <= k; piece++)
        {
            if(piece == q || piece == Q) continue;

            U64 bitboard = game.get_bitboard(piece);

            while(bitboard)
            {
                square = get_ls1b_index(bitboard);

                switch (piece)
                {
                case P:{
                    // positional score
                    pos_score += pawn_positional_score[square];

                    //double pawn penalty
                    double_pawns = count_bits(bitboard & file_masks[square]);
                    if (double_pawns >= 2){
                        pos_score += double_pawn_penalty * double_pawns;
                    } 

                    // isolated pawn penalty
                    if ((bitboard & isolated_masks[square])==0)
                        pos_score += isolated_pawn_penalty; 

                    // passed pawn bonus
                    if ((white_passed_masks[square] & game.get_bitboard(p))==0)
                        pos_score += passed_pawn_bonus[get_rank[square]];

                    break;
                }
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
                {
                    pos_score -= pawn_positional_score[mirror_squares[square]];

                    double_pawns = count_bits(bitboard & file_masks[square]);
                    if (double_pawns >= 2){
                        pos_score -= double_pawn_penalty * double_pawns;
                    } 
                    // isolated pawn penalty
                    if ((bitboard & isolated_masks[square])==0)
                        pos_score -= isolated_pawn_penalty; 

                    // passed pawn bonus
                    if ((black_passed_masks[square] & game.get_bitboard(P))==0)
                        pos_score -= passed_pawn_bonus[get_rank[mirror_squares[square]]];

                    break;
                }
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

        return (game.get_side() == white) ? pos_score : -pos_score;
    }
}