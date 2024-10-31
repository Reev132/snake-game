/*
Changelog:
1/26/24: - Added comments to make it easier to understand the code.

1/27/24: - Added more comments, and rearranged them to the top of each line for better readability.
    - Functions were also split into smaller functions.

1/31/24: - Added the function to pause and resume the game when 'P' is inputted. Input is case-insensitive.
    - Game instructions are added. Made the logic easier to read. User can play again if player loses.

2/3/24: - Added a line that reports to the user if they have ran into their own tail.
    - Added a feature where the game fails if the snake collides with the borders, and reports this to the user.
    - Added a feature to where the user chooses how many fruits they want to the board.
    - Added a feature where the program asks for user's name to add a personal touch.

2/4/24: - Renamed functions for better readability.
    - Rewrote some comments, and wrote more comments for more clarity.

2/5/24: - Added an option for hard mode and easy mode, the hard mode does not allow border collision, while the easy one does.
    - Added an option to where if the player loses, they will choose the fruits they want and the difficulty after each game.

Notes:
    - A lot of the functions tend to be grouped into one function for better modularity.
*/

// SnakeGame - Orig from the Web
// Original code by N. Vitanovic
// see his YouTube video here: https://bit.ly/29WZ5Ml

#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

// String to store the player's name.
string userName;

// Boolean for enabling the game's hard mode.
bool hardMode;

// Boolean to show if the game is over or not.
bool gameOver;

// Width and height of the board.
const int width = 20, height = 20;

// Coordinates of the snake's head.
int x, y;

// Coordinates of the fruits.
int fruitsX[100], fruitsY[100];

 // Number of fruits on the board.
int numFruits;

// The player's score.
int score;

// Stores coordinates of the snake's tail.
int tailX[100], tailY[100];

// Snake's length in number of segments.
int nTail;

// Enumerations initialized for different directions
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Stores direction
eDirecton dir;

// Gets the player's name
void GetName()
{
    cout << "Hello!" << endl;
    cout << "Please enter your name: ";
    cin >> userName;
}

// Function to display game instructions.
void DisplayInstructions()
{
    cout << "\nHello, " << userName << "!" << endl;
    cout << "Welcome to the Snake Game!" << endl;
    cout << "Use the following controls to play:" << endl;
    cout << "   - Press 'W' to move UP" << endl;
    cout << "   - Press 'S' to move DOWN" << endl;
    cout << "   - Press 'A' to move LEFT" << endl;
    cout << "   - Press 'D' to move RIGHT" << endl;
    cout << "   - Press 'P' to pause/unpause the game" << endl;
    cout << "   - Press 'X' to quit the game" << endl;
}

// Asks player if they want hard mode or easy mode.
void AskDifficulty()
{
        cout <<"\nPress 'H' if you want to play in hard mode, in which the snake cannot collide with the borders." << endl;
        cout <<"Otherwise, press any key for easy mode, in which the snake can wrap around the borders.";

        // Inputs the letter from the pressed key into the program.
        char difficulty = _getch();

        // Convert the input character to lowercase for case-insensitivity.
        difficulty = tolower(difficulty);

        // If 'H' is pressed, the hard mode will be enabled.
        if (difficulty == 'h')
        {
            hardMode = true;
        }

        // If 'H' is not pressed, the easy mode will be enabled.
        else
        {
            hardMode = false;
        }
}

// Ask the user for the number of fruits
void GetNumOfFruits()
{
    cout << "\nEnter the number of fruits you want on the board: ";
    cin >> numFruits;
}

// Initialization of game's variables
void GameSetup()
{
    // The game has started, and the player has not lost.
    gameOver = false;

    // The snake is not moving when the game is launched.
    dir = STOP;

    // These 2 lines ensure the snake is at the center position of the board.
    x = width / 2;
    y = height / 2;

    // Generates random coordinates for the desired amount of fruits the player chose to put on the board.
    for (int i = 0; i <= numFruits; i++)
    {
        fruitsX[i] = rand() % width;
        fruitsY[i] = rand() % height;
    }

    // Initial score.
    score = 0;
}

// Function to clear the console screen.
void ClearScreen()
{
    system("cls");
}

// Function to draw the top and bottom borders of the board.
void DrawBorders()
{
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}

// Function to print '#' to create the left border at the beginning of each row.
void DrawBorderRow()
{
    cout << "#";
}

