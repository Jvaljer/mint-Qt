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
    findDialog = nullptr;
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

    // Search action
    findAction = new QAction("&Search", this);
    findAction->setShortcut(QKeySequence::Find); // Ctrl+F
    findAction->setStatusTip("Search for text");
    connect(findAction, &QAction::triggered, this, &MainWindow::showFindDialog);

    // Editing actions
    undoAction = new QAction("&Undo", this);
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z
    undoAction->setStatusTip("Cancel last action");
    connect(undoAction, &QAction::triggered, textEditor, &QTextEdit::undo);

    redoAction = new QAction("&Redo", this);
    redoAction->setShortcut(QKeySequence::Redo); // Ctrl+Y
    redoAction->setStatusTip("Put back last undone action");
    connect(redoAction, &QAction::triggered, textEditor, &QTextEdit::redo);

    cutAction = new QAction("&Cut", this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip("Cut selection");
    connect(cutAction, &QAction::triggered, textEditor, &QTextEdit::cut);

    copyAction = new QAction("&Copy", this);
    copyAction->setShortcut(QKeySequence::Copy); // Ctrl+C
    copyAction->setStatusTip("Copy selection to clipboard");
    connect(copyAction, &QAction::triggered, textEditor, &QTextEdit::copy);

    pasteAction = new QAction("&Paste", this);
    pasteAction->setShortcut(QKeySequence::Paste); // Ctrl+V
    pasteAction->setStatusTip("Paste clipboard's content");
    connect(pasteAction, &QAction::triggered, textEditor, &QTextEdit::paste);

    selectAllAction = new QAction("&Select all", this);
    selectAllAction->setShortcut(QKeySequence::SelectAll); // Ctrl+A
    selectAllAction->setStatusTip("Select all file's text");
    connect(selectAllAction, &QAction::triggered, textEditor, &QTextEdit::selectAll);

    // Automatic update of few actions
    connect(textEditor, &QTextEdit::undoAvailable, undoAction, &QAction::setEnabled);
    connect(textEditor, &QTextEdit::redoAvailable, redoAction, &QAction::setEnabled);
    connect(textEditor, &QTextEdit::selectionChanged, this, &MainWindow::updateEditActions);

    // Actions' initial state
    updateEditActions();

    applyMintTheme();
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
    // Editing menu
    editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
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

    // Edition toolbar
    QToolBar *editToolBar = addToolBar("Edit");
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
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

void MainWindow::updateEditActions()
{
    // Activate actions only on editor's specific states
    bool hasSelection = textEditor->textCursor().hasSelection();

    cutAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);
    // paste is automatically handled by Qt (if clipboard has content)
}

void MainWindow::showFindDialog()
{
    if (!findDialog)
        findDialog = new FindDialog(textEditor, this);

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}


/* --------------- *
 *    UI STYLES    *
 * --------------- */
void MainWindow::applyMintTheme()
{
    setStyleSheet(getMintStyleSheet());
}

