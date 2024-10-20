#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QList>
#include <QSerialPort>
#include <QLabel>
#include <QIcon>
#include <QSerialPortInfo>
#include <QString>
#include <QChar>
#include <QPixmap>
#include <QTimer>
#include <QTranslator>

#include "inc/scena_sonaru.hh"


/*!
 * Definicja przestrzeni nazw dla klasy MainWindow
 */
namespace Ui {
class MainWindow;
}

/*!
 * \brief MainWindow
 *
 * Klasa dziedziczy publiczne obiekty klasy QMainWindow, aby wykorzystywać metody oraz pola w niej dostępne.
 * Klasa odpowiedzialna jest za utworzenie oraz obsługę okien aplikacji stworzony w programie designer. Została stworzona
 * w pełni automatycznie.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void PobierzDaneCzujnika();

private slots:
    void on_Polacz_clicked();
    void on_Rozlacz_clicked();
    void CzytajZPortu();

    void on_Stop_clicked();

    void on_Start_clicked();

    void on_WyborUmiejscowienia_activated(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void DodajDaneOrczyka(QString Poz);
    void DodajDaneOdleglosci(QString Odl);
    void WyslijWiadomosc(QString wiadomosc);
    QSerialPort *Urzadzenie;
    Scena_Sonaru Czujnik;
    Scena_Sonaru *ScenaSon;
    void AnimacjaSerwa(int Poz);
    virtual void changeEvent(QEvent *event);

public:
    bool FlagaPolaczenia = false;
    bool FlagaDzialania = false;
    bool FlagaMiejsca = false;
    bool FlagaA = false;
    bool FlagaB = false;
    bool FlagaC = false;
    //QTimer licznik;
};

#endif // MAINWINDOW_HH
