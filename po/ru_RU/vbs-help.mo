﻿РАБОТА С VBS

«Горячие» клавиши:
* пробел — переключатель режима воспроизведение/пауза
* b — ввод субтитра
* m — вывод субтитра
* n — вывод субттира и ввод  следующего
* s — запись субтитров

ИМПОРТ СУБТИТРОВ

Через меню Subtitles можете импортировать субтитры в форматах «текст» и SubRip, а также указать кодировку.

Форматы:
* Текст: по одному субтитру на каждой строке. Если субтитры длинные, знаком “|” можно указать, где сделать переход на новую строку. 
* SubRip

Из того же меню можно указать кодировку для импорта (по умолчанию UTF-8).

ЗАПИСь СУБТИТРОВ

Субтитры записываются автоматически кажды раз, когда: 
* выводится субтитр (нажата клавиша "n" или "m")
* нажата клавиша "s"
* программа заканчивает работу

Через меню Subtitles можно указать куда записывать субтитры (по умолчанию в /tmp/vbs_export.srt).

Из того же меню можно указать:
* Кодировку для записи (по умолчанию UTF-8)
* Символ новой строки (по умолчанию LF).

РЕДАКТИРОВАНИЕ СПИСАК СУБТИТРОВ

Нажмите на субтитр мышкой чтоб перейти к нему. 

Через меню Edit можно: 
* Ввести новый (пустой) субтитр перед текущим
* Ввести новый (пустой) субтитр после текущего
* Удалить текущий субтитр
* Обнулить время для текущего субтитра
* Обнулить время для всех субтитров после текущего

Двойным щелчком мыши на время ввода или вывода субтитра можно перевести видео за 5 секунд до его начала.

Двойным щелчком мыши на текст субтитра можно его отредактировать.

ВОСПРОИЗВЕДЕНИЕ ВИДЕО

Через меню Video можно открыть нужный видо файл. 

Принимаемые форматы: любые, которые программа для воспроизведения (GStreamer или MPlayer) может показать.

VBS умеет работать с двумя видео программами: 
* MPlayer
* GStreamer

Поддержка мижет быть включена для одного из них или для обоих. Во втором случае по умолчанию будеть выБран Gstreamer. Через меню Video это можно изменить. 

В том же меню можно выбрать метод отображения видео:
* Для Gstreamer работают X11 (самый консервативный с наибольшей нагрузкой на микропроцессор, но работает на всех компьютерах; выБран по умолчанию), Xv (меншая нагрузка на микопроцессор, работает на всех современных видеокартах) и SDL (пользуйтесь только если другие режимы не работают).
* Для MPlayer метод нужно указать в настройках самого MPlayer, а не через VBS. 

РАБОТА С VSB БЕЗ ВИДЕО

VBS можно пользоваться без просмотра видео (например, если фильм смотреть на ТВ). Чтобы обнулить начальное время субтитров, выберите Edit -> Set Start Time в момент начала фильма. 

РАБОТА С СЕТьЮ

Через меню Network можно подключить или выключить связь с сервером vbsd. Когда связь включена, режим работы — отсылка субтитров на сервер (“Send”). Если не пользуетесь vbsd, отключите сеть для ускорения работы (по умолчанию сеть отключена).

Если сеть подключена, через то же меню можно указать: 
* Имя сервера (FQDN); будет принято только после успешной трансляции в IP адрес.
* Номер порта TCP, на котором работает vbsd (по умолчанию 42)
* Ключ для работы с сервером vbsd (по умолчанию 1973); ключ должен быть одинаковым на сервере и в настройках программы.

ДОПОЛНИТЕЛьНыЕ ПРОГРАММы

Кроме этой программы (основной клиент или vbsm), в комплект VBS входят еще: 
* Сервер vbsd – транслирует субтитры от vbsm или vbss к vbss,
* Дополнительный клиент или vbss – для проектирования субтитров поверх другого изображения. Субтитры считываются из локального SubRip файла или получаются по сети от vbsd сервера. 
* Утилита настройки или vbsc – для настройки параметров vbss. 

