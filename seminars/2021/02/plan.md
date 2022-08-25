# Второй семинар

- ответить на вопросы по лекции
- почему работать с new это полная *** [new_sucks.cpp]
- на словах зачем может пригодиться MemoryPool [нужны нормальные примеры кода]
  - интересные видео: [раз](https://youtu.be/nZNd5FjSquk), [два](https://youtu.be/LIb3L4vKZ7U)
- empty-base optimization [ebo.cpp, compressed_pair.cpp]
  - когда происходит, когда нет
  - как используется в [compressed_pair](https://www.boost.org/doc/libs/1_77_0/libs/utility/doc/html/compressed_pair.html)
    - эту задачу они потом будут сдавать, будьте аккуратны, нужно показать те вещи, которые они пока сами не знают
  - зачем нужен compressed_pair (allocators, deleters etc.)
- немного про разницу между нулевыми указателями [nullptr.cpp]
- копирование ссылок, особенности, проблемы [copying_references.cpp]
  - [пост](https://danlark.org/2020/04/13/why-is-stdpair-broken/) Данилы про std::pair
- view [view.cpp]
  - поговорить про то, что объекты до 16 байт стоит передавать по значению
    - можно показать руками, во что превращается код, где передается через стек, а где через регистры
  - напомнить про lifetime, особенно временных объектов
  - TODO: написать или показать function_ref?
- short string optimization [sso.cpp]
  - показать руками хранимую подстроку в объекте
  - опционально рассказать трюк про то, как сделать 23 байта: храним в размере 23 - size
  - интересные особенности вместе со std::string_view (если не уйдет в crash-me)
- выравнивание и иже с ним [padding_and_co.cpp]
- placement new [placement_new.cpp]
  - показать когда нужно: при работе с аллокаторами, да и любой неинициализированной памятью
  - особенности:
    - правильного выравнивания
    - ручной вызов деструктора
  - немного про const-init, как можно сделать глобальный буффер, чтобы потом положить в него какой-нибудь объект