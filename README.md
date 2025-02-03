# Fried Liver King Official Chess Engine

Fried Liver King (FLK) is a chess engine rated
approximately 2500 / 2600 ELO (Grandmaster level). It has been built
as a personal project by Me and [Maresciallo Nino](https://github.com/marescialloNino).
The engine features a variety of techniques such as:

- [Bitboards](https://www.chessprogramming.org/Bitboards)
- [Negamax Search with Alpha-Beta Pruning](https://www.chessprogramming.org/Negamax)
- [Quiescence Search](https://www.chessprogramming.org/Quiescence_Search)
- [Principal Variation Search](https://www.chessprogramming.org/Principal_Variation_Search)
- [Move Ordering](https://www.chessprogramming.org/Move_Ordering), [Killer Moves](https://www.chessprogramming.org/Killer_Heuristic) and [History Moves](https://www.chessprogramming.org/History_Heuristic) heuristics
- [Late Move Reduction](https://www.chessprogramming.org/Late_Move_Reductions)
- [Null Move Pruning](https://www.chessprogramming.org/Null_Move_Pruning)
- [Iterative Deepening](https://www.chessprogramming.org/Iterative_Deepening)

## Build

The engine can be build directly from source or thorugh the
MakeFile provided here. 

**Note**: some compilers compile only
the main file, remeber to instruct the compiler to compile all the `*.cpp` files and not only `main.cpp` when building from source

## Instructions

When the engine is running type `help` to see all
available commands. Moves need to be typed according to the following syntax:

1. Specify the **piece** that you want to move:
    
    - Knight --> N
    - Bishop --> B
    - Rook --> R
    - Queen --> Q
    - King --> K
    - Pawns do not need any specification

2. Specify the **starting square** and the **landing square** (with no whitespaces in between)

3. If the move is a **capture** add the letter `x` after the landing square

4. If the move is an **en passant capture** add the letter `p` after the landing square

5. **Castling** can be done by simply typing `0-0` for short castling and `0-0-0` for long castling

6. For **pawn promotions** specify the piece you want to promote to after the landing square, following the encoding specified in point 1

Examples:

- **e2e4** moves a pawn from e2 to e4
- **Ng1f3** moves a knight from g1 to f3
- **Bc4f7x** moves a bishop from c4 to f7 and captures the piece standing on f7
- **d5c6p** moves a pawn from d5 to c6 while performing an en passant capture
- **0-0** performs a short castle (if allowed)
- **g7g8Q** moves a pawn from g7 to g8 and promotes it to a queen

It is possible to **undo** a move by typing `undo` or `undo-x` where `x` is an integer to undo the last `x` moves

## Currently Working On:

- Simple GUI to enhance user experience
- Transposition Tables using Zobrist Hashing
- Improving Evaluation function
