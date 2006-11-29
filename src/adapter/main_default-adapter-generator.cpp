#include "DefaultAdapterGenerator.h"
#include <common/Log.h>
#include <QApplication>
#include <stdexcept>
#include <iostream>

//--------------------------------------------------------------------------------
int main(int argc, char * argv[]) {

    QApplication app(argc, argv);
    Log::init();

    DefaultAdapterGenerator dag;
    try {
        dag.exec();
    } catch (std::exception & ex) {
        std::cerr << "<Thekla ERROR> " << std::endl;
        std::cerr << ex.what() << std::endl;
        std::cerr << "</Thekla ERROR> " << std::endl;
        return -1;
    }

    return 0;
}
