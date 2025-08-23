search_engine

Описание

search_engine — локальный поисковый движок по файлам. Он индексирует текстовые документы, принимает запросы через JSON и возвращает список документов с указанием их релевантности. Проект использует многопоточность для ускорения поиска и поддержку конфигурационных файлов для гибкой настройки.

Структура проекта

search_engine/
├── config/
│   ├── answers.json
│   ├── config.json
│   └── requests.json
├── include/
│   ├── ConverterJSON.h
│   ├── InvertedIndex.h
│   └── SearchServer.h
├── json/
├── resources/
│   ├── text1.txt
│   └── ...
├── src/
│   ├── CMakeLists.txt
│   ├── ConverterJSON.cpp
│   ├── InvertedIndex.cpp
│   ├── main.cpp
│   └── SearchServer.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── index_tests.cpp
│   ├── main.cpp
│   └── server_tests.cpp
└── CMakeLists.txt

Сборка и запуск

1. Скопируйте всю папку проекта search_engine в удобное место.


2. Создайте папку для сборки:

mkdir build && cd build


3. Запустите CMake для генерации файлов сборки:

cmake -G "Ninja" ../


4. Соберите проект:

cmake --build .


5. Все JSON-файлы (config.json, requests.json, answers.json) должны лежать в одной папке с исполняемым файлом search_engine.exe.


6. Запуск программы:

./search_engine.exe



Функционал

Индексирование текстовых документов из папки resources.

Поиск по запросам из requests.json.

Выдача релевантности каждого документа относительно запроса.

Многопоточность для ускорения обработки запросов.

Гибкая конфигурация через config.json.


Пример использования

config.json

{
  "config": {
    "name": "SearchEngine",
    "version": "0.1"
  },
  "files": [
    "../resources/text1.txt",
    "../resources/text2.txt",
    "../resources/text3.txt"
  ]
}

requests.json

{
  "requests": [
    "кто изобрел первый велосипед",
    "когда появились велосипеды с педалями",
    "особенности пенни-фартинга",
    "что такое блокчейн простыми словами",
    "где используется блокчейн кроме криптовалют",
    "как обеспечивается безопасность блокчейна",
    "какие продукты входят в средиземноморскую диету",
    "влияние средиземноморской диеты на сердце",
    "почему в греции высокая продолжительность жизни"
  ]
}

После запуска search_engine.exe создаётся answers.json:

{
    "answers": {
        "0": {
            "relevance": [
                {
                    "docid": 0,
                    "rank": 1.0
                }
            ],
            "result": true
        },
        "1": {
            "relevance": [
                {
                    "docid": 0,
                    "rank": 1.0
                }
            ],
            "result": true
        },
        "2": {
            "result": false
        },
        "3": {
            "relevance": [
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 0.3333333432674408
                }
            ],
            "result": true
        },
        "4": {
            "relevance": [
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 0.5
                }
            ],
            "result": true
        },
        "5": {
            "relevance": [
                {
                    "docid": 0,
                    "rank": 1.0
                },
                {
                    "docid": 1,
                    "rank": 1.0
                }
            ],
            "result": true
        },
        "6": {
            "relevance": [
                {
                    "docid": 0,
                    "rank": 1.0
                },
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 0.6666666865348816
                }
            ],
            "result": true
        },
        "7": {
            "relevance": [
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 1.0
                }
            ],
            "result": true
        },
        "8": {
            "relevance": [
                {
                    "docid": 2,
                    "rank": 1.0
                },
                {
                    "docid": 0,
                    "rank": 0.75
                },
                {
                    "docid": 1,
                    "rank": 0.75
                }
            ],
            "result": true
        }
    }
}

Требования

Компилятор с поддержкой C++20

CMake ≥ 3.10

Библиотека nlohmann/json