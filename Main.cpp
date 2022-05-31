//
// Tetris
//
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

class Shape
{
public:
    // structure of the shape
    // black = empty 
    sf::Color tiles[4][4];

    // position on board
    sf::Vector2i pos;

    // times since last downward movement
    float time;

    Shape();

    // reinitialise the shape: move to top and randomly choose shape
    void init();

    // move downwards once per second
    void update(float dt);

    // render the shape
    void draw(sf::RenderWindow& w);

    // rotate the shape
    void rotateLeft();
    void rotateRight();
};


void Shape::rotateLeft()
{
    sf::Color tmp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tmp[i][j] = tiles[j][3 - i];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tiles[i][j] = tmp[i][j];
        }
    }
}

void Shape::rotateRight()
{
    rotateLeft();
    rotateLeft();
    rotateLeft();
}

Shape::Shape()
{
    init();
}

void Shape::init()
{
    // move to top and reset timer
    pos.y = 0;
    pos.x = 4;
    time = 0.0f;

    // fill with black tiles
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tiles[i][j] = sf::Color::Black;
        }
    }

    // two hardcoded shapes - this should be changed by you
    // rand()??  I know...
    switch (rand() % 5) {

    case 0:
        tiles[2][0] = sf::Color::Red;
        tiles[2][1] = sf::Color::Red;
        tiles[2][2] = sf::Color::Red;
        tiles[2][3] = sf::Color::Red;
        break;

    case 1:
        tiles[0][2] = sf::Color::Blue;
        tiles[1][2] = sf::Color::Blue;
        tiles[1][1] = sf::Color::Blue;
        tiles[2][1] = sf::Color::Blue;
        break;
    case 2:
        tiles[1][0] = sf::Color::Green;
        tiles[1][1] = sf::Color::Green;
        tiles[2][1] = sf::Color::Green;
        tiles[2][2] = sf::Color::Green;
        break;
    case 3:
        tiles[1][0] = sf::Color::White;
        tiles[2][0] = sf::Color::White;
        tiles[1][1] = sf::Color::White;
        tiles[1][2] = sf::Color::White;
        break;
    case 4:
        tiles[0][0] = sf::Color::Cyan;
        tiles[0][1] = sf::Color::Cyan;
        tiles[1][0] = sf::Color::Cyan;
        tiles[1][1] = sf::Color::Cyan;
        break;
    case 5:
        tiles[2][0] = sf::Color::Yellow;
        tiles[2][1] = sf::Color::Yellow;
        tiles[2][2] = sf::Color::Yellow;
        tiles[3][1] = sf::Color::Yellow;
        break;
    }
}


void Shape::update(float dt)
{
    time += dt;
    if (time > 1) {
        time = 0;
        pos.y += 1;
    }
}

void Shape::draw(sf::RenderWindow& w)
{
    sf::CircleShape s;
    s.setRadius(8);
    s.setOrigin(8, 8);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tiles[i][j] != sf::Color::Black) {
                s.setFillColor(tiles[i][j]);
                s.setPosition(sf::Vector2f(pos.x * 16 + 16 * i + 100, pos.y * 16 + 16 * j + 100));
                w.draw(s);
            }
        }
    }
}


class Board
{
public:
    sf::Color tiles[12][20];

    Board();

    // add a shape to the board
    void add(Shape& shape);

    // check if a shape intersects with the board
    bool intersect(Shape& shape);

    // remove full lines - should be implemented by you
    void reduce(Board& board);

    // render board
    void draw(sf::RenderWindow& w);
};


void Board::reduce(Board& board)
{
    //bool lineFull = false;
    for (int i = 0; i < 20; i++) //for every y
    {
        bool lineNotFull = true;
        for (int j = 0; j < 12; j++) //for every x in y
        {
            if (board.tiles[j][i] == sf::Color::Black)
            {
                lineNotFull = false;
                break;
            }
        }
    }
}

bool Board::intersect(Shape& shape)
{
    bool intersect = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tiles[i + shape.pos.x][j + shape.pos.y] != sf::Color::Black &&
                shape.tiles[i][j] != sf::Color::Black)
                intersect = true;
        }
    }
    return intersect;
}

void Board::draw(sf::RenderWindow& w)
{
    sf::CircleShape s;
    s.setRadius(8);
    s.setOrigin(8, 8);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 20; j++) {
            s.setFillColor(tiles[i][j]);
            s.setPosition(sf::Vector2f(16 * i + 100, 16 * j + 100));
            w.draw(s);
        }
    }
}

void Board::add(Shape& shape)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            if (shape.tiles[i][j] != sf::Color::Black) {
                tiles[i + shape.pos.x][j + shape.pos.y] = shape.tiles[i][j];
            }
        }
    }
}


Board::Board()
{
    // fill with black
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 12; j++) {
            tiles[j][i] = sf::Color::Black;
        }
    }

    // boundary
    for (int i = 0; i < 12; i++) {
        tiles[i][19] = sf::Color::Red;
    }
    for (int i = 0; i < 19; i++) {
        tiles[0][i] = sf::Color::Red;
        tiles[11][i] = sf::Color::Red;
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(512, 512), "Tetris");

    sf::Clock clock;
    clock.restart();

    Shape shape;

    Board board;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    shape.pos.x -= 1;
                    if (board.intersect(shape)) {
                        shape.pos.x += 1;
                        cout << "intersect left" << endl;
                    }
                }
                if (event.key.code == sf::Keyboard::Right) {
                    shape.pos.x += 1;
                    if (board.intersect(shape)) {
                        shape.pos.x -= 1;
                        cout << "intersect right" << endl;
                    }
                }
                if (event.key.code == sf::Keyboard::Down) {
                    shape.pos.y += 1;
                    if (board.intersect(shape)) {
                        shape.pos.y -= 1;
                        cout << "intersect down" << endl;
                    }
                }
                if (event.key.code == sf::Keyboard::Up) {
                    shape.rotateLeft();
                    if (board.intersect(shape)) {
                        shape.rotateRight();
                        cout << "intersect rotate" << endl;
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // move downwards once per second
        shape.update(dt);

        // detect collision
        if (board.intersect(shape)) {
            // resolve collision
            shape.pos.y -= 1;
            // add shape to board
            board.add(shape);
            // remove full lines
            board.reduce(board);
            // start with a new shape on top
            shape.init();
            if (board.intersect(shape)) {
                cout << "GAME OVER" << endl;
            }
        }

        window.clear(sf::Color::Black);

        board.draw(window);
        shape.draw(window);

        window.display();
    }

    return 0;
}