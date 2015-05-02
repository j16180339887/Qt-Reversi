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

    connect(ui->Restart, SIGNAL(clicked(bool)), this, SLOT(Restart()));
    connect(ui->Undo, SIGNAL(clicked(bool)), this, SLOT(Undo()));
    connect(ui->Redo, SIGNAL(clicked(bool)), this, SLOT(Redo()));

    ui->BlackPieceImage->setPixmap(QPixmap(":/Image/BlackChess.png").scaled(40, 40, Qt::KeepAspectRatio));
    ui->WhitePieceImage->setPixmap(QPixmap(":/Image/WhiteChess.png").scaled(40, 40, Qt::KeepAspectRatio));
    ui->ChessBoardLabel->setLayout(ChessBoardLayout);

    /* The Game Begin */
    Restart();
}

SuperWindow::~SuperWindow()
{
    delete ui;
}

void SuperWindow::Restart()
{
    BackUp.clear();

    BackUp.resize(PieceSize * PieceSize);
    for(int i = 0; i < PieceSize * PieceSize; i++){
        BackUp[i].resize(PieceSize);
        for(int j = 0; j < PieceSize; j++){
            BackUp[i][j].resize(PieceSize);
        }
    }

    for(int i = 0; i < PieceSize; i++)
    {
        for(int j = 0; j < PieceSize; j++)
        {
            Pieces[i][j]->setIcon(QIcon());
            Pieces[i][j]->type = Empty;
            BackUp[0][i][j] = Pieces[i][j]->type;
        }
    }

    Pieces[3][3]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png"))); Pieces[3][3]->type = White;
    Pieces[4][4]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png"))); Pieces[4][4]->type = White;
    Pieces[3][4]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png"))); Pieces[3][4]->type = Black;
    Pieces[4][3]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png"))); Pieces[4][3]->type = Black;
    BackUp[0][3][3] = White;
    BackUp[0][4][4] = White;
    BackUp[0][3][4] = Black;
    BackUp[0][4][3] = Black;

    NumberOfBlack = 2;
    NumberOfWhite = 2;
    Player = White;
    Enemy  = Black;

    totalMove = MaxBackUpMove = 0;
    ui->BlackNumber->display(NumberOfBlack);
    ui->WhiteNumber->display(NumberOfWhite);
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

            if(Player == Black){
                NumberOfBlack += Eat;
                NumberOfWhite -= Eat;
            }else{
                NumberOfWhite += Eat;
                NumberOfBlack -= Eat;
            }

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
        NumberOfBlack++;
    }else{
        Pieces[row][column]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
        NumberOfWhite++;
    }

    Pieces[row][column]->type = Player;
    qSwap(Player, Enemy);

    ui->BlackNumber->display(NumberOfBlack);
    ui->WhiteNumber->display(NumberOfWhite);

    totalMove++;
    for(int i = 0; i < PieceSize; i++){
        for(int j = 0; j < PieceSize; j++){
            BackUp[totalMove][i][j] = Pieces[i][j]->type;
        }
    }
    MaxBackUpMove = totalMove > MaxBackUpMove ? totalMove : MaxBackUpMove;
}

void SuperWindow::Undo()
{
    if(totalMove == 0){
        return;
    }

    totalMove--;
    for(int i = 0; i < PieceSize; i++)
    {
        for(int j = 0; j < PieceSize; j++)
        {
            Pieces[i][j]->type = BackUp[totalMove][i][j];
        }
    }

    qSwap(Player, Enemy);
    Refresh();
}

void SuperWindow::Redo()
{
    if(MaxBackUpMove <= totalMove){
        return;
    }

    totalMove++;
    for(int i = 0; i < PieceSize; i++)
    {
        for(int j = 0; j < PieceSize; j++)
        {
            Pieces[i][j]->type = BackUp[totalMove][i][j];
        }
    }

    qSwap(Player, Enemy);
    Refresh();
}

void SuperWindow::Refresh()
{
    int B = 0, W = 0;
    for(int i = 0; i < PieceSize; i++)
    {
        for(int j = 0; j < PieceSize; j++)
        {
            Pieces[i][j]->setIcon(QIcon());
            if(Pieces[i][j]->type == White){
                W++;
                Pieces[i][j]->setIcon(QIcon(QPixmap(":/Image/WhiteChess.png")));
            }else if(Pieces[i][j]->type == Black){
                B++;
                Pieces[i][j]->setIcon(QIcon(QPixmap(":/Image/BlackChess.png")));
            }else{
                Pieces[i][j]->setIcon(QIcon());
            }
        }
    }
    NumberOfBlack = B;
    NumberOfWhite = W;
    ui->BlackNumber->display(NumberOfBlack);
    ui->WhiteNumber->display(NumberOfWhite);
}
