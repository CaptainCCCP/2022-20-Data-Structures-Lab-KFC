#include "flowchart.h"
#include <vector>

FlowChart::FlowChart(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
  initVar();
  setMouseTracking(true);
  update();
}

void FlowChart::initVar() {
  mouseEventType = MOUSE_EVENT_TYPE::NONE;
  curPaintChart = nullptr;
  curSelecChart = nullptr;
  newLineChart = nullptr;
  newLineFromSelectChart = nullptr;
  newLineToSelectChart = nullptr;
}

void FlowChart::resetFlowChartPanel() {
  Chart_Base::resetStaticVal();
  curPaintChart = nullptr;
  curPaintChartType = PaintChartType::NONE;
  curSelecChart = nullptr;
  curSelecChartPos = QPoint();
  newLineChart = nullptr;
  newLineToSelectChart = nullptr;
  newLineFromSelectChart = nullptr;
  magPointDirect = ORIENTION::NONE;
  magPointFromIndex = 0;
  magPointToIndex = 0;
  sizePointDirect = ORIENTION::NONE;
  clearChartsLine();
  charts.reserve(0);
  charts.resize(0);
  line.reserve(0);
  line.resize(0);
  mouseEventType = MOUSE_EVENT_TYPE::NONE;
  fileIsOpened = false;
  fileIsSaved = true;
  filePath = "";
  setFileNameShow(fileIsSaved);
}

void FlowChart::setFileNameShow(bool isSaved) {
  QString tmp;
  if (fileIsOpened) {
    tmp = QString("%1%2").arg(filePath).arg((isSaved ? "" : " *"));
    emit updateWindowTitle(tmp);
  } else {
    tmp = QString("%1%2").arg("Untitle").arg((isSaved ? "" : " *"));
    emit updateWindowTitle(tmp);
  }
}

Chart_Base *FlowChart::getCurPaintChart() { return curPaintChart; }

void FlowChart::setMousePressedFlag(MOUSE_EVENT_TYPE f) { mouseEventType = f; }

void FlowChart::setPaintChart() {
  if (curPaintChart != nullptr)
    delete curPaintChart;

  switch (curPaintChartType) {
  case PaintChartType::RECT: {
    curPaintChart = new Chart_Rect(this);
  } break;
  case PaintChartType::DIAMOND: {
    curPaintChart = new Chart_Diamond(this);
  } break;
  case PaintChartType::ROUNDRECT: {
    curPaintChart = new Chart_RoundRect(this);
  } break;
  case PaintChartType::LINE: {
    curPaintChart = new Chart_Line(this);
  } break;
  default:
  case PaintChartType::NONE: {
    curPaintChart = nullptr;
  } break;
  }
  if (curPaintChart) {
    mouseEventType = MOUSE_EVENT_TYPE::CREATING_CHANGE_SIZE;
  }
}

void FlowChart::setSelecChart(Chart_Base *cb, int x, int y) {

  this->grabKeyboard();
  if (curSelecChart != nullptr)
    curSelecChart->hideMagSize();
  curSelecChart = cb;
  curSelecChart->showMagSize();
  curSelecChartPos = QPoint(x, y);
  mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS;
  Chart_Line *cl = nullptr;
  if (nullptr == (cl = dynamic_cast<Chart_Line *>(cb))) {
    emit sendChartStyle(cb->paintChartDrawPen, cb->paintChartFillPen);
  } else {
    emit sendLineStyle(cl->paintChartDrawPen, cl->getStartLineHeadType(),
                       cl->getEndLineHeadType());
  }
}

bool FlowChart::newChartFile() {
  resetFlowChartPanel();
  return true;
}

bool FlowChart::Trans() {

  int start_id = findStart();
  if (start_id == -1) {
    qDebug() << "找不到开始";
    emit pseudo(QString("找不到开始"));
    return false;
  }
  Chart_Base *start_cb = charts[start_id];
  emit pseudo(QString("start: "));
  emit pseudo(start_cb->chartText.textType2->toPlainText());
  findLoopBlocks();
  findinLoopBlocks();

  endLoops.clear();
  codeBlockTemp.clear();
  condBlock.clear();
  findNext(start_id, 0);

  return true;
}

