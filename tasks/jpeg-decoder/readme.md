# JPEG Decoder

<img src="tests/bad_quality.jpg" alt="harold" width="600"/>

В этом домашнем задании вам предстоит реализовать декодер JPEG. Подробности будут рассказаны на семинарах.

## Установка библиотек

### Ubuntu

```sh
sudo apt install libjpeg-dev libpng-dev libfftw3-dev
```

### OS X

```sh
brew install libjpeg libpng fftw
```

## Оценивание

Задача разделена на 6 подзадач.

| Название | Стоимость |
|----------|-----------|
| [huffman](#huffman) | 2 балла |
| [fftw](#fftw) | 1.5 балла |
| [baseline](#baseline) | 2.5 балла|
| [faster](#faster) | 1.5 балл |
| [progressive](#progressive) | 2.5 балла |
| [fuzz](#jpeg-fuzz) | см. ниже |

* Fuzzing-тестирование на сервере будет запущено после дедлайна для всех решений. В случае, если сданы подзадачи *huffman* и *fftw*, прохождение этих тестов прибавляет **1 балл**. Если также сданы подзадачи *baseline* и *faster*, то прибавляются **еще 1.5 балла**. Таким образом, вы можете получить **10 баллов** без подзадачи *progressive*. 


* В этой задаче есть *пасхалка*. Если вы её найдёте, напишите семинаристу или лектору в личные сообщения. Первые 10 человек, которые смогут объяснить, что и как они нашли, получат бонусный балл.

## Что надо сделать

### <a id="huffman"></a> Huffman

> Суть: реализовать декодирование с помощью дерева Хаффмана

Смотрите секцию [The actual DHT in the JPEG file](https://www.impulseadventure.com/photo/jpeg-huffman-coding.html).
На вход классу подаются размеры кодов, а потом сами значения в дереве.

### <a id="fftw"></a> FFTW

> Cуть: реализовать IDCT (обратное дискретное косинусное преобразование) при помощи FFTW

На вход подаются двумерные матрицы 8x8 в одномерном виде (чтобы было быстрее).

Поищите в [документации](https://www.fftw.org/index.html) по ключевым словам IDCT/DCT:
* какие функции вам надо использовать
* с какими параметрами (спойлер: там должно быть что-то про двумерность)

Переиспользуйте план между вызовами при реализации следующего пункта.

### <a id="baseline"></a> Baseline

> Суть: ~~нарисуйте сову~~ используйте предыдущие две подзадачи, чтобы реализовать декодер, умеющий читать файлы.

Храните информацию о том, сколько байт вы просканировали, чтобы понимать текущую позицию в файле и общий прогресс по JPEG.

### <a id="faster"></a> Faster

> Суть: ускорить свою реализацию, чтобы в релизе она успевала обработать большой jpeg за меньше, чем 8 секунд

Используйте `perf record` и `perf report` для вашего бинаря в релизной сборке
(без ASAN, без TSAN и с `-DCMAKE_BUILD_TYPE=RelWithDebInfo`).

Идеи для оптимизаций:

* Деление на одно и то же число можно заменить на умножение на обратное
* Делать reserve перед push_back
* Оптимизация дерева Хаффмана
* Исправить хранение матриц

Любые хаки в рамках разумного разрешены (никакой глобальной памяти, пожалуйста).

### <a id="progressive"></a> Progressive

> Суть: продвинутая версия JPEG декодера, которая позволит читать любые жпеги мира.

Не обсуждается на семинарах. Здесь вам придется разобраться со [спецификацией JPEG](http://www.w3.org/Graphics/JPEG/itu-t81.pdf).

### <a id="fuzz"></a> Fuzz

> Суть: проверить, что ваше решение не содержит известных багов с помощью fuzzing-тестирования.

Таргеты для fuzzing называются `fuzz_huffman`, `fuzz_fft` и `fuzz_jpeg`. Обратите внимание, для локальной отладки вам потребуется clang (apple clang не подойдет). На Ubuntu его можно установить с помощью [скрипта](https://apt.llvm.org/), на OS X выполните `brew install llvm`.

Чтобы собрать таргеты не дефолтным компилятором, создайте отдельную сборочную директорию и передайте `cmake` аргументы `-DCMAKE_C_COMPILER=PATH_TO_YOUR_CLANG -DCMAKE_CXX_COMPILER=PATH_TO_YOUR_CLANG++`. Проверьте, что компилятор в выводе `cmake` тот, который вы ожидаете.

Локально запускайте fuzzing с корпусом tests. Скопируйте перед этим папку в другое место.
Последите за процессом первые несколько минут.  Как только исправите очевидные вещи, оставьте на ночь.

Если у кого-то найдется изображение с багом, на нем проверят все остальные решения.

## Материалы

* [Wikipedia](https://en.wikipedia.org/wiki/JPEG), русскую версию читать не стоит.
* [Пошаговый разбор с Хабра](https://habrahabr.ru/post/102521/).
* [JPEG-snoop](https://www.impulseadventure.com/photo/jpeg-snoop.html), утилита для дебага,
  без проблем запускается под wine (Ubuntu) или CrossOver (OS X), показывает подробную информацию 
  про JPEG файл. На этом же сайте много полезных статей про JPEG.
* [Спецификация JPEG](http://www.w3.org/Graphics/JPEG/itu-t81.pdf), понадобится для progressive части.
* [Дополнительный пост для любознательных](https://habrahabr.ru/post/206264/).

## Проверка

Перед отправкой решения отключите запись png файлов в тестах.

## Как не страдать при отладке

В проект подключена библиотека логирования и валидации [glog](http://rpg.ifi.uzh.ch/docs/glog.html).
Используйте ее вместо стандартного потока вывода. Что из этой библиотеки может пригодиться вам:

- Разный уровень логирования:

```cpp
DLOG(INFO) << "Preparing coffee";
DLOG(ERROR) << "Oh, our ship is about to blow up!";
```

- Логирование сообщения только при выполнении какого-то предиката:

```cpp
DLOG_IF(INFO, errors > 10) << "Well, we should stop now, I guess";
```

- Логирование не каждого события, а каждых N событий:

```cpp
DLOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th error";
```

- Валидация инвариантов:

```cpp
CHECK(write(x) == 4) << "Write failed!";
CHECK_NE(1, 2) << ": The world must be ending!";
CHECK_EQ(std::string("abc")[1], 'b');
```

Если вы хотите, чтобы в релизной сборке сообщения тоже выводились, уберите префикс D.


## На что стоит обратить внимание

- Разделяйте код на изолированные участки.
  Например, декодирование кода Хаффмана можно реализовать в отдельном классе,
  который ничего не будет знать про jpg.
- Обрабатывайте ошибки и кидайте исключения, если что-то пошло не так. Декодер не должен ломаться вне зависимости от
  ввода.
- Используйте общие интерфейсы: декодер будет принимать `std::istream&`.
- Декодер должен поддерживать маркеры SOI, SOF0, DHT, DQT, APPn, COM, EOI, SOS.
  Progressive декодер также должен поддерживать SOF2.
- Вы можете добавлять свои файлы. Если добавляете `.cpp`, добавьте его в список исходников библиотеки в [CMakeLists.txt](./CMakeLists.txt).