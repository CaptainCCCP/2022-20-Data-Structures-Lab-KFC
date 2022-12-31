#include "chart_label.h"

void Chart_Label::changeText() {}
void Chart_Label::createText() {}
void Chart_Label::mousePressEvent(QMouseEvent *event) {
  setFrameShape(QFrame::Panel);
  adjustSize();
}
void Chart_Label::mouseDoubleClickEvent(QMouseEvent *event) {
  changeText();
  event->ignore();
}
void Chart_Label::mouseMoveEvent(QMouseEvent *event) { event->ignore(); }
void Chart_Label::mouseReleaseEvent(QMouseEvent *event) {
  setFrameShape(QFrame::NoFrame);
  event->ignore();
}
