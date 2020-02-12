#include <iostream>
#include <chrono>
#include <atomic>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include "algorithm"

typedef long long (*testFunction)(int number);


long long int_to_str_1_spintf(int number);

long long int_to_str_2_to_string(int number);

long long int_to_str_3_string_stream(int number);

long long int_to_str_4_own(int number);

char buf[256];
std::string str_buf;
std::vector<int> values;

testFunction functions[4] = {int_to_str_1_spintf, int_to_str_2_to_string, int_to_str_3_string_stream, int_to_str_4_own};

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced();

int main(int argc, char **argv) {
    // Read data from argv
    if (argc != 4) {
        std::cout << "Need 4 command line arguments, found " << argc << std::endl;
    }
    int type = atoi(argv[1]);

    if (type < 1 || type > 4) {
        std::cout << "Wrong type, only 1-4 is allowed" << std::endl;
    }
    type--;

    // read the numbers from file to values vector
    std::fstream file_in(argv[2], std::ios_base::in);
    int n;
    while (file_in >> n) {
        values.push_back(n);
    }
    file_in.close();

    // main loop

    unsigned long long total_time = 0, total_length = 0;
    for (int number: values) {
        total_time += functions[type](number);
        // Case with sprintf
        if (type == 0) {
            total_length += strlen(buf);
        } else {
            total_length += str_buf.length();
        }
    }

    std::cout << "Result in nanoseconds\n" << total_time << std::endl;

    // write the result to file
    std::ofstream file_out(argv[3], std::ios_base::out);
    file_out << total_length;
    file_out.close();
}

long long int_to_str_1_spintf(int number) {
    auto start_time = get_current_time_fenced();

    std::sprintf(buf, "%i", number);

    auto finish_time = get_current_time_fenced();
    auto int_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time);
    std::chrono::duration<long long, std::nano> long_sec = int_ns;
    return long_sec.count();
}

long long int_to_str_2_to_string(int number) {
    auto start_time = get_current_time_fenced();

    str_buf = std::to_string(number);

    auto finish_time = get_current_time_fenced();
    auto int_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time);
    std::chrono::duration<long long, std::nano> long_sec = int_ns;
    return long_sec.count();
}

long long int_to_str_3_string_stream(int number) {
    std::stringstream ss;
    auto start_time = get_current_time_fenced();

    ss << number;
    ss >> str_buf;

    auto finish_time = get_current_time_fenced();
    auto int_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time);
    std::chrono::duration<long long, std::nano> long_sec = int_ns;
    return long_sec.count();
}

long long int_to_str_4_own(int number) {
    auto start_time = get_current_time_fenced();

    str_buf.clear();
    while (number > 0) {
        str_buf.push_back(number % 10 + '0');
        number /= 10;
    }
    std::reverse(str_buf.begin(), str_buf.end());

    auto finish_time = get_current_time_fenced();
    auto int_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time);
    std::chrono::duration<long long, std::nano> long_sec = int_ns;
    return long_sec.count();
}


inline std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}