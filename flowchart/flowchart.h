#ifndef FLOWCHART_H
#define FLOWCHART_H
// #pragma execution_character_set("utf-8")
#include <map>
#include <stdexcept>

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>
#include <QStyleOption>
#include <QWidget>
#include <exception>
#include <vector>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include "class_Charts/chart_base.h"
#include "class_Charts/chart_diamond.h"
#include "class_Charts/chart_line.h"
#include "class_Charts/chart_rect.h"
#include "class_Charts/chart_roundrect.h"

#include "comdef.h"

class FlowChart : public QWidget {
  Q_OBJECT
private:
  friend class Chart_Base;
  Chart_Base *curPaintChart = nullptr;
  PaintChartType curPaintChartType = PaintChartType::NONE;

  Chart_Base *curSelecChart = nullptr;
  QPoint curSelecChartPos;

  Chart_Line *newLineChart = nullptr;
  Chart_Base *newLineToSelectChart = nullptr;
  Chart_Base *newLineFromSelectChart = nullptr;
  ORIENTION magPointDirect =
      ORIENTION::NONE;
  int magPointFromIndex;
  int magPointToIndex;

  Chart_Base *lineSelectChart = nullptr;
  int magPointIndex;
  ORIENTION sizePointDirect =ORIENTION::NONE;
//用到的容器
  std::vector<Chart_Base *> charts;
  std::vector<Chart_Base *> line;
  std::vector<int> doLoops;
  std::vector<int> DiaInDoLoops;

  std::vector<int> whileLoops;
  std::vector<int> endLoops;

  std::vector<int> checkStartTemp;
  std::vector<int> codeBlockTemp;
  std::vector<int> mergeBlock;
  std::vector<int> condBlock;

  std::vector<std::vector<int>> inoutWhileLoops;
  std::vector<std::vector<int>> inoutDoLoop;

  MOUSE_EVENT_TYPE mouseEventType = MOUSE_EVENT_TYPE::NONE;

  bool fileIsOpened = false;
  bool fileIsSaved = true;
  QString filePath;

  void initVar();
  void resetFlowChartPanel();
//  void setFileSetSaved(bool isSaved);
  void setFileNameShow(bool isSaved);
  void clearChartsLine() {
    for (auto it = charts.begin(); it != charts.end(); it++) {
      if (*it)
        delete *it;
      *it = nullptr;
    }
    for (auto it = line.begin(); it != line.end(); it++) {
      if (*it)
        delete *it;
      *it = nullptr;
    }
  }

public:
  explicit FlowChart(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
  virtual ~FlowChart() {
    newChartFile();
    clearChartsLine();
  }

  Chart_Base *getCurPaintChart();
  Chart_Base *getCurSeleChart();
  void addChart(Chart_Base *cb);
  bool delChart(Chart_Base *&cb);           // 删除
  void addLine(Chart_Base *cb);
  bool delLine(Chart_Base *&cb);
  void hideMagSizeAll();
  int findStart();

  void findNext(int start_id, int space);
  bool findLoopBlocks();
  bool findinLoopBlocks();

      std::vector<int> findInOut(int start_id);
      std::vector<int> findDoInOut(int start_id, int target_id);
      int checkStart(int start_id, int cand_id);
  Chart_Base *getChart(int id);

protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void keyPressEvent(QKeyEvent *ev);
  virtual void keyReleaseEvent(QKeyEvent *ev);

signals:
  void sendChartStyle(QPen &qp, QBrush &qb);
  void
  sendLineStyle(QPen &qp, LINE_HEAD_TYPE &startLineHeadType,
                LINE_HEAD_TYPE &endLineHeadType);
  void disableStyle();
  void updateWindowTitle(const QString &);
  void pseudo(QString meg);

public slots:
  void setPen(QPen &qp) { curSelecChart->paintChartDrawPen = qp; }

public slots:
  void setMousePressedFlag(MOUSE_EVENT_TYPE);
  void setPaintChart();
  void setSelecChart(Chart_Base *, int, int);

public slots:
  void setPaintChart_Rect() {
    curPaintChartType = PaintChartType::RECT;
    setPaintChart();
  }
  void setPaintChart_Diamond() {
    curPaintChartType = PaintChartType::DIAMOND;
    setPaintChart();
  }
  void setPaintChart_RoundRect() {
    curPaintChartType = PaintChartType::ROUNDRECT;
    setPaintChart();
  }
  void setPaintChart_Line() {
    curPaintChartType = PaintChartType::LINE;
    setPaintChart();
  }
  void resetPaintChartType() {
    curPaintChartType = PaintChartType::NONE;
  }

public slots://改变鼠标事件模式
  void setTypeChangeSize(ORIENTION i) {
    mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_SIZE;
    sizePointDirect = i;
  }
  void setTypeCreateMagPoint(Chart_Base *cb, ORIENTION d, int i) {
    newLineFromSelectChart = cb;
    mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT;
    magPointDirect = d;
    magPointFromIndex = i;
    newLineChart = nullptr;
  }


  bool newChartFile();  // 新建文件
  bool Trans();  // 新建文件
};

#endif // FLOWCHART_H
