#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
using namespace sf;
using namespace std;
const int numPipes = 3;       // số lượng cặp ống
const int pipeSpacing = 400;  // khoảng cách ngang giữa các ống
vector<bool> pipeScored(numPipes, false); // mảng cờ đánh dấu đã cộng điểm qua ống
const string mapNames[] = { "CLASSIC", "XMAS", "OCEAN", "NIGHT", "MINECRAFT", "HALLOWEEN" };
const int numMaps = 6;
enum MapType { DEFAULT, XMAS, OCEAN, NIGHT, MINECRAFT, HALLOWEEN };
MapType currentMap = DEFAULT;
void loadMapAssets(
    MapType map,
    Texture& backgroundTexture, Texture& pipeTexture, Texture& groundTexture, Texture& birdTexture,Texture& startTexture,   
    string& mapName
) {
    string folder, bg, pipe, ground, bird,start;
    switch (map) {
        case XMAS:
            folder = "xmas/";
            mapName = "XMAS";
            bg = "background-xmas.png";
            pipe = "pipe-xmas.png";
            ground = "ground-xmas.png";
            bird = "bird-xmas.png";
            start = "message-xmas.png";
            break;
        case OCEAN:
            folder = "ocean/";
            mapName = "OCEAN";
            bg = "background-fish.png";
            pipe = "pipe-fish.png";
            ground = "ground-fish.png";
            bird = "bird-fish.png";
            start = "message-fish.png";
            break;
        case NIGHT:
            folder = "night/";
            mapName = "NIGHT";
            bg = "background-night.png";
            pipe = "pipe-night.png";
            ground = "ground-night.png";
            bird = "bird-night.png";
            start = "message-night.png";
            break;
        case MINECRAFT:
            folder = "minecraft/";
            mapName = "MINECRAFT";
            bg = "background-craft.png";
            pipe = "pipe-craft.png";
            ground = "ground-craft.png";
            bird = "bird-craft.png";
            start = "message-craft.png";
            break;
        case HALLOWEEN:
            folder = "halloween/";
            mapName = "HALLOWEEN";
            bg = "background-halloween.png";
            pipe = "pipe-halloween.png";
            ground = "ground-halloween.png";
            bird = "bird-halloween.png";
            start = "message-halloween.png";
            break;
        default:
            folder = "";
            mapName = "CLASSIC";
            bg = "nen.png";
            pipe = "ong.png";
            ground = "dat.png";
            bird = "chim.png";
            start = "xuatphatgame.png";
    }
    backgroundTexture.loadFromFile(folder + bg);
    pipeTexture.loadFromFile(folder + pipe);
    groundTexture.loadFromFile(folder + ground);
    birdTexture.loadFromFile(folder + bird);
    startTexture.loadFromFile(folder + start);
}
// Hàm reset game về trạng thái ban đầu
void resetGame(Sprite& bird,
               vector<Sprite>& pipesTop, vector<Sprite>& pipesBottom,
               int& score, Text& scoreText,
               bool& scored, bool& gameStarted, bool& gameOver,
               float& birdSpeed, float& birdAngle,
               Vector2u bgSize, int groundHeight,
               int pipeWidth, int pipeSpacing, int khoangcach,
               int birdFrameWidth, int birdFrameHeight) 
{
    // Reset chim
    bird.setPosition(100, 300);
    bird.setTextureRect(IntRect(0, 0, birdFrameWidth, birdFrameHeight));
    birdSpeed = 0.0f;
    birdAngle = 0.0f;
    bird.setRotation(0);

    for (int i = 0; i < numPipes; i++) {
    pipeScored[i] = false;
    }
    // Reset pipes
    for (int i = 0; i < (int)pipesTop.size(); i++) {
        int minGapY = 100;
        int maxGapY = bgSize.y - groundHeight - khoangcach - 100;
        int gapY = minGapY + rand() % (maxGapY - minGapY + 1);

        pipesTop[i].setPosition(bgSize.x + i * pipeSpacing, gapY);
        pipesBottom[i].setPosition(bgSize.x + i * pipeSpacing, gapY + khoangcach);
    }
    // Reset score & state
    score = 0;
    scoreText.setString("0");
    scored = false;
    gameStarted = false;
    gameOver = false;
}

