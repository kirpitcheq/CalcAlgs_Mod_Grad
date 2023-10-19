#include <KPEq/msgboxsmpl.h>

MsgBoxSmpl::MsgBoxSmpl(const QString& msg) {
    this->setText(msg);
    this->exec();
}
