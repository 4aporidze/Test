#include "warningdialog.h"
#include <QApplication>

WarningDialog::WarningDialog(QWidget *parent)
    : QDialog(parent)
    , m_textEdit(nullptr)
    , m_acceptButton(nullptr)
    , m_declineButton(nullptr)
{
    setupUI();
}

WarningDialog::~WarningDialog()
{
}

QString WarningDialog::getFormattedText() const
{
    QString html = R"(
        <html>
        <head>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    font-size: 12pt;
                    line-height: 1.6;
                    color: #333333;
                    margin: 20px;
                }
                h1 {
                    font-size: 16pt;
                    font-weight: bold;
                    color: #d9534f;
                    text-align: center;
                    border-bottom: 2px solid #d9534f;
                    padding-bottom: 10px;
                    margin-bottom: 20px;
                }
                h2 {
                    font-size: 14pt;
                    font-weight: bold;
                    color: #f0ad4e;
                    margin-top: 20px;
                    margin-bottom: 10px;
                }
                p {
                    text-align: justify;
                    margin-bottom: 15px;
                }
                .highlight {
                    background-color: #fcf8e3;
                    padding: 10px;
                    border-left: 4px solid #f0ad4e;
                    margin: 15px 0;
                }
                .important {
                    background-color: #f2dede;
                    padding: 10px;
                    border-left: 4px solid #d9534f;
                    margin: 15px 0;
                    font-weight: bold;
                }
                ul {
                    margin: 10px 0;
                    padding-left: 30px;
                }
                li {
                    margin-bottom: 8px;
                }
                .law-reference {
                    font-style: italic;
                    color: #555555;
                    background-color: #f5f5f5;
                    padding: 5px 10px;
                    border-radius: 3px;
                }
            </style>
        </head>
        <body>
            <h1>⚠️ Предупреждение для пользователей</h1>
            
            <p>
                При использовании Программы пользователь обязуется не совершать действий, 
                которые нарушают права и законные интересы других лиц или требования законодательства РФ, 
                в том числе законодательство об интеллектуальной собственности, о персональных данных.
            </p>
            
            <div class="highlight">
                <h2>📍 Работа с геоданными</h2>
                <p>
                    При работе с геоданными в соответствии с 
                    <span class="law-reference">Федеральным законом от 30.12.2015 № 431-ФЗ</span> 
                    «О геодезии, картографии и пространственных данных и о внесении изменений в отдельные 
                    законодательные акты Российской Федерации» необходимо соблюдать требования к их сбору, 
                    обработке, предоставлению.
                </p>
            </div>
            
            <div class="important">
                <h2>❗ Ответственность пользователя</h2>
                <p>Пользователь несет единоличную ответственность за свои действия при использовании Программы, 
                а также при использовании результатов такого использования.</p>
                <p>Это включает, но не ограничиваясь данным перечнем:</p>
                <ul>
                    <li>Использование данных, изображений, текста, контента и др.</li>
                    <li>Изменение данных, включая удаление</li>
                    <li>Размещение данных и контента</li>
                    <li>Дальнейшее использование результатов подобной обработки</li>
                </ul>
            </div>
            
            <p>
                <strong>Лицензиар не несет ответственности</strong> за действия Пользователей, 
                нарушающие закон или права третьих лиц.
            </p>
        </body>
        </html>
    )";
    
    return html;
}

void WarningDialog::setupUI()
{
    setWindowTitle("Предупреждение");
    setMinimumSize(600, 500);
    setModal(true);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Создаем QTextEdit для отображения Rich Text
    m_textEdit = new QTextEdit(this);
    m_textEdit->setReadOnly(true);
    m_textEdit->setHtml(getFormattedText());
    m_textEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    
    mainLayout->addWidget(m_textEdit);
    
    // Создаем кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_declineButton = new QPushButton("Отклонить", this);
    m_declineButton->setMinimumWidth(120);
    connect(m_declineButton, &QPushButton::clicked, this, [this]() {
        reject();
    });
    
    m_acceptButton = new QPushButton("Принять", this);
    m_acceptButton->setMinimumWidth(120);
    m_acceptButton->setDefault(true);
    connect(m_acceptButton, &QPushButton::clicked, this, [this]() {
        accept();
    });
    
    buttonLayout->addWidget(m_declineButton);
    buttonLayout->addWidget(m_acceptButton);
    
    mainLayout->addLayout(buttonLayout);
}
