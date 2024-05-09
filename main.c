#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define COLS 10
#define ROWS 10

const int screenWidth = 800;
const int screenHeight = 800;

// auxiliare

const int cellsWidth = screenWidth / (COLS);
const int cellsHeight = screenHeight / (ROWS);

typedef struct Cell
{
    int x;
    int y;
    bool Mine;
    bool revealed;
    bool checked;
    int adjMines;
} Cell;

Cell grid[COLS][ROWS];

void CellDraw(Cell);
bool IndexIsValid(int, int);
void CellRevealed(int, int);
int CountMines(int, int);
void check0(int x, int y);
void border();

int main()
{

    // resetare random
    srand(time(0));

    InitWindow(screenWidth, screenHeight, "Raylib Template");

    for (int i = 0; i <= COLS + 1; i++)
    {
        for (int j = 0; j <= ROWS + 1; j++)
        {
            grid[i][j].checked = true;
        }
    }

    for (int i = 1; i <= COLS; i++)
    {
        for (int j = 1; j <= ROWS; j++)
        {
            grid[i][j] = (Cell){
                    .x = i,
                    .y = j,
                    .Mine = false,
                    .revealed = false,
                    .checked = false};
        }
    }

    // pune mine
    int placeMines = 30;
    while (placeMines > 0)
    {
        int i = rand() % COLS;
        int j = rand() % ROWS;

        if (grid[i][j].Mine == false)
        {
            grid[i][j].Mine = true;
            grid[i][j].adjMines = 1;
            placeMines--;
        }
    }

    // bordarez grid-ul ca sa nu mai fie probleme la numararea bombelor
    border();

    for (int i = 1; i <= COLS; i++)
    {
        for (int j = 1; j <= ROWS; j++)
        {
            if (grid[i][j].Mine == false)
            {
                grid[i][j].adjMines = CountMines(i, j);
            }
        }
    }

    while (!WindowShouldClose())
    {

        // input mouse
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mPos = GetMousePosition();
            int indexX = mPos.x / cellsWidth + 1;
            int indexY = (mPos.y) / cellsHeight + 1;

            if (IndexIsValid(indexX, indexY) == true)
            {
                check0(indexX, indexY);
                CellRevealed(indexX, indexY);
            }
        }

        // desenez grid-ul
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for (int i = 1; i <= COLS; i++)
        {
            for (int j = 1; j <= ROWS; j++)
            {
                CellDraw(grid[i][j]);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// desenez o celula
void CellDraw(Cell cell)
{
    DrawRectangleLines((cell.x - 1) * cellsWidth, (cell.y - 1) * cellsHeight, cellsWidth, cellsHeight, BLACK);
    if (cell.revealed == true)
    {
        if (cell.Mine == true)
        {
            DrawRectangle((cell.x - 1) * cellsWidth, (cell.y - 1) * cellsHeight, cellsWidth, cellsHeight, RED);
        }

        else
        {
            DrawRectangle((cell.x - 1) * cellsWidth, (cell.y - 1) * cellsHeight, cellsWidth, cellsHeight, LIGHTGRAY);

            if (cell.adjMines != 0)
                DrawText(TextFormat("%d", cell.adjMines), (cell.x - 1) * cellsWidth + 6, (cell.y - 1) * cellsHeight + 4, cellsHeight - 8, BLACK);
        }
    }
}

// verifica daca mouse-ul este pe o celula
bool IndexIsValid(int x, int y)
{
    return x >= 1 && x <= COLS && y >= 1 && y <= ROWS;
}

void CellRevealed(int x, int y)
{
    grid[x][y].revealed = true;

    if (grid[x][y].Mine == true)
    {
        // lose
        DrawRectangle(x * cellsWidth, y * cellsHeight, cellsWidth, cellsHeight, RED);
    }
    else
    {

        // schimb culoarea celulei

        DrawRectangle(x * cellsWidth, y * cellsHeight, cellsWidth, cellsHeight, LIGHTGRAY);
    }
}

// numara minele vecine
int CountMines(int x, int y)
{
    int count = 0;

    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (IndexIsValid(x, y))
                if (grid[i][j].Mine == true)
                {
                    count++;
                }
        }
    }
    return count;
}

void border() // bordare matrice
{
    for (int i = 0; i <= COLS + 1; i++)
    {
        for (int j = 0; j <= ROWS + 1; j++)
        {
            if (i == 0 || j == 0 || i == COLS + 1 || j == ROWS + 1)
            {
                grid[i][j].Mine = false;
                grid[i][j].adjMines = 1;
            }
        }
    }
}
void check0(int x, int y)
{ // arata toate casutele cu 0
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (grid[i][j].checked == false && grid[i][j].adjMines == 0 && IndexIsValid(i, j))
            {
                grid[i][j].checked = true;
                grid[i][j].revealed = true;
                for (int k = i - 1; k <= i + 1; k++)
                {
                    for (int l = j - 1; l <= j + 1; l++)
                    {
                        if (IndexIsValid(k, l))
                            grid[k][l].revealed = true;
                    }
                }

                check0(i, j);
            }
        }
    }
}
