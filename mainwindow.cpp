#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QDebug>
#include "circuitboard.h"
#include <typeinfo>
#include <QTimer>
#include <QInputDialog>
#include <QLabel>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionCustom_Signal->setVisible(false);

    stepInterval = 25;
    stepSize = 1;

    board = new CircuitBoard(this);
    setCentralWidget(board);

    /*/ Start Toolbar /*/
    selectionToolAction = new QAction(QIcon(QPixmap::fromImage(QImage(":/images/cursor_arrow_white.png"))), QString(), ui->mainToolBar);
    selectionToolAction->setCheckable(true);
    selectionToolAction->setChecked(true);
    selectionToolAction->setData(0);
    connect(selectionToolAction,SIGNAL(toggled(bool)),ui->actionSelect,SLOT(setChecked(bool)));

    panToolAction = new QAction(QIcon(QPixmap::fromImage(QImage(":/images/cursor_hand.png"))), QString(), ui->mainToolBar);
    panToolAction->setCheckable(true);
    panToolAction->setData(0);
    connect(panToolAction, SIGNAL(toggled(bool)),ui->actionMove,SLOT(setChecked(bool)));

    connect(selectionToolAction,SIGNAL(triggered()),this,SLOT(setSelectionMode()));
    connect(selectionToolAction,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(panToolAction,SIGNAL(triggered()),this,SLOT(setHandDragMode()));
    connect(panToolAction,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    /*/ End Toolbar /*/


    ui->statusBar->showMessage(tr("Version 1.0.3"));
    ui->mainToolBar->setWindowTitle(tr("Toolbar"));
    ui->mainToolBar->addAction(selectionToolAction);
    ui->mainToolBar->addAction(panToolAction);

    setupMenus();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(step()));
    timer->setInterval(stepInterval);

    timeLabel = new QLabel();
    ui->statusBar->addPermanentWidget(timeLabel);

    setStatus(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenus()
{
    /*/ Start File Menu /*/

    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSave_As->setShortcut(QKeySequence::SaveAs);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionNew->setShortcut(QKeySequence::New);
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(saveAs()));

    recentFilesAct.append(ui->actionRecentFiles1);
    recentFilesAct.append(ui->actionRecentFiles2);
    recentFilesAct.append(ui->actionRecentFiles3);
    recentFilesAct.append(ui->actionRecentFiles4);

    for(int i = 0; i < MaxRecentFiles; i++)
    {
        recentFilesAct[i]->setVisible(false);
        connect(recentFilesAct[i],SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }


    ui->actionSeparator->setSeparator(true);
    ui->actionSeparator->setVisible(false);

    ui->actionNew->setVisible(false);
    ui->actionPrint->setVisible(false);
    ui->actionPrint_Preview->setVisible(false);

    /*/ End File Menu /*/

    /*/ Start Parts Menu /*/
    ui->action2_input_AND->setData((CircuitBoard::ANDF << 10) | 1); //pin count - 1 == 2
    ui->action2_input_NAND->setData((CircuitBoard::NANDF << 10) | 1);
    ui->action2_input_NOR->setData((CircuitBoard::NORF << 10)| 1);
    ui->action2_input_OR->setData((CircuitBoard::ORF << 10) | 1);

    ui->action3_input_AND->setData((CircuitBoard::ANDF << 10) | 2);
    ui->action3_input_NAND->setData((CircuitBoard::NANDF << 10) | 2);
    ui->action3_input_NOR->setData((CircuitBoard::NORF << 10)| 2);
    ui->action3_input_OR->setData((CircuitBoard::ORF << 10)| 2);

    ui->action4_input_AND->setData((CircuitBoard::ANDF << 10) | 3);
    ui->action4_input_NAND->setData((CircuitBoard::NANDF << 10) | 3);
    ui->action4_input_NOR->setData((CircuitBoard::NORF << 10) | 3);
    ui->action4_input_OR->setData((CircuitBoard::ORF << 10)| 3);

    ui->actionXOR->setData(CircuitBoard::XORF << 10);
    ui->actionXNOR->setData(CircuitBoard::XNORF << 10);

    ui->actionBuffer->setData(CircuitBoard::BUFF << 10);
    ui->actionNOT->setData(CircuitBoard::NOTF << 10);

    ui->actionV->setData(CircuitBoard::SOURCEF << 10);
    ui->actionGround->setData(CircuitBoard::GROUNDF << 10);
    ui->actionSwitch->setData(CircuitBoard::SWITCHF << 10);
    ui->actionPush_Button->setData(CircuitBoard::BUTTONF << 10);
    ui->actionClock->setData(CircuitBoard::CLOCKF << 10);

    ui->actionBulb->setData(CircuitBoard::BULBF << 10);
    ui->action7_Segment_LED->setData(CircuitBoard::LED_7F << 10);
    ui->action14_Segment_LED->setData(CircuitBoard::LED_14F << 10);
    ui->action16_Segment_LED->setData(CircuitBoard::LED_16F << 10);

    ui->actionMerger->setData(CircuitBoard::MERGERF << 10);
    ui->actionSplitter->setData(CircuitBoard::SPLITTERF << 10);

    ui->actionMultiplexer->setData(CircuitBoard::MULTIPLEXERF << 10);
    ui->actionCounter->setData(CircuitBoard::COUNTERF << 10);
    ui->actionRegister->setData(CircuitBoard::REGISTERF << 10);
    ui->actionDecoder->setData(CircuitBoard::DECODERF << 10);

    ui->actionPriority_Encoder->setData(CircuitBoard::ENCODERF << 10);
    ui->actionLoadable_Counter->setData(CircuitBoard::L_COUNTERF << 10);
    ui->actionLeft_Shift->setData(CircuitBoard::SL_REGISTERF << 10);
    ui->actionRight_Shift->setData(CircuitBoard::SR_REGISTERF << 10);
    ui->actionBidirectional_Shift->setData(CircuitBoard::SB_REGISTERF << 10);
    ui->actionAdder->setData(CircuitBoard::ADDERF << 10);

    ui->actionD_Flip_Flop->setData(CircuitBoard::D_FFF << 10);
    ui->actionT_Flip_Flop->setData(CircuitBoard::T_FFF << 10);
    ui->actionJ_K_Flip_Flop->setData(CircuitBoard::J_K_FFF << 10);

    connect(ui->action2_input_AND,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action2_input_NAND,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action2_input_NOR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action2_input_OR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->action3_input_AND,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action3_input_NAND,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action3_input_NOR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action3_input_OR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->action4_input_AND,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action4_input_NAND,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action4_input_NOR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action4_input_OR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->actionXOR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionXNOR,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->actionBuffer,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionNOT,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->actionV,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionGround,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionSwitch,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionPush_Button,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionClock,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->actionBulb,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action7_Segment_LED,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action14_Segment_LED,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->action16_Segment_LED,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));

    connect(ui->actionMerger,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionSplitter,SIGNAL(triggered()),this,SLOT(setCCM_Int()));

    connect(ui->actionMultiplexer,SIGNAL(triggered()),this,SLOT(setCCM_SmallInt()));
    connect(ui->actionCounter,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionRegister,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionDecoder,SIGNAL(triggered()),this,SLOT(setCCM_SmallInt()));

    connect(ui->actionPriority_Encoder,SIGNAL(triggered()),this,SLOT(setCCM_SmallInt()));
    connect(ui->actionLoadable_Counter,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionLeft_Shift,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionRight_Shift,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionBidirectional_Shift,SIGNAL(triggered()),this,SLOT(setCCM_Int()));
    connect(ui->actionAdder,SIGNAL(triggered()),this,SLOT(setCCM_Int()));

    connect(ui->actionD_Flip_Flop,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionT_Flip_Flop,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    connect(ui->actionJ_K_Flip_Flop,SIGNAL(triggered()),this,SLOT(setComponentCreationMode()));
    /*/ End Parts Menu /*/

    /*/ Start Simulation Menu /*/
    connect(ui->actionRun,SIGNAL(toggled(bool)),this,SLOT(runFunc(bool)));
    //connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(stopFunc()));
    connect(ui->actionStep,SIGNAL(triggered()),this,SLOT(stepNext()));
    connect(ui->actionReset,SIGNAL(triggered()),this,SLOT(reset()));
    connect(ui->actionStep_Interval, SIGNAL(triggered()),this,SLOT(setInterval()));
    connect(ui->actionStep_Size_Run,SIGNAL(triggered()),this,SLOT(setStepSize()));
    ui->actionStep->setShortcut(tr("ALT+T"));
    //ui->actionStop->setShortcut(tr("ALT+S"));
    ui->actionRun->setShortcut(tr("ALT+R"));
    ui->actionReset->setShortcut(tr("ALT+S"));
    /*/ End Simulation Menu /*/

    /*/ Start View Menu /*/
    ui->actionShow_I_O_Values->setData(CircuitBoard::SHOW_I_O);
    ui->actionShow_Pin_Names->setData(CircuitBoard::DISPLAY_NAMES);
    ui->actionShow_Connections->setData(CircuitBoard::DISPLAY_CONNECTIONS);

    connect(ui->actionShow_I_O_Values,SIGNAL(toggled(bool)),SLOT(setViewProperty(bool)));
    connect(ui->actionShow_Pin_Names,SIGNAL(toggled(bool)),SLOT(setViewProperty(bool)));
    connect(ui->actionShow_Connections,SIGNAL(toggled(bool)),SLOT(setViewProperty(bool)));

    connect(ui->actionFit_To_Window,SIGNAL(triggered()),SLOT(fitToWindow()));
    ui->actionScale_25->setData(.25);
    ui->actionScale_50->setData(.5);
    ui->actionScale_75->setData(.75);
    ui->actionScale_100->setData(1.);
    ui->actionScale_150->setData(1.5);
    ui->actionScale_200->setData(2.);
    ui->actionScale_300->setData(3.);
    ui->actionScale_400->setData(4.);

    connect(ui->actionScale_25,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_50,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_75,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_100,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_150,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_200,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_300,SIGNAL(triggered()),SLOT(zoom()));
    connect(ui->actionScale_400,SIGNAL(triggered()),SLOT(zoom()));

    ui->actionScale_Custom->setVisible(false);

    /*/ End View Menu /*/

    /*/ Start Edit Menu /*/
    ui->actionDelete->setShortcut(QKeySequence::Delete);
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionCut->setShortcut(QKeySequence::Cut);
    ui->actionPaste->setShortcut(QKeySequence::Paste);
    ui->actionSelect_All->setShortcut(QKeySequence::SelectAll);
    connect(ui->actionDelete,SIGNAL(triggered()),this,SLOT(del()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(copy()));
    connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(cut()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(paste()));
    connect(ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(selectAll()));

    connect(ui->actionSelect,SIGNAL(triggered()),selectionToolAction,SIGNAL(triggered()));
    ui->actionSelect->setChecked(true);
    connect(ui->actionMove,SIGNAL(triggered()),panToolAction,SIGNAL(triggered()));
    connect(ui->actionImport,SIGNAL(triggered()),this,SLOT(importAction()));
    connect(ui->actionExport,SIGNAL(triggered()),this,SLOT(exportAction()));

    ui->actionComment->setVisible(false);
    ui->actionSettings->setVisible(false);
    ui->actionLabel->setVisible(false);
    ui->actionDelay->setVisible(false);
    /*/ End Edit Menu /*/

}

/*/ Detects language change, generated by Qt /*/
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*/ Allows panning the screen via a mouse drag /*/
void MainWindow::setHandDragMode()
{
    board->setDragMode(QGraphicsView::ScrollHandDrag);
    QAction* action = ui->mainToolBar->actions().at(ui->mainToolBar->actions().count() - 1);
    foreach(QAction* act, ui->mainToolBar->actions())
    {
        if(act != action)
            act->setChecked(false);
    }
    action->setChecked(true);
}

/*/ Allows a rubberband selection via a mouse drag /*/
void MainWindow::setSelectionMode()
{
    board->setDragMode(QGraphicsView::RubberBandDrag);
    QAction* action = ui->mainToolBar->actions().at(ui->mainToolBar->actions().count() - 2);
    foreach(QAction* act, ui->mainToolBar->actions())
    {
        if(act != action)
            act->setChecked(false);
    }
    action->setChecked(true);
}

/*/ Starts/Stops Timer /*/
void MainWindow::runFunc(bool b)
{
    if(b)
    {
        timer->start();
        ui->actionStep->setDisabled(true);
    }
    else
    {
        timer->stop();
        ui->actionStep->setDisabled(false);
    }
}

/*/ Resets components to their initial state, and resets the simulation /*/
void MainWindow::reset()
{
    board->reset();
    setStatus(QString::number(board->time(),'f',3).append(" ns"));
}

/*/ Stops the timer /*/
void MainWindow::stopFunc()
{
    ui->actionRun->setChecked(false);
}

/*/ The simulation time jumps ahead by stepSize /*/
void MainWindow::step()
{
    setStatus(board->timeStep(stepSize));
}

/*/ Determines the time of the next event and jumps to it /*/
void MainWindow::stepNext()
{
    setStatus(board->timeStep());
}

/*/ Shows the selected component under the cursor, and creates a component on click /*/
void MainWindow::setComponentCreationMode()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
        board->setBuildCFlags(action->data().toInt());
    if(action->data().toInt())
    {
        panToolAction->setChecked(false);
        selectionToolAction->setChecked(false);
    }
}

/*/ For variable-sized components, launches a dialog asking for the size (2 - 32),
    then allows for the creation of chosen component at chosen size /*/
void MainWindow::setCCM_Int()
{
    QAction* action = qobject_cast<QAction*>(sender());
    bool ok = false;
    if(action)
    {
        int count = QInputDialog::getInt(this, action->text(),tr("Width:"),2,2,32,1,&ok);
        if(ok)
            board->setBuildCFlags(action->data().toInt() | ((count - 1) << 5));
    }
    if(ok)
    {
        panToolAction->setChecked(false);
        selectionToolAction->setChecked(false);
    }

}

/*/ Similar to setCCM_Int(), but for multiplexer-sized components /*/
void MainWindow::setCCM_SmallInt()
{
    QAction* action = qobject_cast<QAction*>(sender());
    bool ok = false;
    if(action)
    {
        int count = QInputDialog::getInt(this, action->text(),tr("Multiplexer Width:"),1,1,5,1,&ok);
        if(ok)
            board->setBuildCFlags(action->data().toInt() | (count - 1));
    }
    if(ok)
    {
        panToolAction->setChecked(false);
        selectionToolAction->setChecked(false);
    }

}

/*/ Set chosen view property (such as view connections) to b /*/
void MainWindow::setViewProperty(bool b)
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
        board->setPropertyFlag(action->data().toInt(),b);
}

/*/ Resizes the screen to fit all components, but within bounds (.25 to 4) /*/
void MainWindow::fitToWindow()
{
    if(!board->scene()->items().isEmpty())
    {
        board->fitInView(board->scene()->itemsBoundingRect(),Qt::KeepAspectRatio);
        int scaleF = board->matrix().map(QPointF(1.,1.)).x();
        if(scaleF > 4)
            board->scale(4./scaleF,4./scaleF);
        else if(scaleF < .25)
            board->scale(.25/scaleF,.25/scaleF);
        board->centerOn(board->scene()->itemsBoundingRect().center());
    }
}

/*/ Sets selected zoom /*/
void MainWindow::zoom()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
    {
        board->setTransform(QTransform().scale(action->data().toDouble(),action->data().toDouble()));
    }
}

/*/ Sets status bar text on bottom left /*/
void MainWindow::setStatus(QString s)
{
    timeLabel->setText(s);
    timeLabel->setAlignment(Qt::AlignRight);
}

/*/ Shows simulation time on bottom right /*/
void MainWindow::setStatus(double d)
{
    setStatus(QString::number(d,'f',3).append(" ns"));
}

void MainWindow::loadFile(const QString &fileName)
{
    if(timer->isActive())
        ui->actionRun->setChecked(false);

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    in >> *board;
    board->setTransform(QTransform());
    board->centerOn(board->scene()->itemsBoundingRect().center());

    board->update();
    QApplication::restoreOverrideCursor();

    unsigned int p = board->propertyFlags();
    //io, connection, pinnames;
    ui->actionShow_I_O_Values->setChecked(p & CircuitBoard::SHOW_I_O);
    ui->actionShow_Connections->setChecked(p & CircuitBoard::DISPLAY_CONNECTIONS);
    ui->actionShow_Pin_Names->setChecked(p & CircuitBoard::DISPLAY_NAMES);

    setCurrentFile(fileName);
    setStatus(0);
    statusBar()->showMessage(tr("File loaded"), 5000);
}

void MainWindow::saveFile(const QString &fileName)
{
    if(timer->isActive())
        ui->actionRun->setChecked(false);
    //board->reset();

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << *board;
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 5000);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    setWindowFilePath(currentFile);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFilesAct[i]->setText(text);
        recentFilesAct[i]->setData(files[i]);
        recentFilesAct[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFilesAct[j]->setVisible(false);

    ui->actionSeparator->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::newFile()
{
    MainWindow *other = new MainWindow;
    other->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,"","",tr("Logic File (*.lgc)"));
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::save()
{
    if (currentFile.isEmpty())
        saveAs();
    else
        saveFile(currentFile);
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,"","",tr("Logic File (*.lgc)"));
    if (fileName.isEmpty())
        return;

    saveFile(fileName);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Logic Sim"),
            tr("<b>Logic Sim</b> is a digital logic simulator."
               ""));
}

/*/ Sets simulation update interval... ms per simulation steps /*/
void MainWindow::setInterval()
{
    QAction* action = qobject_cast<QAction*>(sender());
    bool ok = false;
    if(action)
    {
        int i = QInputDialog::getInt(this, action->text(),tr("Interval (ms):"),stepInterval,10,1000,1,&ok);
        if(ok)
        {
            stepInterval = i;
            timer->setInterval(i);
        }
    }
}

/*/ Sets time skip in ns per step /*/
void MainWindow::setStepSize()
{
    QAction* action = qobject_cast<QAction*>(sender());
    bool ok = false;
    if(action)
    {
        double d = QInputDialog::getDouble(this, action->text(),tr("Simulated Step Size (ns):"),stepSize,.001,1000,3,&ok);
        if(ok)
        {
            stepSize = d;
        }
    }
}

/*/ Imports components from a file /*/
void MainWindow::importAction()
{
    QString fileName = QFileDialog::getOpenFileName(this,"","",tr("Logic Export File (*.lgx)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        board->paste(in);

        file.close();
    }
}

/*/ Exports selection to a file /*/
void MainWindow::exportAction()
{
    QString fileName = QFileDialog::getSaveFileName(this,"","",tr("Logic Export File (*.lgx)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        board->copy(out);

        file.close();
    }
}

/*/ Copies selected components /*/
void MainWindow::copy()
{
    board->copy();
}

/*/ Pastes copied components /*/
void MainWindow::paste()
{
    board->paste();
}

/*/ Copies then deletes /*/
void MainWindow::cut()
{
    board->copy();
    board->deleteSelected();
}

/*/ Deletes selected components /*/
void MainWindow::del()
{
    board->deleteSelected();
}

void MainWindow::selectAll()
{
    board->selectAll();
}


