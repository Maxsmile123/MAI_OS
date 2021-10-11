# Лабораторная работа № 2. Процессы.

Задание: Составить и отладить программу на языке Си, осуществляющую работу с процессами и
взаимодействие между ними в одной из двух операционных систем. В результате работы
программа (основной процесс) должен создать для решение задачи один или несколько
дочерних процессов. Взаимодействие между процессами осуществляется через системные
сигналы/события и/или каналы (pipe).
Необходимо обрабатывать системные ошибки, которые могут возникнуть в результате работы.

Вариант 21: Правило фильтрации: нечетные строки отправляются в pipe1, четные в pipe2.
Дочерние процессы инвертируют строки.
![Снимок экрана 2021-10-11 174322](https://user-images.githubusercontent.com/54589783/136811334-cd0cb419-44b3-43c4-a083-71df940dcc76.png)

Выполнено под OC Linux.