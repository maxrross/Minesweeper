#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <random>
#include <fstream>
#include <string>
#include <random>

using namespace std;

class Board{
public:
    void setBoard(string Board);
    int width;
    int height;
    int origMines;
    int mines;
    int rows;
    int cols;
    bool debugMode=false;
    bool gameOver=false;
    bool victory=false;
    int flags;
    bool debugOn;
};

class Random
{
    static std::mt19937 random;
public:
    static int Int(int min, int max);
};

class Tile{
public:
    int mineCount =0;
    bool boolMine=false;
    bool boolFlag=false;
    bool pressed = false;
    sf::Sprite getSprite();
    void setSprite(sf::Sprite);
    sf::Sprite getFlag();
    void setFlag(sf::Sprite);
    sf::Sprite getMine();
    void setMine(sf::Sprite currMine);
    sf::Sprite getNumber();
    void setNumber(int currNumber);
    vector<Tile*> surrounding;
    void Splatter();
private:
    sf::Sprite currSprite;
    sf::Sprite flag;
    sf::Sprite mine;
    sf::Sprite number;
};

void restartGame();

bool checkifwon(vector<vector<Tile>> Tiles);

Board config;
vector<vector <Tile>> Tiles;
map<string, sf::Texture> textures;

int main()
{
    ifstream inFile("boards/config.cfg");
    if (inFile.is_open()) {
        string col;
        string row;
        string tempM;
        while (!inFile.eof()) {
            getline(inFile, col);
            config.width = stoi(col)*32;
            getline(inFile, row);
            config.height = (stoi(row)*32)+100;
            getline(inFile, tempM);
            config.mines = stoi(tempM);
            config.origMines=stoi(tempM);
            config.rows = stoi(row);
            config.cols = stoi(col);
        }
    } else {
        cout << "file isnt open";
    }
    //loading textures
    sf::Texture temp;
    temp.loadFromFile("images/debug.png");
    textures.emplace("debug",temp);
    temp.loadFromFile("images/digits.png");
    textures.emplace("digits",temp);
    temp.loadFromFile("images/face_happy.png");
    textures.emplace("happy",temp);
    temp.loadFromFile("images/face_lose.png");
    textures.emplace("lose",temp);
    temp.loadFromFile("images/face_win.png");
    textures.emplace("win",temp);
    temp.loadFromFile("images/flag.png");
    textures.emplace("flag",temp);
    temp.loadFromFile("images/mine.png");
    textures.emplace("mine",temp);
    temp.loadFromFile("images/number_1.png");
    textures.emplace("1",temp);
    temp.loadFromFile("images/number_2.png");
    textures.emplace("2",temp);
    temp.loadFromFile("images/number_3.png");
    textures.emplace("3",temp);
    temp.loadFromFile("images/number_4.png");
    textures.emplace("4",temp);
    temp.loadFromFile("images/number_5.png");
    textures.emplace("5",temp);
    temp.loadFromFile("images/number_6.png");
    textures.emplace("6",temp);
    temp.loadFromFile("images/number_7.png");
    textures.emplace("7",temp);
    temp.loadFromFile("images/number_8.png");
    textures.emplace("8",temp);
    temp.loadFromFile("images/test_1.png");
    textures.emplace("test1",temp);
    temp.loadFromFile("images/test_2.png");
    textures.emplace("test2",temp);
    temp.loadFromFile("images/test_3.png");
    textures.emplace("test3",temp);
    temp.loadFromFile("images/tile_hidden.png");
    textures.emplace("hidden",temp);
    temp.loadFromFile("images/tile_revealed.png");
    textures.emplace("revealed",temp);

    sf::RenderWindow window(sf::VideoMode(config.width, config.height), "Minesweeper");
    for (int i = 0; i< config.rows; i++){
        vector<Tile> currRow;
        for (int j=0; j<config.cols; j++){
            //need to make a vector for each row then push that back to tiles
            Tile currentTile;
            sf::Sprite hiddenTile;
            hiddenTile.setTexture(textures["hidden"]);
            sf::Sprite hiddenFlag;
            hiddenFlag.setTexture(textures["flag"]);
            sf::Sprite hiddenMine;
            hiddenMine.setTexture(textures["mine"]);
            currentTile.setMine(hiddenMine);
            currentTile.setFlag(hiddenFlag);
            currentTile.setSprite(hiddenTile);

            currRow.push_back(currentTile);
        }
        Tiles.push_back(currRow);
    }
    int currMines = 0;
    while (currMines < config.mines){
        int col = Random::Int(0,config.cols-1);
        int row = Random::Int(0,config.rows-1);
        if (Tiles[row][col].boolMine==false){
            Tiles[row][col].boolMine=true;
            currMines++;
        }
    }
    for (int i=0; i<config.rows; i++){
        for (int j=0; j<config.cols; j++){
            //if row is on top, make first 3 elements null
            if (i==0){
                //if on top and top left
                if (j==0){
                    for (int k = 0; k < 5; k++) {
                        Tiles[i][j].surrounding.push_back(nullptr);
                    }
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j+1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i+1][j]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i+1][j+1]);
                    //if on top and top right
                } else if (j==config.cols-1){
                    for (int k = 0; k < 3; k++) {
                        Tiles[i][j].surrounding.push_back(nullptr);
                    }
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j-1]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(&Tiles[i+1][j-1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i+1][j]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    //else on top
                } else {
                    for (int k = 0; k < 3; k++) {
                        Tiles[i][j].surrounding.push_back(nullptr);
                    }
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j - 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j + 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j - 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j + 1]);
                }
                //if on bottom and bottom left
            } else if (i==config.rows-1){
                if (j==0){
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j+1]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j + 1]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                } else if (j==config.cols-1){
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j - 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j - 1]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                } else {
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j - 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j + 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j - 1]);
                    Tiles[i][j].surrounding.push_back(&Tiles[i][j + 1]);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                    Tiles[i][j].surrounding.push_back(nullptr);
                }
            } else if (j==config.cols-1){
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j - 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j]);
                Tiles[i][j].surrounding.push_back(nullptr);
                Tiles[i][j].surrounding.push_back(&Tiles[i][j - 1]);
                Tiles[i][j].surrounding.push_back(nullptr);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j - 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j]);
                Tiles[i][j].surrounding.push_back(nullptr);
            } else if (j==0){
                Tiles[i][j].surrounding.push_back(nullptr);
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j]);
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j + 1]);
                Tiles[i][j].surrounding.push_back(nullptr);
                Tiles[i][j].surrounding.push_back(&Tiles[i][j + 1]);
                Tiles[i][j].surrounding.push_back(nullptr);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j]);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j + 1]);
            } else {
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j - 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j]);
                Tiles[i][j].surrounding.push_back(&Tiles[i - 1][j + 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i][j - 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i][j + 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j - 1]);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j]);
                Tiles[i][j].surrounding.push_back(&Tiles[i + 1][j + 1]);
            }
        }
    }
    for (int i=0; i<config.rows; i++) {
        for (int j = 0; j < config.cols; j++) {
            for (int k=0; k<Tiles[i][j].surrounding.size(); k++){
                if (Tiles[i][j].surrounding.at(k)!=nullptr && Tiles[i][j].surrounding.at(k)->boolMine==true){
                    Tiles[i][j].mineCount++;
                }
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        window.clear(sf::Color::White);
        //drawing mines
        //rand 0 to col
        //DRAWING SQUARES AND FLAGS
        int x = 0;
        int y = 0;
        int width = 0;
        for (int i = 0; i < config.rows; i++) {
            for (int j = 0; j < config.cols; j++) {
                sf::Sprite tempFlag = Tiles[i][j].getFlag();
                sf::Sprite tempMine = Tiles[i][j].getMine();
                if (Tiles[i][j].boolFlag == true) {
                    tempFlag.setColor(sf::Color(255, 255, 255, 255));
                } else {
                    tempFlag.setColor(sf::Color(255, 255, 255, 0));
                }
                Tiles[i][j].setFlag(tempFlag);
                tempFlag.move(sf::Vector2f(x, y));

                sf::Sprite tempSprite = Tiles[i][j].getSprite();
                tempSprite.move(sf::Vector2f(x, y));
                if (Tiles[i][j].boolMine == true && config.debugMode == true && !config.gameOver) {
                    tempMine.setColor(sf::Color(255, 255, 255, 255));
//                    tempSprite.setTexture(textures["revealed"]);
                } else if (Tiles[i][j].boolMine == true && config.debugMode && config.gameOver && !config.victory) {
                    tempMine.setColor(sf::Color(255, 255, 255, 255));
                    tempSprite.setTexture(textures["revealed"]);
                } else {
                    tempMine.setColor(sf::Color(255, 255, 255, 0));
                }
                //tempMine.setColor(sf::Color(255, 255, 255, 0));
                tempMine.move(sf::Vector2f(x, y));

                sf::Sprite tempNumber = Tiles[i][j].getNumber();
                tempNumber.move(sf::Vector2f(x, y));
                window.draw(tempSprite);
                window.draw(tempNumber);
                window.draw(tempFlag);
                window.draw(tempMine);
//                window.draw(Tiles[i][j].getFlag());
                x += 32.f;
            }
            width = x;
            x = 0;
            y += 32.f;
        }
        x = 0;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (position.y >= 0 && position.y < y) {
                    if (position.x >= 0 && position.x <= width) {
                        int x = (position.x / 32);
                        int y = (position.y / 32);
                        if (Tiles[y][x].pressed == false && !Tiles[y][x].boolFlag && config.gameOver == false && !Tiles[y][x].boolMine) {
                            Tiles[y][x].Splatter();
                        }
                        if (Tiles[y][x].pressed == false && !Tiles[y][x].boolFlag && config.gameOver == false && Tiles[y][x].boolMine == true) {
                            config.gameOver = true;
                        }
                    }
                } else if (position.y >= y && position.y < y + 60) {
                    //if u click on smiley face and game is over, restart game
                    if (position.x >= width / 2 - 32 && position.x < width / 2 + 32) {
//                        if (config.gameOver==true){
                        restartGame();
//                        }
                    }
                    if (position.x >= width / 2 + 96 && position.x < width / 2 + 160) {
                        if (config.debugMode == false && !config.gameOver) {
                            config.debugMode = true;
                            config.debugOn = true;
                        } else if (!config.gameOver){
                            config.debugOn = false;
                            config.debugMode = false;
                        }
                    }
                    if (position.x >= width / 2 + 160 && position.x < width / 2 + 224) {
                        config.setBoard("testboard1");
                    }
                    if (position.x >= width / 2 + 224 && position.x < width / 2 + 288) {
                        config.setBoard("testboard2");
                    }
                    if (position.x >= width / 2 + 288 && position.x < width / 2 + 352) {
                        config.setBoard("testboard3");
                    }
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (position.y >= 0 && position.y < 512) {
                    if (position.x >= 0 && position.x <= 800) {
                        int x = (position.x / 32);
                        int y = (position.y / 32);
                        if (!Tiles[y][x].boolFlag && !Tiles[y][x].pressed && !config.gameOver) {
                            Tiles[y][x].boolFlag = true;
                            config.flags++;
                        } else if (Tiles[y][x].boolFlag && !config.gameOver) {
                            Tiles[y][x].boolFlag = false;
                            config.flags--;
                        }
                    }
                }
            }
        }

        sf::Sprite debug(textures["debug"]);
        debug.setPosition(width / 2 + 96, y);
        sf::Sprite test1(textures["test1"]);
        test1.setPosition(width / 2 + 160, y);
        sf::Sprite test2(textures["test2"]);
        test2.setPosition(width / 2 + 224, y);
        sf::Sprite test3(textures["test3"]);
        test3.setPosition(width / 2 + 288, y);
        if (checkifwon(Tiles)) {
            sf::Sprite smiley(textures["win"]);
            smiley.setPosition(width / 2 - 32, y);
            window.draw(smiley);
            config.victory=true;
            for (int i = 0; i < config.rows; i++) {
                for (int j = 0; j < config.cols; j++) {
                    if (Tiles[i][j].boolMine) {
                        Tiles[i][j].boolFlag = true;
                    }
                }
            }
        } else if (config.gameOver == false) {
            sf::Sprite smiley(textures["happy"]);
            smiley.setPosition(width / 2 - 32, y);
            window.draw(smiley);
        } else {
            sf::Sprite smiley(textures["lose"]);
            smiley.setPosition(width / 2 - 32, y);
            window.draw(smiley);
            config.debugMode = true;
        }
        if (config.flags > config.mines) {
            //set to negative
            sf::Sprite negative(textures["digits"]);
            negative.setTextureRect(sf::Rect<int>(21 * 10, 0, 21, 32));
            negative.setPosition(0, y);
            window.draw(negative);
        }
        if (config.mines-config.flags>100){
            //set to 1
            sf::Sprite hundreds(textures["digits"]);
            hundreds.setTextureRect(sf::Rect<int>(21, 0, 21, 32));
            hundreds.setPosition(21, y);
            window.draw(hundreds);
        } else if(config.mines-config.flags<-100) {
            sf::Sprite hundreds(textures["digits"]);
            hundreds.setTextureRect(sf::Rect<int>(21, 0, 21, 32));
            hundreds.setPosition(21, y);
            window.draw(hundreds);
        }else {
            //sets to zero
            sf::Sprite hundreds(textures["digits"]);
            hundreds.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
            hundreds.setPosition(21, y);
            window.draw(hundreds);
        }
        if (config.flags>config.mines) {
            sf::Sprite tens(textures["digits"]);
            tens.setTextureRect(sf::Rect<int>(21 * ((config.flags-config.mines)/10), 0, 21, 32));
            tens.setPosition(42, y);
            window.draw(tens);
        }
        else if (config.mines-config.flags>100){
            sf::Sprite tens(textures["digits"]);
            tens.setTextureRect(sf::Rect<int>(21 * ((config.mines-config.flags)/100), 0, 21, 32));
            tens.setPosition(42, y);
            window.draw(tens);
        } else {
            sf::Sprite tens(textures["digits"]);
            tens.setTextureRect(sf::Rect<int>(21 * ((config.mines-config.flags)/10), 0, 21, 32));
            tens.setPosition(42, y);
            window.draw(tens);
        }
        if (config.flags>config.mines) {
            sf::Sprite ones(textures["digits"]);
            ones.setTextureRect(sf::Rect<int>(21 * ((config.flags - config.mines) % 10), 0, 21, 32));
            ones.setPosition(63, y);
            window.draw(ones);
        } else {
            sf::Sprite ones(textures["digits"]);
            ones.setTextureRect(sf::Rect<int>(21 * ((config.mines - config.flags) % 10), 0, 21, 32));
            ones.setPosition(63, y);
            window.draw(ones);
        }
        window.draw(debug);
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        window.display();
    }
    return 0;
}

