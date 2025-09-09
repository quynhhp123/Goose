#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
using namespace sf;

int main() {
    srand(time(nullptr)); // Khởi tạo random seed
    
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
    
    // Tạo ống :>>
    Texture pipeTexture; // tạo texture cho ống
    if (!pipeTexture.loadFromFile("ong.png")) { // tải hình ảnh ống từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    // Khởi tạo các biến cho ống
    int groundHeight = 100; // chiều cao của đất
    int pipeX = 400; // vị trí x của ống
    int gapY = 300; // vị trí y của khoảng cách giữa ống trên và ống dưới
    int khoangcach=300; // khoảng cách giữa ống trên và ống dưới là 500
    int pipeHeight = pipeTexture.getSize().y; // chiều cao của ống từ texture
    int pipeWidth = pipeTexture.getSize().x; // chiều rộng của ống từ texture
    
    // Tạo ống trên :>>
    Sprite pipeTop(pipeTexture); // tạo sprite từ texture ống
    pipeTop.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh  
    pipeTop.setRotation(180); // xoay ống trên 180 độ để hiển thị đúng
    pipeTop.setPosition(pipeX, gapY); // vị trí ống trên

    // Tạo ống dưới :>>
    Sprite pipeBottom(pipeTexture); // tạo sprite từ texture ống
    pipeBottom.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh dưới
    pipeBottom.setPosition(pipeX, gapY + khoangcach); // vị trí ống dưới, cách ống trên một khoảng cách nhất định

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
    float gravity = 0.25f; // trọng lực tác động lên chim
    float jumpStrength = -6.5f; // sức mạnh nhảy của chim
    float birdAngle = 0.0f;
    float rotationSpeed = 2.5f;
    
    //Khởi tạo cho di chuyển của ống 
    // Tốc độ của ống :>> 
    float pipeSpeed = 2.0f;
    // Tốc độ của đất :>>
    float groundSpeed = 2.0f; // tốc độ di chuyển của đất, bằng với tốc độ di chuyển của ống để tạo hiệu ứng liền mạch
    
    int score = 0;
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
    scoreText.setPosition(20, 20);
    scoreText.setString("0");

    bool scored = false; // cờ đánh dấu đã cộng điểm qua 1 ống  
    bool gameStarted = false; // trạng thái game ban đầu

    Text startText;
    startText.setFont(font);
    startText.setCharacterSize(40);
    startText.setFillColor(Color::White);
    startText.setOutlineColor(Color::Black);
    startText.setOutlineThickness(3);
    startText.setString("Press SPACE to Start");
    startText.setPosition(bgSize.x/2 - startText.getLocalBounds().width/2, bgSize.y/2 - 50);

    float idleTime = 0.0f;       // thời gian trôi qua khi chưa bắt đầu game
    float idleAmplitude = 8.0f;  // biên độ dao động (chim bay lên xuống cao bao nhiêu px)
    float idleSpeed = 2.0f;      // tốc độ dao động
    float birdStartY = 300;      // vị trí Y ban đầu của chim
    Clock idleClock;

////////////////////////////////////////////////////////////////// END SETUP /////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////// MAIN LOOP //////////////////////////////////////////////////////////////

    while (window.isOpen()) { // vòng lặp chính của chương trình
        Event event; // biến event để lưu trữ các sự kiện xảy ra trong cửa sổ
        while (window.pollEvent(event)) { // kiểm tra các sự kiện xảy ra trong cửa sổ
            if (event.type == Event::Closed) { // nếu sự kiện là cửa sổ bị đóng
                window.close(); // đóng cửa sổ
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) { // nếu sự kiện là phím Space được nhấn
                if(!gameStarted){
                    gameStarted = true; // bắt đầu trò chơi
                }
                birdSpeed = jumpStrength; // đặt vận tốc của chim bằng sức mạnh nhảy
                birdAngle = -25.0f; // nghiêng chim lên khi nhảy
            }
        }

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
                std::cout << "Game Over! Chim da cham dat.\n";
                window.close();
            }

            // Va chạm ống
            if (bird.getGlobalBounds().intersects(pipeTop.getGlobalBounds()) ||
                bird.getGlobalBounds().intersects(pipeBottom.getGlobalBounds())) {
                std::cout << "Game Over! Chim da cham ong.\n";
                window.close();
            }

            // Di chuyển ống
            pipeTop.move(-pipeSpeed, 0);
            pipeBottom.move(-pipeSpeed, 0);

            // Reset ống khi ra khỏi màn hình
            if (pipeTop.getPosition().x < -pipeWidth) {
                int minGapY = 100;
                int maxGapY = bgSize.y - groundHeight - khoangcach - 100;
                int newGapY = minGapY + rand() % (maxGapY - minGapY + 1);

                pipeTop.setPosition(bgSize.x + pipeWidth / 2, newGapY);
                pipeBottom.setPosition(bgSize.x + pipeWidth / 2, newGapY + khoangcach);
                scored = false;
            }

            // Cộng điểm
            if (!scored && bird.getPosition().x > pipeTop.getPosition().x + pipeWidth / 2) {
                score++;
                scored = true;
                scoreText.setString(std::to_string(score));
                std::cout << "Score: " << score << "\n";
            }
        }

        // === VẼ MỌI THỨ ===
        window.clear(Color::Cyan); // vẽ nền xanh cho cửa sổ
        window.draw(background); // vẽ nền
        
        if (gameStarted) {
            // Khi game đã bắt đầu - vẽ ống
            window.draw(pipeTop); // vẽ ống trên
            window.draw(pipeBottom); // vẽ ống dưới
        }
        
        // Luôn vẽ đất và chim
        for (int i = 0; i < numGround; ++i)
            window.draw(groundSprites[i]); // vẽ đất
            
        if (gameStarted) {
            window.draw(scoreText); // vẽ điểm số khi game đã bắt đầu
        } else {
            window.draw(startText); // vẽ text bắt đầu khi chưa bắt đầu game
        }
        
        window.draw(bird); // vẽ chim (luôn vẽ cuối để hiện lên trên)
        window.display(); // hiển thị cửa sổ
    }
    
    /////////////////////////////////////////////////////////////////// END MAIN LOOP /////////////////////////////////////////////////////////////
    return 0; // trả về 0 để kết thúc chương trình
}