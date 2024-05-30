# RU | Модель работы общественного транспорта
## О проекте
Данная модель разработана в рамках обучения в университете, в качестве курсовой работы по дисциплине Технологии Программирования. 

В данной курсовой работе продемострировано приложение, моделирующее оперативную работу общественного транспорта в условиях оживлённого города с двухполосной системой дорог, с использованием принципов объектно-ориентированного программирования.

Такое приложение не только даст пользователям возможность наблюдать со стороны за системой общественного транспорта, но и продемонстрирует, как концепции ООП могут быть применены в разработке программного продукта.

Основной целью курсовой работы является изучение принципов ООП и их применение при создании приложения для моделирования поведения такси и автобусов. Разработка этой системы позволит глубже понять принципы ООП и их практическую применимость, а также покажет преимущества использования объектно-ориентированного подхода в создании программного обеспечения.

## Описание работы модели

При запуске модели сразу создаётся экземпляр класса Bus – автобус, и начинает движение по заранее составленному маршруту, останавливаясь на каждой из существующих остановок.

Стоит заметить, что автобус, при прекращении движения на любой остановке забирает всех пассажиров, которые там находятся. Одновременно с этим, если поле пассажиров _goalbusStopIndex хранит информацию о той остановке, на которой остановился автобус, эти пассажиры начнут выходить из одной двери в направлении от автобуса.

При создании экземпляра класса «Машина такси», воспользуясь полями TextBox, представленными в правой верхней части экрана, пользователь может задать цвет, а также скорость машины такси.

Если на сцене существует хотя бы одна машина такси, в любой момент есть некоторая вероятность генерации пассажира. После этого, сервис (описываемый в классе MyEnvironment) отбирает одну свободную машину из существующих, а та, в свою очередь сообщает пассажиру о своём скором прибытии. Таким образом, одновременно взаимодействуют 3 экземпляра классов: MyEnvironment -> TaxiCar -> Passenger. Взаимодействие осуществляется при помощи событий Event, привязанных к конкретным ссылкам-делегатам соответствующих классов издателей.

При получении геолокации (координат) заказчика, такси осуществляет поиск пути алгоритмом BFS (поиск в ширину), имеющим особые условия выборки в силу двухполосной системы дорог и ПДД, а после того, как доезжает до пассажира, некоторое время везёт его в точку прибытия.