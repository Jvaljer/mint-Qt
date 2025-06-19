#include "MainWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileInfo>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Main window configuration
    setWindowTitle("Mint - Untitled");
    setMinimumSize(800, 600);
    resize(1000, 700);

    // UI init
    setupUI();    
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    // Features & functionnalities
    connect(textEditor, &QTextEdit::cursorPositionChanged,this,&MainWindow::updateCursorPosition);
    connect(textEditor, &QTextEdit::textChanged,this,[this](){
        documentModified = true;
        updateWindowTitle();
    });

    documentModified = false;
    currentFilePath = "";
    updateCursorPosition();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Central widget
    textEditor = new QTextEdit(this);
    textEditor->setFont(QFont("Consolas",11));
    textEditor->setTabStopDistance(40);

    setCentralWidget(textEditor);
}

void MainWindow::createActions()
{
    // New file
    newAction = new QAction("&New",this);
    newAction->setShortcut(QKeySequence::New); // Ctrl+N
    newAction->setStatusTip("Create a new file");
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    // Open file
    openAction = new QAction("&Open", this);
    openAction->setShortcut(QKeySequence::Open); // Ctrl+O
    openAction->setStatusTip("Open an existing file");
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    // Save file
    saveAction = new QAction("&Save", this);
    saveAction->setShortcut(QKeySequence::Save); // Ctrl+S
    saveAction->setStatusTip("Save current file");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    // Save As
    saveAsAction = new QAction("Save &as ...", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs); // Ctrl+Shift+S
    saveAsAction->setStatusTip("Save under new name/extension");
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    // Exit
    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence::Quit); // Ctrl+Q
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
}

void MainWindow::createMenus()
{
    // File menu
    fileMenu = menuBar()->addMenu("&Fichier");
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    // Editing menu (empty yet)
    editMenu = menuBar()->addMenu("&Edit");
    // Help menu (empty yet)
    helpMenu = menuBar()->addMenu("&Help");
}

void MainWindow::createToolBars()
{
    // File toolbar
    fileToolBar = addToolBar("Fichier");
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAsAction);
}

void MainWindow::createStatusBar()
{
    myStatusBar = this->statusBar();
    statusLabel = new QLabel("Ready");
    myStatusBar->addWidget(statusLabel);

    positionLabel = new QLabel("Line: 1, Column: 1");
    myStatusBar->addPermanentWidget(positionLabel);
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEditor->clear();
        setCurrentFile("");
        statusLabel->setText("New document created");
    }
}

void MainWindow::openFile()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "", "Text file (*.txt);;Markdown file (*.md);;All types (*.*)");

        if (!fileName.isEmpty())
        {
            if (loadDocument(fileName))
            {
                setCurrentFile(fileName);
                statusLabel->setText("File successfully opened");
            }
        }
    }
}

void MainWindow::saveFile()
{
    if (currentFilePath.isEmpty())
        saveAsFile();
    else
        saveDocument(currentFilePath);
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as", "", "Text file (*.txt);;Markdown file (*.md);;All types (*.*)");

    if (!fileName.isEmpty())
    {
        if (saveDocument(fileName))
            setCurrentFile(fileName);
    }
}

bool MainWindow::saveDocument(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", QString("File couldn't be saved :\n%1").arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << textEditor->toPlainText();

    documentModified = false;
    updateWindowTitle();
    statusLabel->setText("Document saved");

    return true;
}

bool MainWindow::loadDocument(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", QString("File couldn't be opened :\n%1").arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    textEditor->setPlainText(in.readAll());

    documentModified = false;
    updateWindowTitle();

    return true;
}

bool MainWindow::maybeSave()
{
    if (!documentModified)
        return true;

    QMessageBox::StandardButton result = QMessageBox::question(this, "Document modified", "Document has been modified.\nDo you want to save its changes ?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (result)
    {
        case QMessageBox::Save:
            return saveFile(), true;
        case QMessageBox::Discard:
            return true;
        case QMessageBox::Cancel:
            return false;
        default:
            return false;
    }
}

void MainWindow::setCurrentFile(const QString &filePath)
{
    currentFilePath = filePath;
    documentModified = false;
    updateWindowTitle();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::exitApplication()
{
    close(); // Triggers 'closeEvent'
}

void MainWindow::updateCursorPosition()
{
    QTextCursor cursor = textEditor->textCursor();

    int line = cursor.blockNumber() + 1;
    int column = cursor.columnNumber() + 1;

    positionLabel->setText(QString("Line: %1, Colonne: %2").arg(line).arg(column));
}

void MainWindow::updateWindowTitle()
{
    QString title = "Mint - ";

    if (currentFilePath.isEmpty())
        title += "Untitled";
    else
    {
        QFileInfo fileInfo(currentFilePath);
        title += fileInfo.fileName();
    }

    if (documentModified)
        title += " *";

    setWindowTitle(title);
}