Chart_Base *FlowChart::getCurSeleChart() { return curSelecChart; }

void FlowChart::addChart(Chart_Base *cb) { charts.push_back(cb); }

bool FlowChart::delChart(Chart_Base *&cb) {
  for (auto it = charts.begin(); it != charts.end(); it++) {
    if (*it == cb) {

      Chart_Base *tmp = cb;
      cb = nullptr;
      charts.erase(it);

      for (auto magit = tmp->magPoint.i_point.begin();
           magit != tmp->magPoint.i_point.end(); magit++) {
        for (auto magLineStIt = (*magit)->i_lineStart.begin();
             magLineStIt != (*magit)->i_lineStart.end(); magLineStIt++) {
          dynamic_cast<Chart_Line *>(*magLineStIt)->resetEndChart();
        }
        for (auto magLineEnIt = (*magit)->i_lineEnd.begin();
             magLineEnIt != (*magit)->i_lineEnd.end(); magLineEnIt++) {
          dynamic_cast<Chart_Line *>(*magLineEnIt)->resetStartChart();
        }
      }
      delete tmp;
      return true;
    }
  }
  return false;
}
void FlowChart::addLine(Chart_Base *cb) { line.push_back(cb); }
bool FlowChart::delLine(Chart_Base *&cb) {
  for (auto it = line.begin(); it != line.end(); it++) {
    if (*it == cb) {
      Chart_Line *tmp = dynamic_cast<Chart_Line *>(*it);
      cb = nullptr;
      line.erase(it);
      qDebug() << tmp->magPoint.i_point.size();

      tmp->resetEndChart();
      tmp->resetStartChart();
      delete tmp;
      return true;
    }
  }
  return false;
}

void FlowChart::hideMagSizeAll() {
  for (auto it = charts.begin(); it != charts.end(); it++) {
    (*it)->hideMagSize();
  }
  for (auto it = line.begin(); it != line.end(); it++) {
    (*it)->hideMagSize();
  }
  curSelecChart = nullptr;
  mouseEventType = MOUSE_EVENT_TYPE::NONE;
}

bool contains(std::vector<int> vec, int a) {
  if (vec.empty()) {
    return false;
  }
  for (auto i = 0; i < vec.size(); i++) {
    if (vec[i] == a) {
      return true;
    }
  }
  return false;
}

bool FlowChart::findLoopBlocks() {

  doLoops.clear();
  whileLoops.clear();
  DiaInDoLoops.clear();
  mergeBlock.clear();

  std::vector<int> tempDo;
  std::vector<int> tempIds;
  for (auto it = line.begin(); it != line.end(); it++) {
    Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);
    if (cl->getEndChart()) {
      int c_id = cl->getEndChart()->getID();
      if (tempIds.empty()) {
        tempIds.push_back(c_id);
        continue;
      }
      if (contains(tempIds, c_id)) {
        Chart_Base *c_s = getChart(c_id);
        if (c_s->chartType == PaintChartType::DIAMOND) {
          whileLoops.push_back(c_id);
          continue;
        }
        if (c_s->chartType == PaintChartType::RECT) {
          tempDo.push_back(c_id);
          continue;
        }
      } else {
        tempIds.push_back(c_id);
      }
    }
  }

  for (int i = 0; i < tempDo.size(); i++) {
    for (auto it = line.begin(); it != line.end(); it++) {
      Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);
      if (cl->getEndChart()) {
        if (cl->getEndChart()->getID() == tempDo[i] &&
            cl->getStartChart()->chartType == PaintChartType::DIAMOND) {
          doLoops.push_back(tempDo[i]);
          DiaInDoLoops.push_back(cl->getStartChart()->getID());
        }
      }
    }
  }
  for (int i = 0; i < tempDo.size(); i++) {
    if (!contains(doLoops, tempDo[i])) {
      mergeBlock.push_back(tempDo[i]);
    }
  }

  return true;
}

