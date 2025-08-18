#include <SFML/Graphics.hpp>
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
    

    // Tạo ống :>>
    Texture pipeTexture; // tạo texture cho ống
    if (!pipeTexture.loadFromFile("ong.png")) { // tải hình ảnh ống từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    Sprite pipeTop(pipeTexture); // tạo sprite từ texture ống
    pipeTop.setPosition(400, 0); // vị trí của ống trên cùng
    // ống dưới sẽ được tạo bằng cách lật ngược ống trên
    Sprite pipeBottom(pipeTexture); // tạo sprite từ texture ống
    pipeBottom.setPosition(400, 400); // vị trí của ống dưới
    pipeBottom.setRotation(180); // lật ngược ống dưới



    // Tạo chim :>>
    //CircleShape bird(20); // bán kính 20 pixel
    //bird.setFillColor(Color::Yellow); // màu vàng cho chim
    Texture birdTexture; // tạo texture cho chim
    if (!birdTexture.loadFromFile("chim.png")) { // tải hình ảnh chim từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    
    
    Sprite bird(birdTexture); // tạo sprite từ texture chim
    bird.setPosition(100, 300); // vị trí ban đầu của chim
    
    
    while (window.isOpen()) { // vòng lặp chính của chương trình, chương trình sẽ chạy mãi mãi để cửa sổ tồn tại vĩnh viễn
        Event event; // biến event để lưu trữ các sự kiện xảy ra trong cửa sổ
        while (window.pollEvent(event)) { // kiểm tra các sự kiện xảy ra trong cửa sổ
            if (event.type == Event::Closed) { // nếu sự kiện là cửa sổ bị đóng
                window.close(); // đóng cửa sổ
            }
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