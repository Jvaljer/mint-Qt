#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QAction>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    void setupUI();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createActions();

    // Main widgets
    QTextEdit *textEditor;
    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    // Actions
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    // Toolbars
    QToolBar *fileToolBar;
    QStatusBar *myStatusBar;
    QLabel * statusLabel;
    QLabel *positionLabel;
    // Core features
    bool documentModified;
    QString currentFilePath;
};

#endif // MAINWINDOW_H
