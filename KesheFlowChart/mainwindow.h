#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diagramitem.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
//用到的类声明
class QFontComboBox;
class QComboBox;
class QToolButton;
class QButtonGroup;
class QToolBox;
class QAbstractButton;
class QGraphicsView;
class QGraphicsTextItem;
QT_END_NAMESPACE
class DiagramScene;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void deleteItem();
    void bringToFront();
    void sendToBack();
    //Action基本操作

    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    //字体slots↑
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    //颜色

    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(QAbstractButton *button);//模式选择2
    void pointerGroupClicked();
    //scene对window的反馈↓
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
    //scene
    void sceneScaleChanged(const QString &scale);
    void handleFontChange();
private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolBars();
    //改颜色
    QMenu *createColorMenu(const char *slot,QColor defaultColor);//toolBox内选项事件
    QIcon createColorToolButtonIcon(const QString &image, QColor color);//图标
    QIcon createColorIcon(QColor color);
    //左侧group
    QWidget *createCellWidget(const QString &text,DiagramItem::DiagramType type);//item的
    QWidget *createBackgroudCellWidget(const QString &text,const QString &image);//背景的

private:
    QAction *exitAction;
    QAction *deleteAction;
    QAction *aboutAction;
    QAction *sendBackAction;
    QAction *toFrontAction;
    //Action↑
    QMenu * fileMenu;
    QMenu * itemMenu;
    QMenu * aboutMenu;
    //Menu 指针

    QComboBox *fontSizeCombo;//大小                //字体相关
    QFontComboBox *fontCombo;//字体
    //
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;   //加粗斜体下划线
    //
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;//文本、填充、线
    //颜色toolBox内部选择项的指针
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;
    //
    //toolbar
    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;//模式选择，移动、连线
    QComboBox *sceneScaleCombo;//缩放
    QButtonGroup *pointerTypeGroup;
    //
    QToolBox *toolBox;//左侧选择修改模式
    QButtonGroup *buttonGroup;
    QButtonGroup *backgroundButtonGroup;
    //
    DiagramScene *scene;
    QGraphicsView *view;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
