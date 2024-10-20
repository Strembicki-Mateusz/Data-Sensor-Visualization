#ifndef DANECZUJNIKA_HH
#define DANECZUJNIKA_HH

/*!
 * \brief Klasa DaneCzujnika
 *
 * Klasa posiada dwa pola prywatne (Pozycja oraz Odleglosc) służące do przechowywania danych na temat aktualnych danych
 * odczytanych z portu. Klasa posiada również metody umożliwiające ustawianie oraz pobieranie odpowiednich wartości z jej pól
 * prywatnych, a także kontruktor.
 */
class DaneCzujnika
{
private:
    int Pozycja;
    int Odleglosc;

public:
    DaneCzujnika();

    /*!
     * \brief WezPozycje - metoda pobierająca wartość pozycji orczyka
     * \return Pozycja
     */
    int WezPozycje() {return Pozycja;}
    /*!
     * \brief WezOdleglość - metoda pobierająca wartość odległosci od przeszkody
     * \return Odleglosc
     */
    int WezOdleglosc() {return Odleglosc;}
    /*!
     * \brief UstawPozycje - metoda ustawiająca wartość pozycji
     * \param Poz - wartość pozycji
     */
    void UstawPozycje(int Poz) {Pozycja = Poz;}
    /*!
     * \brief UstawOdleglosc - metoda ustawiająca wartość pozycji
     * \param Odl - wartość odległości
     */
    void UstawOdleglosc(int Odl) {Odleglosc = Odl;}
};

#endif // DANECZUJNIKA_HH
