#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <conio.h>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// Структуры данных
struct Construction {
    string name;
    string address;
    int totalFlats;
    int availableFlats;
    double pricePerFlat;
    string status;
};

struct User {
    string username;
    string password;
    double balance = 0.0;
    vector<string> purchasedFlats;
};

// Глобальные переменные
vector<Construction> constructions;
User currentUser;

// Вспомогательные функции
int get_console_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void print_centered(const string& text) {
    int console_width = get_console_width();
    int left_padding = (console_width - text.length()) / 2;
    if (left_padding < 0) left_padding = 0;
    cout << string(left_padding, ' ') << text << endl;
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}



// Функции работы с файлами
void loadConstructions() {
    constructions.clear();
    ifstream file("constructions.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data.size() >= 6) {
            Construction obj;
            obj.name = data[0];
            obj.address = data[1];
            obj.totalFlats = stoi(data[2]);
            obj.availableFlats = stoi(data[3]);
            obj.pricePerFlat = stod(data[4]);
            obj.status = data[5];
            constructions.push_back(obj);
        }
    }
    file.close();
}

void saveConstructions() {
    ofstream file("constructions.txt");
    for (const auto& obj : constructions) {
        file << obj.name << "," << obj.address << ","
            << obj.totalFlats << "," << obj.availableFlats << ","
            << obj.pricePerFlat << "," << obj.status << "\n";
    }
    file.close();
}

bool userExists(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (!data.empty() && data[0] == username) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void loadUserData() {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data.size() >= 3 && data[0] == currentUser.username) {
            currentUser.balance = stod(data[2]);
            if (data.size() > 3) {
                currentUser.purchasedFlats.assign(data.begin() + 3, data.end());
            }
            break;
        }
    }
    file.close();
}

void saveUserData() {
    vector<string> lines;
    string line;

    // Читаем все данные
    ifstream inFile("users.txt");
    while (getline(inFile, line)) {
        vector<string> data = split(line, ',');
        if (!data.empty() && data[0] == currentUser.username) {
            line = currentUser.username + "," + currentUser.password + "," + to_string(currentUser.balance);
            for (const auto& flat : currentUser.purchasedFlats) {
                line += "," + flat;
            }
        }
        lines.push_back(line);
    }
    inFile.close();

    // Записываем обновленные данные
    ofstream outFile("users.txt");
    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
}

// Функции меню
void registerUser() {
    system("cls");
    print_centered("=== Регистрация ===");

    User newUser;
    cout << "Введите имя пользователя: ";
    getline(cin, newUser.username);

    if (userExists(newUser.username)) {
        cout << "Пользователь уже существует!\n";
        Sleep(1000);
        return;
    }

    cout << "Введите пароль: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!newUser.password.empty()) {
                newUser.password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            newUser.password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << newUser.username << "," << newUser.password << ",0\n";
        file.close();
        cout << "Регистрация успешна!\n";
    }
    else {
        cout << "Ошибка открытия файла!\n";
    }
    Sleep(1000);
}

void loginUser() {
    system("cls");
    print_centered("=== Авторизация ===");

    string username, password;
    cout << "Введите имя пользователя: ";
    getline(cin, username);

    cout << "Введите пароль: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    ifstream file("users.txt");
    bool found = false;
    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data.size() >= 2 && data[0] == username && data[1] == password) {
            found = true;
            currentUser.username = username;
            currentUser.password = password;
            loadUserData();
            break;
        }
    }
    file.close();

    if (found) {
        cout << "Авторизация успешна!\n";
        Sleep(1000);
    }
    else {
        cout << "Неверный логин или пароль!\n";
        Sleep(1500);
    }
}

