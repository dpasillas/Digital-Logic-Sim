#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CircuitBoard;
class QLabel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setStatus(QString s);
    void setStatus(double d);

public slots:
    void setSelectionMode();
    void setHandDragMode();
    void runFunc(bool b);
    void stopFunc();
    void step();
    void stepNext();
    void reset();
    void setComponentCreationMode();
    void setCCM_Int();
    void setCCM_SmallInt();
    void setViewProperty(bool b);
    void fitToWindow();
    void zoom();
    void setInterval();
    void setStepSize();
    void copy();
    void paste();
    void cut();
    void del();
    void selectAll();

private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void openRecentFile();
    void about();
    void importAction();
    void exportAction();

private:
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    void setupMenus();
    void changeEvent(QEvent *e);

    enum {MaxRecentFiles = 4};
    QString currentFile;
    QList<QAction*> recentFilesAct;
    QTimer* timer;
    Ui::MainWindow *ui;
    CircuitBoard* board;
    QLabel *timeLabel;
    double stepSize, stepInterval;

    QAction *selectionToolAction, *panToolAction;
};

#endif // MAINWINDOW_H
