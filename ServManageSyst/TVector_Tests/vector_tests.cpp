// Copyright 2025 Alewa8131
#include <windows.h>
#include <clocale>
#include <ctime>

#include <iostream>
#include <random>

#include "../TVector/my_vector.h"


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
    } else {
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
    } else {
        std::cerr << "Expected: " << expected
            << ", Actual: " << actual << std::endl;
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
    std::cout << count_success + count_failed << " test" <<
        (count_success + count_failed > 1 ? "s" : "") << " ran." << std::endl;
    set_color(2, 0);
    std::cout << "[  PASSED  ] ";
    set_color(7, 0);
    std::cout << count_success << " test" <<
        (count_success > 1 ? "s" : "") << std::endl;
    if (count_failed > 0) {
        set_color(4, 0);
        std::cout << "[  FAILED  ] ";
        set_color(7, 0);
        std::cout << count_failed << " test" <<
            (count_failed > 1 ? "s." : ".") << std::endl;
    }
}
};  // namespace TestSystem


bool test_default_constructor() {
    TVector<int> vec;
    vec.resize(15);
    return TestSystem::check(0u, vec.size()) &&
        TestSystem::check(15u, vec.capacity());
}
bool test_constructor_with_size() {
    TVector<int> vec(5);
    return TestSystem::check(5u, vec.capacity()) &&
        TestSystem::check(true, vec.is_empty());
}
bool test_initializer_list_constructor() {
    TVector<int> vec = { 1, 2, 3 };
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(3, vec.at(2)) &&
        TestSystem::check(2, vec.at(1)) &&
        TestSystem::check(1, vec.front());
}
bool test_array_constructor() {
    int arr[] = { 10, 20, 30 };
    TVector<int> vec(arr, 3);
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(30, vec.back());
}
bool test_null_memory() {
    TVector<int> vec = { 1, 2, 3 };
    vec.pop_back();
    vec.pop_back();
    vec.pop_back();
    vec.shrink_to_fit();
    return TestSystem::check(0u, vec.size()) &&
        TestSystem::check(0u, vec.capacity());
}


bool test_push_front() {
    TVector<int> vec;
    vec.push_front(42);
    vec.push_front(7);
    return TestSystem::check(2u, vec.size()) &&
        TestSystem::check(7, vec.at(0)) &&
        TestSystem::check(42, vec.at(1));
}
bool test_push_front_after_delete() {
    TVector<int> vec = {1, 2, 3, 4};
    vec.pop_front();
    vec.push_front(7);
    return TestSystem::check(4u, vec.size()) &&
        TestSystem::check(7, vec.at(0));
}
bool test_push_back() {
    TVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    return TestSystem::check(2, vec.size()) &&
        TestSystem::check(10, vec.at(0)) &&
        TestSystem::check(20, vec.at(1));
}
bool test_pop_back() {
    TVector<int> vec = { 1, 2, 3 };
    vec.pop_back();
    return TestSystem::check(2u, vec.size()) &&
        TestSystem::check(2, vec.back());
}
bool test_pop_two_back() {
    TVector<int> vec = { 1, 2, 3 };
    vec.pop_back();
    vec.pop_back();
    return TestSystem::check(1u, vec.size()) &&
        TestSystem::check(1, vec.back()) &&
        TestSystem::check(1, vec.front());
}
bool test_erase() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8 };

    vec.erase(3);
    vec.erase(0);
    vec.erase(5);

    return TestSystem::check(5u, vec.size()) &&
        TestSystem::check(2, vec.at(0)) &&
        TestSystem::check(3, vec.at(1)) &&
        TestSystem::check(5, vec.at(2)) &&
        TestSystem::check(6, vec.at(3)) &&
        TestSystem::check(7, vec.at(4));
}
bool test_insert_middle() {
    TVector<int> vec = { 1, 3 };
    vec.insert(1, 2);  // [1, 2, 3]
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(2, vec.at(1));
}
bool test_emplace_replace() {
    TVector<int> vec = { 10, 20, 30 };
    vec.emplace(1, 99);
    return TestSystem::check(99, vec.at(1));
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
    return TestSystem::check(0u, vec.size()) &&
        TestSystem::check(true, vec.is_empty());
}
bool test_clear_then_push_back() {
    TVector<int> vec = { 1, 2, 3 };
    vec.clear();
    vec.push_back(99);
    return TestSystem::check(1u, vec.size()) &&
        TestSystem::check(99, vec.front());
}
bool test_assign_fill() {
    TVector<int> vec;
    vec.assign(3, 7);
    return TestSystem::check(3, vec.size()) &&
        TestSystem::check(7, vec.at(0)) && TestSystem::check(7, vec.at(2));
}