void addConstruction() {
    system("cls");
    print_centered("=== Добавление объекта ===");

    Construction newConstruction;
    cout << "Название объекта: ";
    getline(cin, newConstruction.name);

    cout << "Адрес: ";
    getline(cin, newConstruction.address);

    cout << "Всего квартир: ";
    while (!(cin >> newConstruction.totalFlats) || newConstruction.totalFlats <= 0) {
        cout << "Ошибка! Введите положительное число: ";
 
    }

    cout << "Доступно квартир: ";
    while (!(cin >> newConstruction.availableFlats) || newConstruction.availableFlats < 0 ||
        newConstruction.availableFlats > newConstruction.totalFlats) {
        cout << "Ошибка! Введите число от 0 до " << newConstruction.totalFlats << ": ";

    }

    cout << "Цена квартиры (руб): ";
    while (!(cin >> newConstruction.pricePerFlat) || newConstruction.pricePerFlat <= 0) {
        cout << "Ошибка! Введите положительное число: ";

    }


    cout << "Статус (планируется/в процессе/завершен): ";
    getline(cin, newConstruction.status);

    ofstream file("constructions.txt", ios::app);
    if (file.is_open()) {
        file << newConstruction.name << "," << newConstruction.address << ","
            << newConstruction.totalFlats << "," << newConstruction.availableFlats << ","
            << newConstruction.pricePerFlat << "," << newConstruction.status << "\n";
        file.close();
        cout << "Объект успешно добавлен!\n";
    }
    else {
        cout << "Ошибка открытия файла!\n";
    }
    Sleep(1000);
}

void viewConstructions(bool showAvailableOnly = false) {
    system("cls");
    loadConstructions();

    if (constructions.empty()) {
        print_centered("Нет доступных объектов.");
        Sleep(1000);
        return;
    }

    print_centered("=== Список объектов ===");
    for (size_t i = 0; i < constructions.size(); ++i) {
        const auto& obj = constructions[i];
        if (!showAvailableOnly || obj.availableFlats > 0) {
            cout << "[" << i + 1 << "] " << obj.name
                << "\n  Адрес: " << obj.address
                << "\n  Квартиры: " << obj.availableFlats << "/" << obj.totalFlats
                << "\n  Цена: " << fixed << setprecision(2) << obj.pricePerFlat << " руб."
                << "\n  Статус: " << obj.status << "\n---\n";
        }
    }
    system("pause");
}

void sortConstructions() {
    system("cls");
    print_centered("=== Сортировка объектов ===");
    cout << "1. По названию (А-Я)\n"
        << "2. По цене (по возрастанию)\n"
        << "3. По цене (по убыванию)\n"
        << "Выберите вариант: ";

    int choice;
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Ошибка! Введите число от 1 до 3: ";

    }


    loadConstructions();
    switch (choice) {
    case 1:
        sort(constructions.begin(), constructions.end(),
            [](const Construction& a, const Construction& b) { return a.name < b.name; });
        break;
    case 2:
        sort(constructions.begin(), constructions.end(),
            [](const Construction& a, const Construction& b) { return a.pricePerFlat < b.pricePerFlat; });
        break;
    case 3:
        sort(constructions.begin(), constructions.end(),
            [](const Construction& a, const Construction& b) { return a.pricePerFlat > b.pricePerFlat; });
        break;
    }

    saveConstructions();
    viewConstructions();
}

void searchConstructions() {
    system("cls");
    print_centered("=== Поиск объектов ===");
    cout << "Введите название для поиска: ";
    string query;
    getline(cin, query);

    loadConstructions();
    bool found = false;
    for (const auto& obj : constructions) {
        if (obj.name.find(query) != string::npos) {
            if (!found) {
                print_centered("=== Результаты поиска ===");
                found = true;
            }
            cout << "Название: " << obj.name
                << "\nАдрес: " << obj.address
                << "\nЦена: " << obj.pricePerFlat << " руб."
                << "\nСтатус: " << obj.status << "\n---\n";
        }
    }

    if (!found) {
        print_centered("Объекты не найдены.");
    }
    system("pause");
}

