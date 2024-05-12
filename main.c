#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "raylib.h"
#include <string.h>
#include <unistd.h>

int COLS,ROWS;

const int screenWidth = 800;
const int screenHeight = 800;

// Cells


typedef struct Cell
{
    int x;
    int y;
    bool Mine;
    bool revealed;
    bool checked;
    int adjMines;
    bool flagged;
} Cell;

Cell grid[34][34];

bool IndexIsValid(int, int);
void CellRevealed(int, int);
int CountMines(int, int);
void check0(int x, int y);
void border();

float image_scale;


//grafurile cu intrebari
#define MAX_QUESTION_LENGTH 500

struct Node
{
    char intrebare[MAX_QUESTION_LENGTH];
    int valid;
    struct Node *left1;
    struct Node *left2;
    struct Node *right1;
    struct Node *right2;
};

struct Node *root1;
struct Node *root2;
struct Node *root3;
struct Node *root4;

struct Node *createNode(char varianta[MAX_QUESTION_LENGTH], int correct)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    strcpy(newNode->intrebare, varianta);
    newNode->left1 = NULL;
    newNode->right2 = NULL;
    newNode->left2 = NULL;
    newNode->right1 = NULL;
    newNode->valid = correct;
    return newNode;
}




int main()
{


    //minesweeper
    InitWindow(screenWidth, screenHeight, "Beat The Bomb");
    SetTargetFPS(30);

    // START


    // Loading Textures

    Texture2D bombTexture = LoadTexture("..\\textures\\bomb.png");
    Texture2D boxTexture = LoadTexture("..\\textures\\box.png");
    Texture2D box0Texture = LoadTexture("..\\textures\\box0.png");
    Texture2D box1Texture = LoadTexture("..\\textures\\box1.png");
    Texture2D box2Texture = LoadTexture("..\\textures\\box2.png");
    Texture2D box3Texture = LoadTexture("..\\textures\\box3.png");
    Texture2D box4Texture = LoadTexture("..\\textures\\box4.png");
    Texture2D box5Texture = LoadTexture("..\\textures\\box5.png");
    Texture2D box6Texture = LoadTexture("..\\textures\\box6.png");
    Texture2D box7Texture = LoadTexture("..\\textures\\box7.png");
    Texture2D box8Texture = LoadTexture("..\\textures\\box8.png");
    Texture2D flagTexture = LoadTexture("..\\textures\\flag.png");
    Texture2D startMenu = LoadTexture("..\\textures\\startmenu.png");
    Texture2D difficultyMenu = LoadTexture("..\\textures\\difficulty.png");
    Texture2D youLoseMenu = LoadTexture("..\\textures\\youlosemenu.png");
    Texture2D youWinMenu = LoadTexture("..\\textures\\youwinmenu.png");

    //  Reset Random Generator
    srand((unsigned)time(NULL));

    StartNewGame:
    // Start Menu
    image_scale=0.08;
    COLS=10;
    ROWS=10;
    int numberOfMines = 10;



    bool start = false;
    bool isOptionsMenuOpen = false;
    while (start != true) {

        BeginDrawing();
        if (isOptionsMenuOpen == false) {
            DrawTextureEx(startMenu, (Vector2) {0, 0}, 0, 0.192f, WHITE);
        }
        // Play Game (Start)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isOptionsMenuOpen == false)
        {
            Vector2 mousePosition = GetMousePosition();
            if (mousePosition.x >= 217 && mousePosition.x <= 588 && mousePosition.y >= 220 && mousePosition.y <= 318) {

                start = true;
            }
        }

        // Quit Game
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isOptionsMenuOpen == false)
        {
            Vector2 mousePosition = GetMousePosition();
            if (mousePosition.x >= 220 && mousePosition.x <= 589 && mousePosition.y >= 484 && mousePosition.y <= 583) {
                goto QuitGame;
            }
        }

        // Options Menu (Difficulties)
        clock_t last_action_time = 0;
        const clock_t cooldown = CLOCKS_PER_SEC; // Un cooldown de o secundă


        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isOptionsMenuOpen == false)
        {
            Vector2 mousePosition = GetMousePosition();
            if(mousePosition.x >= 220 && mousePosition.x <= 589 && mousePosition.y >= 351 && mousePosition.y <= 449)
            {
                isOptionsMenuOpen = true;
                SetMousePosition(mousePosition.x,408);
            }

        }


        if (isOptionsMenuOpen == true )
        {
            Vector2 mousePosition = GetMousePosition();
            printf("1");
            DrawTextureEx(difficultyMenu, (Vector2) {0, 0}, 0, 0.192f, WHITE);
            printf("2");
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePosition.x >= 236 && mousePosition.x <= 568 && mousePosition.y >= 209 && mousePosition.y <= 298) {
                image_scale = 0.16;
                COLS = 5;
                ROWS = 5;

                numberOfMines = 4;
                isOptionsMenuOpen = false;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mousePosition.x >= 236 && mousePosition.x <= 568 && mousePosition.y >= 319 && mousePosition.y <= 407) {
                image_scale = 0.08;
                COLS = 10;
                ROWS = 10;
                numberOfMines = 12;
                isOptionsMenuOpen = false;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mousePosition.x >= 236 && mousePosition.x <= 568 && mousePosition.y >= 432 && mousePosition.y <= 520) {
                image_scale = 0.06;
                COLS = 15;
                ROWS = 15;
                numberOfMines =28;
                isOptionsMenuOpen = false;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePosition.x >= 236 && mousePosition.x <= 568 && mousePosition.y >= 543 && mousePosition.y <= 633) {
                image_scale = 0.04;
                COLS = 20;
                ROWS = 20;
                numberOfMines = 58;
                isOptionsMenuOpen = false;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePosition.x >= 236 && mousePosition.x <= 568 && mousePosition.y >= 651 && mousePosition.y <= 742) {
                image_scale=0.03;
                COLS=25;
                ROWS=25;
                numberOfMines =99;
                isOptionsMenuOpen = false;
            }
        }

        EndDrawing();
    }

    // Creating the Grids
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
                    .checked = false,
                    .flagged = false};
        }
    }
    // Placing The Mines
    int minesToPlace = numberOfMines;
    while (minesToPlace > 0)
    {
        int i = 0;
        int j = 0;
        while (i < 1 || j < 1 || i > COLS || j > ROWS)
        {
            i = (rand() % COLS) + 1;
            j = (rand() % ROWS) + 1;
        }
        if (grid[i][j].Mine == false)
        {
            grid[i][j].Mine = true;
            grid[i][j].adjMines = 1;
            minesToPlace--;
        }
    }

    // Grid Border
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

    // Game Interface
    int cellsWidth = screenWidth / COLS;
    int cellsHeight = screenHeight / (ROWS);


    while (!WindowShouldClose())
    {
        // Mouse Input
        // Click on cell
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();
            int cellIndexX = mousePosition.x / cellsWidth + 1;
            int cellIndexY = mousePosition.y / cellsHeight + 1;
            if (IndexIsValid(cellIndexX, cellIndexY) == true)
            {
                check0(cellIndexX, cellIndexY);
                CellRevealed(cellIndexX, cellIndexY);
            }
        }
        // Flags
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mousePosition = GetMousePosition();
            int cellIndexX = mousePosition.x / cellsWidth + 1;
            int cellIndexY = mousePosition.y / cellsHeight + 1;
            const int fontSize2 = 20;
            const int padding2 = 20;
            bool put=false;


            // Crearea întrebărilor și a arborelor

            // Crearea primului arbore
            root1 = createNode("The EU's target commitments until 2030 are the following:", 1);
            root1->left1 = createNode("Protecting by legislation of a minimum of 30% of the EU's land and sea area", 1);
            root1->left2 = createNode("Protection of at least 10% of EU's protected areas, including primary and old-growth forests", 0);
            root1->right1 = createNode("Adequate management of all protected areas, by defining clear conservation and monitoring \nobjectives and measures ", 1);

            // Crearea celui de-al doilea arbore
            root2 = createNode(" The EU biodiversity strategy for the year 2030 supposes the following:", 1);
            root2->left1 = createNode("Protecting and restoring nature in the EU, by consolidating a coherent and effective \nnetwork for protected areas and restoring degraded habitats ", 1);
            root2->left2 = createNode("Enabling a new governance framework to ensure co-resposibility and co-ownership by all \nrelevant actors including setting up new financial opportunities", 1);
            root2->right1 = createNode("Adopting a global biodiversity agenda, to strengthen the contribution of the EU toward \nhealthy global biodiversity loss", 1);
            root2->right2 = createNode("Adopting a global biodiversity agenda, to strenght the contribution of the EU toward \nmaximizing EU use of resources and consumption on other biodiversity-rich areas of the planet", 0);

            // Crearea celui de-al treilea arbore
            root3 = createNode(" Opportunities to conserve and restore biodiversity in agrifood systems through bioeconomy \npractices are the following:", 1);
            root3->left1 = createNode(" closure of the biological resources loop and maximizing the use of residual streams for \nagriculture, food processing, and biobased industries, by using, recycling, or composting waste\n materials and converting them into useful products", 1);
            root3->left2 = createNode(" adopting green products or practices based on bioferitilizers, biopesticides, \nbio-based plastics, bioremediation, or microbiome innovations", 1);
            root3->right1 = createNode(" by using pesticides(of chemical synthesis) in agriculture", 0);
            root3->right2 = createNode(" by introducing the invasive species in these habitats", 0);
            // Crearea celui de-al patrulea arbore
            root4 = createNode(" Bioeconomy can support biodiversity in the following ways:", 1);
            root4->left1 = createNode(" By afforestation and reforestation because this action can reduce habitat loss and \nrestore ecosystems", 1);
            root4->left2 = createNode(" Adopting eco-friendly approaches to restore degraded eco-systems and enhance biodiversity \nby applying biological solutions", 1);
            root4->right1 = createNode(" Carbon sequestration in soil, water, and forests and its storage in harvested products", 1);
            root4->right2 = createNode(" Maintaining the old industrial processes to obtain fossil-based products and raw materials", 0);

            struct Node *currentQuestion = NULL;
            struct Node *currentVarianta = NULL;

            bool truePressed=false;
            bool falsePressed=false;
            while (!WindowShouldClose()) {
                if (currentQuestion == NULL) {
                    int general = rand() % 4;

                    switch (general) {
                        case 0:
                            currentQuestion = root1;
                            break;
                        case 1:
                            currentQuestion = root2;
                            break;
                        case 2:
                            currentQuestion = root3;
                            break;
                        case 3:
                            currentQuestion = root4;
                            break;
                        default:
                            break;
                    }

                    // Alege o singură variantă de răspuns pentru a fi afișată
                    int quiz = rand() % 4;

                    switch (quiz) {
                        case 0:
                            currentVarianta = currentQuestion->left1;
                            break;
                        case 1:
                            currentVarianta = currentQuestion->left2;
                            break;
                        case 2:
                            currentVarianta = currentQuestion->right1;
                            break;
                        case 3:
                           if(currentQuestion == root1) currentVarianta = NULL;
                           else currentVarianta=currentQuestion->right2;
                            break;
                        default:
                            break;
                    }
                }
                if (currentVarianta != NULL) { // Desenează întrebarea și opțiunile
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText(currentQuestion->intrebare, padding2, padding2, fontSize2, RAYWHITE);
                    DrawText(currentVarianta->intrebare, padding2, padding2 + fontSize2 * 3, fontSize2, RAYWHITE);

                    // Desenează instrucțiunile pentru utilizator
                    if (truePressed == false && falsePressed == false) {
                        DrawText("Press [T] for true.", padding2, padding2 + fontSize2 * 7, fontSize2, RAYWHITE);
                        DrawText("Press [F] for false.", padding2, padding2 + fontSize2 * 8, fontSize2, RAYWHITE);
                    }

                    if (IsKeyDown(KEY_T)) {
                        truePressed = true;
                    } else if (IsKeyDown(KEY_F)) {
                        falsePressed = true;
                    }
                    if (truePressed == true) {
                        if (currentVarianta->valid == 1) {
                            DrawText("Correct!", padding2, padding2 + fontSize2 * 9, fontSize2, RAYWHITE);
                            put = true;
                        } else {

                            DrawText("Incorrect!", padding2, padding2 + fontSize2 * 9, fontSize2, RAYWHITE);

                        }

                        DrawText("Press (x) to continue game", padding2, padding2 + fontSize2 * 10, fontSize2,
                                 RAYWHITE);
                    } else if (falsePressed == true) {
                        if (currentVarianta->valid == 0) {

                            DrawText("Correct!", padding2, padding2 + fontSize2 * 9, fontSize2, RAYWHITE);
                            put = true;

                        } else {

                            DrawText("Incorrect!", padding2, padding2 + fontSize2 * 9, fontSize2, RAYWHITE);

                        }

                        DrawText("Press (x) to continue game", padding2, padding2 + fontSize2 * 10, fontSize2,
                                 RAYWHITE);
                    }
                    if (IsKeyDown(KEY_X)) {

                        goto Continu;
                    }


                    EndDrawing();
                }
                else {
                    while(!WindowShouldClose())
                    {
                        BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Wild Card!", padding2, padding2 + 30, fontSize2, RAYWHITE);
                        DrawText("Press (x) to continue game", padding2, padding2 + fontSize2 * 3, fontSize2,RAYWHITE);
                        EndDrawing();
                        put=true;
                        if (IsKeyDown(KEY_X)) {

                            goto Continu;
                        }
                    }


                }
            }
            Continu:


            if (IndexIsValid(cellIndexX, cellIndexY) == true && grid[cellIndexX][cellIndexY].flagged == false && put==true)
            {
                grid[cellIndexX][cellIndexY].flagged = true;
            }
            else if (IndexIsValid(cellIndexX, cellIndexY) == true && grid[cellIndexX][cellIndexY].flagged == true && put==true)
            {
                grid[cellIndexX][cellIndexY].flagged = false;
            }
        }
        // Grids Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int i = 1; i <= COLS; i++)
        {
            for (int j = 1; j <= ROWS; j++)
            {
                DrawRectangleLines((grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight, cellsWidth, cellsHeight, BLACK);
                if (grid[i][j].revealed == true)
                {
                    if (grid[i][j].Mine == true)
                    {
                        DrawRectangle((grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight, cellsWidth, cellsHeight, RED);
                        DrawTextureEx(bombTexture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);
                    }
                    if (grid[i][j].Mine == false)
                    {
                        if (grid[i][j].adjMines == 1)
                            DrawTextureEx(box1Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 2)
                            DrawTextureEx(box2Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 3)
                            DrawTextureEx(box3Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 4)
                            DrawTextureEx(box4Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 5)
                            DrawTextureEx(box5Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 6)
                            DrawTextureEx(box6Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 7)
                            DrawTextureEx(box7Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 8)
                            DrawTextureEx(box8Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);

                        if (grid[i][j].adjMines == 0)
                            DrawTextureEx(box0Texture, (Vector2){(grid[i][j].x - 1) * cellsWidth, (grid[i][j].y - 1) * cellsHeight}, 0, image_scale, WHITE);
                    }
                }
                if (grid[i][j].revealed == false)
                {
                    DrawTextureEx(boxTexture, (Vector2){((float)(grid[i][j].x - 1)) * cellsWidth, ((float)(grid[i][j].y - 1)) * cellsHeight}, 0, image_scale, WHITE);
                    if (grid[i][j].flagged == true)
                    {
                        DrawTextureEx(flagTexture, (Vector2){((float)(grid[i][j].x - 1)) * cellsWidth, ((float)(grid[i][j].y - 1)) * cellsHeight}, 0, image_scale, WHITE);
                    }
                }
            }
        }
        // Win or Lose check
        int flaggedBombsCount = 0;
        bool allCellsRevealed = true;
        bool allSafeCellsRevealed = true;

        for (int i = 1; i <= COLS; i++)
        {
            for (int j = 1; j <= ROWS; j++)
            {
                if (grid[i][j].revealed == false)
                {
                    allCellsRevealed = false;
                }
                if (grid[i][j].revealed == false && grid[i][j].Mine == false)
                {
                    allSafeCellsRevealed = false;
                }
                if (grid[i][j].Mine == true && grid[i][j].flagged == true)
                {
                    flaggedBombsCount++;
                }
            }
        }
        // win
        if (flaggedBombsCount == numberOfMines && allSafeCellsRevealed == true)
        {
            DrawTextureEx(youWinMenu, (Vector2){0, 0}, 0, 0.8f, WHITE);
            Vector2 mousePosition=GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePosition.x >= 487 && mousePosition.x <= 672 && mousePosition.y >= 551 && mousePosition.y <= 646)
            {
                goto QuitGame;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mousePosition.x >= 131 && mousePosition.x <= 320 && mousePosition.y >= 551 && mousePosition.y <= 646)
            {
                goto StartNewGame;
            }
        }

        // lose
        if (allCellsRevealed == true)
        {
            DrawTextureEx(youLoseMenu, (Vector2){0, 0}, 0, 0.8f, WHITE);
            Vector2 mousePosition=GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePosition.x >= 487 && mousePosition.x <= 672 && mousePosition.y >= 551 && mousePosition.y <= 646)
            {
                goto QuitGame;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mousePosition.x >= 131 && mousePosition.x <= 320 && mousePosition.y >= 551 && mousePosition.y <= 646)
            {
                goto StartNewGame;
            }
        }

        EndDrawing();

    }

    QuitGame:
    WindowShouldClose();
    UnloadTexture(boxTexture);
    UnloadTexture(box0Texture);
    UnloadTexture(box1Texture);
    UnloadTexture(box2Texture);
    UnloadTexture(box3Texture);
    UnloadTexture(box4Texture);
    UnloadTexture(box5Texture);
    UnloadTexture(box6Texture);
    UnloadTexture(box7Texture);
    UnloadTexture(box8Texture);
    UnloadTexture(flagTexture);
    UnloadTexture(bombTexture);
    UnloadTexture(startMenu);
    UnloadTexture(difficultyMenu);
    UnloadTexture(youLoseMenu);
    UnloadTexture(youWinMenu);
    return 0;
}

// Checks if the mouse is on a cell
bool IndexIsValid(int x, int y)
{
    return x >= 1 && x <= COLS && y >= 1 && y <= ROWS;
}

void CellRevealed(int x, int y)
{
    grid[x][y].revealed = true;

    // lose
    if (grid[x][y].Mine == true)
    {
        for (int i = 1; i <= COLS; i++)
        {
            for (int j = 1; j <= ROWS; j++)
            {
                grid[i][j].revealed = true;
            }
        }
    }
}

// Counts Adjacent Mines
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

//matrix border
void border()
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

// checks for 0 surrounding mines
void check0(int x, int y)
{
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
