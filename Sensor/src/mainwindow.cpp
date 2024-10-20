#include "inc/mainwindow.hh"
#include "ui_mainwindow.h"
#include "inc/scena_sonaru.hh"


/*!
 * \brief MainWindow::MainWindow
 * \param parent
 *
 * Kontruktor, który pozwala na utworzenie okna dla aplikacji.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Q_INIT_RESOURCE(zasoby);
    this->Urzadzenie = new QSerialPort(this);
    ui->WyborUmiejscowienia->addItem("A");
    ui->WyborUmiejscowienia->addItem("B");
    ui->WyborUmiejscowienia->addItem("C");
    ui->comboBox->addItem(tr("Polski"));
    ui->comboBox->addItem(tr("Angielski"));

    ScenaSon = ui->widget;
    connect(this->Urzadzenie, SIGNAL(readyRead()), this, SLOT(CzytajZPortu()));


    //connect(&licznik, SIGNAL(timeout()), this, SLOT(CzytajZPortu())); // test
}

/*!
 * \brief MainWindow::~MainWindow
 *
 * Destruktor, który usuwa istnienie okna aplikacji w momencie zakończenia pracy programu,
 * co jest równoznaczne z zamknięciem okna aplikacji.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Slot reakcji na przyciśnięcie przycisku.
 *
 * Po naciśnięciu tego przycisku następuje otwarcie portu szeregowego.
 * Następuje przeszukiwanie dostępnych urządzeń dołączonych do urządzenia.
 * Pętla przeszukuje czy mamy podpięte urządzenie pod dany port o danej nazwie.
 * Po tej operacji następuje przypisanie nazwy portu.
 *
 * Kolejnie otwierany jest wybrany port szeregowy tlyko do odczytu danych.
 * Następuje również ustawienie odpowiednich parametrów do komunikacji i zostaje wyświetlony odpowiedni komunikat.
 */
void MainWindow::on_Polacz_clicked()
{
    qDebug() << tr("Szukam urzadzen");

    const QList<QSerialPortInfo> Urzadzenia = QSerialPortInfo::availablePorts();

    QString NazwaPortu;
    for(int i = 0; i < Urzadzenia.count(); i++)
    {
        qDebug() << Urzadzenia.at(i).portName() << Urzadzenia.at(i).description();
        if((Urzadzenia.at(i).portName() == "ttyUSB0") && (Urzadzenia.at(i).description() == "USB Serial"))
        {
            QString Nazwa = Urzadzenia.at(i).portName() + "\t" + Urzadzenia.at(i).description();
            QStringList ListaPortow = Nazwa.split("\t");
            NazwaPortu = ListaPortow.first();
        }
    }
    this->Urzadzenie->setPortName(NazwaPortu);

    //licznik.setInterval(500); // test
    //licznik.start(); // test
    //return; // test

    //Otwórz i skonfiguruj port
    if(!Urzadzenie->isOpen())
    {
        if(Urzadzenie->open(QSerialPort::ReadWrite))
        {
            this->Urzadzenie->setBaudRate(QSerialPort::Baud9600);
            this->Urzadzenie->setDataBits(QSerialPort::Data8);
            this->Urzadzenie->setParity(QSerialPort::NoParity);
            this->Urzadzenie->setStopBits(QSerialPort::OneStop);
            this->Urzadzenie->setFlowControl(QSerialPort::NoFlowControl);

            qDebug() << tr("Otwarto port szeregowy :>");
            //connect(this->Urzadzenie, SIGNAL(readyRead()), this, SLOT(CzytajZPortu()));
            FlagaPolaczenia = 1;
            ui->Dioda->setPixmap(QPixmap(":/diody/zielona"));
        }
        else
        {
            qDebug() << tr("Otwarcie porty szeregowego się nie powiodło :<");
        }
    }
    else
    {
        qDebug() << tr("Port jest juz otwarty!");
        return;
    }
}

/*!
 * \brief Slot reakcji na przyciśnięcie przycisku.
 *
 * Po naciśnięciu tego przycisku następuje zamykanie portu szeregowego.
 * Jeśli ten port nie jest otwarty, w terminalu ukaże się odpowiedni komunikat.
 */
