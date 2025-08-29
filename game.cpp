#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace sf;
int main() {
  
    // Tạo nền :>>
    Texture backgroundTexture; // tạo texture cho nền
    if(!backgroundTexture.loadFromFile("nen.png")) { // tải hình ảnh nền từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }

    // Tạo cửa sổ :>>
    // Lấy kích thước ảnh nền để khớp với kích thước cửa sổ
    Vector2u bgSize = backgroundTexture.getSize();
    RenderWindow window(VideoMode(bgSize.x,bgSize.y),"flaby brird"); // tạo khung cửa sổ và tiêu đề bên  cửa sổ
    window.setFramerateLimit(120);



    Sprite background (backgroundTexture); // tạo sprite từ texture nền
    
    /*==========================================================================*/
    
    // Tạo ống :>>
    Texture pipeTexture; // tạo texture cho ống
    if (!pipeTexture.loadFromFile("ong.png")) { // tải hình ảnh ống từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    // Khởi tạo các biến cho ống
    int groundHeight = 100; // chiều cao của đất
    int pipeX = 400; // vị trí x của ống
    int gapY = 300; // vị trí y của khoảng cách giữa ống   và ống dưới
    int khoangcach=150; // khoảng cách giữa ống   và ống dưới
    int pipeHeight = pipeTexture.getSize().y; // chiều cao của ống từ texture
    int pipeWidth = pipeTexture.getSize().x; // chiều rộng của ống từ texture
    
    // Tạo ống   :>>
    Sprite pipeTop(pipeTexture); // tạo sprite từ texture ống
    pipeTop.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh  
    pipeTop.setRotation(180); // xoay ống   180 độ để hiển thị đúng
    pipeTop.setPosition(pipeX, gapY); // vị trí ống  

    // Tạo ống dưới :>>
    Sprite pipeBottom(pipeTexture); // tạo sprite từ texture ống
    pipeBottom.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh dưới
    //pipeBottom.setRotation(180); // xoay ống dưới 180 độ để hiển thị đúng
    pipeBottom.setPosition(pipeX, gapY + khoangcach ); // vị trí ống dưới, cách ống   một khoảng cách nhất định



    // Tạo đất 
    Texture groundTexture; // tạo texture cho đất
    if (!groundTexture.loadFromFile("dat.png")) { // tải hình ảnh đất từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    int groundWidth = groundTexture.getSize().x; // chiều rộng của đất từ texture
    int groundY = bgSize.y - groundHeight; // vị trí y của đất


    const int numGround = bgSize.x / groundWidth + 4; // +3 để tránh hở
    Sprite groundSprites[numGround];
    for (int i = 0; i < numGround; ++i) {
        groundSprites[i].setTexture(groundTexture);
        groundSprites[i].setPosition(i * groundWidth, groundY);
    }


    // Tạo chim :>>
    //CircleShape bird(20); // bán kính 20 pixel
    //bird.setFillColor(Color::Yellow); // màu vàng cho chim
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
    
    //animation chim :>>
    int  currentFrame=0; // khung hình hiện tại
    float frameDuration=0.15f; // thời gian mỗi khung hình hiển thị
    float elapsedTime=0.0f; // thời gian đã trôi qua
    Clock clock; // đồng hồ để đo thời gian

    
    //Khởi tạo cho di chuyển của ống 
    // Tốc độ của ống :>> 
    float pipeSpeed = 2.0f;
    // Tốc độ của đất :>>
    float groundSpeed = 2.f; // tốc độ di chuyển của đất, bằng với tốc độ di chuyển của ống để tạo hiệu ứng liền mạch
    
    // Khởi tạo cho di chuyển của chim
    float birdSpeed = 0.0f; // vận tốc ban đầu của chim
    float gravity = 0.25f; // trọng lực tác động lên chim
    float jumpStrength = -6.5f; // sức mạnh nhảy của chim



    while (window.isOpen()) { // vòng lặp chính của chương trình, chương trình sẽ chạy mãi mãi để cửa sổ tồn tại vĩnh viễn
        Event event; // biến event để lưu trữ các sự kiện xảy ra trong cửa sổ
        while (window.pollEvent(event)) { // kiểm tra các sự kiện xảy ra trong cửa sổ
            if (event.type == Event::Closed) { // nếu sự kiện là cửa sổ bị đóng
                window.close(); // đóng cửa sổ
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) { // nếu sự kiện là phím Space được nhấn
                birdSpeed = jumpStrength; // đặt vận tốc của chim bằng sức mạnh nhảy
            }
        }

            for (int i = 0; i < numGround; ++i) {
            groundSprites[i].move(-groundSpeed, 0);
            // Nếu sprite đất ra khỏi màn hình trái, chuyển về bên phải
            if (groundSprites[i].getPosition().x <= -groundWidth) {
                // Tìm sprite đất ở xa nhất bên phải
                float maxX = 0;
                for (int j = 0; j < numGround; ++j)
                    if (groundSprites[j].getPosition().x > maxX)
                        maxX = groundSprites[j].getPosition().x;
                groundSprites[i].setPosition(maxX + groundWidth-3, groundY);
                 }
             }

        elapsedTime += clock.restart().asSeconds();
        if (elapsedTime >= frameDuration) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % birdFrame;
        bird.setTextureRect(IntRect(currentFrame * birdFrameWidth, 0, birdFrameWidth, birdFrameHeight));
        }
        

        // Chim chịu tác dộng của trọng lực
        if (bird.getPosition().y < bgSize.y - groundHeight - birdTexture.getSize().y) {
        birdSpeed += gravity;
        }
        bird.move(0, birdSpeed); // di chuyển chim theo vận tốc hiện tại
        
        if (bird.getPosition().y<0){
            bird.setPosition(bird.getPosition().x, 0); // nếu chim bay lên quá cao, đặt lại vị trí chim ở   cùng
        }
        if (bird.getPosition().y > bgSize.y - groundHeight - birdTexture.getSize().y) { // nếu chim rơi xuống đất
            bird.setPosition(bird.getPosition().x, bgSize.y - groundHeight - birdTexture.getSize().y); // đặt lại vị trí chim ở   đất
            //birdSpeed = 0; // đặt vận tốc của chim về 0
        }
        
        
        // Di chuyển ống sang trái
        pipeTop.move(-pipeSpeed, 0); // di chuyển ống   sang trái
        pipeBottom.move(-pipeSpeed, 0); // di chuyển ống dưới sang trái
        if (pipeTop.getPosition().x < -pipeWidth) { // nếu ống   ra ngoài cửa sổ
            
            int minGapY = 100; // khoảng cách tối thiểu giữa ống   và ống dưới
            int maxGapY = bgSize.y - groundHeight - khoangcach - 100;  // khoảng cách tối đa giữa ống   và ống dưới
            int gapY = minGapY + rand() % (maxGapY - minGapY + 1);  // tạo khoảng cách ngẫu nhiên giữa ống   và ống dưới

            pipeTop.setPosition(bgSize.x + pipeWidth / 2, gapY); // vị trí ống  
            pipeBottom.setPosition(bgSize.x + pipeWidth / 2, gapY + khoangcach); // vị trí ống dưới, cách ống   một khoảng cách nhất định
        
        
        
        }
        
        // thứ tự các lệnh vẽ phải đúng thứ tự để hiển thị đúng
        window.clear(Color::Cyan); // vẽ nền xanh cho cửa sổ
        window.draw(background); // vẽ nền
        window.draw(pipeTop); // vẽ ống        ngược lại cũng được vẽ ống dưới trước, ống   thoải mái không ảnh hưởng đến phần hiển thị
        window.draw(pipeBottom); // vẽ ống dưới
        for (int i = 0; i < numGround; ++i)
        window.draw(groundSprites[i]);
        window.draw(bird);         // vẽ chim :>>
        window.display(); // hiển thị cửa sổ
    }
    return 0; // trả về 0 để kết thúc chương trình
}