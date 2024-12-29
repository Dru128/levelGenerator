#include <random>
namespace LevelGenerator
{
    // ����� ������ ���������� �� ����������� ���� ��������
    int commonInterval(int start1, int end1, int start2, int end2, int* commonStart, int* commonEnd)
    {
        if (end1 >= start2 && start1 <= end2)
        { // �������� ������������ �� �������

            *commonStart = std::max(start1, start2);
            *commonEnd = std::min(end1, end2);
            // ����� ����������

            return 1;
        }
        else
            return 0; // ������� �� ������������
    }

    int randomInt(int _min, int _max)
    {
        if (_max - _min + 1 != 0) {
            int r = rand() % (_max - _min + 1) + _min;
            return r;
        }
        else return _min;
    }
}