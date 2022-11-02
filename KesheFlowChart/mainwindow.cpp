#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "diagramscene.h"
#include "arrow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    createMenus();
    createToolBars();
    createToolBox();

    scene = new DiagramScene(itemMenu,this);
    scene->setSceneRect(QRect(0,0,5000,5000));
    view = new QGraphicsView(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    //layout->addWidget(toolBox);
    layout->addWidget(view);

    centralWidget()->setLayout(layout);
//signals
    connect(scene, &DiagramScene::itemInserted,this, &MainWindow::itemInserted);
    connect(scene, &DiagramScene::textInserted,this, &MainWindow::textInserted);
    connect(scene, &DiagramScene::itemSelected,this, &MainWindow::itemSelected);

    //createToolbars();


}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::deleteItem()
{

}

void MainWindow::bringToFront()
{

}

void MainWindow::sendToBack()
{

}

void MainWindow::currentFontChanged(const QFont &font)
{

}

void MainWindow::fontSizeChanged(const QString &size)
{

}

void MainWindow::textButtonTriggered()
{

}

void MainWindow::fillButtonTriggered()
{

}

void MainWindow::lineButtonTriggered()
{

}

void MainWindow::textColorChanged()
{

}

void MainWindow::itemColorChanged()
{

}

void MainWindow::lineColorChanged()
{

}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{

}

void MainWindow::buttonGroupClicked(QAbstractButton *button)
{

}

void MainWindow::pointerGroupClicked()
{

}

void MainWindow::itemInserted(DiagramItem *item)
{

}

void MainWindow::textInserted(QGraphicsTextItem *item)
{

}

void MainWindow::itemSelected(QGraphicsItem *item)
{

}

void MainWindow::sceneScaleChanged(const QString &scale)
{

}

void MainWindow::createToolBox()
{

}

void MainWindow::createActions()
{
    exitAction = new QAction("E&xit",this);
    connect(exitAction,&QAction::triggered,this,&QWidget::close);

    aboutAction = new QAction(tr("A&bout"),this);
    aboutAction->setShortcut(/*tr(F1)*/QKeySequence::HelpContents);
    //tr translate
    auto lambda=[this](){
        QMessageBox::about(this,
                           tr("About Diagram Scene"),
                           tr("The <b>Diagram Scene</b> example shows "
                              "use of the graphics framework."));
    };
    connect(aboutAction,&QAction::triggered,this,lambda);

    toFrontAction = new QAction(QIcon(":/bringtofront.png"),"Bring to &Front",this);
    connect(toFrontAction,&QAction::triggered,this,&MainWindow::bringToFront);

    deleteAction = new QAction(QIcon(":/delete.png"),"Delete",this);
    connect(deleteAction,&QAction::triggered,this,&MainWindow::deleteItem);

    sendBackAction = new QAction(QIcon(":/sendtoback.png"),"Send to back",this);
    connect(sendBackAction,&QAction::triggered,this,&MainWindow::sendToBack);

    //加粗下划线倾斜
    boldAction = new QAction(QIcon(":/bold.png"),"Bold", this);
    boldAction->setCheckable(true);
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    italicAction = new QAction(QIcon(":/italic.png"), "Italic", this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    underlineAction = new QAction(QIcon(":/underline.png"), "Underline", this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, &QAction::triggered, this, &MainWindow::handleFontChange);

}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();//分割
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void MainWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    //不同
    //fontCombo = new QFontComboBox();
    //connect(fontCombo, &QFontComboBox::currentFontChanged,
    //        this, &MainWindow::currentFontChanged);
    //
    //fontSizeCombo = new QComboBox;
    //fontSizeCombo->setEditable(true);

    textToolBar = addToolBar("Text");
    fontCombo = new QFontComboBox();
    fontSizeCombo = new QComboBox();
    fontSizeCombo->setEditable(true);
    for(int i=8;i<30;i = i+2)
        fontSizeCombo->addItem(QString().setNum(i));
    connect(fontCombo,&QFontComboBox::currentFontChanged,
            this,&MainWindow::handleFontChange);
    connect(fontSizeCombo,&QComboBox::currentTextChanged,
            this,&MainWindow::handleFontChange);

    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
//
    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);//风格
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
                                                 Qt::black));//默认颜色
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/textpointer.png", Qt::black));
    //fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::textButtonTriggered);
//
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/floodfill.png", Qt::white));
    connect(fillColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::fillButtonTriggered);
//
    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/linecolor.png", Qt::black));
    connect(lineColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::lineButtonTriggered);

    //放入toolbar
    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);
//模式按钮
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/pointer.png"));

    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/linepointer.png"));
    //建立按钮组
    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);
   //
    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(1);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);

}

QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString &image, QColor color)
{
    return QIcon();
}

QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);//无边框
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}

void MainWindow::handleFontChange()
{

}

QWidget *MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{

}

QWidget *MainWindow::createBackgroudCellWidget(const QString &text, const QString &image)
{

}
