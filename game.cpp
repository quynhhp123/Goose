#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
using namespace sf;  // sử dụng thư viện SFML
using namespace std; // sử dụng thư viện chuẩn
const int numPipes = 3;       // số lượng cặp ống
const int pipeSpacing = 400;  // khoảng cách ngang giữa các ống
vector<bool> pipeScored(numPipes, false); // mảng cờ đánh dấu đã cộng điểm qua ống
const string mapNames[] = { "CLASSIC", "XMAS", "OCEAN", "NIGHT", "MINECRAFT", "HALLOWEEN" }; // tên các bản đồ
const int numMaps = 6; // số lượng bản đồ
enum MapType { DEFAULT, XMAS, OCEAN, NIGHT, MINECRAFT, HALLOWEEN };  // kiểu liệt kê cho các bản đồ
MapType currentMap = DEFAULT; // bản đồ hiện tại
void loadMapAssets( // Hàm tải tài nguyên theo bản đồ
    MapType map,  // loại bản đồ
    Texture& backgroundTexture, Texture& pipeTexture, Texture& groundTexture, Texture& birdTexture,Texture& startTexture, // tham chiếu đến các texture để tải
    string& mapName // tham chiếu đến tên bản đồ để cập nhật
) {
    string folder, bg, pipe, ground, bird,start; // các biến để lưu tên thư mục và tên file hình ảnh
    switch (map) { // chọn theo loại bản đồ
        case XMAS: // nếu là bản đồ Giáng Sinh
            folder = "xmas/"; // thư mục chứa tài nguyên bản đồ Giáng Sinh
            mapName = "XMAS"; // cập nhật tên bản đồ
            bg = "background-xmas.png"; // tên file nền
            pipe = "pipe-xmas.png"; // tên file ống 
            ground = "ground-xmas.png"; // tên file đất
            bird = "bird-xmas.png"; // tên file chim
            start = "message-xmas.png"; // tên file thông điệp bắt đầu
            break; // kết thúc case
        case OCEAN: // nếu là bản đồ Đại Dương
            folder = "ocean/"; // thư mục chứa tài nguyên bản đồ Đại Dương
            mapName = "OCEAN"; // cập nhật tên bản đồ
            bg = "background-fish.png"; // tên file nền
            pipe = "pipe-fish.png"; // tên file ống
            ground = "ground-fish.png"; // tên file đất
            bird = "bird-fish.png"; // tên file chim
            start = "message-fish.png"; // tên file thông điệp bắt đầu
            break; // kết thúc case
        case NIGHT: // nếu là bản đồ Ban Đêm
            folder = "night/"; // thư mục chứa tài nguyên bản đồ Ban Đêm
            mapName = "NIGHT"; // cập nhật tên bản đồ
            bg = "background-night.png"; // tên file nền
            pipe = "pipe-night.png"; // tên file ống  
            ground = "ground-night.png"; // tên file đất
            bird = "bird-night.png"; // tên file chim
            start = "message-night.png"; // tên file thông điệp bắt đầu
            break; // kết thúc case
        case MINECRAFT: // nếu là bản đồ Minecraft
            folder = "minecraft/"; // thư mục chứa tài nguyên bản đồ Minecraft
            mapName = "MINECRAFT"; // cập nhật tên bản đồ
            bg = "background-craft.png"; // tên file nền
            pipe = "pipe-craft.png"; // tên file ống
            ground = "ground-craft.png"; // tên file đất
            bird = "bird-craft.png"; // tên file chim
            start = "message-craft.png"; // tên file thông điệp bắt đầu
            break; // kết thúc case
        case HALLOWEEN: // nếu là bản đồ Halloween
            folder = "halloween/"; // thư mục chứa tài nguyên bản đồ Halloween
            mapName = "HALLOWEEN"; // cập nhật tên bản đồ
            bg = "background-halloween.png"; // tên file nền
            pipe = "pipe-halloween.png"; // tên file ống
            ground = "ground-halloween.png"; // tên file đất
            bird = "bird-halloween.png"; // tên file chim
            start = "message-halloween.png"; // tên file thông điệp bắt đầu
            break; // kết thúc case
        default: // nếu không phải các bản đồ trên, mặc định là bản đồ CLASSIC
            folder = ""; // thư mục gốc
            mapName = "CLASSIC"; // cập nhật tên bản đồ
            bg = "nen.png"; // tên file nền
            pipe = "ong.png"; // tên file ống
            ground = "dat.png"; // tên file đất
            bird = "chim.png"; // tên file chim
            start = "xuatphatgame.png"; // tên file thông điệp bắt đầu
    } // kết thúc switch
    backgroundTexture.loadFromFile(folder + bg); // tải hình ảnh nền từ file
    pipeTexture.loadFromFile(folder + pipe); // tải hình ảnh ống từ file
    groundTexture.loadFromFile(folder + ground); // tải hình ảnh đất từ file
    birdTexture.loadFromFile(folder + bird); // tải hình ảnh chim từ file
    startTexture.loadFromFile(folder + start); // tải hình ảnh thông điệp bắt đầu từ file
} // kết thúc hàm loadMapAssets
void resetGame(Sprite& bird, // Hàm reset game về trạng thái ban đầu
               vector<Sprite>& pipesTop, vector<Sprite>& pipesBottom,
               int& score, Text& scoreText,
               bool& scored, bool& gameStarted, bool& gameOver,
               float& birdSpeed, float& birdAngle,
               Vector2u bgSize, int groundHeight,
               int pipeWidth, int pipeSpacing, int khoangcach,
               int birdFrameWidth, int birdFrameHeight) 
{
    // Reset bird
    bird.setPosition(100, 300); // vị trí ban đầu của chim
    bird.setTextureRect(IntRect(0, 0, birdFrameWidth, birdFrameHeight)); // Set frame đầu tiên (frame 0)
    birdSpeed = 0.0f; // vận tốc ban đầu của chim
    birdAngle = 0.0f; // góc nghiêng ban đầu của chim
    bird.setRotation(0); // đặt góc quay về 0

    for (int i = 0; i < numPipes; i++) { // reset cờ điểm cho tất cả ống
    pipeScored[i] = false; // reset lại cờ tính điểm
    }
    // Reset pipes
    for (int i = 0; i < (int)pipesTop.size(); i++) { // đặt lại vị trí các ống
        int minGapY = 100; // khoảng cách tối thiểu từ đỉnh cửa sổ đến khoảng cách giữa ống trên và ống dưới
        int maxGapY = bgSize.y - groundHeight - khoangcach - 100; // khoảng cách tối đa từ đỉnh cửa sổ đến khoảng cách giữa ống trên và ống dưới
        int gapY = minGapY + rand() % (maxGapY - minGapY + 1); // vị trí y ngẫu nhiên của khoảng cách giữa ống trên và ống dưới

        pipesTop[i].setPosition(bgSize.x + i * pipeSpacing, gapY); // vị trí ống trên
        pipesBottom[i].setPosition(bgSize.x + i * pipeSpacing, gapY + khoangcach); // vị trí ống dưới, cách ống trên một khoảng cách nhất định
    }
    // Reset score & state
    score = 0; // điểm số ban đầu nó là 0
    scoreText.setString("0"); // cập nhật lại text điểm số
    scored = false; // cờ đánh dấu đã cộng điểm qua 1 ống
    gameStarted = false; // trạng thái game chưa bắt đầu
    gameOver = false; // trạng thái game chưa kết thúc
} // kết thúc hàm resetGame


