#include <QApplication>
#include "warningdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    WarningDialog dialog;
    
    if (dialog.exec() == QDialog::Accepted) {
        // Пользователь принял условия
        qDebug() << "Пользователь принял условия";
        // Здесь можно продолжить запуск основного приложения
    } else {
        // Пользователь отклонил условия
        qDebug() << "Пользователь отклонил условия";
        return 1;
    }
    
    return 0;
}