bool checkifwon(vector<vector<Tile>> Tiles) {
    int tileCount= (config.rows*config.cols)-config.mines;
    for (int i=0; i<config.rows; i++){
        for (int j=0; j<config.cols; j++){
            if(Tiles[i][j].pressed){
                tileCount--;
            }
        }
    }
    if (tileCount==0){
        config.flags=config.mines;
        config.gameOver=true;
        return true;
    } else {
        return false;
    }
}

void restartGame() {
    config.gameOver=false;
    config.victory=false;
    config.flags=0;
    config.debugMode=config.debugOn;
    int currMines = 0;
    for (int i=0; i<config.rows; i++){
        for (int j=0; j<config.cols; j++){
            Tiles[i][j].boolMine=false;
            Tiles[i][j].boolFlag=false;
            Tiles[i][j].pressed=false;
            Tiles[i][j].mineCount=0;
            Tiles[i][j].setNumber(0);
            sf::Sprite tmpSprite = Tiles[i][j].getSprite();
            tmpSprite.setTexture(textures["hidden"]);
            Tiles[i][j].setSprite(tmpSprite);
        }
    }
    config.mines=config.origMines;
    while (currMines < config.origMines){
        int col = Random::Int(0,config.cols-1);
        int row = Random::Int(0,config.rows-1);
        if (Tiles[row][col].boolMine==false){
            Tiles[row][col].boolMine=true;
            currMines++;
        }
    }
    for (int i=0; i<config.rows; i++) {
        for (int j = 0; j < config.cols; j++) {
            for (int k=0; k<Tiles[i][j].surrounding.size(); k++){
                if (Tiles[i][j].surrounding.at(k)!=nullptr && Tiles[i][j].surrounding.at(k)->boolMine==true){
                    Tiles[i][j].mineCount++;
                }
            }
        }
    }
}
void Tile::Splatter() {
    if (mineCount > 0 && !boolFlag) {
        sf::Sprite tmpSprite = this->getSprite();
        tmpSprite.setTexture(textures["revealed"]);
        this->setSprite(tmpSprite);
        this->pressed = true;
        this->setNumber(this->mineCount);
    } else if (!boolFlag) {
        sf::Sprite tmpSprite = this->getSprite();
        tmpSprite.setTexture(textures["revealed"]);
        this->setSprite(tmpSprite);
        this->pressed = true;
        for (int c = 0; c < 8; c++) {
            if (surrounding[c] != nullptr && !(surrounding[c]->boolMine) && !(surrounding[c]->pressed)){
                surrounding[c]->Splatter();
            }
        }
    }
}



