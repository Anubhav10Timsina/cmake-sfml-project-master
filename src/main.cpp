#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


const int ROWS = 6;
const int COLS = 7;
const float HOLE_RADIUS = 20.0f; //  hole radius
const float SPACING = 60.0f; // Smaller spacing
const float GRID_WIDTH = COLS * SPACING;
const float GRID_HEIGHT = ROWS * SPACING;
const float WINDOW_WIDTH = 800.0f; // Window width
const float WINDOW_HEIGHT = 600.0f; // Window height 

enum class Player { None, Red, Blue };

class ConnectFour {
private:
    void draw();
    void handleInput();
    bool dropPiece(int col);
    bool checkWin();
    void reset();
    void showEndScreen(const std::string& message);
    
    sf::RenderWindow window;
    sf::CircleShape redPiece;
    sf::CircleShape bluePiece;
    sf::Font font;
    sf::Text player1Label;
    sf::Text player2Label;
    sf::Text endMessage;
    sf::RectangleShape playAgainButton;
    sf::RectangleShape quitButton;
    sf::Text playAgainText;
    sf::Text quitText;
    std::vector<std::vector<Player> > board;
    Player currentPlayer;
    bool gameOver;

public:
    ConnectFour();
    void run();
};

ConnectFour::ConnectFour()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Four in a Row"),
      redPiece(HOLE_RADIUS), bluePiece(HOLE_RADIUS),
      board(ROWS, std::vector<Player>(COLS, Player::None)),
      currentPlayer(Player::Red),
      gameOver(false)
{
    redPiece.setFillColor(sf::Color::Red);
    bluePiece.setFillColor(sf::Color::Blue);

    // Load font
    // if (!font.loadFromFile("arial.ttf")) {
    //     std::cerr << "Failed to load font!" << std::endl;
    // }

    // Initialize player labels
    player1Label.setFont(font);
    player1Label.setString("Player 1");
    player1Label.setCharacterSize(24);
    player1Label.setFillColor(sf::Color::White);
    player1Label.setPosition(10, 10);

    player2Label.setFont(font);
    player2Label.setString("Player 2");
    player2Label.setCharacterSize(24);
    player2Label.setFillColor(sf::Color::Black);
    player2Label.setPosition(WINDOW_WIDTH - 120, 10);

    // Initialize end message
    endMessage.setFont(font);
    endMessage.setCharacterSize(36);
    endMessage.setFillColor(sf::Color::Black);
    endMessage.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100);

    // Initialize buttons
    playAgainButton.setSize(sf::Vector2f(200, 50));
    playAgainButton.setFillColor(sf::Color::Green);
    playAgainButton.setPosition(WINDOW_WIDTH / 2 - 220, WINDOW_HEIGHT / 2 + 50);

    quitButton.setSize(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT / 2 + 50);

    // Initialize button text
    playAgainText.setFont(font);
    playAgainText.setString("Play Again");
    playAgainText.setCharacterSize(24);
    playAgainText.setFillColor(sf::Color::White);
    playAgainText.setPosition(playAgainButton.getPosition().x + 30, playAgainButton.getPosition().y + 10);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(24);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(quitButton.getPosition().x + 70, quitButton.getPosition().y + 10);
}

void ConnectFour::run() {
    while (window.isOpen()) {
        handleInput();
        draw();
    }
}

void ConnectFour::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (gameOver) {
                // Check if "Play Again" or "Quit" is clicked
                if (playAgainButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    reset();
                    gameOver = false;
                } else if (quitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    window.close();
                }
            } else {
                int col = (event.mouseButton.x - (WINDOW_WIDTH - GRID_WIDTH) / 2) / SPACING; // Adjusted for centered grid
                if (col >= 0 && col < COLS && dropPiece(col)) {
                    if (checkWin()) {
                        std::string winner = (currentPlayer == Player::Red) ? "Player 1 Wins!" : "Player 2 Wins!";
                        showEndScreen(winner);
                        gameOver = true;
                    } else {
                        currentPlayer = (currentPlayer == Player::Red ? Player::Blue : Player::Red);
                    }
                }
            }
        }
    }
}

bool ConnectFour::dropPiece(int col) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (board[row][col] == Player::None) {
            board[row][col] = currentPlayer;
            return true;
        }
    }
    return false;
}

bool ConnectFour::checkWin() {
    // Check horizontal win
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col <= COLS - 4; ++col) {
            if (board[row][col] == currentPlayer &&
                board[row][col + 1] == currentPlayer &&
                board[row][col + 2] == currentPlayer &&
                board[row][col + 3] == currentPlayer) {
                return true;
            }
        }
    }

    // Check vertical win
    for (int col = 0; col < COLS; ++col) {
        for (int row = 0; row <= ROWS - 4; ++row) {
            if (board[row][col] == currentPlayer &&
                board[row + 1][col] == currentPlayer &&
                board[row + 2][col] == currentPlayer &&
                board[row + 3][col] == currentPlayer) {
                return true;
            }
        }
    }

    // Check diagonal win (top-left to bottom-right)
    for (int row = 0; row <= ROWS - 4; ++row) {
        for (int col = 0; col <= COLS - 4; ++col) {
            if (board[row][col] == currentPlayer &&
                board[row + 1][col + 1] == currentPlayer &&
                board[row + 2][col + 2] == currentPlayer &&
                board[row + 3][col + 3] == currentPlayer) {
                return true;
            }
        }
    }

    // Check diagonal win (bottom-left to top-right)
    for (int row = 3; row < ROWS; ++row) {
        for (int col = 0; col <= COLS - 4; ++col) {
            if (board[row][col] == currentPlayer &&
                board[row - 1][col + 1] == currentPlayer &&
                board[row - 2][col + 2] == currentPlayer &&
                board[row - 3][col + 3] == currentPlayer) {
                return true;
            }
        }
    }
    return false;
}

void ConnectFour::reset() {
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), Player::None);
    }
    currentPlayer = Player::Red;
}

void ConnectFour::showEndScreen(const std::string& message) {
    endMessage.setString(message);
}

void ConnectFour::draw() {
    window.clear(sf::Color(105, 105, 105)); // Grey background

    if (gameOver) {
        window.draw(endMessage);
        window.draw(playAgainButton);
        window.draw(quitButton);
        window.draw(playAgainText);
        window.draw(quitText);
    } else {
        // Draw player labels
        window.draw(player1Label);
        window.draw(player2Label);

        // Draw the grid, centered in the window
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                sf::CircleShape piece(HOLE_RADIUS);
                piece.setPosition(col * SPACING + (WINDOW_WIDTH - GRID_WIDTH) / 2, 
                                  row * SPACING + (WINDOW_HEIGHT - GRID_HEIGHT) / 2 + 50); // Adjust for centering
                piece.setOutlineColor(sf::Color::Black);
                piece.setOutlineThickness(2);
                piece.setFillColor(sf::Color::White);

                // Set the fill color based on the player
                if (board[row][col] == Player::Red) {
                    piece.setFillColor(sf::Color::Red);
                } else if (board[row][col] == Player::Blue) {
                    piece.setFillColor(sf::Color::Blue);
                }

                window.draw(piece);
            }
        }
    }

    window.display();
}

int main() {
    ConnectFour game;
    game.run();
    return 0;
}