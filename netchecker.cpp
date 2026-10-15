#include "netchecker.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QCoreApplication>

#ifdef USE_QCORO
#include <qcoro/qcorotask.h>
#include <qcoro/qcoronetworkreply.h>
#endif

NetChecker::NetChecker(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_nam(new QNetworkAccessManager(this))
{
    // Таймер срабатывает каждые N мс и запускает проверку доступности google.com
    m_timer->setInterval(10000);
    m_timer->setTimerType(Qt::CoarseTimer);

#ifdef USE_QCORO
    connect(m_timer, &QTimer::timeout, this, [this]() {
        // Запускаем корутину; co_await внутри «замораживает» её
        // до получения сетевого ответа, не блокируя event loop.
        checkGoogleAsync();
    });
#else
    connect(m_timer, &QTimer::timeout, this, &NetChecker::checkGoogle);
#endif
}

NetChecker::~NetChecker()
{
}

void NetChecker::start(int intervalMs)
{
    m_timer->setInterval(intervalMs);
    m_timer->start();

    // Первая проверка — сразу, без ожидания таймера
#ifdef USE_QCORO
    checkGoogleAsync();
#else
    checkGoogle();
#endif
}

void NetChecker::stop()
{
    m_timer->stop();
}

#ifdef USE_QCORO

// ---- Реализация с QCoro ----------------------------------------------------
QCoro::Task<> NetChecker::checkGoogleAsync()
{
    QNetworkRequest req{QUrl(QStringLiteral("https://www.google.com"))};
    req.setTransferTimeout(5000); // 5 секунд на весь запрос
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QStringLiteral("Mozilla/5.0 (WarningDialogApp net-checker)"));

    // HEAD-запрос: нас интересует только факт доступности
    QNetworkReply *reply = co_await m_nam->head(req);

    const bool ok = (reply->error() == QNetworkReply::NoError);
    const int status =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QString info;
    if (ok)
        info = QStringLiteral("google.com доступен (HTTP %1)").arg(status);
    else
        info = QStringLiteral("google.com недоступен: %1")
                   .arg(reply->errorString());

    reply->deleteLater();

    if (ok != m_lastKnown) {
        m_lastKnown = ok;
        emit availabilityChanged(ok, info);
    }
    qDebug().noquote() << info;
}

#else

// ---- Fallback-реализация без QCoro ------------------------------------------
void NetChecker::checkGoogle()
{
    QNetworkRequest req{QUrl(QStringLiteral("https://www.google.com"))};
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    req.setTransferTimeout(5000);
#endif
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QStringLiteral("Mozilla/5.0 (WarningDialogApp net-checker)"));

    QNetworkReply *reply = m_nam->head(req);
    connect(reply, &QNetworkReply::finished,
            this, [this, reply]() { onReplyFinished(reply); });
}

void NetChecker::onReplyFinished(QNetworkReply *reply)
{
    const bool ok = (reply->error() == QNetworkReply::NoError);
    const int status =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QString info;
    if (ok)
        info = QStringLiteral("google.com доступен (HTTP %1)").arg(status);
    else
        info = QStringLiteral("google.com недоступен: %1")
                   .arg(reply->errorString());

    reply->deleteLater();

    if (ok != m_lastKnown) {
        m_lastKnown = ok;
        emit availabilityChanged(ok, info);
    }
    qDebug().noquote() << info;
}

#endif // USE_QCORO