void updatePipes(
    vector<Sprite> &pipesTop,
    vector<Sprite> &pipesBottom,
    vector<bool> &pipeScored,
    Sprite &bird,
    int &score,
    Text &scoreText,
    int numPipes,
    int pipeWidth,
    int pipeSpacing,
    int khoangcach,
    int groundHeight,
    float pipeSpeed,
    Vector2u bgSize,
    RenderWindow &window
) {
    for (int i = 0; i < numPipes; i++) {
        // Di chuyển ống
        pipesTop[i].move(-pipeSpeed, 0);
        pipesBottom[i].move(-pipeSpeed, 0);

        // Reset khi ống ra khỏi màn hình
        if (pipesTop[i].getPosition().x < -pipeWidth) {
            int minGapY = 100;
            int maxGapY = bgSize.y - groundHeight - khoangcach - 100;
            int newGapY = minGapY + rand() % (maxGapY - minGapY + 1);

            // tìm ống xa nhất bên phải
            float maxX = 0;
            for (int j = 0; j < numPipes; j++) {
                if (pipesTop[j].getPosition().x > maxX) {
                    maxX = pipesTop[j].getPosition().x;
                }
            }

            pipesTop[i].setPosition(maxX + pipeSpacing, newGapY);
            pipesBottom[i].setPosition(maxX + pipeSpacing, newGapY + khoangcach);

            pipeScored[i] = false; // reset lại cờ tính điểm
        }

        // Tính điểm
        float pipeX = pipesTop[i].getPosition().x + pipeWidth / 2;
        if (bird.getPosition().x > pipeX && !pipeScored[i]) {
            score++;
            pipeScored[i] = true;
            scoreText.setString(std::to_string(score));
        }

        // Vẽ ống
        window.draw(pipesTop[i]);
        window.draw(pipesBottom[i]);
    }
}




void updateMedal(int score, Texture &medalTexture, Sprite &medalSprite, Vector2u bgSize) {
    string medalFile = "";

    if (score >= 300) medalFile = "thachdau.png";
    else if (score >= 200) medalFile = "dct.png";
    else if (score >= 100) medalFile = "caothu.png";
    else if (score >= 70) medalFile = "rankkc.png";
    else if (score >= 60) medalFile = "ranklb.png";
    else if (score >= 50) medalFile = "rankbk.png";
    else if (score >= 40) medalFile = "rankvang.png";
    else if (score >= 30) medalFile = "rankbac.png";
    else if (score >= 20) medalFile = "rankdong.png";
    else if (score >= 10) medalFile = "ranksat.png";

    if (medalFile != "" && medalTexture.loadFromFile(medalFile)) {
        medalSprite.setTexture(medalTexture);
        medalSprite.setPosition(bgSize.x/2 - 220, bgSize.y/2 - 120 ); 
        // chỉnh toạ độ cho đẹp (bên trái finalScoreText)
    }
}



