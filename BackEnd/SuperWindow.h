#ifndef SUPERWINDOW_H
#define SUPERWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGridLayout>
#include <QDebug>
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
    int NumberOfPieceEat(int, int, int, int);
    Type Player;
    Type Enemy;
    int NumberOfBlack;
    int NumberOfWhite;

private slots:
    void getDropPiece(int, int );
    void Restart();
    void Undo();
    void Redo();
};

#endif // SUPERWINDOW_H