void purchaseFlat() {
    viewConstructions(true); // Только с доступными квартирами
    if (constructions.empty()) return;

    cout << "Выберите номер объекта: ";
    int objIndex;
    while (!(cin >> objIndex) || objIndex < 1 || objIndex > constructions.size()) {
        cout << "Ошибка! Введите число от 1 до " << constructions.size() << ": ";
    }


    Construction& obj = constructions[objIndex - 1];
    if (obj.availableFlats <= 0) {
        cout << "Квартиры закончились!\n";
        Sleep(1000);
        return;
    }

    cout << "\nВы выбрали:\n"
        << "Название: " << obj.name << "\n"
        << "Цена: " << obj.pricePerFlat << " руб.\n"
        << "Ваш баланс: " << currentUser.balance << " руб.\n\n"
        << "1. Подтвердить покупку\n"
        << "2. Отмена\n"
        << "Выберите действие: ";

    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Ошибка! Введите 1 или 2: ";

    }


    if (choice == 1) {
        if (currentUser.balance >= obj.pricePerFlat) {
            currentUser.balance -= obj.pricePerFlat;
            obj.availableFlats--;
            string flatInfo = obj.name + " (квартира №" + to_string(obj.totalFlats - obj.availableFlats) + ")";
            currentUser.purchasedFlats.push_back(flatInfo);

            saveConstructions();
            saveUserData();

            cout << "Покупка успешна! Остаток: " << currentUser.balance << " руб.\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
        Sleep(1500);
    }
}

void depositBalance() {
    system("cls");
    print_centered("=== Пополнение баланса ===");
    cout << "Текущий баланс: " << currentUser.balance << " руб.\n"
        << "Введите сумму для пополнения: ";

    double amount;
    while (!(cin >> amount) || amount <= 0) {
        cout << "Ошибка! Введите положительное число: ";
    }


    currentUser.balance += amount;
    saveUserData();
    cout << "Баланс успешно пополнен! Текущий баланс: " << currentUser.balance << " руб.\n";
    Sleep(1500);
}

void viewPersonalAccount() {
    system("cls");
    print_centered("=== Личный кабинет ===");
    cout << "Пользователь: " << currentUser.username << "\n"
        << "Баланс: " << currentUser.balance << " руб.\n\n";

    if (!currentUser.purchasedFlats.empty()) {
        cout << "Купленные квартиры:\n";
        for (const auto& flat : currentUser.purchasedFlats) {
            cout << " - " << flat << "\n";
        }
    }
    else {
        cout << "У вас нет купленных квартир.\n";
    }

    cout << "\n1. Пополнить баланс\n"
        << "2. Вернуться в меню\n"
        << "Выберите действие: ";

    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Ошибка! Введите 1 или 2: ";

    }


    if (choice == 1) {
        depositBalance();
    }
}

void userMenu() {
    int choice;
    do {
        system("cls");
        print_centered("=== Меню пользователя ===");
        cout << "1. Просмотр объектов\n"
            << "2. Сортировка объектов\n"
            << "3. Поиск объекта\n"
            << "4. Купить квартиру\n"
            << "5. Личный кабинет\n"
            << "6. Выйти\n"
            << "Выберите действие: ";

        while (!(cin >> choice) || choice < 1 || choice > 6) {
            cout << "Ошибка! Введите число от 1 до 6: ";

        }


        switch (choice) {
        case 1: viewConstructions(); break;
        case 2: sortConstructions(); break;
        case 3: searchConstructions(); break;
        case 4: purchaseFlat(); break;
        case 5: viewPersonalAccount(); break;
        case 6: return;
        }
    } while (true);
}

void adminPanel() {
    int choice;
    do {
        system("cls");
        print_centered("=== Панель администратора ===");
        cout << "1. Добавить объект\n"
            << "2. Просмотреть все объекты\n"
            << "3. Выйти\n"
            << "Выберите действие: ";

        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Ошибка! Введите число от 1 до 3: ";

        }


        switch (choice) {
        case 1: addConstruction(); break;
        case 2: viewConstructions(); break;
        case 3: return;
        }
    } while (true);
}

