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
    
    /*==========================================================================*/
    
    // Tạo ống :>>
    Texture pipeTexture; // tạo texture cho ống
    if (!pipeTexture.loadFromFile("ong.png")) { // tải hình ảnh ống từ file
        return -1; // nếu không tải được, trả về -1 để kết thúc chương trình
    }
    // Khởi tạo các biến cho ống
    int goundHeight = 100; // chiều cao của mặt đất, có thể thay đổi để phù hợp với hình nền
    int pipeX= 400; // vị trí ống theo trục X, có thể thay đổi để di chuyển ống
    int pipeY = 0; // vị trí ống theo trục Y, sẽ được tính toán dựa trên khoảng cách giữa ống trên và ống dưới
    int pipeTopY = -50; // vị trí ống trên, có thể thay đổi để tạo độ khó
    int khoangcach=150; // sẽ là khoảng cách giữa ống trên và ống dưới, có thể thay đổi để tạo độ khó
    int gap = khoangcach; // dùng biến tạm sao chép khoảng cách giữa ống trên và ống dưới
    int pipeHeight = pipeTexture.getSize().y; // lấy chiều cao của ống từ texture
    int pipeWidth = pipeTexture.getSize().x; // lấy chiều rộng của ống từ texture

    // Ống trên
    Sprite pipeTop(pipeTexture); // tạo sprite từ texture ống
    //pipeTop.setPosition(400, 300); // vị trí của ống trên cùng, mặc dù vẽ ống trên cùng nhưng chả hiểu sao nó lại là cái ống bển dưới =))
    pipeTop.setOrigin(pipeWidth / 2, 0); // đặt gốc của ống trên tại giữa cạnh trên của nó
    pipeTop.setPosition(pipeX,pipeTopY); // vị trí của ống trên, nó sẽ nằm ở trên cùng của cửa sổ
    // ống dưới sẽ được tạo bằng cách lật ngược ống trên
    Sprite pipeBottom(pipeTexture); // tạo sprite từ texture ống
    pipeBottom.setOrigin(pipeWidth / 2, pipeHeight); // đặt gốc của ống dưới tại giữa cạnh dưới của nó
    //pipeBottom.setPosition(400, 600); // vị trí của ống dưới, nó lại là cái ống bển trên =))
    pipeBottom.setRotation(180); // lật ngược ống dưới
    pipeBottom.setPosition(pipeX, pipeHeight + gap + pipeTopY - 100); // vị trí của ống dưới, nó sẽ nằm ở dưới ống trên với khoảng cách đã định


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