// Hàm cập nhật vị trí và trạng thái của các ống
void updatePipes( // Hàm cập nhật vị trí và trạng thái của các ống
    vector<Sprite> &pipesTop, // tham chiếu đến mảng sprite ống trên
    vector<Sprite> &pipesBottom, // tham chiếu đến mảng sprite ống dưới
    vector<bool> &pipeScored, // tham chiếu đến mảng cờ đánh dấu đã cộng điểm qua ống
    Sprite &bird,
    int &score,
    Text &scoreText,
    int numPipes,
    int pipeWidth,
    int pipeSpacing,
    int khoangcach,
    int groundHeight,
    float pipeSpeed,
    Vector2u bgSize, // kích thước nền
    RenderWindow &window // tham chiếu đến cửa sổ để vẽ các ống
) {
    for (int i = 0; i < numPipes; i++) { // lặp qua từng cặp ống
        // Di chuyển ống
        pipesTop[i].move(-pipeSpeed, 0); // di chuyển ống lên bên trái
        pipesBottom[i].move(-pipeSpeed, 0); // di chuyển ống xuống bên trái

        // Reset khi ống ra khỏi màn hình
        if (pipesTop[i].getPosition().x < -pipeWidth) { // nếu ống đã đi hết màn hình bên trái 
            int minGapY = 100; // khoảng cách tối thiểu từ đỉnh cửa sổ đến khoảng cách giữa ống trên và ống dưới
            int maxGapY = bgSize.y - groundHeight - khoangcach - 100; // khoảng cách tối đa từ đỉnh cửa sổ đến khoảng cách giữa ống trên và ống dưới
            int newGapY = minGapY + rand() % (maxGapY - minGapY + 1); // vị trí y ngẫu nhiên của khoảng cách giữa ống trên và ống dưới

            // tìm ống xa nhất bên phải
            float maxX = 0; // vị trí x lớn nhất hiện tại
            for (int j = 0; j < numPipes; j++) { // lặp qua từng ống
                if (pipesTop[j].getPosition().x > maxX) { // nếu vị trí x của ống hiện tại lớn hơn vị trí x lớn nhất
                    maxX = pipesTop[j].getPosition().x; // cập nhật vị trí x lớn nhất
                } // kết thúc if
            } // kết thúc for

            pipesTop[i].setPosition(maxX + pipeSpacing, newGapY); // đặt lại vị trí ống trên, cách ống xa nhất bên phải một khoảng pipeSpacing
            pipesBottom[i].setPosition(maxX + pipeSpacing, newGapY + khoangcach); // đặt lại vị trí ống dưới, cách ống trên một khoảng khoangcach

            pipeScored[i] = false; // reset lại cờ tính điểm
        }

        // Tính điểm
        float pipeX = pipesTop[i].getPosition().x + pipeWidth / 2; // vị trí x của ống (gốc giữa cạnh)
        if (bird.getPosition().x > pipeX && !pipeScored[i]) { // nếu chim đã vượt qua ống và chưa cộng điểm cho ống này
            score++; // tăng điểm
            pipeScored[i] = true; // đánh dấu đã cộng điểm cho ống này
            scoreText.setString(std::to_string(score)); // cập nhật lại text điểm số
        } // kết thúc if

        // Vẽ ống
        window.draw(pipesTop[i]); // vẽ ống trên
        window.draw(pipesBottom[i]); // vẽ ống dưới
    }
}



