#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QTextEdit *textEdit, QWidget *parent = nullptr);

private slots:
    void findNext();
    void findPrevious();
    void replace();
    void replaceAll();

private:
    void setupUI();
    bool findText(const QString &text, bool forward = true);

    QTextEdit *textEditor;
    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
    QPushButton *findNextButton;
    QPushButton *findPrevButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QCheckBox *caseSensitiveCheck;
    QCheckBox *wholeWordsCheck;
};

#endif // FINDDIALOG_H
