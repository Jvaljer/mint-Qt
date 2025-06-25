#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>

#include "FindDialog.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // UI styles
    void applyMintTheme();
    QString getMintStyleSheet();
    // Initialization methods
    void setupUI();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createActions();
    // File handling methods
    bool saveDocument(const QString &filePath);
    bool loadDocument(const QString &filePath);
    bool maybeSave(); // ask if save is needed
    void setCurrentFile(const QString &filePath);

    // Main widgets
    QTextEdit *textEditor;
    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    // User/File actions
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    // Editing actions
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *selectAllAction;
    QAction *findAction;
    FindDialog *findDialog;
    // Toolbars
    QToolBar *fileToolBar;
    QStatusBar *myStatusBar;
    QLabel * statusLabel;
    QLabel *positionLabel;
    // Core features
    bool documentModified;
    QString currentFilePath;

private slots:
    // Menu actions' slots
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void exitApplication();
    // Core features
    void updateCursorPosition();
    void updateWindowTitle();
    void updateEditActions();
    // UI
    void showFindDialog();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