void MainWindow::on_Rozlacz_clicked()
{
    if(this->Urzadzenie->isOpen() && FlagaDzialania == 0)
    {
      this->Urzadzenie->close();
      qDebug() << tr("Zamykam port.");
      FlagaPolaczenia = 0;
      ui->Dioda->setPixmap(QPixmap(":/diody/czerwona"));
    }
    else
    {
      qDebug() << tr("Port nie jest otwarty lub urządzenie wciąż działa!");
      return;
    }
}

/*!
 * \brief Metoda pozwalająca na wypisywanie danych na temat pozycji orczyka
 * \param Poz - pozycja orczyka w stopniach
 *
 * Prosta metoda umożliwiająca łatwe wypisywanie pozycji orczyka serwomechanizmu
 * zawsze w taki sam sposób, tzn. "Pozycja: x", gdzie w miejsce x wpisywana jest wartość
 */
void MainWindow::DodajDaneOrczyka(QString Poz)
{
    ui->DanePozycjiOrczyka->append(tr("Pozycja: %1").arg(Poz));
}

/*!
 * \brief Metoda pozwalająca na wypisywanie danych na temat odległości od przeszkody
 * \param Odl - odleglosc w cm.
 *
 * Prosta metoda umożliwiająca łatwe wypisywanie odległości, w cm, od obiekt
 * zawsze w taki sam sposób, tzn. "Odleglosc: x cm", gdzie w miejsce x wpisywana jest wartość.
 */
void MainWindow::DodajDaneOdleglosci(QString Odl)
{
    ui->TekstDaneOOdleglosci->append(tr("Odleglosc: %1").arg(Odl)); //+ "cm");
}

/*!
 * \brief Metoda umożliwiająca czytanie z portu szeregowego.
 *
 * Metoda umożliwia odczytanie danych wysyłanych przez urządzenie. Ramka danych ma postać: "#,int,int,!B", gdzie
 * pierwszy int to pozycja orczyka, a drugi odległość od przeszkody. Metoda sprawdza czy port szeregowy został otwarty,
 * następnie sprawdza czy możliwe jest odczytanie ramki. Jeśli nie to wyświetlony zostaje odpowiedni komunikat. W przeciwnym
 * wypadku czyta dane z ramki i odpowiednio je sprawdza. Jednocześnie na tym etapie wykonywane jest wypisywanie danych
 * w odpowiednich okienkach w aplikaji oraz realizowana jest animacja działania serwomechanizmu.
 */

/* Napisy na qDebug() napisy - podejrzane sekwencje przed i po readlinie, 153 linia*/
void MainWindow::CzytajZPortu()
{
    //FlagaPolaczenia = 1; // test
    if(FlagaPolaczenia == 1)
    {
        while(this->Urzadzenie->canReadLine())
        //bool petla = true; // test
        {
            QString Linia = this->Urzadzenie->readLine(); // sprawdzenie długości (rozmiaru) linii !!!
            //petla = false; // test

            //QString Linia = "#,55,156,!B";
            QString pom;

            pom.append(Linia[0]);
            pom.append(Linia[9]);
            pom.append(Linia[10]);

            if(Linia[0] != "#")
            {
                qDebug() << tr("Nie moje dane");
                break;
            }

            else
            {
                pom.clear();
                pom.append(Linia[2]);
                pom.append(Linia[3]);
                Czujnik.DaneSensora.UstawPozycje(pom.toInt());
                ScenaSon->DaneSensora.UstawPozycje(pom.toInt());
                AnimacjaSerwa(Czujnik.DaneSensora.WezPozycje());
                this->DodajDaneOrczyka(pom);

                pom.clear();
                pom.append(Linia[5]);
                pom.append(Linia[6]);
                pom.append(Linia[7]);
                pom.append(Linia[8]);

                Czujnik.DaneSensora.UstawOdleglosc(pom.toInt());
                ScenaSon->DaneSensora.UstawOdleglosc(pom.toInt());
                ScenaSon->PrzypiszDoListy(); //test
                this->DodajDaneOdleglosci(pom);
                ScenaSon->update();
                //qApp->processEvents();
                //qDebug() << "Działa?";
            }
        }
    }
    else
    {
        qDebug() << tr("Nie udało się odczytać danych");
    }
    //qDebug() << "Coś";
}