int main() {
    srand(time(nullptr)); // Khởi tạo random seed
    string mapName = "CLASSIC";
///////////////////////////////////////////////////////////////////SETUP/////////////////////////////////////////////////////////////////
    // Tạo nền :>>
    Texture backgroundTexture; // tạo texture cho nền
    if(!backgroundTexture.loadFromFile("nen.png")) { // tải hình ảnh nền từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }

    // Tạo cửa sổ :>>
    // Lấy kích thước ảnh nền để khớp với kích thước cửa sổ
    Vector2u bgSize = backgroundTexture.getSize();
    RenderWindow window(VideoMode(bgSize.x,bgSize.y),"Flappy Bird"); // tạo khung cửa sổ và tiêu đề bên cửa sổ
    window.setFramerateLimit(120);

    Sprite background (backgroundTexture); // tạo sprite từ texture nền
    
    Image icon;
    if (!icon.loadFromFile("Flappy_Bird_icon.png")) {
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Tạo ống :>>
    Texture pipeTexture; // tạo texture cho ống
    if (!pipeTexture.loadFromFile("ong.png")) { // tải hình ảnh ống từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    // Khởi tạo các biến cho ống
    int groundHeight = 100; // chiều cao của đất
    int gapY = 300; // vị trí y của khoảng cách giữa ống trên và ống dưới
    int khoangcach=300; // khoảng cách giữa ống trên và ống dưới là 500
    int pipeHeight = pipeTexture.getSize().y; // chiều cao của ống từ texture
    int pipeWidth = pipeTexture.getSize().x; // chiều rộng của ống từ texture
    int pipeX = bgSize.x + pipeWidth / 2; // vị trí x của ống
    
    // Tạo ống trên :>>
    Sprite pipeTop(pipeTexture); // tạo sprite từ texture ống
    pipeTop.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh  
    pipeTop.setRotation(180); // xoay ống trên 180 độ để hiển thị đúng
    //pipeTop.setPosition(pipeX, gapY); // vị trí ống trên

    // Tạo ống dưới :>>
    Sprite pipeBottom(pipeTexture); // tạo sprite từ texture ống
    pipeBottom.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh dưới
    //pipeBottom.setPosition(pipeX, gapY + khoangcach); // vị trí ống dưới, cách ống trên một khoảng cách nhất định


    vector<Sprite> pipesTop;
    vector<Sprite> pipesBottom;

    for (int i = 0; i < numPipes; i++) {
        Sprite top(pipeTexture);
        Sprite bottom(pipeTexture);

        top.setOrigin(pipeWidth / 2, 0);
        top.setRotation(180);

        bottom.setOrigin(pipeWidth / 2, 0);

        int minGapY = 100;
        int maxGapY = bgSize.y - groundHeight - khoangcach - 100;
        int gapY = minGapY + rand() % (maxGapY - minGapY + 1);

        // Mỗi ống cách nhau pipeSpacing
        top.setPosition(bgSize.x + i * pipeSpacing+200, gapY);
        bottom.setPosition(bgSize.x + i * pipeSpacing+200, gapY + khoangcach);

        pipesTop.push_back(top);
        pipesBottom.push_back(bottom);
    }

    // Tạo đất 
    Texture groundTexture; // tạo texture cho đất
    if (!groundTexture.loadFromFile("dat.png")) { // tải hình ảnh đất từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    int groundWidth = groundTexture.getSize().x; // chiều rộng của đất từ texture
    int groundY = bgSize.y - groundHeight; // vị trí y của đất

    const int numGround = bgSize.x / groundWidth + 4; // +4 để tránh hở
    Sprite groundSprites[numGround]; // mảng sprite đất
    for (int i = 0; i < numGround; ++i) { // khởi tạo các sprite đất
        groundSprites[i].setTexture(groundTexture); // gán texture cho sprite đất
        groundSprites[i].setPosition(i * groundWidth, groundY); // vị trí sprite đất
    }

    // Tạo chim :>>
    Texture birdTexture; // tạo texture cho chim
    if (!birdTexture.loadFromFile("chim.png")) { // tải hình ảnh chim từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    const int birdFrame=3; // số khung hình của chim trong ảnh
    int birdFrameWidth=birdTexture.getSize().x/birdFrame; // chiều rộng của mỗi khung hình chim 
    int birdFrameHeight=birdTexture.getSize().y; // chiều cao của chim
    birdTexture.setSmooth(true); // làm mịn texture chim    
    Sprite bird(birdTexture); // tạo sprite từ texture chim
    bird.setPosition(100, 300); // vị trí ban đầu của chim
    // Set frame đầu tiên (frame 0)
    bird.setTextureRect(IntRect(0, 0, birdFrameWidth, birdFrameHeight));
    bird.setOrigin(birdFrameWidth / 2, birdFrameHeight / 2); // gốc xoay ở giữa

    //animation chim :>>
    int currentFrame=0; // khung hình hiện tại
    float frameDuration=0.15f; // thời gian mỗi khung hình hiển thị
    float elapsedTime=0.0f; // thời gian đã trôi qua
    Clock animClock; // đồng hồ để đo thời gian animation

    // Khởi tạo cho di chuyển của chim
    float birdSpeed = 0.0f; // vận tốc ban đầu của chim
    float gravity = 0.2f; // trọng lực tác động lên chim
    float jumpStrength = -5.0f; // sức mạnh nhảy của chim
    float birdAngle = 0.0f;
    float rotationSpeed = 2.5f;
    
    //Khởi tạo cho di chuyển của ống 
    // Tốc độ của ống :>> 
    float pipeSpeed = 2.0f;
    // Tốc độ của đất :>>
    float groundSpeed = 2.0f; // tốc độ di chuyển của đất, bằng với tốc độ di chuyển của ống để tạo hiệu ứng liền mạch
    
    int score = 0; // điểm số ban đầu nó là 0
    Font font;
    if (!font.loadFromFile("04B_19__.TTF")) { // font để tính điểm
        return -1;
    }
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::White);
    scoreText.setOutlineColor(Color::Black);   // viền đen
    scoreText.setOutlineThickness(3);          // viền dày 3px
    FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition(bgSize.x / 2.f - scoreBounds.width / 2.f,20);
    scoreText.setString("0");

    bool scored = false; // cờ đánh dấu đã cộng điểm qua 1 ống  
    bool gameStarted = false; // trạng thái game ban đầu
    bool gameOver = false; // trạng thái game over
    
    //Tạo tên game
    Texture titleTexture;
    if (!titleTexture.loadFromFile("tengame.png")) { // load hình ảnh title
        std::cout << "Khong the load title.png\n";
        return -1;
    }
    Sprite titleSprite(titleTexture);
    titleSprite.setScale(1.f, 1.f);
    FloatRect titleBounds = titleSprite.getGlobalBounds();
    titleSprite.setPosition(bgSize.x/2 - titleBounds.width/2-20,-300);
    
    // Tạo text bắt đầu vào game
    // Text startText;
    // startText.setFont(font);
    // startText.setCharacterSize(40);
    // startText.setFillColor(Color::White);
    // startText.setOutlineColor(Color::Black);
    // startText.setOutlineThickness(3);
    // startText.setString("Press SPACE to Start");
    // startText.setPosition(bgSize.x/2 - startText.getLocalBounds().width/2, bgSize.y/2 - 50);

    Texture startTexture;
    if (!startTexture.loadFromFile("xuatphatgame.png")) { // load hình ảnh nút bắt đầu
        std::cout << "Khong the load start.png\n";
        return -1;
    }
    Sprite startSprite(startTexture); // tạo sprite từ texture nút bắt đầu 
    startSprite.setScale(1.f, 1.f); // đặt tỉ lệ cho nút bắt đầu
    FloatRect startBounds = startSprite.getGlobalBounds(); // lấy kích thước của nút bắt đầu
    startSprite.setPosition(bgSize.x/2 - startBounds.width/2, bgSize.y/2 - 150); // đặt vị trí nút bắt đầu ở giữa

    // Tạo text Game Over (thay vì load hình)
    Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("gameover.png")) { // load hình ảnh game over
        std::cout << "Khong the load gameover.png\n";
        return -1;
    }
    Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setScale(1.f, 1.f);
    FloatRect goBounds = gameOverSprite.getGlobalBounds();
    gameOverSprite.setPosition(bgSize.x/2 - goBounds.width/2+10, bgSize.y/2 - goBounds.height/2-200);

    // Load hình ảnh nút Restart
    Texture restartTexture;
    if (!restartTexture.loadFromFile("restart.png")) { // load hình ảnh nút restart
        std::cout << "Khong the load restart.png\n";
        return -1;
    }
    Sprite restartSprite(restartTexture);
    // Đặt vị trí ở giữa màn hình, phía dưới game over
    restartSprite.setPosition(bgSize.x/2 - restartTexture.getSize().x/2, bgSize.y/2 + 100);

    // Tạo text Leaderboard (thay vì load hình)
    Texture leaderTexture;
    if (!leaderTexture.loadFromFile("leader.png")) { // load hình ảnh bảng xếp hạng
        std::cout << "Khong the load leader.png\n";
        return -1;
    }
    Sprite leaderSprite(leaderTexture);
    leaderSprite.setScale(1.f, 1.f);
    FloatRect leaderBounds = leaderSprite.getGlobalBounds();
    leaderSprite.setPosition(bgSize.x/2 - leaderBounds.width/2-20, bgSize.y/2-350);

    // Thêm final score
    Text finalScoreText;
    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(40);
    finalScoreText.setFillColor(Color::White);
    finalScoreText.setOutlineColor(Color::Black);
    finalScoreText.setOutlineThickness(3);
    finalScoreText.setPosition(bgSize.x/2, bgSize.y/2 - 60);

    // Thêm best score
    int bestScore = 0;
    Text bestScoreText;
    bestScoreText.setFont(font);
    bestScoreText.setCharacterSize(30);
    bestScoreText.setFillColor(Color::Yellow);
    bestScoreText.setOutlineColor(Color::Black);
    bestScoreText.setOutlineThickness(2);
    bestScoreText.setString("0");
    bestScoreText.setPosition(bgSize.x/2+180, bgSize.y/2 - 20);

    // Medal / Rank sprit
    
    Texture medalTexture;
    Sprite medalSprite;
    medalSprite.setScale(1.f, 1.f);
 
    loadMapAssets(currentMap, backgroundTexture, pipeTexture, groundTexture, birdTexture,startTexture, mapName);
    Text mapText;
    mapText.setFont(font);
    mapText.setCharacterSize(24);
    mapText.setFillColor(Color::Yellow);
    mapText.setString(mapName);
    mapText.setPosition(10, 10);


    float idleTime = 0.0f;       // thời gian trôi qua khi chưa bắt đầu game
    float idleAmplitude = 8.0f;  // biên độ dao động (chim bay lên xuống cao bao nhiêu px)
    float idleSpeed = 2.0f;      // tốc độ dao động
    float birdStartY = 300;      // vị trí Y ban đầu của chim
    Clock idleClock;

    //////////////////////////////////////////////////////SOUND//////////////////////////////////////////////////////
    // Nhạc nền (chạy xuyên suốt game)
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("wind.mp3")) {
    std::cout << "Khong the load wind.mp3\n";
    return -1;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    // Âm thanh vỗ cánh
    SoundBuffer wingBuffer;
    if (!wingBuffer.loadFromFile("sfx_wing.wav")) {
    std::cout << "Khong the load sfx_wing.wav\n";
    return -1;
    }
    Sound wingSound;
    wingSound.setBuffer(wingBuffer);
    
    // Âm thanh va chạm ống/đất
    SoundBuffer hitBuffer;
    if (!hitBuffer.loadFromFile("sfx_hit.wav")) {
    std::cout << "Khong the load sfx_hit.wav\n";
    return -1;
    }
    Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    SoundBuffer dieBuffer;
    if (!dieBuffer.loadFromFile("sfx_die.wav")) {
    std::cout << "Khong the load sfx_die.wav\n";
    return -1;
    }
    Sound dieSound;
    dieSound.setBuffer(dieBuffer);

////////////////////////////////////////////////////////////////// END SETUP /////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////// MAIN LOOP //////////////////////////////////////////////////////////////

    while (window.isOpen()) { // vòng lặp chính của chương trình
        Event event; // biến event để lưu trữ các sự kiện xảy ra trong cửa sổ
        while (window.pollEvent(event)) { // kiểm tra các sự kiện xảy ra trong cửa sổ
            if (event.type == Event::Closed) { // nếu sự kiện là cửa sổ bị đóng
                window.close(); // đóng cửa sổ
            }
        
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) { // nếu sự kiện là phím Space được nhấn
                wingSound.play(); // phát âm thanh vỗ cánh
                if(gameOver) {
                    // Reset game khi ở màn hình game over
                    resetGame(bird, pipesTop, pipesBottom, score, scoreText, scored,
                              gameStarted, gameOver, birdSpeed, birdAngle,
                              bgSize, groundHeight, pipeWidth, pipeSpacing, khoangcach,
                              birdFrameWidth, birdFrameHeight);

                    if (score > bestScore) {
                                            bestScore = score;
                                            bestScoreText.setString(std::to_string(bestScore));
                                            }
                } else if(!gameStarted) {
                    gameStarted = true; // bắt đầu trò chơi
                    birdSpeed = jumpStrength; // đặt vận tốc của chim bằng sức mạnh nhảy
                    birdAngle = -25.0f; // nghiêng chim lên khi nhảy
                } else {
                    birdSpeed = jumpStrength; // đặt vận tốc của chim bằng sức mạnh nhảy
                    birdAngle = -25.0f; // nghiêng chim lên khi nhảy
                }
            }
             if (!gameStarted && !gameOver) {
                if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Num1) currentMap = DEFAULT;
                if (event.key.code == Keyboard::Num2) currentMap = XMAS;
                if (event.key.code == Keyboard::Num3) currentMap = OCEAN;
                if (event.key.code == Keyboard::Num4) currentMap = NIGHT;
                if (event.key.code == Keyboard::Num5) currentMap = MINECRAFT;
                if (event.key.code == Keyboard::Num6) currentMap = HALLOWEEN;
                // Sau khi đổi map, load lại asset
                loadMapAssets(currentMap, backgroundTexture, pipeTexture, groundTexture, birdTexture, startTexture, mapName);
                // Cập nhật lại sprite các đối tượng nếu cần
                background.setTexture(backgroundTexture);
                startSprite.setTexture(startTexture, true);
                // Nếu muốn căn lại vị trí, hãy cập nhật lại vị trí startSprite nếu cần
                FloatRect startBounds = startSprite.getGlobalBounds();
                startSprite.setPosition(bgSize.x/2 - startBounds.width/2, bgSize.y/2 - 150);
                // ... cập nhật lại các sprite khác tương tự
                }
            }
            
            // Xử lý click chuột cho nút restart
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (gameOver && restartSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    resetGame(bird, pipesTop, pipesBottom, score, scoreText, scored,
                    gameStarted, gameOver, birdSpeed, birdAngle,
                    bgSize, groundHeight, pipeWidth, pipeSpacing, khoangcach,
                    birdFrameWidth, birdFrameHeight);
                }
            }
        }

        if (!gameOver) {
            // === DI CHUYỂN ĐẤT (LUÔN CHẠY) ===
            for (int i = 0; i < numGround; ++i) {
                groundSprites[i].move(-groundSpeed, 0);
                if (groundSprites[i].getPosition().x <= -groundWidth) {
                    float maxX = 0;
                    for (int j = 0; j < numGround; ++j)
                        if (groundSprites[j].getPosition().x > maxX)
                            maxX = groundSprites[j].getPosition().x;
                    groundSprites[i].setPosition(maxX + groundWidth - 3, groundY);
                }
            }

            // === ANIMATION CHIM (LUÔN CHẠY) ===
            elapsedTime += animClock.restart().asSeconds();
            if (elapsedTime >= frameDuration) {
                elapsedTime = 0.0f;
                currentFrame = (currentFrame + 1) % birdFrame;
                bird.setTextureRect(IntRect(currentFrame * birdFrameWidth, 0,
                                            birdFrameWidth, birdFrameHeight));
            }

            if (!gameStarted) {
                // === LOGIC MÀN HÌNH CHỜ ===
                idleTime += idleClock.restart().asSeconds();
                float offset = sin(idleTime * idleSpeed) * idleAmplitude;
                bird.setPosition(bird.getPosition().x, birdStartY + offset);
            } else {
                // === LOGIC GAME KHI ĐÃ BẮT ĐẦU ===

                // Chim chịu gravity
                birdSpeed += gravity;
                bird.move(0, birdSpeed);

                if (birdSpeed > 0 && birdAngle < 90.f) {
                    birdAngle += rotationSpeed;
                }
                bird.setRotation(birdAngle);

                // Giới hạn va chạm đất/trần
                if (bird.getPosition().y < 0) {
                    bird.setPosition(bird.getPosition().x, 0);
                }
                if (bird.getPosition().y >= bgSize.y - groundHeight - birdFrameHeight) {
                    bird.setPosition(bird.getPosition().x,
                                     bgSize.y - groundHeight - birdFrameHeight);
                    //std::cout << "Game Over! Chim da cham dat.\n";
                    hitSound.play();   // va chạm
                    dieSound.play();   // sau đó chết
                    gameOver = true;
                    finalScoreText.setString(to_string(score));
                    finalScoreText.setPosition(bgSize.x/2 - finalScoreText.getLocalBounds().width/2+190, bgSize.y/2 - 110);
                    if (score > bestScore) {
                                            bestScore = score;
                                            }
                    bestScoreText.setString(std::to_string(bestScore));
                    // Cập nhật huy chương nếu có
                    updateMedal(score, medalTexture, medalSprite,bgSize);
                }

                // Va chạm ống
                // if (bird.getGlobalBounds().intersects(pipeTop.getGlobalBounds()) ||
                //     bird.getGlobalBounds().intersects(pipeBottom.getGlobalBounds())) {
                //     //std::cout << "Game Over! Chim da cham ong.\n";
                //     gameOver = true;
                //     
                //     
                // }

                for (int i = 0; i < numPipes; i++) {
                    if (bird.getGlobalBounds().intersects(pipesTop[i].getGlobalBounds()) ||
                    bird.getGlobalBounds().intersects(pipesBottom[i].getGlobalBounds())) {
                        hitSound.play();
                        dieSound.play();
                    // xử lý thua game
                    gameOver = true;
                    finalScoreText.setString(to_string(score));
                    finalScoreText.setPosition(bgSize.x/2 - finalScoreText.getLocalBounds().width/2+190, bgSize.y/2 - 110);
                    if (score > bestScore) {
                                            bestScore = score;
                                            }
                    bestScoreText.setString(std::to_string(bestScore));
                    // Cập nhật huy chương nếu có
                    updateMedal(score, medalTexture, medalSprite,bgSize);
                    }
                }

        //          for (int i = 0; i < numPipes; i++) {
        //     // Di chuyển
        //     pipesTop[i].move(-pipeSpeed, 0);
        //     pipesBottom[i].move(-pipeSpeed, 0);

        //     // Reset khi ống ra khỏi màn hình
        //     if (pipesTop[i].getPosition().x < -pipeWidth) {
        //     int minGapY = 100;
        //     int maxGapY = bgSize.y - groundHeight - khoangcach - 100;
        //     int newGapY = minGapY + rand() % (maxGapY - minGapY + 1);

        //     // Tìm ống xa nhất bên phải
        //     float maxX = 0;
        //     for (int j = 0; j < numPipes; j++) {
        //     if (pipesTop[j].getPosition().x > maxX) {
        //         maxX = pipesTop[j].getPosition().x;
        //         }
        //     }

        //     pipesTop[i].setPosition(maxX + pipeSpacing, newGapY);
        //     pipesBottom[i].setPosition(maxX + pipeSpacing, newGapY + khoangcach);
             
        //     // reset cờ điểm cho ống vừa được tái sử dụng
        //     pipeScored[i] = false;
        //     }

        // // Vẽ ống
        // //window.draw(pipesTop[i]);
        // //window.draw(pipesBottom[i]);
        // }
        // for (int i = 0; i < numPipes; i++) {
        // float pipeX = pipesTop[i].getPosition().x + pipeWidth / 2;

        // if (bird.getPosition().x > pipeX && !pipeScored[i]) {
        // score++;
        // pipeScored[i] = true;
        // scoreText.setString(std::to_string(score));
        // }

        //     // if (pipesTop[i].getPosition().x < -pipeWidth) {
        //     // pipeScored[i] = false; // reset lại khi ống ra khỏi màn hình
        //     // }
        // }
        if (gameStarted && !gameOver) {
            updatePipes(pipesTop, pipesBottom, pipeScored,
                        bird, score, scoreText,
                        numPipes, pipeWidth, pipeSpacing,
                        khoangcach, groundHeight, pipeSpeed,
                        bgSize, window);
                    }


                // // Di chuyển ống
                // pipeTop.move(-pipeSpeed, 0);
                // pipeBottom.move(-pipeSpeed, 0);

                // // Reset ống khi ra khỏi màn hình
                // if (pipeTop.getPosition().x < -pipeWidth) {
                //     int minGapY = 100;
                //     int maxGapY = bgSize.y - groundHeight - khoangcach - 100;
                //     int newGapY = minGapY + rand() % (maxGapY - minGapY + 1);

                //     pipeTop.setPosition(bgSize.x + pipeWidth / 2, newGapY);
                //     pipeBottom.setPosition(bgSize.x + pipeWidth / 2, newGapY + khoangcach);
                //     scored = false;
                // }

                // Cộng điểm
                // if (!scored && bird.getPosition().x > pipeTop.getPosition().x + pipeWidth / 2) {
                //     score++;
                //     scored = true;
                //     scoreText.setString(std::to_string(score));
                //     //std::cout << "Score: " << score << "\n";
                // }
                // for (int i = 0; i < numPipes; i++) {
                //     float pipeX = pipesTop[i].getPosition().x + pipeWidth / 2;

                //     // Nếu chim vừa bay qua ống và chưa được tính điểm
                //     if (bird.getPosition().x > pipeX && !scored) {
                //     score++;
                //     scored = true;
                //     scoreText.setString(std::to_string(score));
                //     }

                //     // Reset cờ "scored" khi ống vượt khỏi màn hình
                //     if (pipesTop[i].getPosition().x < -pipeWidth) {
                //         scored = false;
                //         }
                //     }
            }
        }

        // === VẼ MỌI THỨ ===
        window.clear(Color::Cyan); // vẽ nền xanh cho cửa sổ
        window.draw(background); // vẽ nền
        
        if (gameStarted && !gameOver) {
            // Khi game đã bắt đầu và chưa game over - vẽ ống
            //window.draw(pipeTop); // vẽ ống trên
            //window.draw(pipeBottom); // vẽ ống dưới
            for (int i = 0; i < numPipes; i++) {
            window.draw(pipesTop[i]);
            window.draw(pipesBottom[i]);
            }
        }
        //  for (int i = 0; i < numPipes; i++) {
        // window.draw(pipesTop[i]);
        // window.draw(pipesBottom[i]);
        //}
        // Luôn vẽ đất và chim (trừ khi game over)
        for (int i = 0; i < numGround; ++i)
            window.draw(groundSprites[i]); // vẽ đất
        //window.draw(bird);   
        if (gameOver) {
            // Vẽ màn hình Game Over với tất cả hình ảnh
            window.draw(gameOverSprite);  // Hình "Game Over"
            window.draw(leaderSprite);    // Hình bảng xếp hạng
            window.draw(finalScoreText);  // Điểm hiện tại
            window.draw(bestScoreText);   // Điểm cao nhất
            window.draw(restartSprite);   // Nút restart
            if (score >= 10) window.draw(medalSprite); // Vẽ huy chương nếu điểm >= 10
        } else if (gameStarted) {
            window.draw(scoreText); // vẽ điểm số khi game đã bắt đầu
        } else {
             for (int i = 0; i < numMaps; ++i) {
             Text menuText;
             menuText.setFont(font);
             menuText.setCharacterSize(32);
             menuText.setFillColor(i == currentMap ? Color::Yellow : Color::White);
             menuText.setString(to_string(i+1) + ". " + mapNames[i]);
             menuText.setPosition(40, 60 + i * 40);
             window.draw(menuText);
            }   
            Text guideText;
            guideText.setFont(font);
            guideText.setCharacterSize(20);
            guideText.setFillColor(Color::Cyan);
            guideText.setString("Nhan phim 1-6 de chon map");
            guideText.setPosition(40, 60 + numMaps * 40 + 10);
            window.draw(guideText);
            window.draw(titleSprite); // vẽ tên game khi chưa bắt đầu game
            window.draw(startSprite); // vẽ nút bắt đầu khi chưa bắt đầu game
        }
        
        if (!gameOver) {
            window.draw(bird); // vẽ chim (trừ khi game over)
        }
        window.display(); // hiển thị cửa sổ
    }
    
    /////////////////////////////////////////////////////////////////// END MAIN LOOP /////////////////////////////////////////////////////////////
    return 0; // trả về 0 để kết thúc chương trình
}