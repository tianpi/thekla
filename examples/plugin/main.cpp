#include "thekla_form.h"
#include <QApplication>
#include <stdexcept>
int main(int argc, char* argv[])
{
QApplication a(argc, argv);
QDialog container;
Ui::THEKLA::AppId ui;
try {
ui.setupUi(&container);
} catch (std::exception & ex) {
std::cerr << "++++++++++ <THEKLA ERROR> " << std::endl;
std::cerr << ex.what();
std::cerr << "++++++++++ </THEKLA ERROR> " << std::endl;
}
container.show();
return a.exec();
}