// Function to draw the snake's head or tail in a cell.
void DrawSnakeSegment(int i, int j)
{
    // "O" is printed on the output depending on the coordinates to indicate the snake's head.
    if (i == y && j == x)
        cout << "O";
    else
    {
        /* printSnakeOrFruit checks if a segment of the snake, or a fruit, should be printed or not.
           This is designed to prevent both of them from printing in the same cell. */
        bool printSnakeOrFruit = false;

        // Check and print snake's tail
        for (int k = 0; k < nTail; k++)
        {
            if (tailX[k] == j && tailY[k] == i)
            {
                // Print "o" to represent a segment of the snake's tail.
                cout << "o";
                printSnakeOrFruit = true;

                 // Break out of the loop once the tail is printed.
                break;
            }
        }

        // If no tail segment was found in the current cell, check and print fruits.
        if (!printSnakeOrFruit)
        {
            for (int k = 0; k < numFruits; k++)
            {
                if (fruitsX[k] == j && fruitsY[k] == i)
                {
                    // Print "F" to represent a fruit.
                    cout << "F";
                    printSnakeOrFruit = true;

                    // Break out of the loop once a fruit is printed.
                    break;
                }
            }
        }

        // If no tail or fruit was found in the current cell, print an empty space.
        if (!printSnakeOrFruit)
            cout << " ";
    }
}

// Function to draw a row of the game board. 'i' is the row number.
void DrawBoardRow(int i)
{
    // Draw the left border.
    DrawBorderRow();

    // 'j' is the column number.
    for (int j = 0; j < width; j++)
    {

        // Draw the snake or fruit in the current cell
        DrawSnakeSegment(i, j);

        // Draw the right border
        if (j == width - 1)
            cout << "#";
    }

    cout << endl;
}

// Outputs the score.
void DrawScore()
{
    cout << userName << "'s Score: " << score << endl;
}

// Function to draw the entire game board.
void DrawGameBoard()
{
    // Clear the console screen.
    ClearScreen();

    // Draw the top border of the board.
    DrawBorders();

    // Iterate over each row of the game board.
    for (int i = 0; i < height; i++)
    {
        // Draw a row of the game board.
        DrawBoardRow(i);
    }

    // Draw the bottom border of the board.
    DrawBorders();

    // Display the player's score.
    DrawScore();
}

// Creates keyboard controls to change the snake's direction and handle pause/resume.
void GameControls()
{
    // Checks if a key has been pressed.
    if (_kbhit())
    {
        // Inputs the letter from the pressed key into the program.
        char key = _getch();

        // Convert the input character to lowercase for case-insensitivity.
        key = tolower(key);

        {
            // Handle regular movement controls.
            switch (key)
            {
                // Pressing 'a' makes the snake go left.
                case 'a':
                    dir = LEFT;
                    break;

                // Pressing 'd' makes the snake go right.
                case 'd':
                    dir = RIGHT;
                    break;

                // Pressing 'w' makes the snake go up.
                case 'w':
                    dir = UP;
                    break;

                // Pressing 's' makes the snake go down.
                case 's':
                    dir = DOWN;
                    break;

                // Pressing 'x' stops the game.
                case 'x':
                    gameOver = true;
                    break;

                // Pressing 'p' stops the game.
                case 'p':
                    if (!gameOver)
                    {
                        cout << "Game Paused. Press any key to resume." << endl;

                        // Wait for a key press.
                        getch();
                    }
            }
        }
    }
}

// Function to update the positions of the snake's tail.
void UpdateTail()
{
    // Saves the previous X-coordinate of the snake's head.
    int prevHeadX = tailX[0];

    // Saves the previous Y-coordinate of the snake's head.
    int prevHeadY = tailY[0];

    // Previous coordinates of the current snake's tail segment.
    int prevSegmentX, prevSegmentY;

    // Update the first segment of the tail to the current head position.
    tailX[0] = x;
    tailY[0] = y;

    // Update the positions of the remaining tail segments.
    for (int i = 1; i < nTail; i++)
    {
        // Previous X-coordinate of the current tail segment.
        prevSegmentX = tailX[i];

        // Previous Y-coordinate of the current tail segment.
        prevSegmentY = tailY[i];

        // Moves the current tail segment to the previous head position.
        tailX[i] = prevHeadX;
        tailY[i] = prevHeadY;

        // Updates the previous head position for the next iteration.
        prevHeadX = prevSegmentX;
        prevHeadY = prevSegmentY;
    }
}

