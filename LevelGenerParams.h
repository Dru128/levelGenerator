#pragma once
// параметры генерации уровня
class LevelGenerParams {
private:
    int
        minRoomSize,
        maxRoomSize,
        probabOfDivide;
    // вероятность деления комнаты [0..100]
public:
    LevelGenerParams(int _minRoomSize, int _maxRoomSize, int _probabOfDivide);
    ~LevelGenerParams();
    int getMinRoomSize();
    int getMaxRoomSize();
    int getProbabOfDivide();
};