bool test_shrink_to_fit_clears_deleted() {
    TVector<int> vec = { 1, 2, 3, 4 };
    vec.erase(1);
    vec.shrink_to_fit();
    return TestSystem::check(3u, vec.size()) &&
        TestSystem::check(4u, vec.capacity()) &&
        TestSystem::check(1, vec[0]) &&
        TestSystem::check(3, vec[1]) &&
        TestSystem::check(4, vec[2]);
}
bool test_shrink_to_fit_empty_vector() {
    TVector<int> vec;
    vec.push_back(5);
    vec.clear();
    vec.shrink_to_fit();
    return TestSystem::check(0u, vec.size()) &&
        TestSystem::check(0u, vec.capacity());
}

bool test_shrink_to_fit_no_action_when_full() {
    TVector<int> vec;
    for (int i = 0; i < 5; ++i)
        vec.push_back(i);
    size_t before = vec.capacity();
    vec.shrink_to_fit();
    return TestSystem::check(before, vec.capacity());
}
bool test_resize_expand() {
    TVector<int> vec = { 1, 2 };
    vec.resize(15);
    return TestSystem::check(15u, vec.capacity()) &&
        TestSystem::check(0, vec[3]);
}
bool test_resize_shrink() {
    TVector<int> vec = { 1, 2, 3, 4 };
    vec.resize(2);
    return TestSystem::check(2u, vec.size()) &&
        TestSystem::check(2, vec.back());
}
bool test_resize_expand_size() {
    TVector<int> vec = { 7 };
    vec.resize(5);
    return TestSystem::check(1u, vec.size()) &&
        TestSystem::check(true, vec.capacity() >= 5);
}
bool test_ensure_capacity_triggers_reserve() {
    TVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);  // _capacity filled
    size_t cap_before = vec.capacity();
    vec.push_back(3);  // should cause reserve()
    return TestSystem::check(true, vec.capacity() > cap_before);
}


