#include "move.h"

// Constructors:
// Directly
Move::Move(int source, int target, int piece, int promoted_piece,
           int capture, int double_push, int en_passant, int castling) {
    move = encode_move(source, target, piece, promoted_piece, capture,
                       double_push, en_passant, castling);
}

// From a string
Move::Move(std::string move_string, int side) {

    // Castling moves
    if((move_string == "0-0" || move_string == "O-O") && side == white) {
        move = encode_move(e1, g1, K, 0, 0, 0, 0, 1);
        return;
    }

    if((move_string == "0-0-0" || move_string == "O-O-O") && side == white) {
        move = encode_move(e1, c1, K, 0, 0, 0, 0, 1);
        return;
    }

    if((move_string == "0-0" || move_string == "O-O") && side == black) {
        move = encode_move(e8, g8, k, 0, 0, 0, 0, 1);
        return;
    }

    if((move_string == "0-0-0" || move_string == "O-O-O") && side == black) {
        move = encode_move(e8, c8, k, 0, 0, 0, 0, 1);
        return;
    }

    // Flags
    int source = 0, target = 0, 
        piece = 0, promoted_piece = 0, 
        double_push = 0, capture = 0, en_passant = 0;

    // Checks if it is a capture
    size_t found = move_string.find("x");
    if(found != std::string::npos) {
        // Set the capture flag an delete the capture character
        capture = 1;
        move_string.erase(found, 1);
    }

    // Check if it is an en passant capture
    found = move_string.find("p");
    if(found != std::string::npos) {
        // Set the en passant flag an delete the en passant character
        en_passant = 1;
        move_string.erase(found, 1);
    }

    // Now we can treat this string as a normal string.
    // If the move has length = 4 then it must be a pawn move
    if(move_string.length() == 4) {
        if(side == white)
        {
            piece = P;
            
            // Check if it is a double pawn push (white version)
            if(static_cast<int>(move_string[3] - '0') - static_cast<int>(move_string[1] - '0') == 2)
                double_push = 1;
        }
        else
        {
            piece = p;

            // Check if it is a double pawn push (black version)
            if(static_cast<int>(move_string[1] - '0') - static_cast<int>(move_string[3] - '0') == 2)
                double_push = 1;
        }

        // Get the starting square
        source = static_cast<int>(move_string[0] - 'a') + 
                (8 - static_cast<int>(move_string[1] - '0')) * 8;

        // Get the landing square
        target = static_cast<int>(move_string[2] - 'a')  + 
                (8 - static_cast<int>(move_string[3] - '0')) * 8;

        move = encode_move(source, target, piece, promoted_piece, capture, double_push, en_passant, 0);
        return;
    }
    // If move_string has length = 5 then either we moved a piece or
    // we have a pawn promotion
    if(move_string.length() == 5) {
        // If the last character is a digit then it is
        // a piece move
        if(isdigit(move_string[4]))
        {
            if(side == white)
                piece = char_pieces[move_string[0]];
            else
                piece = char_pieces[tolower(move_string[0])];

            // Get the starting square
            source = static_cast<int>(move_string[1] - 'a') + 
                (8 - static_cast<int>(move_string[2] - '0')) * 8;

            // Get the landing square
            target = static_cast<int>(move_string[3] - 'a')  + 
                     (8 - static_cast<int>(move_string[4] - '0')) * 8;
        }
        // If the last character is a digit then it is
        // a piece move
        else {
            if(side == white)
            {
                piece = P;
                promoted_piece = char_pieces[move_string[4]];
            }
            else
            {
                piece = p;
                promoted_piece = char_pieces[tolower(move_string[4])];
            }

            // Get the starting square
            source = static_cast<int>(move_string[0] - 'a') + 
                     (8 - static_cast<int>(move_string[1] - '0')) * 8;

            // Get the landing square
            target = static_cast<int>(move_string[2] - 'a')  + 
                     (8 - static_cast<int>(move_string[3] - '0')) * 8;
        }
        move = encode_move(source, target, piece, promoted_piece, capture, double_push, en_passant, 0);
    }
}

/*   Move encoding
 *           
 *         binary move bits                               hexidecimal 
 *   
 *   0000 0000 0000 0000 0011 1111    source square       0x3f
 *   0000 0000 0000 1111 1100 0000    target square       0xfc0
 *   0000 0000 1111 0000 0000 0000    piece               0xf000
 *   0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
 *   0001 0000 0000 0000 0000 0000    capture flag        0x100000
 *   0010 0000 0000 0000 0000 0000    double push flag    0x200000
 *   0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
 *   1000 0000 0000 0000 0000 0000    castling flag       0x800000
 */

// Encodes a move as in the format above
int Move::encode_move(int source,
                  int target,      
                  int piece,      
                  int promoted_piece,
                  int capture,   
                  int double_push, 
                  int en_passant,  
                  int castling) {
    
    return             source |
                (target << 6) |
                (piece << 12) |
       (promoted_piece << 16) |
              (capture << 20) |
          (double_push << 21) |
           (en_passant << 22) |
             (castling << 23) ;
}

// Getters
int Move::get_source_square() {
    return move & 0x3f;
}

int Move::get_target_square() {
    return (move & 0xfc0) >> 6;
}

int Move::get_piece_moved() {
    return (move & 0xf000) >> 12;
}

bool Move::is_capture() {
    return move & 0x100000;
}

bool Move::is_double_push() {
    return move & 0x200000;
}

bool Move::is_en_passant() {
    return move & 0x400000;
}

bool Move::is_castling() {
    return move & 0x800000;
}

int Move::get_promoted_piece() {
    return (move & 0xf0000) >> 16;
}

// Print function
void Move::print_move(bool all_info) {
    printf("%s%s%c", square_to_coordinates[get_source_square()],
                       square_to_coordinates[get_target_square()],
                       promoted_pieces[get_promoted_piece()]);
    if(all_info) {
        std::cout << " (piece moved: " << int_to_char_pieces[get_piece_moved()]
                  << " | promotion: " << (get_promoted_piece() ? "Yes | " : "No | ")
                  << "capture: " << (is_capture() ? "Yes | " : "No | ")
                  << "double push: " << (is_double_push() ? "Yes | " : "No | ")
                  << "en passant: " << (is_en_passant() ? "Yes | " : "No | ")
                  << "castling: " << (is_castling() ? "Yes)" : "No)");
    }
}