#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class WarningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WarningDialog(QWidget *parent = nullptr);
    ~WarningDialog();

private:
    void setupUI();
    QString getFormattedText() const;

    QTextEdit *m_textEdit;
    QPushButton *m_acceptButton;
    QPushButton *m_declineButton;
};

#endif // WARNINGDIALOG_H