QString MainWindow::getMintStyleSheet()
{
    const QString mainColor = "#4DD599";
    const QString secondaryColor = "#00918E";
    const QString accentColor1 = "#FFDC34";
    const QString accentColor2 = "#110133";
    const QString textColor1 = "#FFF3E1";
    const QString bgLight = "#F8FFFE";
    const QString bgWhite = "#FFFFFF";

    return QString(R"(
        /* === FENÊTRE PRINCIPALE === */
        QMainWindow {
            background-color: %1;
            color: %2;
        }

        /* === ÉDITEUR DE TEXTE === */
        QTextEdit {
            background-color: %3;
            color: %2;
            border: 2px solid %1;
            border-radius: 6px;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 11pt;
            padding: 8px;
            selection-background-color: %1;
            selection-color: %2;
        }

        QTextEdit:focus {
            border: 2px solid %4;
            background-color: #FDFFFD;
        }

        /* === BARRE DE MENUS === */
        QMenuBar {
            background-color: %1;
            color: %2;
            font-weight: 500;
            padding: 4px;
            border-bottom: 2px solid %4;
        }

        QMenuBar::item {
            background-color: transparent;
            padding: 8px 16px;
            border-radius: 4px;
            margin: 2px;
        }

        QMenuBar::item:selected {
            background-color: %4;
            color: %5;
        }

        QMenuBar::item:pressed {
            background-color: %6;
            color: %2;
        }

        /* === MENUS DÉROULANTS === */
        QMenu {
            background-color: %3;
            color: %2;
            border: 2px solid %1;
            border-radius: 6px;
            padding: 4px;
        }

        QMenu::item {
            padding: 8px 20px;
            border-radius: 4px;
            margin: 2px;
        }

        QMenu::item:selected {
            background-color: %1;
            color: %2;
        }

        QMenu::separator {
            height: 2px;
            background-color: %1;
            margin: 4px 8px;
        }

        /* === BARRES D'OUTILS === */
        QToolBar {
            background-color: %1;
            border: none;
            padding: 4px;
            spacing: 2px;
        }

        QToolBar::separator {
            background-color: %4;
            width: 2px;
            margin: 4px 2px;
        }

        /* === BOUTONS D'ACTION === */
        QToolBar QToolButton {
            background-color: transparent;
            color: %2;
            border: 1px solid transparent;
            border-radius: 4px;
            padding: 6px;
            margin: 1px;
            font-weight: 500;
        }

        QToolBar QToolButton:hover {
            background-color: %4;
            color: %5;
            border: 1px solid %4;
        }

        QToolBar QToolButton:pressed {
            background-color: %6;
            color: %2;
        }

        QToolBar QToolButton:disabled {
            background-color: transparent;
            color: #A0A0A0;
        }

        /* === BARRE D'ÉTAT === */
        QStatusBar {
            background-color: %4;
            color: %5;
            border-top: 2px solid %1;
            font-weight: 500;
        }

        QStatusBar QLabel {
            color: %5;
            padding: 4px 8px;
        }

        /* === DIALOGS ET BOUTONS === */
        QDialog {
            background-color: %7;
            color: %2;
        }

        QPushButton {
            background-color: %1;
            color: %2;
            border: 2px solid %1;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 500;
            min-width: 80px;
        }

        QPushButton:hover {
            background-color: %4;
            color: %5;
            border-color: %4;
        }

        QPushButton:pressed {
            background-color: %6;
            color: %2;
            border-color: %6;
        }

        QPushButton:disabled {
            background-color: #E0E0E0;
            color: #A0A0A0;
            border-color: #E0E0E0;
        }

        /* === CHAMPS DE SAISIE === */
        QLineEdit {
            background-color: %3;
            color: %2;
            border: 2px solid %1;
            border-radius: 4px;
            padding: 6px;
            font-size: 10pt;
        }

        QLineEdit:focus {
            border-color: %4;
            background-color: #FDFFFD;
        }

        /* === CASES À COCHER === */
        QCheckBox {
            color: %2;
            font-weight: 500;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 2px solid %1;
            border-radius: 3px;
            background-color: %3;
        }

        QCheckBox::indicator:checked {
            background-color: %1;
            border-color: %4;
        }

        QCheckBox::indicator:hover {
            border-color: %4;
        }

        /* === LABELS === */
        QLabel {
            color: %2;
        }

        /* === GROUPBOX === */
        QGroupBox {
            color: %2;
            font-weight: bold;
            border: 2px solid %1;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 4px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 8px 0 8px;
            background-color: %1;
            color: %2;
            border-radius: 4px;
        }

        /* === SCROLLBARS === */
        QScrollBar:vertical {
            background-color: #F0F0F0;
            width: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background-color: %1;
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: %4;
        }

        /* === COMBO BOX === */
        QComboBox {
            background-color: %3;
            color: %2;
            border: 2px solid %1;
            border-radius: 4px;
            padding: 6px;
            min-width: 100px;
        }

        QComboBox:hover {
            border-color: %4;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            width: 12px;
            height: 12px;
            image: none;
            border-left: 6px solid transparent;
            border-right: 6px solid transparent;
            border-top: 6px solid %2;
        }

        /* === SPINBOX === */
        QSpinBox {
            background-color: %3;
            color: %2;
            border: 2px solid %1;
            border-radius: 4px;
            padding: 6px;
        }

        QSpinBox:focus {
            border-color: %4;
        }
    )")
        .arg(mainColor)      // %1
        .arg(accentColor2)   // %2
        .arg(bgWhite)        // %3
        .arg(secondaryColor) // %4
        .arg(textColor1)     // %5
        .arg(accentColor1)   // %6
        .arg(bgLight);       // %7
}