/*!
 * \brief Metoda służąca do animacji działania serwomechanizmu.
 * \param Poz - pozycja
 *
 * Prosta metoda realizująca animacje działania serwomechanizmu w oparciu o podaną pozycje orczyka.
 */
void MainWindow::AnimacjaSerwa(int Poz)
{
    switch(Poz)
    {
        case 0:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo0"));
            break;
        case 5:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo5"));
            break;
        case 10:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo10"));
            break;
        case 15:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo15"));
            break;
        case 20:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo20"));
            break;
        case 25:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo25"));
            break;
        case 30:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo30"));
            break;
        case 35:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo35"));
            break;
        case 40:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo40"));
            break;
        case 45:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo45"));
            break;
        case 50:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo50"));
            break;
        case 55:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo55"));
            break;
        case 60:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo60"));
            break;
        case 65:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo65"));
            break;
        case 70:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo70"));
            break;
        case 75:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo75"));
            break;
        case 80:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo80"));
            break;
        case 85:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo85"));
            break;
        case 90:
            ui->AnimacjaSerwa->setPixmap(QPixmap(":/serwo/serwo90"));
            break;
        default: qDebug() << tr("Złe dane!");
    }
}

/*!
 * \brief Metoda pozwalająca wysyłać wiadomości do mikrokontroler
 * \param wiadomosc - zmienna typu QString przechuwująca tekst do wysłania
 *
 * Metoda ta pozwala na przesył danych z aplikacji do mikrokontrolera. Sprawdza czy jest nawiązane połączenie oraz
 * czy można jest możliwy zapis danych na port komunikacyjny. Następnie następuje zapis wiadomości po wcześniejszej
 * konwersji typu QString na tablice znakową.
 */
void MainWindow::WyslijWiadomosc(QString wiadomosc)
{
    if((FlagaPolaczenia == 1) && (FlagaMiejsca == 1) && this->Urzadzenie->isWritable())
    {
        this->Urzadzenie->write(wiadomosc.toStdString().c_str());
    }
    else
    {
        qDebug() << tr("Nie mogę wysłać wiadomości. Port nie jest otwarty!");
    }
}

/*!
 * \brief Slot obsługi przycisku STOP
 *
 * Po wciśnięciu tego przycisku następuje przesłanie informacji (o kodzie 9) do sterownika, który
 * odczytając ten kod powinien zatrzymać działanie urządzenia. Dodatkowo ustawiana jest flaga działania urządzenia na 0.
 */
void MainWindow::on_Stop_clicked()
{
    this->WyslijWiadomosc("9");
    FlagaDzialania = false;
    FlagaMiejsca = false;
}

/*!
 * \brief Slot obsługi przycisku START
 *
 * Po wciśnięciu tego przycisku następuje przesłanie informacji (o kodzie 1) do sterownika, który
 * odczytując ten kon powinien uruchomić urządzenie. Dodatkowo ustawia flagę działania urządzenia na 1.
 */
void MainWindow::on_Start_clicked()
{
    if(FlagaMiejsca == true)
    {
        this->WyslijWiadomosc("1");
        FlagaDzialania = true;
    }
}

/*!
 * \brief Slot odpowiedzialny za wybór umiejscowienia serwa w aplikajci.
 * \param arg1 - parametr odpowiedzialny za przechowywanie opcji.
 *
 * W slocie tym następuje wybór odpowiedniego umiejscowienia. Od wyboru zależy, którą flagę włączymy. Następnie podświetlane
 * jest miejsce w aplikacji, gdzie sensor się znajduje. Kolor zielony określa wybraną pozycję, a czerwony - dezaktywowaną.
 * Będzie to pomocne przy wyrysowywaniu danych sensora, aby określić początek układu współrzędnych dla nowego umiejscowienia.
 */
