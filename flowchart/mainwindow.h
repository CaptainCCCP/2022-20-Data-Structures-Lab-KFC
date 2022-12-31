#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include "comdef.h"
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  // explicit MainWindow(QWidget *parent = nullptr);
  MainWindow();
  // ~MainWindow();

public slots:
  void updateMsg(const QString &);

private:
  Ui::MainWindow *ui;
  void createToolBox();
  void createActions();
  void createMenus();
  void createToolBars();
  // public slots:
  //   void changeWindowTitle(const QString &);
};

#endif // MAINWINDOW_H
