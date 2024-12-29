#include "LevelGenerParams.h"
namespace LevelGenerator
{
    LevelGenerParams::LevelGenerParams(int _minRoomSize, int _maxRoomSize, int _probabOfDivide)
    {
        this->minRoomSize = _minRoomSize;
        this->maxRoomSize = _maxRoomSize;
        this->probabOfDivide = _probabOfDivide;
    }
    LevelGenerParams::~LevelGenerParams() {}
    int LevelGenerParams::getMinRoomSize()
    {
        return this->minRoomSize;
    }
    int LevelGenerParams::getMaxRoomSize()
    {
        return this->maxRoomSize;
    }
    int LevelGenerParams::getProbabOfDivide()
    {
        return this->probabOfDivide;
    }
}