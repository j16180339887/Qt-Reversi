#ifndef SUPERWINDOW_H
#define SUPERWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGridLayout>
#include <QDebug>
#include "Piece.h"
#define PieceSize 8

typedef QVector< QVector<Piece*> > ChessPieces;
typedef QVector< QVector< QVector<Type> > > BACKUP;

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
    int totalMove;
    int MaxBackUpMove;
    BACKUP BackUp;
    void Refresh();

private slots:
    void getDropPiece(int, int );
    void Restart();
    void Undo();
    void Redo();
};

#endif // SUPERWINDOW_H
