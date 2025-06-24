#include "FindDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextCursor>
#include <QTextDocument>
#include <QMessageBox>

FindDialog::FindDialog(QTextEdit *textEdit, QWidget *parent) : QDialog(parent), textEditor(textEdit)
{
    setWindowTitle("Search & Replace");
    setModal(false); // allow to keep editing
    setupUI();
    findLineEdit->setFocus();
}

void FindDialog::setupUI()
{
    // Widgets creation
    findLineEdit = new QLineEdit;
    replaceLineEdit = new QLineEdit;
    findNextButton = new QPushButton("Next");
    findPrevButton = new QPushButton("Previous");
    replaceButton = new QPushButton("Replace");
    replaceAllButton = new QPushButton("Replace All");
    caseSensitiveCheck = new QCheckBox("Case Sensitive");
    wholeWordsCheck = new QCheckBox("Whole words only");

    // Grid layout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Search"), 0,0);
    layout->addWidget(findLineEdit, 0,1,1,2);
    layout->addWidget(new QLabel("Replace with:"), 1,0);
    layout->addWidget(replaceLineEdit, 1,1,1,2);
    layout->addWidget(caseSensitiveCheck, 2,0,1,3);
    layout->addWidget(wholeWordsCheck, 3,0,1,3);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(findNextButton);
    buttonLayout->addWidget(findPrevButton);
    buttonLayout->addWidget(replaceButton);
    buttonLayout->addWidget(replaceAllButton);

    layout->addLayout(buttonLayout, 4,0,1,3);
    setLayout(layout);

    // Connections
    connect(findNextButton, &QPushButton::clicked, this, &FindDialog::findNext);
    connect(findPrevButton, &QPushButton::clicked, this, &FindDialog::findPrevious);
    connect(replaceButton, &QPushButton::clicked, this, &FindDialog::replace);
    connect(replaceAllButton, &QPushButton::clicked, this, &FindDialog::replaceAll);

    // Real time search
    connect(findLineEdit, &QLineEdit::textChanged, this, &FindDialog::findNext);
}

bool FindDialog::findText(const QString &text, bool forward)
{
    if (text.isEmpty()) return false;

    // Search options
    QTextDocument::FindFlags flags;

    if (!forward)
        flags |= QTextDocument::FindBackward;

    if (caseSensitiveCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    if (wholeWordsCheck->isChecked())
        flags |= QTextDocument::FindWholeWords;

    // Search
    QTextDocument *document = textEditor->document();
    QTextCursor cursor = document->find(text, textEditor->textCursor(), flags);

    if (cursor.isNull())
    {
        // Not found - search from start/end
        QTextCursor startCursor = textEditor->textCursor();
        startCursor.movePosition(forward ? QTextCursor::Start : QTextCursor::End);

        // New search from start/end
        cursor = document->find(text, startCursor, flags);

        if (cursor.isNull())
        {
            QMessageBox::information(this, "Search", "Text not found.");
            return false;
        }
    }

    // Put found cursor within editor
    textEditor->setTextCursor(cursor);
    return true;
}

void FindDialog::findNext()
{
    findText(findLineEdit->text(), true);
}

void FindDialog::findPrevious()
{
    findText(findLineEdit->text(), false);
}

void FindDialog::replace()
{
    QTextCursor cursor = textEditor->textCursor();

    if (cursor.hasSelection() && cursor.selectedText() == findLineEdit->text())
        cursor.insertText(replaceLineEdit->text());

    findNext(); // Search for next occurence
}

void FindDialog::replaceAll()
{
    QString searchText = findLineEdit->text();
    QString replaceText = replaceLineEdit->text();

    if (searchText.isEmpty()) return;

    QTextCursor cursor = textEditor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    textEditor->setTextCursor(cursor);

    int replacements = 0;
    while (findText(searchText, true))
    {
        QTextCursor current = textEditor->textCursor();
        current.insertText(replaceText);
        replacements++;
    }

    QMessageBox::information(this, "Search all", QString("Did %1 replacement(s).").arg(replacements));
}
