#pragma once

#include "interpreter.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <mutex>
#include <map>

#define FILE_THREADS_NUMBER 2

// Идентификатор контекста
using handle_t = std::uint32_t;

// Тип для коллекции контекстов
using contexts_collection = std::map<handle_t, std::unique_ptr<Interpreter>>;

using contexts_iter = std::map<handle_t, std::unique_ptr<Interpreter>>::const_iterator;

/**
* @brief Класс для сбора и управления контекстами обработки данных.
*/
class ContextCollector
{
public:
    ContextCollector()
        : console_mutex_ptr( std::make_shared<std::mutex>() )
        , io_ptr(std::make_shared<ConsoleWriter>(console_mutex_ptr))
        , iw_ptr(std::make_shared<FileWriter>(FILE_THREADS_NUMBER, console_mutex_ptr))
        , ig_ptr(std::make_shared<DataGatherer>(io_ptr, iw_ptr))
    {}

    ~ContextCollector() {
        ig_ptr->flush_pieces();

        // Ждём обработки всех отправленных данных.
        while (!iw_ptr->empty() || !io_ptr->empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Останавливаем потоки.
        io_ptr->stop_process();
        iw_ptr->stop_process();
        

        // Ждём корректного завершения всех потоков.
        while (iw_ptr->active() || io_ptr->active()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "\nContext Collector - Destructor" << std::endl;
    }

    /**
    * Добавление нового контекста.
    * @param bulk_size размер ограничения для порции данных
    * @return номер контекста
    */
    handle_t add_context(const std::size_t bulk_size);

    /**
    * Обработка введённой строки.
    * @param context_id номер контекста
    * @param data данные
    * @param data_len длина данных
    */
    void handle_message(const handle_t context_id, const char* data, const std::size_t data_len);

    /**
    * Прекращение процесса обработки для указанного контекста.
    * @param context_id номер контекста
    */
    void stop_handling(const handle_t context_id);
    
private:
    // Коллекция контекстов.
    contexts_collection contexts;
    
    // Мьютекс для синхронизации доступа к коллекци.
    std::mutex collection_mutex;
 
    // Счетчик контекстов.
    handle_t context_counter{ 0 };

    // Мьютекс для синхронизации вывода на консоль из разных потоков.
    std::shared_ptr<std::mutex> console_mutex_ptr;

    // Объект для вывода в консоль в отдельном потоке.
    const std::shared_ptr<IWriter> io_ptr;

    // Объект для вывода в файлы через пул потоков.
    const std::shared_ptr<IWriter> iw_ptr;

    // Объект для сбора и объединения порций данных.
    const std::shared_ptr<IGatherer> ig_ptr;
};