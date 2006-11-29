#include <QApplication>
#include <QMainWindow>
#include "thekla_Dialog_Output.h"

//--------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QDialog d;

    Ui::THEKLA::OutputDialog ui;
    try {
        ui.setupUi(&d);
    } catch (std::exception & ex) {
        std::cerr << "++++++++++ <THEKLA-ERROR>" << std::endl;
        std::cerr << ex.what();
        std::cerr << "++++++++++ </THEKLA-ERROR>" << std::endl;
    }

    d.show();

    return app.exec();
}
