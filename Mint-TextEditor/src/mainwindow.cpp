#include "MainWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

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
    textEditor->clear();
    setWindowTitle("Mint - Untitled");
    statusLabel->setText("New document");
}

void MainWindow::openFile()
{
    statusLabel->setText("'openFile' still to implement ...");
}

void MainWindow::saveFile()
{
    statusLabel->setText("'saveFile' still to implement ...");
}

void MainWindow::saveAsFile()
{
    statusLabel->setText("'saveAsFile' still to implement ...");
}

void MainWindow::exitApplication()
{
    statusLabel->setText("Closing ...");
    QApplication::quit();
}
