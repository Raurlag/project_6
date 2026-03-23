// project_6.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

//ЗАДАЧА 1. В массиве представлены предельные нагрузки сети за каждый час
// в течение суток.Необходимо выяснить, какое самое напряженное время для
// системных администраторов(утро, день, вечер или ночь).



#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Period {
    int startHour;      // Час начала периода (0-23)
    int endHour;        // Час конца периода (0-23)
    string name;
};

// ФУНКЦИЯ: находит максимальный элемент в заданном диапазоне массива
// Параметры:
//   loads[] - массив нагрузок
//   start   - начало
//   end     - конец
//   maxValue - ссылка, куда сохранится максимальное значение
int findMaxInRange(int loads[], int start, int end, int& maxValue) {
    int maxIndex = start;
    maxValue = loads[start];

    for (int i = start + 1; i <= end; i++) {
        if (loads[i] > maxValue) {
            maxValue = loads[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

int main() {
    setlocale(LC_ALL, "Ru");
    srand(time(0));

    const int HOURS = 24;
    int loads[HOURS];

    cout << "Нагрузки сети за каждый час:\n";
    cout << "Час\tНагрузка\n";
    cout << "-----------------\n";
    for (int i = 0; i < HOURS; i++) {
        loads[i] = rand() % 1001;
        cout << i << ":00\t" << loads[i] << "\n";
    }

    // Все данные о периодах
    const int PERIODS_COUNT = 4;
    Period periods[PERIODS_COUNT] = {
        {6, 11, "Утро (6:00-11:59)"},
        {12, 17, "День (12:00-17:59)"},
        {18, 23, "Вечер (18:00-23:59)"},
        {0, 5, "Ночь (0:00-5:59)"}
    };

    cout << "\n=== Самые напряженные часы ===\n";

    try {
        // ЦИКЛ ПО МАССИВУ ПЕРИОДОВ
        for (int i = 0; i < PERIODS_COUNT; i++) {
            int maxValue;

            // Получаем данные из структуры через оператор точка (.)
            int startHour = periods[i].startHour;
            int endHour = periods[i].endHour;
            string periodName = periods[i].name;

            // Находим час с максимальной нагрузкой в этом периоде
            int maxIndex = findMaxInRange(loads, startHour, endHour, maxValue);

            // Выводим результат
            cout << periodName << ": пик - " << maxIndex << ":00 (" << maxValue << ")\n";
        }
    }
    catch (...) {
        cout << "Ошибка при обработке данных!\n";
    }

    return 0;
}


//ЗАДАЧА 2. Дан текст в формате HTML.Найти в нем все гиперссылки. 
//Гиперссылки могут быть такими:
//https:/www.susu.ru/ru
//https:/ya.ru/?npr=1&utm_referrer=https%3A%2F%2Fyandex.ru%2F
//http:/web-archive.com

/*
#include <iostream>
#include <string>
using namespace std;

//проверка URL-ом с проверкой длины
bool isValidURL(const string& url) {
    // Проверка на пустую строку
    if (url.length() == 0) {
        return false;
    }

    // Проверка на минимум
    if (url.length() < 8) {
        return false;
    }

    // Проверка протокола
    if (url.find("http://") == 0 || url.find("https://") == 0) {
        return true;
    }
    return false;
}

//извлечение всех гиперссылок из текста
int extractLinks(const string& text, string links[], int maxLinks) {
    int linkCount = 0;
    size_t pos = 0;

    try {
        while (linkCount < maxLinks) {
            // Ищем начало ссылки: http:// или https://
            size_t httpPos = text.find("http://", pos);
            size_t httpsPos = text.find("https://", pos);

            size_t startPos;
            string protocol;

            // Выбираем ближайшую найденную ссылку
            if (httpPos != string::npos && httpsPos != string::npos) {
                if (httpPos < httpsPos) {
                    startPos = httpPos;
                    protocol = "http://";
                }
                else {
                    startPos = httpsPos;
                    protocol = "https://";
                }
            }
            else if (httpPos != string::npos) {
                startPos = httpPos;
                protocol = "http://";
            }
            else if (httpsPos != string::npos) {
                startPos = httpsPos;
                protocol = "https://";
            }
            else {
                break;
            }

            //Поиск конца URL после протокола
            size_t urlStart = startPos + protocol.length();
            size_t endPos = string::npos;

            //Разделители
            size_t quotePos = text.find('"', startPos);
            size_t apostrophePos = text.find('\'', startPos);
            size_t spacePos = text.find(' ', startPos);

            //Ближайший разделитель
            if (quotePos != string::npos) {
                endPos = quotePos;
            }
            if (apostrophePos != string::npos && (endPos == string::npos || apostrophePos < endPos)) {
                endPos = apostrophePos;
            }
            if (spacePos != string::npos && (endPos == string::npos || spacePos < endPos)) {
                endPos = spacePos;
            }

            // Если разделитель не найден, берём до конца строки
            if (endPos == string::npos) {
                endPos = text.length();
            }

            // Извлекаем URL
            string url = text.substr(startPos, endPos - startPos);

            // Проверка: URL не пустой и валидный
            if (url.length() > protocol.length() && isValidURL(url)) {
                links[linkCount] = url;
                linkCount++;
            }

            //Поиск после найденной ссылки
            pos = startPos + 1;
        }
    }
    catch (...) {
        cout << "Ошибка при обработке текста!\n";
    }

    return linkCount;
}

int main() {
    setlocale(LC_ALL, "Ru");

    //ТЕКСТ ДЛЯ ТЕСТА
    string text =
        "Вот список ссылок:\n"
        "https://www.susu.ru/ru - сайт университета\n"
        "https://ya.ru/?npr=1&utm_referrer=https%3A%2F%2Fyandex.ru%2F это Яндекс\n"
        "http://web-archive.com архив\n"
        "https://github.com\n"
        "https://stackoverflow.com вопрос\n"
        "/local-link это локальная ссылка (не будет найдена)\n"
        "ftp://example.com FTP ссылка (не будет найдена)\n"
        "Обычный текст с ссылкой https://example.com в тексте\n"
        "Ссылка в кавычках \"https://google.com\"\n"
        "Ссылка в апострофах 'https://yandex.ru'\n"
        "Ссылка с пробелом https://site.com/page и продолжение текста\n"
        "https://en.cppreference.com - полезный ресурс\n"
        "http://site.com\n";

    // Выводим текст
    cout << "=== ТЕКСТ ДЛЯ ПОИСКА ===\n";
    cout << text << "\n\n";

    // Массив для хранения найденных ссылок
    const int MAX_LINKS = 500;
    string links[MAX_LINKS];

    // Извлекаем ссылки
    cout << "=== ПОИСК ГИПЕРССЫЛОК ===\n";
    int linkCount = extractLinks(text, links, MAX_LINKS);

    // Выводим результаты
    if (linkCount == 0) {
        cout << "Гиперссылки не найдены.\n";
    }
    else {
        cout << "Найдено валидных гиперссылок (http:// или https://): " << linkCount << "\n\n";
        for (int i = 0; i < linkCount; i++) {
            cout << i + 1 << ". " << links[i] << "\n";
        }
    }

    return 0;
}

*/


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
