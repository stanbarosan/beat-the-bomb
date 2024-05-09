#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define COLS 10
#define ROWS 10



const int screenWidth=800;
const int screenHeight = 480;

//auxiliare

const int cellsWidth=screenWidth/COLS;
const int cellsHeight=screenHeight/ROWS;

typedef struct Cell
{
    int x;
    int y;
    bool Mine;
    bool revealed;
}Cell;

Cell grid[COLS][ROWS];

void CellDraw(Cell);
bool IndexIsValid(int,int);
void CellRevealed(int, int);

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

    int placeMines=10;
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
    if(cell.revealed==true)
    {
        if(cell.Mine==true)
            DrawRectangleLines(cell.x*cellsWidth, cell.y*cellsHeight, cellsWidth, cellsHeight, RED);
        else
            DrawRectangleLines(cell.x*cellsWidth, cell.y*cellsHeight, cellsWidth, cellsHeight, LIGHTGRAY);
    }
    else
    DrawRectangleLines(cell.x*cellsWidth, cell.y*cellsHeight, cellsWidth, cellsHeight, BLACK);

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
        DrawRectangleLines(x*cellsWidth, y*cellsHeight, cellsWidth, cellsHeight, RED);
    }
    else
    {

        //schimb culoarea celulei
        DrawRectangleLines(x*cellsWidth, y*cellsHeight, cellsWidth, cellsHeight, LIGHTGRAY);

    }

}
