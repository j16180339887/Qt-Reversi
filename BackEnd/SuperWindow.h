#ifndef SUPERWINDOW_H
#define SUPERWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGridLayout>
#include "Piece.h"
#define PieceSize 8

typedef QVector< QVector<Piece*> > ChessPieces;

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
    ChessPieces Pieces;

private slots:
    void getDropPiece(int, int);
};

#endif // SUPERWINDOW_H
