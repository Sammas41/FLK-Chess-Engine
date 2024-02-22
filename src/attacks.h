#ifndef ATTACKS_H
#define ATTACKS_H

#include "general.h"

class Attacks {

    private:
        // Attack tables
        U64 pawn_attacks[COLORS][SQUARES];
        U64 knight_attacks[SQUARES];
        U64 king_attacks[SQUARES];
        U64 bishop_attacks[SQUARES][512];
        U64 rook_attacks[SQUARES][4096];
        
    public:
    
        Attacks();
        ~Attacks() { }

        // Retrieve the attacks from the tables
        U64 get_pawn_attack(int, int);
        U64 get_knight_attack(int);
        U64 get_king_attack(int);
        U64 get_bishop_attack(int, U64);
        U64 get_rook_attack(int, U64);

        // Masks
        U64 bishop_mask[SQUARES];
        U64 rook_mask[SQUARES];
        
        // Initialization functions
        void init_leaper_pieces_attacks();
        void init_slider_pieces_attacks();

        // Attacks generation
        U64 generate_pawn_attacks(int, U64);
        U64 generate_knight_attacks(U64);
        U64 generate_king_attacks(U64); 

        // Masks and occupancies generation
        U64 generate_bishop_mask(int);
        U64 generate_rook_mask(int);
        U64 generate_occupancy(int, int, U64);

        U64 generate_bishop_attacks_with_blockers(int, U64);
        U64 generate_rook_attacks_with_blockers(int, U64);

        // Magic indices generation
        int compute_magic_index(int, U64, int);

        // Constants
        const U64 NOT_A_FILE = 18374403900871474942ULL;
        const U64 NOT_H_FILE = 9187201950435737471ULL;
        const U64 NOT_GH_FILE = 4557430888798830399ULL;
        const U64 NOT_AB_FILE = 18229723555195321596ULL;
        
        const int bits_in_bishop_mask[SQUARES] = {	
            6, 5, 5, 5, 5, 5, 5, 6, 
            5, 5, 5, 5, 5, 5, 5, 5, 
            5, 5, 7, 7, 7, 7, 5, 5, 
            5, 5, 7, 9, 9, 7, 5, 5, 
            5, 5, 7, 9, 9, 7, 5, 5, 
            5, 5, 7, 7, 7, 7, 5, 5, 
            5, 5, 5, 5, 5, 5, 5, 5, 
            6, 5, 5, 5, 5, 5, 5, 6  
        };

        const int bits_in_rook_mask[SQUARES] = {
            12, 11, 11, 11, 11, 11, 11, 12, 
            11, 10, 10, 10, 10, 10, 10, 11, 
            11, 10, 10, 10, 10, 10, 10, 11, 
            11, 10, 10, 10, 10, 10, 10, 11, 
            11, 10, 10, 10, 10, 10, 10, 11, 
            11, 10, 10, 10, 10, 10, 10, 11, 
            11, 10, 10, 10, 10, 10, 10, 11, 
            12, 11, 11, 11, 11, 11, 11, 12 
        };

        // Magic numbers
        const U64 rook_magics[SQUARES] = {
            9979994641325359136ULL,
            90072129987412032ULL,
            180170925814149121ULL,
            72066458867205152ULL,
            144117387368072224ULL,
            216203568472981512ULL,
            9547631759814820096ULL,
            2341881152152807680ULL,
            140740040605696ULL,
            2316046545841029184ULL,
            72198468973629440ULL,
            81205565149155328ULL,
            146508277415412736ULL,
            703833479054336ULL,
            2450098939073003648ULL,
            576742228899270912ULL,
            36033470048378880ULL,
            72198881818984448ULL,
            1301692025185255936ULL,
            90217678106527746ULL,
            324684134750365696ULL,
            9265030608319430912ULL,
            4616194016369772546ULL,
            2199165886724ULL,
            72127964931719168ULL,
            2323857549994496000ULL,
            9323886521876609ULL,
            9024793588793472ULL,
            562992905192464ULL,
            2201179128832ULL,
            36038160048718082ULL,
            36029097666947201ULL,
            4629700967774814240ULL,
            306244980821723137ULL,
            1161084564161792ULL,
            110340390163316992ULL,
            5770254227613696ULL,
            2341876206435041792ULL,
            82199497949581313ULL,
            144120019947619460ULL,
            324329544062894112ULL,
            1152994210081882112ULL,
            13545987550281792ULL,
            17592739758089ULL,
            2306414759556218884ULL,
            144678687852232706ULL,
            9009398345171200ULL,
            2326183975409811457ULL,
            72339215047754240ULL,
            18155273440989312ULL,
            4613959945983951104ULL,
            145812974690501120ULL,
            281543763820800ULL,
            147495088967385216ULL,
            2969386217113789440ULL,
            19215066297569792ULL,
            180144054896435457ULL,
            2377928092116066437ULL,
            9277424307650174977ULL,
            4621827982418248737ULL,
            563158798583922ULL,
            5066618438763522ULL,
            144221860300195844ULL,
            281752018887682ULL
        };

        const U64 bishop_magics[SQUARES] = {
            18018832060792964ULL,
            9011737055478280ULL,
            4531088509108738ULL,
            74316026439016464ULL,
            396616115700105744ULL,
            2382975967281807376ULL,
            1189093273034424848ULL,
            270357282336932352ULL,
            1131414716417028ULL,
            2267763835016ULL,
            2652629010991292674ULL,
            283717117543424ULL,
            4411067728898ULL,
            1127068172552192ULL,
            288591295206670341ULL,
            576743344005317120ULL,
            18016669532684544ULL,
            289358613125825024ULL,
            580966009790284034ULL,
            1126071732805635ULL,
            37440604846162944ULL,
            9295714164029260800ULL,
            4098996805584896ULL,
            9223937205167456514ULL,
            153157607757513217ULL,
            2310364244010471938ULL,
            95143507244753921ULL,
            9015995381846288ULL,
            4611967562677239808ULL,
            9223442680644702210ULL,
            64176571732267010ULL,
            7881574242656384ULL,
            9224533161443066400ULL,
            9521190163130089986ULL,
            2305913523989908488ULL,
            9675423050623352960ULL,
            9223945990515460104ULL,
            2310346920227311616ULL,
            7075155703941370880ULL,
            4755955152091910658ULL,
            146675410564812800ULL,
            4612821438196357120ULL,
            4789475436135424ULL,
            1747403296580175872ULL,
            40541197101432897ULL,
            144397831292092673ULL,
            1883076424731259008ULL,
            9228440811230794258ULL,
            360435373754810368ULL,
            108227545293391872ULL,
            4611688277597225028ULL,
            3458764677302190090ULL,
            577063357723574274ULL,
            9165942875553793ULL,
            6522483364660839184ULL,
            1127033795058692ULL,
            2815853729948160ULL,
            317861208064ULL,
            5765171576804257832ULL,
            9241386607448426752ULL,
            11258999336993284ULL,
            432345702206341696ULL,
            9878791228517523968ULL,
            4616190786973859872ULL
        };
};

#endif