// hàm cập nhật huy chương dựa trên điểm số
void updateMedal(int score, Texture &medalTexture, Sprite &medalSprite, Vector2u bgSize) {
    string medalFile = ""; // biến để lưu tên file hình huy chương

    if (score >= 300) medalFile = "thachdau.png"; // nếu điểm số >= 300, đặt tên file huy chương là "thachdau.png"
    else if (score >= 200) medalFile = "dct.png"; // nếu điểm số >= 200, đặt tên file huy chương là "dct.png"
    else if (score >= 100) medalFile = "caothu.png"; // nếu điểm số >= 100, đặt tên file huy chương là "caothu.png"
    else if (score >= 70) medalFile = "rankkc.png"; // nếu điểm số >= 70, đặt tên file huy chương là "rankkc.png"
    else if (score >= 60) medalFile = "ranklb.png"; // nếu điểm số >= 60, đặt tên file huy chương là "ranklb.png"
    else if (score >= 50) medalFile = "rankbk.png"; // nếu điểm số >= 50, đặt tên file huy chương là "rankbk.png"
    else if (score >= 40) medalFile = "rankvang.png"; // nếu điểm số >= 40, đặt tên file huy chương là "rankvang.png"
    else if (score >= 30) medalFile = "rankbac.png"; // nếu điểm số >= 30, đặt tên file huy chương là "rankbac.png"
    else if (score >= 20) medalFile = "rankdong.png"; // nếu điểm số >= 20, đặt tên file huy chương là "rankdong.png"
    else if (score >= 10) medalFile = "ranksat.png"; // nếu điểm số >= 10, đặt tên file huy chương là "ranksat.png"
    else medalFile = ""; // nếu điểm số < 10, không có huy chương

    if (medalFile != "" && medalTexture.loadFromFile(medalFile)) { // nếu có tên file huy chương và tải được hình ảnh từ file
        medalSprite.setTexture(medalTexture); // đặt texture cho sprite huy chương
        medalSprite.setPosition(bgSize.x/2 - 220, bgSize.y/2 - 120 ); // chỉnh toạ độ cho đẹp (bên trái finalScoreText)
    }
}



