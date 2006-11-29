#ifndef THEKLA_THEKLA_UI_APPID_H
#define THEKLA_THEKLA_UI_APPID_H
#include "ui_form.h"
#include <client/Client.h>
#include <QFile>
class THEKLA_Ui_AppId : public Ui::AppId
{
protected:
THEKLA::InputClient theklaClient_;
public:
THEKLA_Ui_AppId()
: theklaClient_(THEKLA::Client::QT_APP)
{}
virtual ~THEKLA_Ui_AppId()
{
}
public:
void setupUi(QDialog * AppId)
{
Ui::AppId::setupUi(AppId);
QFile configFile("form-thekla.xml");
theklaClient_.init(configFile, AppId);
}
};

namespace Ui {
namespace THEKLA {
class AppId : public THEKLA_Ui_AppId {};
}
}
#endif // THEKLA_THEKLA_UI_APPID_H
