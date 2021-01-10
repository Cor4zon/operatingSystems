# operatingSystems 0/63

[Конспект лекций Хекслет](https://www.notion.so/f2e1f0c4250d448199e878bd6d996d67?v=bc4507d478fa4b278fe16a64ccfdd784)

[статья про кучу всего](https://proglib.io/p/how-os-work)

[wiki про процессы](https://ru.wikipedia.org/wiki/Процесс_(информатика))

[прерывания PIC APIC MIC](https://habr.com/ru/post/430548/)

## Список вопросов:

- [ ]  ОС - определение, место ОС в системе ПО ЭВМ. Классификация ОС и их особенности. Ресурсы вычислительной системы.
- [ ]  Ядро системы - определение. Классификация ядер ОС. ОС с монолитным ядром. ОС с микроядром: реализация взаимодействия процессов по модели клиент-сервер. Достоинства и недостатки микроядерной архитектуры.
- [ ]  Режимы ядра и задачи - переключение в режим ядра - классификация событий. Иерархическая машина, виртуальная машина.
- [ ]  Ядро ОС: многопоточное ядро; взаимоисключение в ядре – спин ‐ блокировки
- [ ]  у виртуальной и иерарх машины нужно рассказать про декомпозицию системы на уровни иерархии
- [ ]  централизованный и распределенные алгоритмы
- [ ]  локальное, глобльное перемещение

### Режимы

- [ ]  Три режима работы компьютера на базе процессоров Intel
- [ ]  Защищ режим. Уровни привилегий
- [ ]  Перевод компьютера из реального режима в защищенный и обратно
- [ ]  Защищенные режим: системные таблицы – GDT, LDT, IDT, теневые регистры; их назначение
- [ ]  Защищенный режим: EMS, преобразование адреса при страничном преобразовании в процессорах Intel

### Память

- [ ]  Виртуальная память: распределение страницами по запросам, свойство локальности, анализ страничного поведения процессов, рабочее множество. Схемы адресации. Достоинства и недостатки.
- [ ]  Методы управления виртуальной памятью
- [ ]  аномалия алгоритма FIFO
- [ ]  схема преобразования виртуального адреса, способы организации таблиц сегментов, стратегии выбора сегментов, особенности замещения сегментов
- [ ]  Управление памятью: распределение памяти сегментами по запросам[в архитектуре х86??] ; стратегии выделения памяти; фрагментация.
- [ ]  тип организации таблиц сегментов. формат дескриптора сегмента
- [ ]  Спецификация XM ( XMS ): Conventional, HMA, UMA, EMA????
- [ ]  XMS, линия A20 в защищенном режиме- адресное заворачивание(((((

### Процессы

- [ ]  Понятие процесса. Процесс как единица декомпозиции системы. Диаграмма состояний процесса и переход из одного состояния в другое. Контекст процесса.
- [ ]  Процессы и потоки.
- [ ]  Планирование и диспетчеризация. Классификация алгоритмов планирования. Алгоритм адаптивного планирования.
- [ ]  пересчет динамических приоритетов
- [ ]  Процессы в Unix: состояния, иерархия. Сироты, зомби, демоны. Примеры системных демонов, правила создания демонов.
- [ ]  создание процесса в UNIX и запуск новой программы (fork, exec)
- [ ]  Выполнение: последовательное/квазипараллельное/реальная параллельность
- [ ]  Средства взаимодействия процессов.
    - [ ]  Сообщения. Три состояния процесса при передаче сообщений.
    - [ ]  Мониторы - простой монитор, монитор "кольцевой буфер"
    - [ ]  их там еще много оказалось
- [ ]  Unix: команды - fork(); wait(); exec(); pipe(); signal().
- [ ]  UNIX разделяемая память (shmget(), shmat()) и семафоры (struct sem, semget(), semop()). примеры.
- [ ]  ipc(Межпроцессное взаимодействие (англ. inter-process communication)) ОС системы system V: очереди сообщений и программные каналы

### Взаимоисключение и синхронизация

- [ ]  Взаимоисключение и синхронизация процессов и потоков, монопольный доступ. Аппаратное взаимоисключение. Алгоритмы Деккера, Лампорта (“булочная”).
- [ ]  Организация монопольного доступа с помощью команды test-and-set
- [ ]  Семафоры, мьютексы: виды, примеры. Считающие и множественные семафоры.
- [ ]  программная реализация взаимоисключения — примеры
- [ ]  Мониторы: простые, с кольцевым буфером, Хоара (“читатели-писатели”).
- [ ]  монопольное использование — реализация; типы реализации взаимоисключения
- [ ]  Процессы UNIX: IPC-сигналы, программные каналы, разделяемая память; примеры использования
- [ ]  Бесконечное откладывание , зависание
- [ ]  синхронизация
- [ ]  Синхронизация потоков в ОС Windows: мьютексы, события
- [ ]  Win32 API : CreateThread(), WaitForSingleObject(), WaitForMultipleObject()
- [ ]  Тупики - определение, условия возникновения тупиков, методы обхода тупиков. Алгоритмы обхода: алгоритм Дейкстры (банкира). Обнаружение тупиков для повторно используемых ресурсов методом редукции графа и методы восстановления работоспособности системы

### Прерывания

- [ ]  Система прерываний: типы прерываний и их особенности, прерывания в последовательности ввода-вывода обслуживание запроса процесса на ввод-вывод. Контроллер прерывания-задачи,структура.
- [ ]  Прерывания: классификация, приоритеты прерываний, прерывания в последовательности ввода‐вывода
- [ ]  Адресация прерываний в защищённом режиме
- [ ]  аппаратные прерывания - последовательность операций при выполнении аппаратного прерывания.
- [ ]  аппаратные прерывания: задачи обработчика прерывания от системного таймера в защищенном режиме.
- [ ]  обработчики аппаратных прерываний
- [ ]  Прерывание int 8h (реальный режим) ‐функции.
- [ ]  Прерывания точные и неточные.
- [ ]  Прерывания в Linux: нижние и верхние половины, тасклеты, softirq. Обслуживание прерываний в ОС Windows и DPC.

### Управление устройствами; IO

- [ ]  Управление устройствами: физические принципы управления устройствами. Буферизация ввода-вывода управление буферами. Буферный пул, кеширование.
- [ ]  Подсистема ввода-вывода (управления устройствами): задача, место в ОС. Основные функции ОС, связанные с управлением устройствами. Программные принципы управления. Драйверы. Точки входа в драйверы. Устройства в Unix. Модули ядра в Linux: особенности, пример.
- [ ]  Синхронный и асинхронный ввод-вывод
- [ ]  Пять моделей ввода-вывода в Linux: диаграммы, особенности. Модель ввода-вывода с мультиплексированием.
- [ ]  Методы организации ввода‐вывода: программируемый, с прерываниями, прямой доступ к памяти.
- [ ]  последовательность операций при выполнении запроса на ввод/вывод

## Задачи:

- [ ]  Задача “обедающие философы”.
- [ ]  Задача “производство-потребление”.

Обеспечение моноп. доступа к разделяемым данным в задаче "писатели‐читатели", используя Win32 API .

читатели-писатели с использованием семафоров Дейкстра для ОС UNIX

Синхронизация потоков в ОС Windows: мьютексы, события ; пояснить особенности использования на примере задачи «читатели и писатели».

Проблема спящего парикмахера