int FlowChart::checkStart(int start_id, int cand_id) {

  int result = 0;
  if (getChart(start_id)->chartType == PaintChartType::ROUNDRECT) {
    result++;
  }
  for (auto it = line.begin(); it != line.end(); it++) {
    Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);

    if (cl->getStartChart()) {

      if (cl->getStartChart()->getID() == start_id) {
        int end_id = cl->getEndChart()->getID();
        if (contains(checkStartTemp, end_id)) {//防止无限循环
          continue;
        } else {
          checkStartTemp.push_back(end_id);
        }
        if (cl->getEndChart()->chartType == PaintChartType::ROUNDRECT) {
          result++;
        }
        if (end_id != cand_id) {
          result = result + checkStart(end_id, cand_id);
        }
      }
    }//TODO:
  }
  return result;
}

std::vector<int> FlowChart::findInOut(int start_id) {
  std::vector<int> cand_id;
  int inloop, outloop;

  for (auto it = line.begin(); it != line.end(); it++) {
    Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);
    if (cl->getStartChart()) {
      if (cl->getStartChart()->getID() == start_id) {
        int end_id = cl->getEndChart()->getID();
        checkStartTemp.clear();
        int result = checkStart(end_id, start_id);
        if (result == 0) {
          inloop = end_id;
        } else {
          outloop = end_id;
        }
      }
    }
  }
  cand_id.push_back(inloop);
  cand_id.push_back(outloop);
  return cand_id;
}

std::vector<int> FlowChart::findDoInOut(int start_id, int target_id) {

  std::vector<int> cand_id;
  int inloop, outloop;
  for (auto it = line.begin(); it != line.end(); it++) {
    Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);
    if (cl->getStartChart()) {
      if (cl->getStartChart()->getID() == start_id) {
        int end_id = cl->getEndChart()->getID();
        if (end_id == target_id) {
          inloop = end_id;
        } else {
          outloop = end_id;
        }
      }
    }
  }
  cand_id.push_back(inloop);
  cand_id.push_back(outloop);
  return cand_id;
}

bool FlowChart::findinLoopBlocks() {
  std::vector<int> stempIds;

  inoutWhileLoops.clear();
  inoutDoLoop.clear();
  for (auto i = 0; i < whileLoops.size(); i++) {
    inoutWhileLoops.push_back(findInOut(whileLoops[i]));
  }

  for (auto i = 0; i < DiaInDoLoops.size(); i++) {
    inoutDoLoop.push_back(findDoInOut(DiaInDoLoops[i], doLoops[i]));
  }
  return true;
}

int FlowChart::findStart() {

  for (auto it = line.begin(); it != line.end(); it++) {
    Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);
    if (cl->getStartChart()) {
      int c_id = cl->getStartChart()->getID();
      Chart_Base *c_s = getChart(c_id);
      if (c_s->chartType == PaintChartType::ROUNDRECT) {
        return c_id;
      }
    }
  }

  return -1;
}

int findIndex(std::vector<int> vec, int a) {
  for (int i = 0; i < vec.size(); i++) {
    if (a == vec[i])
      return i;
  }
  return -1;
}

Chart_Base *FlowChart::getChart(int id) {
  for (auto i = 0; i < charts.size(); i++) {
    if (charts[i]->getID() == id) {
      return charts[i];
    }
  }
  return charts[-1];
}

