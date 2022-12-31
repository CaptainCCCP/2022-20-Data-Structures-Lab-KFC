#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qnamespace.h>

MainWindow::MainWindow() : ui(new Ui::MainWindow) {
  ui->setupUi(this);

//  setMouseTracking(true);
//  createActions();
//  createMenus();
//  createToolBars();
//  createToolBox();

//  scene = new DiagramScene(itemMenu,this);
//  scene->setSceneRect(QRect(0,0,5000,5000));
//  view = new QGraphicsView(scene);

//  QHBoxLayout *layout = new QHBoxLayout;
//  layout->addWidget(toolBox);
//  layout->addWidget(view);

//  centralWidget()->setLayout(layout)

  connect(ui->chart_rect, SIGNAL(clicked()), ui->flowChart_widget,SLOT(setPaintChart_Rect()));
  connect(ui->chart_diam, SIGNAL(clicked()), ui->flowChart_widget,SLOT(setPaintChart_Diamond()));
  connect(ui->chart_roundrect, SIGNAL(clicked()), ui->flowChart_widget,SLOT(setPaintChart_RoundRect()));

  connect(ui->actionNewFile, SIGNAL(triggered(bool)), ui->flowChart_widget,SLOT(newChartFile()));
  connect(ui->actionSetting, SIGNAL(triggered(bool)), ui->flowChart_widget,SLOT(Trans()));
  connect(ui->flowChart_widget, SIGNAL(pseudo(const QString &)), this,SLOT(updateMsg(const QString &)));

  ui->verticalLayout->setAlignment(Qt::AlignTop);
  ui->gridLayout_3->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
}

void MainWindow::updateMsg(const QString &value) {
  ui->console->append(value);
}
