> Здесь пока будут содержаться мои зафиксированные размышления, структура проекта, идеи...

# Сущности
- Ядро игры;
- Хранение поля и кораблей: моего и врага;
- Обработчик комманд;
- Парсер;
- Стратегии;

А теперь подробнее.


## Ядро игры (Kernel)

Здесь содержаться все настройки игры, выбранный тип стратегии, и нужные размеры поля и кораблей.

1. Тип игрока: мастер или слуга(master or slave);
2. Размер поля;
3. Выбранная стратегия;
4. Количество соответственных кораблей;
5. Статус игры: выиграли или нет;
6. Непосредственно собственное поле и поле врага;


## Хранение поля и кораблей (Map)

Данная структура данных должна оптимально хранить информацию о поле, причем с ограничением в 32 Mb. (Макс значений в поле 2\**64 * 2\**64 и их два: один принадлежит нам, другой врагу).\
Каждое поле есть перечень из всех исследованных клеток, в которых находится пустота или часть корабля - может быть, и уже подбитого.
Конечно, было бы гениально реализовать архиватор, который бы сжимал поле до приемлемых размеров и получал нужную область(часть поля) при каком-либо действии, но пока это стоит просто оставить как дополнительную задачу на будущее.\
Поле самостоятельно генерируется исходя из выбранных настроек. (стратегии или соперника, если программа запущена под slave-ом).

Поле: [boost хэш таблица](https://www.boost.org/doc/libs/1_36_0/doc/html/boost/unordered_map.html), которая дает возможность обращаться по ключу, представленного в виде пары координат [x и y](https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key).


## Обработчик комманд (Proccesor)

Хорошее название, а главное - верное, так как именно он будет заниматься всем: администрирование игры и ее руководство.\
Этот объект отвечает за весь ввод потока и вывода, и - при необходимости, а точнее, если только это возможно - делегирует его другим сущностям.

| Команда                      | Ответ          | Описание |
| -------                      | -----          | --------                                    |
| ping                         |  pong          |   тестовая команда                          |
| exit                         |  ok            |   программа завершается                     |
| create [master,slave]        |  ok            |   создать игру в режиме master или slave соответственно       |
| start                        |  ok            |   старт игры                     |
| stop                         |  ok            |   остановка текущей партии       |
| set width  N                 |  ok/failed     |   установить ширину поля (N положительное, влезает в uint64_t)       |
| get width                    |  N             |   получить длину поля  (N положительное, влезает в uint64_t)      |
| set height N                 |  ok/failed     |   установить высоту поля (N положительное, влезает в uint64_t)        |
| get height                   |  N             |   получить высоту поля  (N положительное, влезает в uint64_t)      |
| set count [1,2,3,4]  N       |  ok/failed     |   установить количество кораблей определенного типа (N положительное, влезает в uint64_t)        |
| get count [1,2,3,4]          |  N             |   получить количество кораблей определенного типа (N положительное, влезает в uint64_t)        |
| set strategy [ordered,custom]|  ok            |   выбрать стратегию для игры        |
| shot X Y                     |  miss/hit/kill |   выстрел по вашим короаблям в координатах (X,Y) (X,Y положительные, влезают в uint64_t)      | 
| shot                         |  X Y           |   вернуть координаты вашего следующего выстрела, в ответе два числа через пробел  (X,Y положительные, влезают в uint64_t)       |
| set result [miss,hit,kill]   |  ok            |   установить результат последнего выстрела программы       |
| finished                     |  yes/no        |   окончена ли текущая партия       |
| win                          |  yes/no        |   являетесь ли вы победителем       |
| lose                         |  yes/no        |   являетесь ли вы проигравшим       |
| dump PATH                    |  ok            |   сохранить размер поля и вашу текущую расстановку кораблей в файл        |
| load PATH                    |  ok            |   загрузить размер поля и расстановку кораблей из файла      |

Немного подробнее
finished - должно вернуть да, если есть однозначно статус победы или проигрыша
win - должно вернуть да, если мы уже гарантировано знаем, что победили, иначе - нет. (кол-во подбитых клеток врага равно начальному количеству заданных кораблей)
lose - должно вернуть да, если мы однозначно проиграли, иначе - нет.

## Парсер (Parser)

Тут все очевидно. Реализован парсер информации о настройках игры и передан в `Processor`.

Пример

```
10 10
1 v 0 0
2 h 3 4
4 h 1 8
```


## Стратегии (Strategy)

Реализованы две стратегии: ordered, custom. Названия сами за себя говорят, так что добавить мне здесь нечего. Пока я не знаю, как именно я их реализую, но они будут как раз отвечать за ходы: какую клетку стоит бить дальше.


## Дерево команд
```mermaid
flowchart LR
		%% creating
    create
		master
		slave
		create --> master
		create --> slave
		
		%% slave setting
		set
		slave --> set
		width
		height
		strategy(strategy)
		set --> width
		set --> height
		set --> strategy

		%% set strategy
		ordered
		custom
		others@{ shape: braces, label: "others..." }
		strategy --> ordered
		strategy --> custom
		strategy --> others

		%% set count
		count["count (setting the n deck)"]
		count1
		count2
		count3
		count4
		count --> count1["set count 1"]
		count --> count2["set count 2"]
		count --> count3["set count 3"]
		count --> count4["set count 4"]
		set --> count

		%% load
		load{{load}}
		slave --> load


		%% master getters
		get
		master --> get
		widthM[width]
		heightM[height]
		get --> widthM
		get --> heightM

		%% get count
		countM["count (get the n deck)"]
		count1M
		count2M
		count3M
		count4M
		countM --> count1M["get count 1"]
		countM --> count2M["get count 2"]
		countM --> count3M["get count 3"]
		countM --> count4M["get count 4"]
		get --> countM


		%% verify / waiting
		verifyS["verifying that all values are set"]
		width --> verifyS
		height --> verifyS
		count1 --> verifyS
		count2 --> verifyS
		count3 --> verifyS
		count4 --> verifyS

		verifyM["just waiting for the start cmd"]
		widthM --> verifyM
		heightM --> verifyM
		count1M --> verifyM
		count2M --> verifyM
		count3M --> verifyM
		count4M --> verifyM


		%% start
		start((start))
		verifyS --> start
		verifyM --> start
		load --> start

		%% dump
		dump
		start --> dump

		%% shots
		shot
		shotCoord[shot X Y]
		result[set result]
		start --> shot
		start --> shotCoord
		shotCoord --> result

		%% win / lose
		game[players are playing]
		stop
		finished
		win
		lose
		shot --> game
		result --> game
		game --> stop
		stop --> finished
		game --> finished
		finished --> win
		finished --> lose
		exit(((exit)))
		win --> exit
		lose --> exit


		%% others
		ping([ping])
```