void MainWindow::on_WyborUmiejscowienia_activated(const QString &arg1)
{
    if(arg1 == "A")
    {
        if(FlagaB == true || FlagaC == true)
        {
            FlagaB = false;
            ui->MiejsceB->setPixmap(QPixmap(":/diody/czerwona"));
            FlagaC = false;
            ui->MiejsceC->setPixmap(QPixmap(":/diody/czerwona"));
            ScenaSon->UstawFlageB(0);
            ScenaSon->UstawFlageC(0);
        }
        FlagaA = true;
        ScenaSon->UstawFlageA(1);
        FlagaMiejsca = true;
        ui->MiejsceA->setPixmap(QPixmap(":/diody/zielona"));
    }
    else if(arg1 == "B")
    {
        if(FlagaA == true || FlagaC == true)
        {
            FlagaA = false;
            ui->MiejsceA->setPixmap(QPixmap(":/diody/czerwona"));
            FlagaC = false;
            ui->MiejsceC->setPixmap(QPixmap(":/diody/czerwona"));
            ScenaSon->UstawFlageA(0);
            ScenaSon->UstawFlageC(0);
        }
        FlagaB = true;
        ScenaSon->UstawFlageB(1);
        FlagaMiejsca = true;
        ui->MiejsceB->setPixmap(QPixmap(":/diody/zielona"));
    }
    else
    {
        if(FlagaB == true || FlagaA == true)
        {
            FlagaB = false;
            ui->MiejsceB->setPixmap(QPixmap(":/diody/czerwona"));
            FlagaA = false;
            ui->MiejsceA->setPixmap(QPixmap(":/diody/czerwona"));
            ScenaSon->UstawFlageA(0);
            ScenaSon->UstawFlageB(0);
        }
        FlagaC = true;
        ScenaSon->UstawFlageC(1);
        FlagaMiejsca = true;
        ui->MiejsceC->setPixmap(QPixmap(":/diody/zielona"));
    }
}

/*!
 * \brief Metoda zmiany zdarzenia
 * \param event - zdarzenie
 *
 * Metoda ta ma za zadanie wykrywać zmianę języka w aplikacji i odpowiednio na nią reagować.
 */
void MainWindow::changeEvent(QEvent *event)
{
    if ( event->type() == QEvent::LanguageChange ) {
            qDebug() << tr("Zmiana");
            ui->retranslateUi(this);
            return;
        }
        QMainWindow::changeEvent(event);
}

/*!
 * \brief Slot odpowiedzialny za zmianę języka
 * \param Idx - wybrany język
 *
 * Slot ten ma za zadanie wykrywać zmianę w wyborze języka i wgrywać tłumaczenia z pliku *.qm do aplikacji. Dzięki temu,
 * po stworzeniu pliku z tłumaczeniami aplikacja zyskuje na jakości, bo może być używana w różnych krajach. Należy jednak
 * pamiętać o stworzeniu odpowiednich tłumaczeń wykorzystująć narzędzia takie jak lupdate, linguist i oraz lrelease, które
 * służą do tworzenia plików z tłumaczeniami.
 *
 * Metoda ta wykrywa jaka pozycja została wykryta i w opariu o wybrany indeks odpowiednio reaguje i wgrywa plik *.qm
 * z tłumaczeniami dla danego wyboru.
 */
void MainWindow::on_comboBox_currentIndexChanged(int Idx)
{
    static QTranslator *wTlumaczENG = new QTranslator();

        switch (Idx) {
            case 0: {
               qApp->removeTranslator(wTlumaczENG);
               ui->comboBox->setCurrentIndex(0);
               break;
            }
            case 1: {
                   if (wTlumaczENG->load("lang.qm","/home/thinkpad/Pulpit/WDS/Radar_Mateusz_Strembicki_WDS/prj/Sensor/inne/lang") ) {
                       qApp->installTranslator(wTlumaczENG);
                       ui->comboBox->setCurrentIndex(1);
                   } else qDebug() << tr("Błąd");
                   break;
                }
            }
}
