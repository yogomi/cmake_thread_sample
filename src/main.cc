// Copyright 2017 Makoto Yano

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <random>
#include <functional>

void ThreadFunc(std::mutex *mutex) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint32_t> random(10, 300);
  std::function<std::chrono::milliseconds()> random_milliseconds = [&]() {
    return std::chrono::milliseconds(random(mt));
  };

  for (int i = 0; i < 100; i++) {
    std::this_thread::sleep_for(random_milliseconds());
    std::lock_guard<std::mutex> lock(*mutex);
    std::cout << "thread_id : " << std::this_thread::get_id() << ", count : " << i << std::endl;
  }
}

int main() {
  std::mutex mutex;

  std::thread th1(ThreadFunc, &mutex);
  std::thread th2(ThreadFunc, &mutex);
  std::thread th3(ThreadFunc, &mutex);

  th1.join();
  th2.join();
  th3.join();

  return 0;
}
