#include<iostream>
#include"game.hpp"
#include <utility>


int main()
{
    Card::load_texuture(); //ładowanie arkusza, dla wszystkich obiektów
    Game gra;

    //bufory do move
    std::optional<Card> taken_card=std::nullopt;
    Deck* wherefrom_card_is_taken=nullptr; //stos z którego bierzemy // foooking british english old use
    Deck* whereto_card_is_taken=nullptr;

    while(gra.okno.isOpen())
    {
        sf::Event event;
        while (gra.okno.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                {
                    gra.okno.close();
                    break;
                }
            case sf::Event::MouseButtonPressed:
                {
                    
                    wherefrom_card_is_taken = [&]() -> Deck* /*tu odbywa się walidacja skąd możemy zabrać karte*/ 
                    {
                        sf::Vector2f point (event.mouseButton.x,event.mouseButton.y);

                        for(/*const*/Deck deck: gra.get_board().get_decks_arrays().first) //pola bankowe sprawdzamy, nigdy nie można wziąć karty z banku
                        {
                            if(deck.get_rect().contains(point)) return nullptr;
                        }

                        for(/*const*/Deck deck: gra.get_board().get_decks_arrays().second) //pola zew sprawdzamy, zawsze można wziąć karty
                        {
                            if(deck.get_rect().contains(point))
                            {
                                Deck* wskaznik = &deck;//tu jest bug ale niew wiem jak go naprawić wherefrome_car_is_taken nie dostaje dobrego wskaźnika
                                return wskaznik;
                            }
                        }

                        if(gra.get_players_pointer()->get_deck_pointer()->get_rect().contains(point)) return gra.get_players_pointer()->get_deck_pointer(); //pozwalamy na dobranie karty ze swojej tali
                        if(gra.get_players_pointer()->get_trash_pointer()->get_rect().contains(point)) return gra.get_players_pointer()->get_trash_pointer(); //pozwalamy na dobranie karty ze swojego kosza
                        if(gra.get_opponents_pointer()->get_deck_pointer()->get_rect().contains(point)) return nullptr; //nie pozwalamy na dobranie karty z tali przeciwnika
                        if(gra.get_opponents_pointer()->get_trash_pointer()->get_rect().contains(point)) return nullptr; //nie pozwalamy na dobranie karty z kosza przeciwnika

                        //jeżeli gdzie kolwiek indziej klikneliśmy
                        return nullptr;
                    }();
                    

                    if(wherefrom_card_is_taken!=nullptr)//sprawdz czy kliknąłeś karte i //sprawdzenie czy nie wzieliśmy karty od przeciwnika, ani ze strefy środkowej
                    {
                        //taken_card = std::move(wherefrom_card_is_taken.value()->top());
                        //taken_card = wherefrom_card_is_taken.value()->top();
                        //nie wiem czemu wywala błąd w tych linijkach

                        taken_card.emplace(wherefrom_card_is_taken->top()); //tu błędu nie ma
                        wherefrom_card_is_taken->pop(); //usuwa karte ze stosu 
                        taken_card.value().setPosition(event.mouseButton.x,event.mouseButton.y);
                        
                        if(wherefrom_card_is_taken->top()==taken_card.value()) throw std::runtime_error("Card obtaining Error");
                    }
                    
                    break;
                }
            case sf::Event::MouseMoved:
				{
					if(taken_card.has_value() && wherefrom_card_is_taken!=nullptr)
					{
						taken_card.value().setPosition(event.mouseButton.x,event.mouseButton.y);
						break;
					}
				}
            case sf::Event::MouseButtonReleased:
                {

                    if(taken_card.has_value() && wherefrom_card_is_taken!=nullptr)
                    {
                        //ustalamy wskaźnik do decku na miejsce gdzie jest kursor
                        whereto_card_is_taken = [&]() ->Deck* /*tu NIE odbywa się walidacja dokąd możemy dać karte, sprawdzanie robie gra.players_move()*/
                        {
                            sf::Vector2f point (event.mouseButton.x,event.mouseButton.y);

                            for(/*const*/Deck deck: gra.get_board().get_decks_arrays().first) //pola bankowe sprawdzamy
                            {
                                if(deck.get_rect().contains(point)) return &deck;
                            }

                            for(/*const*/Deck deck: gra.get_board().get_decks_arrays().second) //pola zew sprawdzamy
                            {
                                if(deck.get_rect().contains(point)) return &deck;//tu jest bug ale niew wiem jak go naprawić wherefrome_car_is_taken nie dostaje dobrego wskaźnika
                            }

                            if(gra.get_players_pointer()->get_deck_pointer()->get_rect().contains(point)) return gra.get_players_pointer()->get_deck_pointer();
                            if(gra.get_players_pointer()->get_trash_pointer()->get_rect().contains(point)) return gra.get_players_pointer()->get_trash_pointer(); 
                            if(gra.get_opponents_pointer()->get_deck_pointer()->get_rect().contains(point)) return gra.get_opponents_pointer()->get_deck_pointer();
                            if(gra.get_opponents_pointer()->get_trash_pointer()->get_rect().contains(point)) return gra.get_opponents_pointer()->get_trash_pointer(); 

                            //jeżeli gdzie kolwiek indziej klikneliśmy
                            return nullptr;
                        }();

                        if(whereto_card_is_taken) //karta została położona na jakimś decku
                        {
                            //NIE ODPALAM TEGO BO POPRZEDNI KOD NIE DZIAŁA PRAWIDŁOWO
                            //whereto_card_is_taken='wkaźnik do stosu na którym jest myszka'
                            //konstruktor move
                            //Move ruch(wherefrom_card_is_taken.value(), whereto_card_is_taken, std::move(taken_card.value()));
                            //gra.players_move(gra.get_players_pointer(),ruch);
                            //odłuż karte karte jeśli ruch się powiódł
                        }
                        else //karta została puszczona ale na złe nie na żaden deck, została położona na tle, cofamy ruch
                        {
                            wherefrom_card_is_taken->push(taken_card.value());
                            taken_card.reset();
                            wherefrom_card_is_taken=nullptr; //resetujemy bufory
                        }
                        
                    }
                    break;
                }
                
            }




        }


        //okno
        gra.okno.clear(sf::Color::Green);

        gra.okno.draw(gra);
        if(taken_card.has_value()) gra.okno.draw(taken_card.value());

        gra.okno.display();
        

    }

    return 0;
}