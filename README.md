---
title: Flappy Bird C++ với SFML

---

# Flappy Bird C++ với SFML

## Giới thiệu

Đây là dự án game Flappy Bird viết bằng ngôn ngữ C++ sử dụng thư viện đồ họa **SFML**. Game mô phỏng lại cách chơi Flappy Bird kinh điển: người chơi điều khiển chú chim bay qua các cặp ống, tránh va chạm và ghi điểm cao nhất có thể.

---

## Cấu trúc dự án

- **game.cpp**: Toàn bộ mã nguồn chính của game.
- **Các thư mục map**: `xmas`, `ocean`, `night`, `minecraft`, `halloween` chứa hình ảnh cho từng chủ đề bản đồ.
- **Các file ảnh**: Nền, ống, đất, chim, bảng điểm, game over, restart, huy chương...
- **Âm thanh**: Nhạc nền, hiệu ứng vỗ cánh, va chạm, chết.

---

## Giải thích từng phần mã nguồn

### 1. Khởi tạo & Tải tài nguyên

- **Texture & Sprite**: Tạo các đối tượng hình ảnh cho nền, ống, đất, chim, bảng điểm, game over, restart, huy chương...
- **loadMapAssets**: Hàm tải tài nguyên theo map, cho phép đổi chủ đề game dễ dàng.
- **resetGame**: Đặt lại trạng thái game về ban đầu khi chơi lại.

### 2. Xử lý sự kiện

- **Bắt phím SPACE**: Chim bay lên, bắt đầu game hoặc chơi lại khi game over.
- **Bắt phím số 1-6**: Đổi map (chủ đề) ngay tại menu chờ.
- **Bắt chuột trái**: Nhấn nút restart để chơi lại.

### 3. Logic game

- **Di chuyển chim**: Chim chịu trọng lực, bay lên khi nhấn SPACE, xoay góc khi rơi.
- **Di chuyển ống & đất**: Các sprite ống và đất di chuyển sang trái, lặp lại khi ra khỏi màn hình.
- **Tính điểm**: Khi chim vượt qua ống, điểm số tăng lên.
- **Va chạm**: Chim va vào ống hoặc đất thì game over.

### 4. Hiển thị & giao diện

- **Menu chọn map**: Hiển thị danh sách map ở góc dưới bên trái, nhấn phím số để chọn.
- **Màn hình chờ**: Hiệu ứng chim bay lên xuống, hiện tên game, nút bắt đầu.
- **Màn hình game**: Hiển thị điểm số, các sprite động.
- **Màn hình game over**: Hiện bảng điểm, điểm cao nhất, huy chương, nút restart.

### 5. Âm thanh

- **Nhạc nền**: Phát liên tục khi chơi.
- **Hiệu ứng**: Vỗ cánh, va chạm, chết.

---

## Hướng dẫn chơi

- **SPACE**: Chim bay lên, bắt đầu game hoặc chơi lại.
- **Phím số 1-6**: Đổi chủ đề map khi đang ở menu chờ.
- **Chuột trái**: Nhấn nút restart để chơi lại khi game over.

---

## Tùy biến

- Thêm map mới: Tạo thư mục mới, thêm hình ảnh đúng tên, sửa hàm `loadMapAssets`.
- Thay đổi tốc độ, khoảng cách ống, trọng lực... bằng cách chỉnh các biến đầu file.
- Thay đổi hiệu ứng, âm thanh bằng cách thay file tương ứng.

---

## Yêu cầu

- **SFML** (cài đặt thư viện SFML cho C++)
- **C++11** trở lên

---

## Tác giả

- Code: DuckWuynh
- Ý tưởng: Flappy Bird

---

## Ghi chú

- Nếu gặp lỗi không load được hình ảnh hoặc âm thanh, kiểm tra lại đường dẫn và tên file.
- Game chạy tốt trên Windows, có thể cần chỉnh lại đường dẫn khi chạy trên hệ điều hành khác.

---
