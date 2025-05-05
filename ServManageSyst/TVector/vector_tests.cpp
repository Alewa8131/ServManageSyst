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
bool test_constructor_with_size() {
    TVector<int> vec(5);
    return TestSystem::check(5u, vec.size()) &&
        TestSystem::check(5u, vec.capacity()) &&
        TestSystem::check(false, vec.is_empty());
}
bool test_initializer_list_constructor() {
    TVector<int> vec = { 1, 2, 3 };
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(3, vec[2]) &&
        TestSystem::check(2, vec[1]) &&
        TestSystem::check(1, vec.front());
}
bool test_array_constructor() {
    int arr[] = { 10, 20, 30 };
    TVector<int> vec(arr, 3);
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(30, vec.back());
}


bool test_push_front() {
    TVector<int> vec;
    vec.push_front(42);
    vec.push_front(7);
    return TestSystem::check(2u, vec.size()) &&
        TestSystem::check(7, vec[0]) &&
        TestSystem::check(42, vec[1]);
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
bool test_erase() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8 };

    vec.erase(3); // 4
    vec.erase(0); // 1
    vec.erase(5); // 8 (теперь он по индексу 5)

    return TestSystem::check(5u, vec.size()) &&
        TestSystem::check(2, vec[0]) &&
        TestSystem::check(3, vec[1]) &&
        TestSystem::check(5, vec[2]) &&
        TestSystem::check(6, vec[3]) &&
        TestSystem::check(7, vec[4]);
}
bool test_insert_middle() {
    TVector<int> vec = { 1, 3 };
    vec.insert(1, 2); // [1, 2, 3]
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(2, vec[1]);
}
bool test_emplace_replace() {
    TVector<int> vec = { 10, 20, 30 };
    vec.emplace(1, 99);
    return TestSystem::check(99, vec[1]);
}
bool test_emplace_out_of_range() {
    TVector<int> vec = { 1, 2, 3 };
    try {
        vec.emplace(10, 42);
        return false;
    }
    catch (...) {
        return true;
    }
}
bool test_clear() {
    TVector<int> vec = { 1, 2, 3 };
    vec.clear();
    return TestSystem::check(0u, vec.size()) && TestSystem::check(true, vec.is_empty());
}
bool test_assign_fill() {
    TVector<int> vec;
    vec.assign(3, 7);
    return TestSystem::check(3, vec.size()) && TestSystem::check(7, vec[0]) && TestSystem::check(7, vec[2]);
}


bool test_shrink_to_fit() {
    TVector<int> vec = { 1, 2, 3 };
    vec.pop_back();
    vec.shrink_to_fit();
    return TestSystem::check(vec.size(), vec.capacity());
}
bool test_resize_expand() {
    TVector<int> vec = { 1, 2 };
    vec.resize(4);
    return TestSystem::check(4u, vec.size()) &&
        TestSystem::check(0, vec[3]);
}
bool test_resize_shrink() {
    TVector<int> vec = { 1, 2, 3, 4 };
    vec.resize(2);
    return TestSystem::check(2u, vec.size()) &&
        TestSystem::check(2, vec.back());
}


bool test_index_operator() {
    TVector<int> vec = { 5, 10, 15 };
    vec[1] = 100;
    return TestSystem::check(100, vec[1]);
}
bool test_at_operator() {
    TVector<int> vec = { 1, 2, 3 };
    try {
        return TestSystem::check(2, vec.at(1));
    }
    catch (...) {
        return false;
    }
}
bool test_at_operator_exception() {
    TVector<int> vec = { 1, 2 };
    try {
        vec.at(5);
        return false;
    }
    catch (const std::out_of_range&) {
        return true;
    }
}
bool test_equals_operator() {
    TVector<int> a = { 1, 2, 3 };
    TVector<int> b = { 1, 2, 3 };
    return TestSystem::check(true, a == b);
}
bool test_not_equals_operator() {
    TVector<int> a = { 1, 2, 3 };
    TVector<int> b = { 1, 2 };
    return TestSystem::check(true, a != b);
}



int main() {
    setlocale(LC_ALL, "Russian");
    TestSystem::print_init_info();

    // Конструкторы
    RUN_TEST(test_default_constructor);
    RUN_TEST(test_constructor_with_size);
    RUN_TEST(test_initializer_list_constructor);
    RUN_TEST(test_array_constructor);

    // Изменения элементов
    RUN_TEST(test_push_front);
    RUN_TEST(test_push_back);
    RUN_TEST(test_pop_back);
    RUN_TEST(test_erase);
    RUN_TEST(test_insert_middle);
    RUN_TEST(test_emplace_replace);
    RUN_TEST(test_emplace_out_of_range);
    RUN_TEST(test_clear);
    RUN_TEST(test_assign_fill);

    // Работа с памятью
    RUN_TEST(test_shrink_to_fit);
    RUN_TEST(test_resize_expand);
    RUN_TEST(test_resize_shrink);

    // Операторы
    RUN_TEST(test_index_operator);
    RUN_TEST(test_at_operator);
    RUN_TEST(test_at_operator_exception);
    RUN_TEST(test_equals_operator);
    RUN_TEST(test_not_equals_operator);



    TestSystem::print_final_info();
    return 0;
}
