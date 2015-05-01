#include "SuperWindow.h"
#include "ui_SuperWindow.h"

SuperWindow::SuperWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SuperWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Qt-Reversi");





    /* Pieces[PieceSize][PieceSize] */
    Pieces.resize(PieceSize);
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
            Pieces[i][j]->setIconSize(Pieces[i][j]->size());
            connect(Pieces[i][j], SIGNAL(DropPiece(int,int)), this, SLOT(getDropPiece(int,int)));

            ChessBoardLayout->addWidget(Pieces[i][j], i*40, j*40);
        }
    }

    Pieces[3][3]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
    Pieces[4][4]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
    Pieces[3][4]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png")));
    Pieces[4][3]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png")));

    Pieces[3][3]->type = White;
    Pieces[4][4]->type = White;
    Pieces[3][4]->type = Black;
    Pieces[4][3]->type = Black;

    Player = White;
    Enemy  = Black;

    ui->ChessBoardLabel->setLayout(ChessBoardLayout);
}

SuperWindow::~SuperWindow()
{
    delete ui;
}

int SuperWindow::NumberOfPieceEat(int row, int column, int deltaX, int deltaY)
{
    int deltaRow = row + deltaY,
        deltaColumn = column + deltaX,
        eat = 0;

    while (deltaColumn != -1 && deltaRow != -1 && deltaColumn != PieceSize && deltaRow != PieceSize) {
        if(Pieces[deltaRow][deltaColumn]->type != Enemy){
            break;
        }

        deltaRow += deltaY;
        deltaColumn += deltaX;
        eat++;
    }

    if(deltaColumn == -1 || deltaRow == -1 || deltaColumn == PieceSize || deltaRow == PieceSize){
        return 0;
    }

    if(Pieces[deltaRow][deltaColumn]->type != Player || eat == 0){
        return 0;
    }

    if(deltaRow != -1 && deltaColumn != -1 && deltaRow != PieceSize && deltaColumn != PieceSize){
        /* Start Eating */
        int EatRow = row + deltaY,
            EatColumn = column + deltaX;

        while (EatRow != -1 && EatColumn != -1 && EatRow != PieceSize && EatColumn != PieceSize && Pieces[EatRow][EatColumn]->type != Player) {

            if(Player == Black){
                Pieces[EatRow][EatColumn]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png")));
                Pieces[EatRow][EatColumn]->type = Black;
            }else{
                Pieces[EatRow][EatColumn]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
                Pieces[EatRow][EatColumn]->type = White;
            }

            EatRow += deltaY;
            EatColumn += deltaX;
        }
    }

    return eat;
}

void SuperWindow::getDropPiece(int row, int column)
{
    if(Pieces[row][column]->type != Empty){
        return;
    }

    int MaxEat = 0;

    for(int deltaY = -1; deltaY <= 1; deltaY++){
        for(int deltaX = -1; deltaX <= 1; deltaX++){

            if((deltaX == 0 && deltaY == 0) || row+deltaY == -1 || row+deltaY == PieceSize || column+deltaX == -1 || column+deltaX == PieceSize){
                continue;
            }

            int Eat = NumberOfPieceEat(row, column, deltaX, deltaY);

            if(MaxEat < Eat){
                MaxEat = Eat;
            }
        }
    }

    if (MaxEat == 0){
        return;
    }

    if(Player == Black){
        Pieces[row][column]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png")));
    }else{
        Pieces[row][column]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
    }

    Pieces[row][column]->type = Player;
    qSwap(Player, Enemy);
}
