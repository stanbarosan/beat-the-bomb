#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define COLS 10
#define ROWS 10



const int screenWidth=800;
const int screenHeight = 800;

//auxiliare

const int cellsWidth=screenWidth/COLS;
const int cellsHeight=screenHeight/ROWS;

typedef struct Cell
{
    int x;
    int y;
    bool Mine;
    bool revealed;
    int adjMines;
}Cell;

Cell grid[COLS][ROWS];

void CellDraw(Cell);
bool IndexIsValid(int,int);
void CellRevealed(int, int);
int CountMines(int, int);

int main()
{

    //resetare random
    srand(time(0));

    InitWindow(screenWidth, screenHeight, "Raylib Template");

    for(int i=0;i<COLS;i++)
    {
        for(int j =0; j<ROWS; j++)
        {
            grid[i][j]= (Cell)
                    {
                            .x=i,
                            .y=j,
                            .Mine=false,
                            .revealed=false
                    };
        }

    }



    //pune mine
    int placeMines=20;
    while(placeMines>0)
    {
        int i=rand()%COLS;
        int j=rand()%ROWS;

        if(grid[i][j].Mine==false)
        {
            grid[i][j].Mine=true;
            placeMines--;
        }


    }


    for(int i=0;i<COLS;i++)
    {
        for(int j =0; j<ROWS; j++)
        {
            if(grid[i][j].Mine==false)
            {
                grid[i][j].adjMines= CountMines(i,j);

            }
        }

    }


    while (!WindowShouldClose())
    {

        //input mouse
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mPos = GetMousePosition();
            int indexX=mPos.x/cellsWidth;
            int indexY=mPos.y/cellsHeight;

            if(IndexIsValid(indexX, indexY)==true)
            {
                CellRevealed(indexX,indexY);

            }


        }

        //desenez grid-ul
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for(int i=0;i<COLS;i++)
        {
            for(int j =0; j<ROWS; j++)
            {
                CellDraw(grid[i][j]);
            }

        }




        EndDrawing();


    }

    CloseWindow();

    return 0;
}

//desenez o celula
void CellDraw(Cell cell)
{
    DrawRectangleLines(cell.x*cellsWidth, cell.y*cellsHeight, cellsWidth, cellsHeight, BLACK);
    if(cell.revealed==true)
    {
        if(cell.Mine==true)
        {
            DrawRectangle(cell.x*cellsWidth, cell.y*cellsHeight, cellsWidth, cellsHeight, RED);

        }

        else
        {
            DrawRectangle(cell.x*cellsWidth, cell.y*cellsHeight, cellsWidth, cellsHeight, LIGHTGRAY);

            if(cell.adjMines!=0)
            DrawText(TextFormat("%d",cell.adjMines),cell.x*cellsWidth + 6, cell.y*cellsHeight + 4 ,cellsHeight - 8,BLACK);
        }

    }



}


//verifica daca mouse-ul este pe o celula
bool IndexIsValid(int x, int y)
{
    return x>=0 && x< COLS && y>=0 && y<ROWS;
}

void CellRevealed(int x, int y)
{
    grid[x][y].revealed=true;

    if(grid[x][y].Mine==true)
    {
        //lose
        DrawRectangle(x*cellsWidth, y*cellsHeight, cellsWidth, cellsHeight, RED);
    }
    else
    {

        //schimb culoarea celulei
        DrawRectangle(x*cellsWidth, y*cellsHeight, cellsWidth, cellsHeight, LIGHTGRAY);

    }

}


//numara minele vecine
int CountMines(int x,int y)
{
    int count=0;

    int vest=x-1;
    int nord=y-1;
    int est=x+1;
    int sud=y+1;


    if(y==0) nord=0;
    if(x==0) vest=0;
    if(y==ROWS) sud=x;
    if(x==COLS) est=y;


    for(int i=vest;i<=est;i++)
    {
        for(int j=nord;j<=sud;j++)
        {
            if(IndexIsValid(x,y))
                if(grid[i][j].Mine==true)
                {
                    count++;
                }

        }
    }
return count;
}
