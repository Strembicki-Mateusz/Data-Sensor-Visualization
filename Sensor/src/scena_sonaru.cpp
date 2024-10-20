#include "inc/scena_sonaru.hh"

#include <iostream>

/*!
 * \brief Konstruktor klasy Scena_Sonaru
 *
 * Przypisuje początkowe wartości do pól przy tworzeniu obiektu klasy.
 */
Scena_Sonaru::Scena_Sonaru(QWidget *parent) : QWidget(parent)
{
    PoprzedniaOdleglosc = 0;
    FlA = false;
    FlB = false;
    FlC = false;

}

void Scena_Sonaru::PrzypiszDoListy()
{
    if(FlA == true)
    {
        listaA.append(DaneSensora.WezOdleglosc());
        listaB.append(0);
        listaC.append(0);
        if((DaneSensora.WezPozycje() == 90) && (!listaA.isEmpty()))
        {
            listaA.clear();
            listaB.clear();
            listaC.clear();
        }
    }
    else if(FlB == true)
    {
        listaB.append(DaneSensora.WezOdleglosc());
        listaA.append(0);
        listaC.append(0);
        if((DaneSensora.WezPozycje() == 90) && (!listaB.isEmpty()))
        {
            listaA.clear();
            listaB.clear();
            listaC.clear();
        }
    }
    else if(FlC == true)
    {
        listaC.append(DaneSensora.WezOdleglosc());
        listaB.append(0);
        listaA.append(0);
        if((DaneSensora.WezPozycje() == 90) && (!listaC.isEmpty()))
        {
            listaA.clear();
            listaB.clear();
            listaC.clear();
        }
    }

    kat.prepend(DaneSensora.WezPozycje());
    if((DaneSensora.WezPozycje() == 90) && (!kat.isEmpty()))
    {
        kat.clear();
    }
}

/*!
 * \brief Metoda virtualna redefiniująca metodę paintEvent
 * \param event = zdarzenie
 *
 * W metodzie tej odrysowywane są linie, które wizualizują odległość w danej pozycji od przeszkody. Metoda ta korzysta z klasy QPainter
 * do wyrysowywania odpowiednich kształtów. W tym przypadku są to linie. Metoda ta rozpoznaje również wybrane miejsce urządzenia
 * i wyrysowuje pomiary dla odpowiednich umiejscowień przyrządu.
 */
void Scena_Sonaru::paintEvent(QPaintEvent *event)
{

    //QWidget::paintEvent(event);
    QPainter Rysownik;
    QImage Zielony(":/znaczki/ziel");
    QImage Czerwony(":/znaczki/czer");
    QImage Zolty(":/znaczki/zol");
    int start_rys = 45*16;
    int koniec_rys = 45*16;



    QRectF prostokat2(width()/4, height()/10, 3*width()/6, 3*height()/4); // środkowy
    //QRectF prostokat(width()/2.75, height()/2, 3*width()/12, 3*height()/8); // środkowy

    QRectF prostokat1(width()/50, height()/10, 3*width()/6, 3*height()/4); // lewy
    //QRectF prostokat1(width()/7.5, height()/2, 3*width()/12, 3*height()/8); // lewy

    QRectF prostokat3(width()/1.95, height()/10, 3*width()/6, 3*height()/4); // prawy
    //QRectF prostokat( width()/(1.95- (0.001 * (1000 - DaneSensora->WezOdleglosc()))) , height()/(10-(0.01 * (1000 - DaneSensora->WezOdleglosc()))), 3*width()/(6*(1000/DaneSensora->WezOdleglosc())), 3*height()/(4*(1000/DaneSensora->WezOdleglosc()))); // prawy
    //QRectF prostokat( width()/(1.55) , height()/(2), 3*width()/12, 3*height()/(8)); // prawy


    QRectF prostokat( width()/(1.55) , 29*height()/20, 3*width()/12, 3*height()/(8)); // prawy


/*
    Rysownik.begin(this);


    Rysownik.setPen(Qt::magenta);
    Rysownik.drawArc(prostokat3, start_rys, koniec_rys);
    Rysownik.setPen(Qt::red);
    Rysownik.drawArc(prostokat1, start_rys, koniec_rys);
    Rysownik.setPen(Qt::green);
    Rysownik.drawArc(prostokat2, start_rys, koniec_rys);
*/

    //std::cout << FlA << " " << FlB << " " << FlC << "dziala" << std::endl;
    Rysownik.begin(this);
    if(FlA == true)
    {
        Rysownik.translate(width()/4, height());
        Rysownik.rotate(-45-DaneSensora.WezPozycje());
        Rysownik.setPen(Qt::red);
        Rysownik.drawLine(0,0,DaneSensora.WezOdleglosc()/5,0);
        for(int i = 0; i<listaA.length(); ++i)
            Rysownik.drawImage((int)(cos((kat[i])*6.28/360)*listaA[i]/5), (int)(sin((kat[i])*6.28/360)*listaA[i]/5)-5, Czerwony);
    }
    else if(FlB == true)
    {
        Rysownik.translate(width()/2, height());
        Rysownik.rotate(-45-DaneSensora.WezPozycje());
        Rysownik.setPen(Qt::green);
        Rysownik.drawLine(0,0,DaneSensora.WezOdleglosc()/5,0);
        for(int i = 0; i<listaB.length(); ++i)
            Rysownik.drawImage((int)(cos((kat[i])*6.28/360)*listaB[i]/5), (int)(sin((kat[i])*6.28/360)*listaB[i]/5)-5, Zielony);
    }
    else if(FlC == true)
    {
        Rysownik.translate(3*width()/4, height());
        Rysownik.rotate(-45-DaneSensora.WezPozycje());
        Rysownik.setPen(Qt::yellow);
        Rysownik.drawLine(0,0,DaneSensora.WezOdleglosc()/5,0);
        for(int i = 0; i<listaC.length(); ++i)
            Rysownik.drawImage((int)(cos((kat[i])*6.28/360)*listaC[i]/5), (int)(sin((kat[i])*6.28/360)*listaC[i]/5)-5, Zolty);
        //Rysownik.translate(listaC[i],-5);
    }

    Rysownik.end();
}
