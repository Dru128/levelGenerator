Алгоритм состоит из четырёх функциональных блоков:
1.	Дробление комнат
Сначала создаётся одна комната с размерами соответсвующими размеру уровня. На каждом этапе для каждой комнаты решается: закончит она своё деление или будет делиться дальше. Выбор происходит, опираясь на условие: комната должна быть больше минимального заданного размера комнаты и меньше максимального. При делении комнаты, две новые делятся по большей стороне старой (длине или ширине). 
Также присутствует элемент случайности, который входит в входные данные алгоритма и измеряется в процентах: чем больше вероятность – тем больше комнат минимального размера, чем меньше – соответсвенно больше комнат максимального размера размера (при вероятности 50% комнат максимального и минмального размера равномерное количество). При каждой итерации деления с заданной вероятностью комната больше не будет делится и ее размер становится окончательным (помечается специальным флажком).
2.	Построение графа соседних комнат
Строится матрица инциндентности взвешенного неориентированного графа, в которой верщины соответсвуют комнатам, ребра – общим стенам между комнатами. На данном этапе определяются для каждой комнаты соседние комнаты и общие стены, в которых возможно будет создать проходы на следующих этапах. Вес рёбер определяется случайно.
Для каждой комнаты проверяются другие комнаты: являются ли они соседями. Если две комнаты имеют общию стену, то с помощью функции поиска подотрезка двух пересекающихся отрезков определяется общий участок стены, в котором возможно будет сделать проход между комнатами.
3.	Построение соединений комнат
С помощью алгоритма Прима строится минимальное остовное дерево этого графа. Таким образом получается граф, гарантирующий достижимость из любой комнаты любую другую комнату.
Вес рёбер (общих участков стен между комнатами) был определен случайным образом на предыдущих этапах. Берётся первая комната и находится её стена с наименьшим весом. Найденная стена и соединяемые ей две комнаты образуют дерево. Далее рассматриваются стены, одна сторона которых пренадлежит графу комнат, а вторая – нет, из них выбирается стена с наименьшим весом. Выбираемая на каждом шаге стена присоединяет новую комнату к дереву. Дерево растёт, пока не присоединятся все комнаты.
4.	Конвертирование графа в матрицу
На основе графа строится итоговая матрица, которая задается значениями: 0 – пустое пространство, 1 – стена.
Сначала матрица полностью заполняется стенами, затем места комнат и проходов между ними выборочно освобождается пустым пространством.
