#pragma once

#include "../concurrent/concurrent_queue.h"
#include "../Interfaces/interface_writer.h" 
#include "../threads_joiner.h"

#include <atomic>
#include <boost/format.hpp> 
#include <condition_variable> 

#include <iostream>

/**
* @brief Класс для записи в файлы из двух потоков. Содержит потокобезопасную очередь данных для записи.
*/
class FileWriter final : public IWriter
{

public:
    FileWriter(const uint32_t thr_number, std::shared_ptr<std::mutex>& cm_ptr)
        : console_mutex_ptr(cm_ptr)
        , threads_joiner(threads)
    {
        const uint32_t max_thr_number = std::thread::hardware_concurrency();
        const uint32_t threads_number = (thr_number > max_thr_number ? max_thr_number : thr_number);

        threads.reserve(threads_number);

        for (uint32_t i = 0; i < threads_number; ++i) {
            threads.emplace_back(&FileWriter::worker_thread, this, i);
            ++active_threads_number;
        }

        std::cout << "File writer - Constructor" << std::endl;
    }

    ~FileWriter() {
        if (!done) {
            for (unsigned int i = 0; i < threads.size(); ++i) {
                if (threads[i].joinable()) {
                    threads[i].join();
                }
            }
        }

        std::cout << "File writer - Destructor" << std::endl;
    }
   
    /**
    * Добавление данных в потокобезопасную очередь.
    * @param hid номер контекста.
    * @param time время. 
    * @param data данные.
    */
    void add_data(const std::uint32_t& hid, const std::uint64_t& time, const std::string& data) override {
        // Добавляем данные в коллекцию.
        data_to_write.push(hid, time, data);
        
        // Оповещаем ожидающий поток.
        cond_var.notify_one();
    }

    /**
    * @return является ли очередь с данными пустой.
    */
    bool empty() override {
        // Пытаемся получить доступ к очереди с данными.
        std::lock_guard<std::mutex> q_lock(data_mutex);

        return data_to_write.empty();
    }

    /**
    * @return есть ли хотя бы один активный поток.
    */
    bool active() override {
        return (active_threads_number > 0);
    }

    /**
    * Сообщить потокам о прекращении обработки данных.
    */
    void stop_process() override {
        done = true;
        for (unsigned int i = 0; i < threads.size(); ++i) {
            cond_var.notify_one();
        }
    }

private: // Methods

    /**
    * Метод, работающий в отдельном потоке.
    * @param thread_id номер потока.
    */
    void worker_thread(const uint32_t thread_id);

private: // Data
    // Мьютекс для синхронизации вывода на консоль из разных потоков.
    std::shared_ptr<std::mutex> console_mutex_ptr;

    std::condition_variable cond_var; // Для нотификации появления данных в очереди.
    std::mutex data_mutex; // Мьютекс для синхронизации доступа к коллекции данных.

    ConcurrentQueue<std::string> data_to_write; // Коллекция данных.

    std::vector<std::thread> threads; // Потоки для выполнения.

    ThreadsJoiner threads_joiner; // Для завершения потоков наверняка.
};