void FlowChart::findNext(int start_id, int space) {
  for (auto it = line.begin(); it != line.end(); it++) {
    Chart_Line *cl = dynamic_cast<Chart_Line *>(*it);
    if (cl->getStartChart()) {
      int s_id = cl->getStartChart()->getID();
      if (s_id == start_id) {
        Chart_Base *e_cb = cl->getEndChart();
        int e_id = e_cb->getID();
        if (contains(codeBlockTemp, e_id)) {//第二次遇到一个图形，情况有merge、do、while内循环结束

          if (contains(whileLoops, e_id)) {
            int index = findIndex(whileLoops, e_id);
            int out_id = inoutWhileLoops[index][1];//
            Chart_Base *temp1 = getChart(out_id);

            if (temp1->chartType != PaintChartType::DIAMOND) {//出口不是另一循环
              emit pseudo(QString(space - 2, ' ') +
                          temp1->chartText.textType2->toPlainText());
              findNext(out_id, space - 2);
            } else {//循环嵌套时出口是另一个循环
              if (contains(whileLoops, out_id)) {
                findNext(out_id, space - 4);
              } else if (contains(DiaInDoLoops, out_id)) {
                emit pseudo(QString(space - 4, ' ') + QString("while") +
                            temp1->chartText.textType2->toPlainText());
                findNext(out_id, space - 4);
              } else {
                findNext(out_id, space - 4);
              }
            }
          } else if (contains(mergeBlock, e_id)) {//TODO:可以加else,是缩进回退的地方
            emit pseudo(QString(space - 2, ' ') +
                        e_cb->chartText.textType2->toPlainText());
            findNext(e_id, space - 2);
          }
          continue;
        } else {
          codeBlockTemp.push_back(e_cb->getID());
        }//以下为第一次遇到的操作
        if (cl->chartText.textType2->toPlainText() != "") {//线的信息
          emit pseudo(QString(space, ' ') +
                      cl->chartText.textType2->toPlainText());
        }
        switch (e_cb->chartType) {

        case PaintChartType::RECT: {
          int e_id = e_cb->getID();
          if (contains(doLoops, e_id)) {//可能是doloop开始点
            // int index = findIndex(doLoops, e_id);
            if (contains(endLoops, e_id)) {
              continue;
            } else {
              endLoops.push_back(e_id);
              emit pseudo(QString(space, ' ') + QString("do"));
              emit pseudo(QString(space + 2, ' ') + e_cb->chartText.textType2->toPlainText());
              findNext(e_id, space + 2);
            }
          } else if (contains(mergeBlock, e_id)) {
            continue;
          } else { //顺序打印
            emit pseudo(QString(space, ' ') +
                        e_cb->chartText.textType2->toPlainText());
            findNext(e_id, space);
          }
        } break;
        case PaintChartType::DIAMOND: {//棱形第一次遇到
          int e_id = e_cb->getID();
          if (contains(DiaInDoLoops, e_id)) {
            if (contains(endLoops, e_id)) {
              continue;
            } else {
              endLoops.push_back(e_id);
              emit pseudo(QString(space - 2, ' ') + QString("while (") +
                          e_cb->chartText.textType2->toPlainText()+ QString(")"));

              int index = findIndex(DiaInDoLoops, e_id);
              int out_id = inoutDoLoop[index][1];
              Chart_Base *temp1 = getChart(out_id);
              if (temp1->chartType != PaintChartType::DIAMOND) {//doloop里out出来不是另外一个菱形
                emit pseudo(QString(space - 2, ' ') +
                            temp1->chartText.textType2->toPlainText());
                findNext(out_id, space - 2);
              } else {
                // emit pseudo(QString("out do loop"));
                findNext(out_id, space - 4);//是另一个循环，所以要继续缩进
              }
            }
          } else if (contains(whileLoops, e_id)) {
            if (contains(endLoops, e_id)) {//TODO://
              emit pseudo(QString("out while loop"));
              int index = findIndex(whileLoops, e_id);
              int out_id = inoutWhileLoops[index][1];
              Chart_Base *temp1 = getChart(out_id);

              if (temp1->chartType != PaintChartType::DIAMOND) {
                emit pseudo(QString(space - 2, ' ') +
                            temp1->chartText.textType2->toPlainText());
                findNext(out_id, space - 2);
              } else {
                if (contains(whileLoops, out_id)) {
                  findNext(out_id, space - 4);
                } else if (contains(DiaInDoLoops, out_id)) {
                  emit pseudo(QString(space - 4, ' ') + QString("while ") +
                              temp1->chartText.textType2->toPlainText());
                  findNext(out_id, space - 4);//TODO://
                } else {
                  findNext(out_id, space - 4);
                }
              }

              emit pseudo(QString(space - 2, ' ') +
                          temp1->chartText.textType2->toPlainText());
              findNext(out_id, space - 2);
            } else {
              endLoops.push_back(e_id);
              emit pseudo(QString(space, ' ') + QString("while (") +e_cb->chartText.textType2->toPlainText()+QString(")"));
              int index = findIndex(whileLoops, e_id);
              int in_id = inoutWhileLoops[index][0];//是内循环
              Chart_Base *temp1 = getChart(in_id);
              emit pseudo(QString(space + 2, ' ') + temp1->chartText.textType2->toPlainText());

              codeBlockTemp.push_back(in_id);
              findNext(in_id, space + 2);
            }
          } else {//剩一个if条件控制
            emit pseudo(QString(space, ' ') + QString("if ") +e_cb->chartText.textType2->toPlainText());
            int new_space = space + 2;
            condBlock.push_back(e_id);//TODO:del
            findNext(e_id, new_space);
          }
        } break;
        case PaintChartType::ROUNDRECT: {
          emit pseudo(QString(space, ' ') + QString("return ") + e_cb->chartText.textType2->toPlainText());
          // return;
        } break;
        default: {
        } break;
        }
      }
    }
  }
}