/* Function to move the snake's head based on the current direction.
Since this is the code for the hard difficulty, it will NOT contain code
for wrapping around the borders. */
void MoveHeadHardDifficulty()
{
    switch (dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }


}

/*Function to move the snake's head based on the current direction.
Since this is the code for the easy difficulty, it will contain code
for wrapping around the borders. */
void MoveHeadEasyDifficulty()
{
    switch (dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // Wrap around the borders.
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;
}

// Main function to move the snake.
void MoveSnake()
{
    // Update the positions of the snake's tail.
    UpdateTail();

      // Move the snake's head based on the current direction, and border wrapping based on the difficulty.
    if (hardMode)
    {
        MoveHeadHardDifficulty();
    }

    else
    {
        MoveHeadEasyDifficulty();
    }
}

// Check for collision with the snake's own tail.
void CheckCollisionWithSnakeTail()
{
    // Check for collision with the snake's own tail.
    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            gameOver = true;
            cout << "You have lost because you have collided with your own tail." << endl;

            // Function ends if there is a collision with the tail.
            return;
        }
    }
}

// Check for collision with the borders.
void CheckCollisionWithBorders()
{
    // The game fails if the snake tries to go beyond the borders.
    if (y == -1 || y == height || x == -1 || x == width)
    {
        gameOver = true;
        cout << "You have lost because you have collided with the borders." << endl;

        // Function ends if there is a collision with the top or bottom border.
        return;
    }
}

// Both collision functions are combined into one function. This is only used for the hard mode.
void CheckCollision()
{
    CheckCollisionWithSnakeTail();
    CheckCollisionWithBorders();
}

// Generates fruits after one is consumed.
void GenerateFruits()
{
    for (int i = 0; i < numFruits; i++)
    {
        // Check if the snake has eaten a fruit
        if (x == fruitsX[i] && y == fruitsY[i])
        {
            // Increase the score and place a new fruit at a random position.
            score += 10;
            fruitsX[i] = rand() % width;
            fruitsY[i] = rand() % height;

            // Increase the length of the snake's tail.
            nTail++;
        }
    }
}

// Tells the player if they want to play again or not.
void PlayAgain()
{
    // Displays final score and tells player if they want to play again or not.
    cout << "\nGame Over! Your final score: " << score << endl;
    cout << "\nDo you want to play again? (Press 'y' for yes, any other key for no) " << endl;

    // Input response from the player.
    char response = _getch();

    if (response == tolower('Y'))
    {
        // Resets game variables and start a new game.

        // Reset the score
        score = 0;

         // Reset the snake's tail
        nTail = 0;

        //The game ends, and the setup process initializes again.
        gameOver = false;
        GameSetup();
    }
    else
    {
        // Exit the game.
        cout << "\nThanks for playing, " << userName << "!" << endl;
        gameOver = true;
        cout;
    }
}

int main()
{
    // Collects name and displays instructions.
    GetName();
    DisplayInstructions();

    while (true)
    {
        // Gets number of fruits and difficulty setting.
        GetNumOfFruits();
        AskDifficulty();

        // Initializes the base of the game.
        GameSetup();

        // Main game loop
        if (hardMode)
        {
            while (!gameOver)
            {
                // These functions make up the entire structure of the game.
                // All collision settings, snake tail and border, are applied here.
                DrawGameBoard();
                GameControls();
                MoveSnake();
                CheckCollision();
                GenerateFruits();
            }
        }
        else
        {
            while (!gameOver)
            {
                // These functions make up the entire structure of the game.
                /* Since border wrapping is allowed in easy mode, only the collision with
                the snake tail will be checked, the border collision will not be applied here,
                hence, stating "CheckCollisionWithSnakeTail" instead of "CheckCollision". */
                DrawGameBoard();
                GameControls();
                MoveSnake();
                CheckCollisionWithSnakeTail();
                GenerateFruits();
            }
        }

        // Asks the player if they want to play again after they lose.
        PlayAgain();

        /* Should they wish to play again, the while(True) loop be repeated from the beginning,
        asking for amount of fruits and difficulty setting. */

        // Break out of the loop if the player chooses not to play again.
        if (gameOver)
            break;
    }

    return 0;
}
