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
    RenderWindow window(VideoMode(bgSize.x,bgSize.y),"flaby brird"); // tạo khung cửa sổ và tiêu đề bên trên cửa sổ
    

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
    int gapY = 300; // vị trí y của khoảng cách giữa ống trên và ống dưới
    int khoangcach=150; // khoảng cách giữa ống trên và ống dưới
    int pipeHeight = pipeTexture.getSize().y; // chiều cao của ống từ texture
    int pipeWidth = pipeTexture.getSize().x; // chiều rộng của ống từ texture
    
    // Tạo ống trên :>>
    Sprite pipeTop(pipeTexture); // tạo sprite từ texture ống
    pipeTop.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh trên
    pipeTop.setRotation(180); // xoay ống trên 180 độ để hiển thị đúng
    pipeTop.setPosition(pipeX, gapY); // vị trí ống trên

    // Tạo ống dưới :>>
    Sprite pipeBottom(pipeTexture); // tạo sprite từ texture ống
    pipeBottom.setOrigin(pipeWidth / 2, 0); // gốc giữa cạnh dưới
    //pipeBottom.setRotation(180); // xoay ống dưới 180 độ để hiển thị đúng
    pipeBottom.setPosition(pipeX, gapY + khoangcach ); // vị trí ống dưới, cách ống trên một khoảng cách nhất định

    // Tạo chim :>>
    //CircleShape bird(20); // bán kính 20 pixel
    //bird.setFillColor(Color::Yellow); // màu vàng cho chim
    Texture birdTexture; // tạo texture cho chim
    if (!birdTexture.loadFromFile("chim.png")) { // tải hình ảnh chim từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    
    
    Sprite bird(birdTexture); // tạo sprite từ texture chim
    bird.setPosition(100, 300); // vị trí ban đầu của chim
    
    
    // Tốc độ của ống :>> 
    float pipeSpeed = 0.3f;
    
    
    while (window.isOpen()) { // vòng lặp chính của chương trình, chương trình sẽ chạy mãi mãi để cửa sổ tồn tại vĩnh viễn
        Event event; // biến event để lưu trữ các sự kiện xảy ra trong cửa sổ
        while (window.pollEvent(event)) { // kiểm tra các sự kiện xảy ra trong cửa sổ
            if (event.type == Event::Closed) { // nếu sự kiện là cửa sổ bị đóng
                window.close(); // đóng cửa sổ
            }
        }

        // Di chuyển ống sang trái
        pipeTop.move(-pipeSpeed, 0); // di chuyển ống trên sang trái
        pipeBottom.move(-pipeSpeed, 0); // di chuyển ống dưới sang trái
        if (pipeTop.getPosition().x < -pipeWidth) { // nếu ống trên ra ngoài cửa sổ
            
            int minGapY = 100; // khoảng cách tối thiểu giữa ống trên và ống dưới
            int maxGapY = bgSize.y - groundHeight - khoangcach - 100;  // khoảng cách tối đa giữa ống trên và ống dưới
            int gapY = minGapY + rand() % (maxGapY - minGapY + 1);  // tạo khoảng cách ngẫu nhiên giữa ống trên và ống dưới

            pipeTop.setPosition(bgSize.x + pipeWidth / 2, gapY); // vị trí ống trên
            pipeBottom.setPosition(bgSize.x + pipeWidth / 2, gapY + khoangcach); // vị trí ống dưới, cách ống trên một khoảng cách nhất định
        
        
        
        }
        // thứ tự các lệnh vẽ phải đúng thứ tự để hiển thị đúng
        window.clear(Color::Cyan); // vẽ nền xanh cho cửa sổ
        window.draw(background); // vẽ nền
        window.draw(pipeTop); // vẽ ống trên      ngược lại cũng được vẽ ống dưới trước, ống trên thoải mái không ảnh hưởng đến phần hiển thị
        window.draw(pipeBottom); // vẽ ống dưới
        window.draw(bird);         // vẽ chim :>>
        window.display(); // hiển thị cửa sổ
    }
    return 0; // trả về 0 để kết thúc chương trình
}