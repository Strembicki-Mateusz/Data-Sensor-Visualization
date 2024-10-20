#include "inc/mainwindow.hh"
#include "inc/scena_sonaru.hh"

#include <QApplication>
/*!
 * \brief main - funkcja główna programu.
 *
 * Najprostsza, a zarazem najbardziej podstawowa funkcja
 * wywoływania okienka aplikacji. W pierwszym kroku następuje tworzenie
 * obiektu klasy QApplication z argumentami wywołania. Następnie tworzony jest
 * obiekt klasy MainWindow (okno aplikacji). Kolejno wymuszane jest ukazanie się okna aplikacji
 * metodą show(), a na końcu wywoływana jest pętla obsługi zdarzeń exec().
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