void FlowChart::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.setPen(QColor(150, 150, 150));
  const int gap = 30;
  for (int i = 1; i * gap < width(); i++) {
    if (i % 5 == 0)
      p.drawLine(QLineF(i * gap, 0, i * gap, height()));
  }
  for (int i = 1; i * gap < height(); i++) {
    if (i % 5 == 0)
      p.drawLine(QLineF(0, i * gap, width(), i * gap));
  }

  p.setPen(QColor(200, 200, 200, 150));
  for (int i = 1; i * gap < width(); i++) {
    if (i % 5 == 0)
      continue;
    p.drawLine(QLineF(i * gap, 0, i * gap, height()));
  }
  for (int i = 1; i * gap < height(); i++) {
    if (i % 5 == 0)
      continue;
    p.drawLine(QLineF(0, i * gap, width(), i * gap));
  }

  QStyleOption opt;
  opt.init(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}
void FlowChart::mousePressEvent(QMouseEvent *event) {
  event->ignore();
  switch (mouseEventType) {
  case MOUSE_EVENT_TYPE::CREATING_CHANGE_SIZE: {
    int x = event->pos().rx();
    int y = event->pos().ry();
    if (curSelecChart)
      curSelecChart->hideMagSize();
    curSelecChart = curPaintChart;
    curPaintChart->setXY(x, y);
    if (curPaintChart->chartType == PaintChartType::LINE) {
      connect(curPaintChart, SIGNAL(sendThisClass(Chart_Base *, int, int)),
              this, SLOT(setSelecChart(Chart_Base *, int, int)));
      connect(curPaintChart, SIGNAL(setTypeChangeSize(ORIENTION)), this,
              SLOT(setTypeChangeSize(ORIENTION)));
      addLine(curPaintChart);
      Chart_Line *cl = dynamic_cast<Chart_Line *>(curSelecChart);
      emit sendLineStyle(cl->paintChartDrawPen, cl->getStartLineHeadType(),
                         cl->getEndLineHeadType());
    } else {
      connect(curPaintChart, SIGNAL(sendThisClass(Chart_Base *, int, int)),
              this, SLOT(setSelecChart(Chart_Base *, int, int)));
      connect(curPaintChart, SIGNAL(setTypeChangeSize(ORIENTION)), this,
              SLOT(setTypeChangeSize(ORIENTION)));
      connect(curPaintChart,
              SIGNAL(setTypeCreateMagPoint(Chart_Base *, ORIENTION, int)), this,
              SLOT(setTypeCreateMagPoint(Chart_Base *, ORIENTION, int)));
      addChart(curPaintChart);
      emit sendChartStyle(curSelecChart->paintChartDrawPen,
                          curSelecChart->paintChartFillPen);
    }
    curPaintChart->update();
    curPaintChart->show();
    event->accept();
  } break;
  default:
  case MOUSE_EVENT_TYPE::NONE: {
    for (auto it = charts.begin(); it != charts.end(); it++) {
      (*it)->overlapChartMousePressed(event);
      if (event->isAccepted())
        return;
    }
    for (auto a : line) {
      a->overlapChartMousePressed(event);
      if (event->isAccepted())
        return;
    }
    hideMagSizeAll();
  } break;
  }
}
void FlowChart::mouseMoveEvent(QMouseEvent *event) {
  event->ignore();
  switch (mouseEventType) {
  default:
  case MOUSE_EVENT_TYPE::NONE: {
    for (auto it = charts.begin(); it != charts.end(); it++) {
      (*it)->overlapChartMouseMove(event);
      if (event->isAccepted())
        return;
    }
    for (auto it = line.begin(); it != line.end(); it++) {
      (*it)->overlapChartMouseMove(event);
      if (event->isAccepted())
        return;
    }
    if (curSelecChart == nullptr) {
      setCursor(QCursor(Qt::ArrowCursor));
      hideMagSizeAll();
    }
  } break;
  case MOUSE_EVENT_TYPE::CREATING_CHANGE_SIZE: {
    int x = event->pos().rx(), y = event->pos().ry();
    if (curSelecChart) {
      if (curSelecChart->chartType == PaintChartType::LINE) {
        for (auto it = charts.begin(); it != charts.end(); it++) {
          if ((*it)->autoSetMagi(x, y, magPointIndex)) {
            lineSelectChart = *it;
            break;
          } else {
            lineSelectChart = nullptr;
          }
        }
      }
      curPaintChart->setWidthHeight(x, y);
    }
    event->accept();
  } break;
  case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_SIZE: {
    int x = event->pos().rx(), y = event->pos().ry();

    if (curSelecChart->chartType == PaintChartType::LINE) {
      for (auto it = charts.begin(); it != charts.end(); it++) {
        if ((*it)->autoSetMagi(x, y, magPointIndex)) {
          lineSelectChart = *it;
          break;
        } else {
          lineSelectChart = nullptr;
        }
      }
    }
    curSelecChart->setWidthHeight(x, y, sizePointDirect);
    event->accept();
  } break;
  case MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT: {
    if (newLineChart == nullptr) {
      newLineChart = new Chart_Line(this);
      if (curSelecChart)
        curSelecChart->hideMagSize();
      curSelecChart = newLineChart;
      addLine(newLineChart);
      connect(newLineChart, SIGNAL(sendThisClass(Chart_Base *, int, int)), this,
              SLOT(setSelecChart(Chart_Base *, int, int)));
      connect(newLineChart, SIGNAL(setTypeChangeSize(ORIENTION)), this,
              SLOT(setTypeChangeSize(ORIENTION)));
      newLineChart->setXY(
          newLineFromSelectChart->getMagiPointAbsX(magPointFromIndex),
          newLineFromSelectChart->getMagiPointAbsY(magPointFromIndex));
      newLineChart->setStartChart(newLineFromSelectChart);
      newLineChart->setStartMagIndex(magPointFromIndex);
      newLineChart->setStartDirect(magPointDirect);
      newLineChart->update();
      newLineChart->show();

      newLineFromSelectChart->addMagiPointStartLine(magPointFromIndex,
                                                    newLineChart);
    }
    int x = event->pos().rx(), y = event->pos().ry();

    for (auto it = charts.begin(); it != charts.end(); it++) {
      if ((*it)->autoSetMagi(x, y, magPointToIndex)) {
        newLineToSelectChart = *it;
        break;
      } else {
        newLineToSelectChart = nullptr;
      }
    }
    newLineChart->setWidthHeight(x, y, ORIENTION::ENDPOINT);
    event->ignore();
  } break;
  case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS: {
    if (curSelecChart->chartType != PaintChartType::LINE) {
      curSelecChart->setXY(event->pos().rx() - curSelecChartPos.rx(),
                           event->pos().ry() - curSelecChartPos.ry());
      event->accept();
    }
  } break;
  }
}