int main() {
    srand(time(nullptr)); // Khởi tạo random seed
    string mapName = "CLASSIC"; // tên bản đồ hiện tại
///////////////////////////////////////////////////////////////////SETUP/////////////////////////////////////////////////////////////////
    // Tạo nền :>>
    Texture backgroundTexture; // tạo texture cho nền
    if(!backgroundTexture.loadFromFile("nen.png")) { // tải hình ảnh nền từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }

    // Tạo cửa sổ :>>
    // Lấy kích thước ảnh nền để khớp với kích thước cửa sổ
    Vector2u bgSize = backgroundTexture.getSize(); // kích thước nền
    RenderWindow window(VideoMode(bgSize.x,bgSize.y),"Flappy Bird"); // tạo khung cửa sổ và tiêu đề bên cửa sổ
    window.setFramerateLimit(120); // giới hạn số khung hình trên giây

    Sprite background (backgroundTexture); // tạo sprite từ texture nền
    
    Image icon; // tạo biến icon
    if (!icon.loadFromFile("Flappy_Bird_icon.png")) { // tải hình ảnh icon từ file
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()); // đặt icon cho cửa sổ

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


    vector<Sprite> pipesTop;  // mảng sprite ống trên
    vector<Sprite> pipesBottom; // mảng sprite ống dưới

    for (int i = 0; i < numPipes; i++) { // khởi tạo các cặp ống
        Sprite top(pipeTexture); // tạo sprite từ texture ống
        Sprite bottom(pipeTexture); // tạo sprite từ texture ống

        top.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh trên
        top.setRotation(180); // xoay ống trên 180 độ để hiển thị đúng

        bottom.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh dưới

        int minGapY = 100; // khoảng cách tối thiểu từ đỉnh cửa sổ đến khoảng cách giữa ống trên và ống dưới
        int maxGapY = bgSize.y - groundHeight - khoangcach - 100; // khoảng cách tối đa từ đỉnh cửa sổ đến khoảng cách giữa ống trên và ống dưới
        int gapY = minGapY + rand() % (maxGapY - minGapY + 1); // vị trí y ngẫu nhiên của khoảng cách giữa ống trên và ống dưới

        // Mỗi ống cách nhau pipeSpacing
        top.setPosition(bgSize.x + i * pipeSpacing+200, gapY); // vị trí ống trên
        bottom.setPosition(bgSize.x + i * pipeSpacing+200, gapY + khoangcach); // vị trí ống dưới, cách ống trên một khoảng cách nhất định

        pipesTop.push_back(top); // thêm ống trên vào mảng
        pipesBottom.push_back(bottom); // thêm ống dưới vào mảng
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
    bird.setTextureRect(IntRect(0, 0, birdFrameWidth, birdFrameHeight)); // cắt lấy khung hình đầu tiên của chim
    bird.setOrigin(birdFrameWidth / 2, birdFrameHeight / 2); // gốc xoay ở giữa

    //animation chim :>>
    int currentFrame=0; // khung hình hiện tại
    float frameDuration=0.15f; // thời gian mỗi khung hình hiển thị
    float elapsedTime=0.0f; // thời gian đã trôi qua
    Clock animClock; // đồng hồ để đo thời gian animation

    // Khởi tạo cho di chuyển của chim
    float birdSpeed = 0.0f; // vận tốc ban đầu của chim
    float gravity = 0.25f; // trọng lực tác động lên chim
    float jumpStrength = -9.0f; // sức mạnh nhảy của chim
    float birdAngle = 0.0f;
    float rotationSpeed = 2.5f;
    
    //Khởi tạo cho di chuyển của ống 
    // Tốc độ của ống :>> 
    float pipeSpeed = 2.0f;
    // Tốc độ của đất :>>
    float groundSpeed = 2.0f; // tốc độ di chuyển của đất, bằng với tốc độ di chuyển của ống để tạo hiệu ứng liền mạch
    
    int score = 0; // điểm số ban đầu nó là 0
    Font font; // tạo font để hiển thị text
    if (!font.loadFromFile("04B_19__.TTF")) { // font để tính điểm
        return -1;
    }
    Text scoreText; // tạo text để hiển thị điểm số
    scoreText.setFont(font); // gán font cho text
    scoreText.setCharacterSize(50); // kích thước chữ
    scoreText.setFillColor(Color::White); // màu chữ trắng
    scoreText.setOutlineColor(Color::Black);   // viền đen
    scoreText.setOutlineThickness(3);          // viền dày 3px
    FloatRect scoreBounds = scoreText.getLocalBounds(); // lấy kích thước của text
    scoreText.setPosition(bgSize.x / 2.f - scoreBounds.width / 2.f,20); // đặt vị trí ở giữa phía trên cửa sổ
    scoreText.setString("0"); // khởi tạo điểm số là 0

    bool scored = false; // cờ đánh dấu đã cộng điểm qua 1 ống  
    bool gameStarted = false; // trạng thái game ban đầu
    bool gameOver = false; // trạng thái game over
    
    //Tạo tên game
    Texture titleTexture; // tạo texture cho title
    if (!titleTexture.loadFromFile("tengame.png")) { // load hình ảnh title
        std::cout << "Khong the load title.png\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
        return -1;
    }
    Sprite titleSprite(titleTexture); // tạo sprite từ texture title
    titleSprite.setScale(1.f, 1.f); // đặt tỉ lệ cho title
    FloatRect titleBounds = titleSprite.getGlobalBounds(); // lấy kích thước của title
    titleSprite.setPosition(bgSize.x/2 - titleBounds.width/2-20,-300); // đặt vị trí title ở giữa, hơi lên trên
    
    // Tạo text bắt đầu vào game
    // Text startText;
    // startText.setFont(font);
    // startText.setCharacterSize(40);
    // startText.setFillColor(Color::White);
    // startText.setOutlineColor(Color::Black);
    // startText.setOutlineThickness(3);
    // startText.setString("Press SPACE to Start");
    // startText.setPosition(bgSize.x/2 - startText.getLocalBounds().width/2, bgSize.y/2 - 50);

    Texture startTexture; // tạo texture cho nút bắt đầu
    if (!startTexture.loadFromFile("xuatphatgame.png")) { // load hình ảnh nút bắt đầu
        std::cout << "Khong the load start.png\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
        return -1;
    }
    Sprite startSprite(startTexture); // tạo sprite từ texture nút bắt đầu 
    startSprite.setScale(1.f, 1.f); // đặt tỉ lệ cho nút bắt đầu
    FloatRect startBounds = startSprite.getGlobalBounds(); // lấy kích thước của nút bắt đầu
    startSprite.setPosition(bgSize.x/2 - startBounds.width/2, bgSize.y/2 - 150); // đặt vị trí nút bắt đầu ở giữa

    // Tạo text Game Over (thay vì load hình)
    Texture gameOverTexture; // tạo texture cho game over
    if (!gameOverTexture.loadFromFile("gameover.png")) { // load hình ảnh game over
        std::cout << "Khong the load gameover.png\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
        return -1;
    }
    Sprite gameOverSprite(gameOverTexture); // tạo sprite từ texture game over
    gameOverSprite.setScale(1.f, 1.f); // đặt tỉ lệ cho game over
    FloatRect goBounds = gameOverSprite.getGlobalBounds(); // lấy kích thước của game over
    gameOverSprite.setPosition(bgSize.x/2 - goBounds.width/2+10, bgSize.y/2 - goBounds.height/2-200); // đặt vị trí game over ở giữa, hơi lên trên 

    // Load hình ảnh nút Restart
    Texture restartTexture; // tạo texture cho nút restart
    if (!restartTexture.loadFromFile("restart.png")) { // load hình ảnh nút restart
        std::cout << "Khong the load restart.png\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
        return -1;
    }
    Sprite restartSprite(restartTexture); // tạo sprite từ texture nút restart
    // Đặt vị trí ở giữa màn hình, phía dưới game over
    restartSprite.setPosition(bgSize.x/2 - restartTexture.getSize().x/2, bgSize.y/2 + 100); // đặt vị trí nút restart ở giữa, phía dưới game over

    // Tạo text Leaderboard (thay vì load hình)
    Texture leaderTexture; // tạo texture cho bảng xếp hạng
    if (!leaderTexture.loadFromFile("leader.png")) { // load hình ảnh bảng xếp hạng
        std::cout << "Khong the load leader.png\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
        return -1;
    }
    Sprite leaderSprite(leaderTexture); // tạo sprite từ texture bảng xếp hạng
    leaderSprite.setScale(1.f, 1.f); // đặt tỉ lệ cho bảng xếp hạng
    FloatRect leaderBounds = leaderSprite.getGlobalBounds(); // lấy kích thước của bảng xếp hạng
    leaderSprite.setPosition(bgSize.x/2 - leaderBounds.width/2-20, bgSize.y/2-350); // đặt vị trí bảng xếp hạng ở giữa, hơi lên trên

    // Thêm final score
    Text finalScoreText; // tạo text để hiển thị điểm số cuối cùng
    finalScoreText.setFont(font); // gán font cho text
    finalScoreText.setCharacterSize(40); // kích thước chữ
    finalScoreText.setFillColor(Color::White); // màu chữ trắng
    finalScoreText.setOutlineColor(Color::Black); // viền đen
    finalScoreText.setOutlineThickness(3); // viền dày 3px
    finalScoreText.setPosition(bgSize.x/2, bgSize.y/2 - 60); // đặt vị trí ở giữa, hơi lên trên (bên phải medalSprite)

    // Thêm best score
    int bestScore = 0; // điểm số cao nhất
    Text bestScoreText; // tạo text để hiển thị điểm số cao nhất
    bestScoreText.setFont(font); // gán font cho text
    bestScoreText.setCharacterSize(30); // kích thước chữ
    bestScoreText.setFillColor(Color::Yellow); // màu chữ vàng
    bestScoreText.setOutlineColor(Color::Black); // viền đen
    bestScoreText.setOutlineThickness(2); // viền dày 2px
    bestScoreText.setString("0"); // khởi tạo điểm số cao nhất là 0
    bestScoreText.setPosition(bgSize.x/2+180, bgSize.y/2 - 20); // đặt vị trí ở giữa, hơi lên trên (bên phải finalScoreText)

    // Medal / Rank sprite

    Texture medalTexture; // tạo texture cho huy chương
    Sprite medalSprite; // tạo sprite từ texture huy chương
    medalSprite.setScale(1.f, 1.f); // đặt tỉ lệ cho huy chương
 
    loadMapAssets(currentMap, backgroundTexture, pipeTexture, groundTexture, birdTexture,startTexture, mapName); // tải tài nguyên theo bản đồ hiện tại
    Text mapText; // tạo text để hiển thị tên bản đồ
    mapText.setFont(font); // gán font cho text
    mapText.setCharacterSize(24); // kích thước chữ
    mapText.setFillColor(Color::Yellow); // màu chữ vàng
    mapText.setString(mapName); // khởi tạo tên bản đồ
    mapText.setPosition(10, 10); // đặt vị trí ở góc trên bên trái


    float idleTime = 0.0f;       // thời gian trôi qua khi chưa bắt đầu game
    float idleAmplitude = 8.0f;  // biên độ dao động (chim bay lên xuống cao bao nhiêu px)
    float idleSpeed = 2.0f;      // tốc độ dao động
    float birdStartY = 300;      // vị trí Y ban đầu của chim
    Clock idleClock; // đồng hồ để đo thời gian idle

    //////////////////////////////////////////////////////SOUND//////////////////////////////////////////////////////
    // Nhạc nền (chạy xuyên suốt game)
    Music backgroundMusic; // tạo biến music cho nhạc nền
    if (!backgroundMusic.openFromFile("wind.mp3")) { // tải nhạc nền từ file
    std::cout << "Khong the load wind.mp3\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
    return -1;
    }
    backgroundMusic.setLoop(true); // lặp lại nhạc nền
    backgroundMusic.play(); // phát nhạc nền

    // Âm thanh vỗ cánh
    SoundBuffer wingBuffer; // tạo buffer cho âm thanh vỗ cánh
    if (!wingBuffer.loadFromFile("sfx_wing.wav")) { // tải âm thanh vỗ cánh từ file
    std::cout << "Khong the load sfx_wing.wav\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
    return -1;
    }
    Sound wingSound; // tạo biến sound cho âm thanh vỗ cánh
    wingSound.setBuffer(wingBuffer); // gán buffer cho sound
    
    // Âm thanh va chạm ống/đất
    SoundBuffer hitBuffer; // tạo buffer cho âm thanh va chạm
    if (!hitBuffer.loadFromFile("sfx_hit.wav")) { // tải âm thanh va chạm từ file
    std::cout << "Khong the load sfx_hit.wav\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
    return -1;
    }
    Sound hitSound; // tạo biến sound cho âm thanh va chạm
    hitSound.setBuffer(hitBuffer); // gán buffer cho sound

    SoundBuffer dieBuffer; // tạo buffer cho âm thanh chết
    if (!dieBuffer.loadFromFile("sfx_die.wav")) { // tải âm thanh chết từ file
    std::cout << "Khong the load sfx_die.wav\n"; // nếu không tải được, trả về -1 để kết thúc chương trình
    return -1;
    }
    Sound dieSound; // tạo biến sound cho âm thanh chết
    dieSound.setBuffer(dieBuffer); // gán buffer cho sound

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
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) { // nếu sự kiện là chuột trái được nhấn
                Vector2i mousePos = Mouse::getPosition(window); // lấy vị trí chuột trong cửa sổ
                if (gameOver && restartSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) { // nếu đang ở màn hình game over và chuột nhấn vào nút restart
                    // Reset game khi nhấn nút restart
                    resetGame(bird, pipesTop, pipesBottom, score, scoreText, scored, // gọi hàm resetGame
                    gameStarted, gameOver, birdSpeed, birdAngle, 
                    bgSize, groundHeight, pipeWidth, pipeSpacing, khoangcach,
                    birdFrameWidth, birdFrameHeight);
                }
            }
        }

        if (!gameOver) { // nếu chưa game over
            // === DI CHUYỂN ĐẤT (LUÔN CHẠY) ===
            for (int i = 0; i < numGround; ++i) {
                groundSprites[i].move(-groundSpeed, 0); // di chuyển đất sang trái
                if (groundSprites[i].getPosition().x <= -groundWidth) { // nếu đất đã đi hết màn hình bên trái
                    float maxX = 0;
                    for (int j = 0; j < numGround; ++j) // tìm đất xa nhất bên phải
                        if (groundSprites[j].getPosition().x > maxX) // nếu vị trí x của đất hiện tại lớn hơn vị trí x lớn nhất
                            maxX = groundSprites[j].getPosition().x; // cập nhật vị trí x lớn nhất
                    groundSprites[i].setPosition(maxX + groundWidth - 3, groundY); // đặt lại vị trí đất, cách đất xa nhất bên phải một khoảng groundWidth
                }
            }

            // === ANIMATION CHIM (LUÔN CHẠY) ===
            elapsedTime += animClock.restart().asSeconds(); // cập nhật thời gian đã trôi qua
            if (elapsedTime >= frameDuration) { // nếu thời gian đã trôi qua lớn hơn hoặc bằng thời gian mỗi khung hình
                elapsedTime = 0.0f; // reset thời gian đã trôi qua
                currentFrame = (currentFrame + 1) % birdFrame; // chuyển sang khung hình tiếp theo
                bird.setTextureRect(IntRect(currentFrame * birdFrameWidth, 0, /// cắt lấy khung hình tiếp theo của chim
                                            birdFrameWidth, birdFrameHeight));
            }

            if (!gameStarted) { 
                // === LOGIC MÀN HÌNH CHỜ ===
                idleTime += idleClock.restart().asSeconds(); // cập nhật thời gian trôi qua khi chưa bắt đầu game
                float offset = sin(idleTime * idleSpeed) * idleAmplitude; // tính toán vị trí y mới của chim dựa trên hàm sin để tạo hiệu ứng bay lên xuống
                bird.setPosition(bird.getPosition().x, birdStartY + offset); // cập nhật vị trí y của chim
            } else {
                // === LOGIC GAME KHI ĐÃ BẮT ĐẦU ===

                // Chim chịu gravity
                birdSpeed += gravity; //    cập nhật vận tốc của chim
                bird.move(0, birdSpeed); // cập nhật vị trí của chim

                if (birdSpeed > 0 && birdAngle < 90.f) { // nếu chim đang rơi và góc nghiêng chưa đạt 90 độ
                    birdAngle += rotationSpeed; // tăng góc nghiêng
                }
                bird.setRotation(birdAngle); // cập nhật góc nghiêng của chim

                // Giới hạn va chạm đất/trần
                if (bird.getPosition().y < 0) { // nếu chim chạm trần
                    bird.setPosition(bird.getPosition().x, 0); // đặt vị trí y của chim bằng 0
                }
                if (bird.getPosition().y >= bgSize.y - groundHeight - birdFrameHeight) { // nếu chim chạm đất
                    bird.setPosition(bird.getPosition().x,
                                     bgSize.y - groundHeight - birdFrameHeight); // đặt vị trí y của chim bằng vị trí y của đất
                    //std::cout << "Game Over! Chim da cham dat.\n";
                    hitSound.play();   // va chạm
                    dieSound.play();   // sau đó chết
                    gameOver = true; // xử lý thua game
                    finalScoreText.setString(to_string(score)); // hiển thị điểm số cuối cùng
                    finalScoreText.setPosition(bgSize.x/2 - finalScoreText.getLocalBounds().width/2+190, bgSize.y/2 - 110); //  đặt vị trí ở giữa, hơi lên trên (bên phải medalSprite)
                    if (score > bestScore) {
                                            bestScore = score; // cập nhật điểm cao nhất
                                            }
                    bestScoreText.setString(std::to_string(bestScore)); // hiển thị điểm số cao nhất
                    updateMedal(score, medalTexture, medalSprite,bgSize); // cập nhật huy chương dựa trên điểm số
                }

                for (int i = 0; i < numPipes; i++) { // kiểm tra va chạm với tất cả các ống
                    if (bird.getGlobalBounds().intersects(pipesTop[i].getGlobalBounds()) ||
                    bird.getGlobalBounds().intersects(pipesBottom[i].getGlobalBounds())) { // nếu chim va chạm với ống trên hoặc ống dưới
                        hitSound.play(); // va chạm
                        dieSound.play(); // sau đó chết
                    // xử lý thua game
                    gameOver = true; // đặt trạng thái game over
                    finalScoreText.setString(to_string(score)); // hiển thị điểm số cuối cùng
                    finalScoreText.setPosition(bgSize.x/2 - finalScoreText.getLocalBounds().width/2+190, bgSize.y/2 - 110); //  đặt vị trí ở giữa, hơi lên trên (bên phải medalSprite)
                    if (score > bestScore) {
                                            bestScore = score; // cập nhật điểm cao nhất
                                            }
                    bestScoreText.setString(std::to_string(bestScore));
                    // Cập nhật huy chương nếu có
                    updateMedal(score, medalTexture, medalSprite,bgSize); // cập nhật huy chương dựa trên điểm số
                    }
                }


        if (gameStarted && !gameOver) { // nếu game đã bắt đầu và chưa game over
            updatePipes(pipesTop, pipesBottom, pipeScored,
                        bird, score, scoreText,
                        numPipes, pipeWidth, pipeSpacing,
                        khoangcach, groundHeight, pipeSpeed,
                        bgSize, window); // cập nhật ống
                    }
            }
        }

        // === VẼ MỌI THỨ ===
        window.clear(Color::Cyan); // vẽ nền xanh cho cửa sổ
        window.draw(background); // vẽ nền
        
        if (gameStarted && !gameOver) {
            // Khi game đã bắt đầu và chưa game over - vẽ ống
            for (int i = 0; i < numPipes; i++) {
            window.draw(pipesTop[i]);// vẽ ống
            window.draw(pipesBottom[i]); // vẽ ống
            }
        }
        for (int i = 0; i < numGround; ++i)
            window.draw(groundSprites[i]); // vẽ đất
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
        } else { // Màn hình chờ
             float menuStartX = 40;
            float menuStartY = bgSize.y - (numMaps * 40) - 150; // 40 là khoảng cách giữa các dòng, 120 là khoảng cách lên trên một chút

            for (int i = 0; i < numMaps; ++i) {
            Text menuText;
            menuText.setFont(font);
            menuText.setCharacterSize(18);
            menuText.setFillColor(i == currentMap ? Color::Yellow : Color::Black);
            menuText.setString(to_string(i+1) + ". " + mapNames[i]);
            menuText.setPosition(menuStartX, menuStartY + i * 40); // vẽ từ dưới lên
            window.draw(menuText);
        }

        Text guideText;
        guideText.setFont(font);
        guideText.setCharacterSize(20);
        guideText.setFillColor(Color::Red);
        guideText.setString("Nhan phim 1-6 de chon map");
        guideText.setPosition(menuStartX, menuStartY + numMaps * 40 + 10); // dưới cùng của menu
        window.draw(guideText);

        window.draw(titleSprite);
        window.draw(startSprite);
        }
        
        if (!gameOver) {
            window.draw(bird); // vẽ chim (trừ khi game over)
        }
        window.display(); // hiển thị cửa sổ
    }
    
    /////////////////////////////////////////////////////////////////// END MAIN LOOP /////////////////////////////////////////////////////////////
    return 0; // trả về 0 để kết thúc chương trình
}