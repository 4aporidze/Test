#ifndef NETCHECKER_H
#define NETCHECKER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>

#ifdef USE_QCORO
#include <qcoro/qcorotask.h>   // QCoro::Task<>
#include <qcoro/qcorofuture.h> // co_await для QFuture
#include <qcoro/qcoronetworkreply.h> // co_await для QNetworkReply*
#else
#include <QNetworkReply>
#endif

// Класс periodically проверяет доступность google.com по таймеру.
class NetChecker : public QObject
{
    Q_OBJECT

public:
    explicit NetChecker(QObject *parent = nullptr);
    ~NetChecker();

    void start(int intervalMs = 10000); // запуск таймера проверки
    void stop();

signals:
    void availabilityChanged(bool available, const QString &info);

private slots:
#ifdef USE_QCORO
    // Вариант с QCoro: корутина, которая «ждёт» ответ сети без блокировки потока
    QCoro::Task<> checkGoogleAsync();
#else
    // Fallback-вариант без QCoro (обычный асинхронный запрос)
    void checkGoogle();
    void onReplyFinished(QNetworkReply *reply);
#endif

private:
    QTimer *m_timer;
    QNetworkAccessManager *m_nam;
    bool m_lastKnown = false;
};

#endif // NETCHECKER_H
