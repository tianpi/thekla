#ifndef THEKLA_OUTPUT_DIALOG_H
#define THEKLA_OUTPUT_DIALOG_H

#include "ui_Dialog.h"

#include <client/Client.h>

//--------------------------------------------------------------------------------
class THEKLA_Ui_Output_Dialog : public Ui::Dialog
{

protected:

    THEKLA::OutputClient theklaClient_;
    THEKLA::OutputClient theklaClient2_;

public:

    THEKLA_Ui_Output_Dialog()
        : theklaClient_(THEKLA::Client::QT_APP),
          theklaClient2_(THEKLA::Client::QT_APP)
        {}

    virtual ~THEKLA_Ui_Output_Dialog()
        {}

public:

    void setupUi(QDialog * Dialog)
        {
            Ui::Dialog::setupUi(Dialog);

            using namespace THEKLA;

            {

                theklaClient_.init(Dialog->objectName(), "localhost", 20000, 20001);
                theklaClient_.setReadInterval(100);

                QtObject * qtSlider = theklaClient_.selectQtObject("QSlider", "slider");
                QtSignal * qtSlider_valueChanged = theklaClient_.selectQtSignal(qtSlider, "valueChanged(int)");
                theklaClient_.connectQtSignalToQtSlot(qtSlider_valueChanged, slider, "setValue(int)");

                QtObject * qtToggleButton = theklaClient_.selectQtObject("QPushButton", "togglebutton");
                QtSignal * qtToggleButton_clicked = theklaClient_.selectQtSignal(qtToggleButton, "clicked(bool)");
                theklaClient_.connectQtSignalToQtSlot(qtToggleButton_clicked, togglebutton, "setChecked(bool)");

                QtObject * qtComboBox = theklaClient_.selectQtObject("QComboBox", "comboBox");
                QtProperty * qtComboBox_currentText = theklaClient_.selectQtProperty(qtComboBox, "currentIndex", "int");
                theklaClient_.connectQtPropertyToQtProperty(qtComboBox_currentText, comboBox, "currentIndex");

                QtObject * qtLineEdit = theklaClient_.selectQtObject("QLineEdit", "lineEdit");
                QtProperty * qtLineEdit_html = theklaClient_.selectQtProperty(qtLineEdit, "text", "QString");
                theklaClient_.connectQtPropertyToQtProperty(qtLineEdit_html, lineEdit, "text");

                QtObject * qtTextEdit = theklaClient_.selectQtObject("QTextEdit", "textEdit");
                QtProperty * qtTextEdit_html = theklaClient_.selectQtProperty(qtTextEdit, "html", "QString");
                theklaClient_.connectQtPropertyToQtProperty(qtTextEdit_html, textEdit, "html");

            }
            {
                theklaClient2_.init("SoQt4-Viewer", "localhost", 20000, 20001);
                theklaClient_.setReadInterval(100);

                OivField * oivCameraPos = theklaClient2_.selectOivField("SFVec3f", "cameraPos");
                theklaClient2_.connectOivFieldToQtProperty(oivCameraPos, label_cameraPos, "text");
            }

        }


};

namespace Ui {
    namespace THEKLA {
        class OutputDialog: public THEKLA_Ui_Output_Dialog {};
    } // namespace THEKLA
} // namespace Ui


#endif // THEKLA_OUTPUT_DIALOG_H
