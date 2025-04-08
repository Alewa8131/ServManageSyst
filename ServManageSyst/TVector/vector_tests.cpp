#include <clocale>
#include <iostream>
#include <windows.h>
#include <initializer_list>

#include <my_vector.h>

void set_color(int text_color, int bg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | text_color);
}

#define RUN_TEST(test) TestSystem::start_test(test, #test)

namespace TestSystem {
    int count_success = 0, count_failed = 0;

    void start_test(bool(*test)(), const char* name_of_test) {
        set_color(2, 0);
        std::cout << "[ RUN      ] ";
        set_color(7, 0);
        std::cout << "Object." << name_of_test << std::endl;

        bool status = test();

        if (status == true) {
            set_color(2, 0);
            std::cout << "[       OK ]" << std::endl;
            count_success++;
        }
        else {
            set_color(4, 0);
            std::cout << "[  FAILED  ]" << std::endl;
            count_failed++;
        }
        set_color(7, 0);
    }

    template <class T1, class T2>
    bool check(const T1& expected, const T2& actual) {
        if (expected == actual) {
            return true;
        }
        else {
            std::cerr << "Expected: " << expected << ", Actual: " << actual << std::endl;
            return false;
        }
    }

    void print_init_info() {
        set_color(2, 0);
        std::cout << "[==========] " << std::endl;
        set_color(7, 0);
    }

    void print_final_info() {
        set_color(2, 0);
        std::cout << "[==========] ";
        set_color(7, 0);
        std::cout << count_success + count_failed << " test" << (count_success + count_failed > 1 ? "s" : "") << " ran." << std::endl;
        set_color(2, 0);
        std::cout << "[  PASSED  ] ";
        set_color(7, 0);
        std::cout << count_success << " test" << (count_success > 1 ? "s" : "") << std::endl;
        if (count_failed > 0) {
            set_color(4, 0);
            std::cout << "[  FAILED  ] ";
            set_color(7, 0);
            std::cout << count_failed << " test" << (count_failed > 1 ? "s." : ".") << std::endl;
        }
    }
};

bool test_default_constructor() {
    TVector<int> vec;
    return TestSystem::check(0u, vec.size()) &&
        TestSystem::check(0u, vec.capacity());
}

bool test_push_back() {
    TVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    return TestSystem::check(2, vec.size()) &&
        TestSystem::check(10, vec[0]) &&
        TestSystem::check(20, vec[1]);
}

bool test_pop_back() {
    TVector<int> vec = { 1, 2, 3 };
    vec.pop_back();
    return TestSystem::check(2u, vec.size()) &&
        TestSystem::check(2, vec.back());
}



int main() {
    setlocale(LC_ALL, "Russian");
    TestSystem::print_init_info();

    RUN_TEST(test_default_constructor);
    RUN_TEST(test_push_back);
    RUN_TEST(test_pop_back);


    TestSystem::print_final_info();
    return 0;
}
