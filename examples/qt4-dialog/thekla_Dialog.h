#ifndef THEKLA_DIALOG_H
#define THEKLA_DIALOG_H

#include "ui_Dialog.h"
#include <client/Client.h>
#include <QFile>

//--------------------------------------------------------------------------------
class THEKLA_Ui_Dialog : public Ui::Dialog
{

protected:

    THEKLA::InputClient theklaClient_;

public:
    THEKLA_Ui_Dialog()
        : theklaClient_(THEKLA::Client::QT_APP)
        {}

    virtual ~THEKLA_Ui_Dialog()
        {}

public:

    void setupUi(QDialog * Dialog)
        {
            Ui::Dialog::setupUi(Dialog);

            QFile configFile("Dialog-thekla.xml");
            theklaClient_.init(configFile, Dialog, false);
            theklaClient_.setWriteInterval(100);
            theklaClient_.setReadInterval(100);
        }

};

namespace Ui {
    namespace THEKLA {
        class Dialog: public THEKLA_Ui_Dialog {};
    } // namesapce THEKLA
} // namespace Ui


#endif // THEKLA_DIALOG_H