void FlowChart::mouseReleaseEvent(QMouseEvent *event) {
  switch (mouseEventType) {
  case MOUSE_EVENT_TYPE::CREATING_CHANGE_SIZE: {
    resetPaintChartType();
    if (curSelecChart->chartType == PaintChartType::LINE) {
      Chart_Line *cl = reinterpret_cast<Chart_Line *>(curSelecChart);

      if (lineSelectChart) {
        lineSelectChart->addMagiPointEndLine(magPointIndex, curSelecChart);
        lineSelectChart->hideMagOnly();
        cl->setEndChart(lineSelectChart);
        cl->setEndMagIndex(magPointIndex);
        cl->setEndDirect(lineSelectChart->getMagiPointDirect(magPointIndex));
        cl->update();
        lineSelectChart = nullptr;
      } else {
        cl->resetEndChart();
      }
    }

    curPaintChart = nullptr;
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
    event->accept();
  } break;
  case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_SIZE: {
    if (curSelecChart->chartType == PaintChartType::LINE) {
      Chart_Line *cl = reinterpret_cast<Chart_Line *>(curSelecChart);
      if (lineSelectChart) {
        if (sizePointDirect == ORIENTION::STARTPOINT) {
          cl->resetStartChart();
          lineSelectChart->addMagiPointStartLine(magPointIndex, curSelecChart);
          lineSelectChart->hideMagOnly();
          cl->setStartChart(lineSelectChart);
          cl->setStartMagIndex(magPointIndex);
          cl->setStartDirect(
              lineSelectChart->getMagiPointDirect(magPointIndex));
        } else if (sizePointDirect == ORIENTION::ENDPOINT) {
          cl->resetEndChart();
          lineSelectChart->addMagiPointEndLine(magPointIndex, curSelecChart);
          lineSelectChart->hideMagOnly();
          cl->setEndChart(lineSelectChart);
          cl->setEndMagIndex(magPointIndex);
          cl->setEndDirect(lineSelectChart->getMagiPointDirect(magPointIndex));
        }
        cl->update();
      } else {
        if (sizePointDirect == ORIENTION::STARTPOINT) {
          cl->resetStartChart();
        } else if (sizePointDirect == ORIENTION::ENDPOINT) {
          cl->resetEndChart();
        }
      }
    }
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
    lineSelectChart = nullptr;
    event->accept();
  } break;
  case MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT: {
    if (newLineToSelectChart) {
      newLineToSelectChart->addMagiPointEndLine(magPointToIndex, newLineChart);
      newLineToSelectChart->hideMagOnly();
      newLineChart->setEndChart(newLineToSelectChart);
      newLineChart->setEndMagIndex(magPointToIndex);
      newLineChart->setEndDirect(
          newLineToSelectChart->getMagiPointDirect(magPointToIndex));
      newLineChart->update();
    } else {
      if (newLineChart)
        newLineChart->resetEndChart();
    }
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
    newLineChart = nullptr;
    newLineFromSelectChart = nullptr;
    newLineToSelectChart = nullptr;
    event->accept();
  } break;
  case MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS: {
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
    event->accept();
  } break;
  default:
  case MOUSE_EVENT_TYPE::NONE: {
    event->ignore();
  } break;
  }
}

void FlowChart::keyPressEvent(QKeyEvent *ev) {
  ev->ignore();
  switch (ev->key()) {
  case Qt::Key_Escape: {
    if (curSelecChart) {
      curSelecChart->hideMagSize();
      curSelecChart = nullptr;
      this->releaseKeyboard();
    }
  } break;
  case Qt::Key_Delete: {
    if (curSelecChart) {
      if (curSelecChart->chartType == PaintChartType::LINE) {
        if (!delLine(curSelecChart)) {
          qDebug() << "Error";
        }
      } else {
        if (!delChart(curSelecChart)) {
          qDebug() << "Error";
        }
      }
      curSelecChart = nullptr;
    }
  } break;
  default: {
    ev->ignore();
  }
  }
}
void FlowChart::keyReleaseEvent(QKeyEvent *ev) { ev->ignore(); }
