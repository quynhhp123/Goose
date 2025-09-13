if (score > bestScore) {
                                            bestScore = score;
                                            }
                    bestScoreText.setString(std::to_string(bestScore));
                    // Cập nhật huy chương nếu có
                    updateMedal(score, medalTexture, medalSprite,bgSize);