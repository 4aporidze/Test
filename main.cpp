#include <QApplication>
#include <QDebug>
#include "warningdialog.h"
#include "netchecker.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Проверка доступности google.com по таймеру (каждые 10 секунд).
    // При наличии QCoro внутри используется корутина QCoro::Task<> с co_await.
    NetChecker netChecker;
    QObject::connect(&netChecker, &NetChecker::availabilityChanged,
                     [](bool available, const QString &info) {
        qDebug() << (available ? "[NET] OK  :" : "[NET] FAIL:") << info;
    });
    netChecker.start(10000);

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
