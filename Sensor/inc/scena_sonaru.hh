#ifndef SCENA_SONARU_HH
#define SCENA_SONARU_HH

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QList>
#include <cmath>

#include "inc/daneczujnika.hh"


/*!
 * \brief Klasa odpowiedzialna za wyrysowywanie i przetrzymywanie informacji dotyczące danych z czujnika ultradźwiękowego.
 *
 * W klasie mamy pola odpowiedzialne za flagę wyboru miejsca dla urządzenia. W tej klase redefiniowana jest metoda paintEvent
 * służąca do wyrysowywania odległości od przeszkód.
 */
class Scena_Sonaru : public QWidget
{
    Q_OBJECT
public:
    int PoprzedniaOdleglosc;
    bool FlA;
    bool FlB;
    bool FlC;
    QList<int> listaA;
    QList<int> listaB;
    QList<int> listaC;
    QList<int> kat;

public:
    explicit Scena_Sonaru(QWidget *parent = nullptr);
    DaneCzujnika DaneSensora;
    virtual void paintEvent(QPaintEvent *event) override;
    void UstawFlageA(bool stan) {FlA = stan;}
    void UstawFlageB(bool stan) {FlB = stan;}
    void UstawFlageC(bool stan) {FlC = stan;}

    void PrzypiszDoListy();


};

#endif // SCENA_SONARU_HH
