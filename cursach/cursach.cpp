#include <iostream>
#include <Windows.h>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;
int get_console_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}   
void print_centered(const string& text) {
    int console_width = 80;  

#ifdef _WIN32
    console_width = get_console_width();
#else
#endif
    int left_padding = (console_width - text.length()) / 2;
    if (left_padding < 0) left_padding = 0;  
    cout << string(left_padding, ' ') << text << endl;
}
struct User {
    string username;
    string password;
};

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
void loginAdmin() {
    ifstream file("admin.txt");
    if (!file.is_open()) {
        cout << "Файл администратора не найден!" << endl;
        return;
    }

    string adminUsername, adminPassword;
    cout << "=== Вход для администратора ===" << endl;
    cout << "Введите логин администратора: ";
    cin >> adminUsername;
    cout << "Введите пароль администратора: ";
    cin >> adminPassword;

    string line;
    bool isAdmin = false;
    while (getline(file, line)) {
        vector<string> adminData = split(line, ',');
        if (adminData.size() >= 2 && adminData[0] == adminUsername && adminData[1] == adminPassword) {
            isAdmin = true;
            break;
        }
    }
    file.close();

    if (isAdmin) {
        cout << "Доступ разрешен. Добро пожаловать, администратор!" << endl;
    }
    else {
        cout << "Неверные данные администратора!" << endl;
    }
    system("cls");
}

void registerUser() {
    ofstream file("users.txt", ios::app); // Открываем файл для добавления данных
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    User newUser;
    cout << "=== Регистрация ===" << endl;
    cout << "Введите имя пользователя: ";
    cin >> newUser.username;
    cout << "Введите пароль: ";
    cin >> newUser.password;

    file << newUser.username << "," << newUser.password << "\n";
    file.close();

    cout << "Регистрация прошла успешно!" << endl;
    system("cls");
}

void loginUser() {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cout << "Ошибка открытия файла! Пользователи не найдены." << endl;
        return;
    }

    string username, password;
    cout << "=== Авторизация ===" << endl;
    cout << "Введите имя пользователя: ";
    cin >> username;
    cout << "Введите пароль: ";
    cin >> password;

    string line;
    bool found = false;
    while (getline(file, line)) {
        vector<string> userData = split(line, ',');
        if (userData.size() >= 2 && userData[0] == username && userData[1] == password) {
            found = true;
            break;
        }
    }
    file.close();
    if (found) {
        cout << "Авторизация успешна! Добро пожаловать, " << username << "!" << endl; 
    }
    else {
        cout << "Неверный логин или пароль!" << endl;
    }
    Sleep(1000);
    system("cls");
}

void showMenu() {
    int choice;
    do {
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
        print_centered( "=== Меню ===");
        print_centered("1. Регистрация");
        print_centered("2. Авторизация");
        print_centered("3. Войти как администратор");
        print_centered( "4. Выход");
        print_centered( "Выберите опцию: ");
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser();
            break;
        case 3:
            loginAdmin();
            break;
        case 4:
            cout << "Выход из программы..." << endl;
            break;
        default:
            cout << "Неверный выбор!" << endl;
            Sleep(500);
            system("cls");
        }
    } while (choice != 4);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("color f1");
    ofstream adminFile("admin.txt");
    if (adminFile.is_open()) {
        adminFile << "admin,admin123\n";
        adminFile.close();
    }
    showMenu();
    return 0;
}