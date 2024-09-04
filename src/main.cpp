#include "search.h"
#include "moveGenerator.h"
#include "evaluation.h"

#include<chrono>

int main()
{
	init_all_attacks();
	/*
	std::string trickyPos = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	std::string endgamePos = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
	std::string checkPos = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	std::string bugPos = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	std::string genericPos = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
	*/

	std::string matePos = "3k4/8/3K4/8/Q7/8/8/8 w - - 0 1";
	std::string morphy = "kbK5/pp6/1P6/8/8/8/8/R7 w - - 0 1";
	std::string pos2 = "6k1/pp4p1/2p5/2bp4/8/P5Pb/1P3rrP/2BRRN1K b - - 0 1";
	std::string pos2_1 = "r2qk2r/pb4pp/1n2Pb2/2B2Q2/p1p5/2P5/2B2PPP/RN2R1K1 w - - 1 0";
	std::string pos3 = "2r3k1/p4p2/3Rp2p/1p2P1pK/8/1P4P1/P3Q2P/1q6 b - - 0 1";
	std::string pos3_1 = "6k1/ppp2ppp/8/2n2K1P/2P2P1P/2Bpr3/PP4r1/4RR2 b - - 0 1"; 
	
	//std::string quiescenceTest = "qrr3k1/5ppp/8/8/8/2Q5/2R5/2R3K1 w - - 0 1";
	
	Game game(pos3_1);
	game.print_board();	

	int depth = 6, bestMove = 0;
	Mover m;
	//flk::Perft(game, depth);

	auto t1 = std::chrono::high_resolution_clock::now();
	int score = flk::Negamax(game, depth, -10000, 10000, bestMove);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "BestMove: ";
	m.print_move(bestMove);
	std::cout << "\n";
	std::cout << "Number of nodes searched: " << flk::nodes << "\n";

	std::cout << "Score: " << score << "\n";
	std::cout << "Found in: " << t.count() * 0.001 << " seconds\n";
	return 0;
}
