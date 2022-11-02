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
    layout->addWidget(toolBox);
    layout->addWidget(view);

    centralWidget()->setLayout(layout);
//signals
    connect(scene, &DiagramScene::itemInserted,this, &MainWindow::itemInserted);
    connect(scene, &DiagramScene::textInserted,this, &MainWindow::textInserted);
    connect(scene, &DiagramScene::itemSelected,this, &MainWindow::itemSelected);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {//知道类型
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
         if (item->type() == DiagramItem::Type)
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
         scene->removeItem(item);
         delete item;
     }
}

void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
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
    const QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    for (QAbstractButton *myButton : buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == QString("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/background1.png"));
    else if (text == QString("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/background2.png"));
    else if (text == QString("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/background4.png"));

    scene->update();
    view->update();
}

void MainWindow::buttonGroupClicked(QAbstractButton *button)
{
    const int id = buttonGroup->id(button);
        const QList<QAbstractButton *> buttons = buttonGroup->buttons();
        for (QAbstractButton *button : buttons) {
            if (buttonGroup->button(id) != button)
                button->setChecked(false);
        }
        if (id == InsertTextButton) {
            scene->setMode(DiagramScene::InsertText);
        } else {
            scene->setItemType(DiagramItem::DiagramType(id));
            scene->setMode(DiagramScene::InsertItem);
        }

}

void MainWindow::pointerGroupClicked()
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void MainWindow::textInserted(QGraphicsTextItem *item)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
    TextDiagramItem *textItem =
    qgraphicsitem_cast<TextDiagramItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void MainWindow::createToolBox()
{

        buttonGroup = new QButtonGroup(this);
        buttonGroup->setExclusive(false);
        connect(buttonGroup,QOverload<QAbstractButton *>::of( &QButtonGroup::buttonClicked),
                this, &MainWindow::buttonGroupClicked);

        //connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
        //      [=](QAbstractButton *button){ /* ... */ });
    QWidget *itemWidget = new QWidget;
    QGridLayout *layout = new QGridLayout;

    layout->addWidget(createCellWidget(QString("分支"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(QString("过程"), DiagramItem::Step),0, 1);
    layout->addWidget(createCellWidget(QString("输入输出"), DiagramItem::Io), 1, 0);
    layout->addWidget(createCellWidget(QString("起点终点"), DiagramItem::StartEnd), 1, 1);
    layout->addWidget(createCellWidget(QString("注释"), DiagramItem::zhuShi), 2, 1);
    QToolButton *textButton = new QToolButton;//文本
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    //
    QWidget *backgroundWidget = new QWidget;
    //
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, QString("图形"));
    //toolBox->addItem(backgroundWidget, QString("背景"));//暂未完成


    //
    QGridLayout *textLayout = new QGridLayout;

    textButton->setIcon(QIcon(QPixmap(":/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(QString("Text")), 1, 0, Qt::AlignCenter);

    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);

    itemWidget->setLayout(layout);

    layout->addWidget(textWidget, 2, 0);
    layout->setRowStretch(3, 10);//拉伸
    layout->setColumnStretch(3, 10);


    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, QOverload<QAbstractButton *>::
            of(&QButtonGroup::buttonClicked),
            this, &MainWindow::backgroundButtonGroupClicked);

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                           ":/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                           ":/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                           ":/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                           ":/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    backgroundWidget->setLayout(backgroundLayout);


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
    //editToolBar = addToolBar(tr("Edit"));
    //editToolBar->addAction(deleteAction);
    //editToolBar->addAction(toFrontAction);
    //editToolBar->addAction(sendBackAction);

    //不同
    //fontCombo = new QFontComboBox();
    //connect(fontCombo, &QFontComboBox::currentFontChanged,
    //        this, &MainWindow::currentFontChanged);
    //
    //fontSizeCombo = new QComboBox;
    //fontSizeCombo->setEditable(true);

    //textToolBar = addToolBar("Text");
    //fontCombo = new QFontComboBox();
    //fontSizeCombo = new QComboBox();
    //fontSizeCombo->setEditable(true);
    //for(int i=8;i<30;i = i+2)
    //    fontSizeCombo->addItem(QString().setNum(i));
    //connect(fontCombo,&QFontComboBox::currentFontChanged,
    //        this,&MainWindow::handleFontChange);
    //connect(fontSizeCombo,&QComboBox::currentTextChanged,
    //        this,&MainWindow::handleFontChange);

    //textToolBar->addWidget(fontCombo);
    //textToolBar->addWidget(fontSizeCombo);//暂未 完成
    //textToolBar->addAction(boldAction);
    //textToolBar->addAction(italicAction);
    //textToolBar->addAction(underlineAction);
//
    //fontColorToolButton = new QToolButton;
    //fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);//风格
    //fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
    //                                             Qt::black));//默认颜色
    //textAction = fontColorToolButton->menu()->defaultAction();
    //fontColorToolButton->setIcon(createColorToolButtonIcon(
    //                                 ":/textpointer.png", Qt::black));
    //fontColorToolButton->setAutoFillBackground(true);
    //connect(fontColorToolButton, &QAbstractButton::clicked,
    //        this, &MainWindow::textButtonTriggered);
//
    //fillColorToolButton = new QToolButton;
    //fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    //fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    //暂未完成
    //fillAction = fillColorToolButton->menu()->defaultAction();
    //fillColorToolButton->setIcon(createColorToolButtonIcon(
    //                                 ":/floodfill.png", Qt::white));
    //connect(fillColorToolButton, &QAbstractButton::clicked,
    //       this, &MainWindow::fillButtonTriggered);
//
    //lineColorToolButton = new QToolButton;
    //lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    //lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    //暂未完成
    //lineAction = lineColorToolButton->menu()->defaultAction();
    //lineColorToolButton->setIcon(createColorToolButtonIcon(
    //                                ":/linecolor.png", Qt::black));
    //connect(lineColorToolButton, &QAbstractButton::clicked,
    //        this, &MainWindow::lineButtonTriggered);

    //放入toolbar
    //colorToolBar = addToolBar(tr("Color"));
    //暂未完成
    //colorToolBar->addWidget(fontColorToolButton);
    //colorToolBar->addWidget(fillColorToolButton);
    //colorToolBar->addWidget(lineColorToolButton);
//模式按钮
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/Mover.jpg"));

    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/linePointer.jpg"));
    //建立按钮组
    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);//group int
   //
    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(1);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

    pointerToolbar = addToolBar(QString("Mode"));
    pointerToolbar->addWidget(sceneScaleCombo);
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);

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
        action->setData(colors.at(i));//设置颜色值
        action->setIcon(createColorIcon(colors.at(i)));//通过颜色设置icon
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);//默认调用的action
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    //pixmap.fill(Qt::transparent);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    //水平居中
    QRect target(4, 0, 42, 43);//一边留4
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
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

QWidget *MainWindow::createCellWidget(const QString &text,
                                      DiagramItem::DiagramType type)
{
    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());//调出图像
//
    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setText(text);//更改
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));
//
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);//上面是button下面是text
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
                                               const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