void loginAdmin() {
    system("cls");
    print_centered("=== Вход для администратора ===");

    string adminUsername, adminPassword;
    cout << "Логин: ";
    getline(cin, adminUsername);

    cout << "Пароль: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!adminPassword.empty()) {
                adminPassword.pop_back();
                cout << "\b \b";
            }
        }
        else {
            adminPassword.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    ifstream file("admin.txt");
    bool isAdmin = false;
    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data.size() >= 2 && data[0] == adminUsername && data[1] == adminPassword) {
            isAdmin = true;
            break;
        }
    }
    file.close();

    if (isAdmin) {
        cout << "Доступ разрешен!\n";
        Sleep(1000);
        adminPanel();
    }
    else {
        cout << "Неверные данные!\n";
        Sleep(1500);
    }
}

void showMenu() {
    int choice;
    do {
        system("cls");
        print_centered("                                  . - +-.                                           ");
        print_centered("                               :=*#####*=:.                                         ");
        print_centered("                            .=+###+-. .-+###+=:                                     ");
        print_centered("                        .-+###*=:    :-.  :=*###+-.:########*                       ");
        print_centered("                     :=*###+-.       =##*=:  .-+###*##=:::*#*                       ");
        print_centered("                 .-+###+=:            :=*###+-.  .=*##:   +#*                       ");
        print_centered("              :=*##*=:.              ::  .-=###*=:   -.   +#*.                      ");
        print_centered("          .-+###+-.                  =##+-.  .-+###+-.    =###+-.                   ");
        print_centered("      .-+*##*=:                      .:=*##*=:. .:=*##*=:.  :=*##*+-.               ");
        print_centered("   :=*###+-.                         ..  .-+###+-.  .-+###+=.  .-+###*=:            ");
        print_centered(".-++++=:                             =#*=-   :=*##*+-.  :=*##:     :=++++-.         ");
        print_centered("        =##                          .-+###*=:  .-+###*=:  .-. =##                  ");
        print_centered("        -##.                         ..  :=*###+-.  :=*###+-.  =##                  ");
        print_centered("        -##.                         =#*=:  .-+###*=:  .-+*##: =##                  ");
        print_centered("        -##.                         .=*###+-.  :=*###+-.  .=. =##                  ");
        print_centered("        -##.                         .  .-+*##*=:  .:=*##*=:   =##                  ");
        print_centered("        -##.                         =#+-.  .-+###+-.  .-+###: =##                  ");
        print_centered("        -##.                         :=*##*=:   :=*##*=:.  :=. =##                  ");
        print_centered("        -##.                            .-+###+-.  .-+###+-:   =##                  ");
        print_centered("        -##.                         =*=:   :=*##*=:   :=*##*. =##                  ");
        print_centered("        -##.                                                   =##                  ");
        print_centered("        -#########################################################                  ");
        cout << endl;
        print_centered(" Строительная компания 'BuildTech'");
        cout << endl;
        print_centered("=== Главное меню ===");
        cout << "1. Регистрация\n"
            << "2. Авторизация\n"
            << "3. Вход для администратора\n"
            << "4. Выход\n"
            << "Выберите действие: ";

        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Ошибка! Введите число от 1 до 4: ";

        }


        switch (choice) {
        case 1: registerUser(); break;
        case 2:
            loginUser();
            if (!currentUser.username.empty()) {
                userMenu();
            }
            break;
        case 3: loginAdmin(); break;
        case 4: return;
        }
    } while (true);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("color f1");

    // Создаем файлы при первом запуске
    ofstream adminFile("admin.txt", ios::app);
    adminFile << "admin,admin123\n";
    adminFile.close();

    ofstream constructionsFile("constructions.txt", ios::app);
    constructionsFile.close();

    ofstream usersFile("users.txt", ios::app);
    usersFile.close();

    showMenu();
    return 0;
}