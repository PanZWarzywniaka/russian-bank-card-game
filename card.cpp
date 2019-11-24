#include"card.hpp"

Card::Card() //konstruktor domyślny
{
    wartosc = Value::Ace;
    kolor = Colour::spades;
}

Card::Card(Value wart, Colour kol)
{
    wartosc = wart;
    kolor = kol;
}

Card::Colour Card::get_colour() const
{
    return kolor;
}


Card::Value Card::get_value() const
{
    return wartosc;
}