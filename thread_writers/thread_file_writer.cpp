#include "thread_file_writer.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <boost/format.hpp> 

// Метод, работающий в отдельном потоке.
void FileWriter::worker_thread(const uint32_t thread_id)
{
    {
        std::lock_guard<std::mutex> lg(*console_mutex_ptr);
        std::cout << "File writer thread " << thread_id << " started " << std::endl;
    }

    while (!done) {
        // Пытаемся получить доступ к коллекции с данными.
        std::unique_lock<std::mutex> data_lock(data_mutex);

        // Отпускаем мьютекс и ожидаем, пока в очереди появятся данные.
        cond_var.wait(data_lock, [this]()->bool { return !data_to_write.empty() || done; });

        // Мьютекс снова захвачен.

        if (data_to_write.empty()) { 
            data_lock.unlock();
            continue;
        }

        // Берём первый доступный элемент в коллекции с данными.
        const auto element = std::move(data_to_write.front());

        // Удаляем из очереди принятый в обработку элемент.
        data_to_write.pop();

        // Освобождаем доступ к коллекции.
        data_lock.unlock();

        // Записываем данные в файл.
        const auto hid  = std::get<0>(element); // handle (контекст) id
        const auto time = std::get<1>(element);

        std::string filename;
        uint64_t delta{ 0 };

        //const std::string filename = boost::str(boost::format("bulk_%1%_id[%2%]_%3%%4%.log") % time % thread_id % hid % delta);

        while (true) {
            filename = boost::str(boost::format("bulk_%1%_id[%2%]_%3%%4%.log") % time % thread_id % hid % delta);
            if (!std::filesystem::exists(filename)) {
                break;
            }
            ++delta;
        }

        std::ofstream file(filename, std::ofstream::out);
        
        if (file.is_open()) {
            file << std::get<2>(element);
        }
    }

    {
        std::lock_guard<std::mutex> lg(*console_mutex_ptr);
        std::cout << "File writer thread " << thread_id << " finished " << std::endl;
    }

    --active_threads_number;
}