bool test_index_operator() {
    TVector<int> vec = { 5, 10, 15 };
    vec[1] = 100;
    return TestSystem::check(100, vec.at(1));
}
bool test_at_operator() {
    TVector<int> vec = { 1, 2, 3, 4 };
    try {
        vec.erase(1);
        return TestSystem::check(3, vec.at(1));
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

bool test_shuffle_empty_vector() {
    TVector<int> vec;
    shuffle(vec);
    return TestSystem::check(0u, vec.size());
}
bool test_shuffle_single_element() {
    TVector<int> vec;
    vec.push_back(777);
    shuffle(vec);
    return TestSystem::check(1u, vec.size()) && TestSystem::check(777, vec[0]);
}

bool test_hoar_sort_deleted() {
    TVector<int> vec = {5, 2, 9, 1, 4, 13, 18, 3};
    vec.resize(10);
    vec.erase(2);
    vec.erase(2);

    hoar_sort(vec);

    return TestSystem::check(6u, vec.size()) &&
        TestSystem::check(2, vec.at(0)) &&
        TestSystem::check(3, vec.at(1)) &&
        TestSystem::check(4, vec.at(2)) &&
        TestSystem::check(5, vec.at(3)) &&
        TestSystem::check(13, vec.at(4)) &&
        TestSystem::check(18, vec.at(5));
}
bool test_hoar_sort_empty() {
    TVector<int> vec;
    hoar_sort(vec);
    return TestSystem::check(0u, vec.size());
}
bool test_hoar_sort_duplicates() {
    TVector<int> vec = { 3, 1, 2, 1, 3 };

    hoar_sort(vec);

    return TestSystem::check(5u, vec.size()) &&
        TestSystem::check(1, vec.at(0)) &&
        TestSystem::check(1, vec.at(1)) &&
        TestSystem::check(2, vec.at(2)) &&
        TestSystem::check(3, vec.at(3)) &&
        TestSystem::check(3, vec.at(4));
}
bool test_hoar_sort_reverse_sorted() {
    TVector<int> vec = {5, 4, 3, 2, 1, 0, -1, -2};

    hoar_sort(vec);

    return TestSystem::check(8u, vec.size()) &&
        TestSystem::check(-2, vec.at(0)) &&
        TestSystem::check(-1, vec.at(1)) &&
        TestSystem::check(0, vec.at(2)) &&
        TestSystem::check(1, vec.at(3)) &&
        TestSystem::check(2, vec.at(4)) &&
        TestSystem::check(3, vec.at(5)) &&
        TestSystem::check(4, vec.at(6)) &&
        TestSystem::check(5, vec.at(7));
}

bool test_find_first_basic() {
    TVector<int> vec = {10, 20, 30, 20, 40};
    return TestSystem::check(1u, find_first(vec, 20)) &&
        TestSystem::check(0u, find_first(vec, 10)) &&
        TestSystem::check(4u, find_first(vec, 40)) &&
        TestSystem::check(-1, find_first(vec, 99));
}
bool test_find_last_basic() {
    TVector<int> vec = { 10, 20, 30, 20, 40 };
    return TestSystem::check(3u, find_last(vec, 20)) &&
        TestSystem::check(0u, find_last(vec, 10)) &&
        TestSystem::check(4u, find_last(vec, 40)) &&
        TestSystem::check(-1, find_last(vec, 99));
}
bool test_find_all_basic() {
    TVector<int> vec = {5, 7, 5, 1, 5, 3};
    TVector<size_t> result = find_all(vec, 5);
    if (!TestSystem::check(3u, result.size())) return false;
    return TestSystem::check(0u, result[0]) &&
        TestSystem::check(2u, result[1]) &&
        TestSystem::check(4u, result[2]);
}
bool test_find_all_empty_result() {
    TVector<int> vec = { 1, 2, 3, 4 };
    vec.push_back(5);
    TVector<size_t> result = find_all(vec, 0);
    return TestSystem::check(0u, result.size());
}


int main() {
    setlocale(LC_ALL, "Russian");
    TestSystem::print_init_info();

    // Constructors
    RUN_TEST(test_default_constructor);
    RUN_TEST(test_constructor_with_size);
    RUN_TEST(test_initializer_list_constructor);
    RUN_TEST(test_array_constructor);
    RUN_TEST(test_null_memory);

    // Changes in elements
    RUN_TEST(test_push_front);
    RUN_TEST(test_push_front_after_delete);
    RUN_TEST(test_push_back);
    RUN_TEST(test_pop_back);
    RUN_TEST(test_pop_two_back);
    RUN_TEST(test_erase);
    RUN_TEST(test_insert_middle);
    RUN_TEST(test_emplace_replace);
    RUN_TEST(test_emplace_out_of_range);
    RUN_TEST(test_clear);
    RUN_TEST(test_clear_then_push_back);
    RUN_TEST(test_assign_fill);

    // Work with memory
    RUN_TEST(test_shrink_to_fit_clears_deleted);
    RUN_TEST(test_shrink_to_fit_empty_vector);
    RUN_TEST(test_shrink_to_fit_no_action_when_full);
    RUN_TEST(test_resize_expand);
    RUN_TEST(test_resize_shrink);
    RUN_TEST(test_resize_expand_size);
    RUN_TEST(test_ensure_capacity_triggers_reserve);

    // Operators
    RUN_TEST(test_index_operator);
    RUN_TEST(test_at_operator);
    RUN_TEST(test_at_operator_exception);
    RUN_TEST(test_equals_operator);
    RUN_TEST(test_not_equals_operator);

    // Shuffle
    RUN_TEST(test_shuffle_empty_vector);
    RUN_TEST(test_shuffle_single_element);

    // Sort
    RUN_TEST(test_hoar_sort_deleted);
    RUN_TEST(test_hoar_sort_empty);
    RUN_TEST(test_hoar_sort_duplicates);
    RUN_TEST(test_hoar_sort_reverse_sorted);

    // Find
    RUN_TEST(test_find_first_basic);
    RUN_TEST(test_find_last_basic);
    RUN_TEST(test_find_all_basic);
    RUN_TEST(test_find_all_empty_result);

    TestSystem::print_final_info();
    return 0;
}
