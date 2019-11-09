#ifndef SCALE_SELECTOR_H
#define SCALE_SELECTOR_H
#include <QSpinBox>

class scale_selector : public QSpinBox
{
public:
    scale_selector(QWidget *parent);
    void keyPressEvent(QKeyEvent *event);
};

#endif // SCALE_SELECTOR_H
