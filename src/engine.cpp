#include "engine.h"

Engine::Engine() {
    
}

Engine::Engine(std::string fen) : game(fen) {
    
}

void Engine::play(char side) {

    if(side == 'w' || side == 'W')
    {
        while(is_running())
        {
            // Print board
            game.print_board();

            // Read player's move
            int player_move = get_player_move(white);
            
            // Make player's move
            game.make_move(player_move);
        }
    }
    else
    {
        if (side == 'b' || side == 'B')
        {
            int player_move = get_player_move(black);
        }
        else
            std::cout << "[ERROR] Chose a valid side\n";
    }   
}

bool Engine::is_running() {
    return true;
}

int Engine::get_player_move(int colour) {
    std::string string_move;

    std::cout << "Insert the move you want to play (Piece + starting square + landing square):\n";
    std::cin >> string_move;

    int player_move = string_to_int(string_move, colour);

    return player_move;
}

int Engine::string_to_int(std::string string_move, int colour) {

    // Blank move
    int move = 0;

    int source_square = 0,
        landing_square = 0,
        piece = 0, 
        promotion = 0, 
        capture = 0,
        double_push = 0,
        en_passant = 0, 
        castle = 0;

    // Castling moves are the easiest to detect
    //if(string_move == "0-0" || string_move == "O-O")
        //return short_castle;

    // If the move has four characters then we have moved a pawn
    if(string_move.length() == 4)
    {
        if(colour == white)
        {
            piece = P;
            
            // Check if it is a double pawn push (white version)
            if(static_cast<int>(string_move[3] - '0') - static_cast<int>(string_move[1] - '0') == 2)
                double_push = 1;
        }
        else
        {
            piece = p;

            // Check if it is a double pawn push (black version)
            if(static_cast<int>(string_move[1] - '0') - static_cast<int>(string_move[3] - '0') == 2)
                double_push = 1;
        }

        // Get the starting square
        source_square = static_cast<int>(string_move[0] - 'a') + 
                            (8 - static_cast<int>(string_move[1] - '0')) * 8;

        // Get the landing square
        landing_square = static_cast<int>(string_move[2] - 'a')  + 
                            (8 - static_cast<int>(string_move[3] - '0')) * 8;

        // Check if it is an en passant capture
        if(string_move[0] != string_move[2])
        {
            capture = 1;
            en_passant = 1;
        }
    }
    else
    {
        // If the last character is a digit then it
        // is a piece move. Ex: Nb1c3
        if( isdigit(string_move[4]) )
        {
            if(colour == white)
                piece = char_pieces[string_move[0]];
            else
                piece = char_pieces[tolower(string_move[0])];

            // Get the starting square
            int source_square = static_cast<int>(string_move[1] - 'a') + 
                                (8 - static_cast<int>(string_move[2] - '0')) * 8;

            // Get the landing square
            int landing_square = static_cast<int>(string_move[3] - 'a')  + 
                                (8 - static_cast<int>(string_move[4] - '0')) * 8;
        }
        // If the last character is a letter then it
        // is a pawn promotion
        else
        {
            if(colour == white)
            {
                piece = P;
                promotion = char_pieces[string_move[4]];
            }
            else
            {
                piece = p;
                promotion = char_pieces[tolower(string_move[4])];
            }
        }
    }

    // Compose the move
    move =       source_square |
                (landing_square << 6) |
                (piece << 12) |
             (promotion << 16) |
              (capture << 20) |
           (double_push << 21) |
            (en_passant << 22) |
             (castle << 23) ;

    std::cout << "Source: " << source_square << "\n";
    std::cout << "Target: " << landing_square << "\n";
    std::cout << "Piece: " << piece << "\n";
    std::cout << "Promotion: " << promotion << "\n";
    std::cout << "Capture: " << capture << "\n";
    std::cout << "Double push: " << double_push << "\n";
    std::cout << "En passant: " << en_passant << "\n";
    std::cout << "Castle: " << castle << "\n";

    return move;
}