#include "DefaultAdapterInputFileGenerator.h"
#include <QApplication>
#include <iostream>
#include <stdexcept>
#include <common/Log.h>

//----------------------------------------
int main(int argc, char * argv[]) {
    QApplication a(argc, argv);
    Log::init();

    try {

        DefaultAdapterInputFileGenerator::exec();

    } catch (std::exception & ex) {
        std::cerr << "<Thekla ERROR> " << std::endl;
        std::cerr << ex.what() << std::endl;
        std::cerr << "</Thekla ERROR> " << std::endl;
        return -1;
    }

    return 0;
}
