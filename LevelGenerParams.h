#pragma once
// ��������� ��������� ������
class LevelGenerParams {
private:
    int
        minRoomSize,
        maxRoomSize,
        probabOfDivide;
    // ����������� ������� ������� [0..100]
public:
    LevelGenerParams(int _minRoomSize, int _maxRoomSize, int _probabOfDivide);
    ~LevelGenerParams();
    int getMinRoomSize();
    int getMaxRoomSize();
    int getProbabOfDivide();
};