sf::Sprite Tile::getSprite() {
    return currSprite;
}

void Tile::setSprite(sf::Sprite sprite) {
    currSprite = sprite;
}

sf::Sprite Tile::getFlag() {
    return flag;
}

void Tile::setFlag(sf::Sprite currFlag) {
    flag = currFlag;
}

sf::Sprite Tile::getMine() {
    return mine;
}

void Tile::setMine(sf::Sprite currMine) {
    mine = currMine;
}

sf::Sprite Tile::getNumber() {
    return number;
}

void Tile::setNumber(int currNumber) {
    //say number is 1
//    set number equal to the sprite of one
    sf::Sprite tempSprite;
    tempSprite.setTexture(textures[std::to_string(currNumber)]);
    number = tempSprite;
}

std::mt19937 Random::random(time(0));

int Random::Int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(random);
}

void Board::setBoard(string Board){
    ifstream inFile("boards/"+Board+".brd", ios_base::binary);
    if (inFile.is_open()) {
        //start import here
        config.gameOver=false;
        config.debugMode=false;
        config.flags=0;
        for (int i=0; i<config.rows; i++){
            for (int j=0; j<config.cols; j++){
                Tiles[i][j].boolMine=false;
                Tiles[i][j].boolFlag=false;
                Tiles[i][j].pressed=false;
                Tiles[i][j].mineCount=0;
                Tiles[i][j].setNumber(0);
                sf::Sprite tmpSprite = Tiles[i][j].getSprite();
                tmpSprite.setTexture(textures["hidden"]);
                Tiles[i][j].setSprite(tmpSprite);
            }
        }
        //end import here
        int linenum=0;
        config.mines=0;
        for (string line; getline(inFile, line);)
        {
            for (int i=0; i<line.size(); i++){
                if (line[i]=='1'){
//                    set a bomb at linenum, i
                    Tiles[linenum][i].boolMine=true;
                    config.mines++;
                } else {
                    Tiles[linenum][i].boolMine=false;
                }
            }
            linenum++;
        }
        for (int i=0; i<config.rows; i++) {
            for (int j = 0; j < config.cols; j++) {
                for (int k=0; k<Tiles[i][j].surrounding.size(); k++){
                    if (Tiles[i][j].surrounding.at(k)!=nullptr && Tiles[i][j].surrounding.at(k)->boolMine==true && !Tiles[i][j].boolMine){
                        Tiles[i][j].mineCount++;
                    }
                }
            }
        }
    } else {
        cout << "file isnt open";
    }
}


