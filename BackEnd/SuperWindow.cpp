#include "SuperWindow.h"
#include "ui_SuperWindow.h"

SuperWindow::SuperWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SuperWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Qt-Reversi");

    Pieces.resize(PieceSize);

    /* Pieces[PieceSize][PieceSize] */
    for(int i = 0; i < PieceSize; i++){
        Pieces[i].resize(PieceSize);
    }

    QGridLayout* ChessBoardLayout = new QGridLayout;

    ChessBoardLayout->setMargin(0);
    ChessBoardLayout->setSpacing(0);

    /* Construct Pieces[PieceSize][PieceSize] */
    for(int i = 0; i < PieceSize; i++)
    {
        for(int j = 0; j < PieceSize; j++)
        {
            Pieces[i][j] = new Piece();
            Pieces[i][j]->setIndex(i, j);
            Pieces[i][j]->setFixedSize(40, 40);

            connect(Pieces[i][j], SIGNAL(DropPiece(int,int)), this, SLOT(getDropPiece(int,int)));

            ChessBoardLayout->addWidget(Pieces[i][j], i*40, j*40);
        }
    }

    ui->ChessBoardLabel->setLayout(ChessBoardLayout);
}

SuperWindow::~SuperWindow()
{
    delete ui;
}

void SuperWindow::getDropPiece(int i, int j)
{
    Pieces[i][j]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
    Pieces[i][j]->setIconSize(Pieces[i][j]->size());
}
