# HeckingCompiler
Создание компилятора, обнаруживающего ошибки в коде анализируемой программы

На вход подаются один или несколько файлов .cpp/.h</br>
__Программа должна уметь следующее:__
- [ ] Распарсить содержимое нескольких файлов
- [ ] Определить наличие:
  - [ ] Собственных функций и типов данных (typedef)
  - [ ] Неиспользуемых переменных и функций
  - [ ] Рекурсивных функций, зацикленностей
  - [ ] Вложенных циклов (сосчитать их глубину)
  - [ ] Циклов во вложениях файлов (a1.h включает a2.h, a2.h включает a3.h, a3.h вкл. a1.h)
- [ ] Убедиться в:
  - [ ] Корректности имен функций и переменных (camelCase для переменных и PascalStyle для функций)
  - [ ] Наличии начальных значений у переменных (заданы явно/получаемы от пользователя/из функции)
- [x] Отформатировать исходный код:
  - [x] Общее форматирование
  - [x] Убрать лишние строки и пробелы, неправильную разметку
  - [x] Заменить многострочные комментарии на однострочные
  - [x] Заменить многострочные комментарии внутри строки на однострочные (a = /* 10 */ 1; → //a = 10; a=1;)
  - [x] Сохранить результат в новый файл(ы)
- [ ] Вывести на экран:
  - [ ] Список найденных проблем
  - [ ] Граф вызова функций
