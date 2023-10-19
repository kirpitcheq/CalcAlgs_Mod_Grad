#pragma once

#include <QMessageBox>

class MsgBoxSmpl : public QMessageBox
{
    Q_OBJECT
public:
    MsgBoxSmpl(const QString &msg);
};
