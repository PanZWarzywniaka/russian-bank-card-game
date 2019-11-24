#include <set>
#include <random>
#include <stdexcept>
#include <iterator>
#include"player.hpp"
#include"board.hpp"
#include"card.hpp"

class Game
{
    private:

    Player blue_player, red_player;
    Board my_board;
    Player* whos_turn;
    public:

    Game();
    void player_init(Player&); //inicjuje i tasuje talie graczy
    void game_start(Player&, Player&, Board&); //daje początkowo karty na stół i początkową karte z kosza
    void players_move(Player*, Board&); //funckja obsługująca ruchy gracza, do momentu zakończenia tury

};