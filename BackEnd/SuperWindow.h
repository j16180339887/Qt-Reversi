#ifndef SUPERWINDOW_H
#define SUPERWINDOW_H

#include <QMainWindow>

namespace Ui {
class SuperWindow;
}

class SuperWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SuperWindow(QWidget *parent = 0);
    ~SuperWindow();

private:
    Ui::SuperWindow *ui;
};

#endif // SUPERWINDOW_H
