#include "context_collector.h"

// Обработка введённой строки.
handle_t ContextCollector::add_context(const std::size_t bulk_size)
{
    std::lock_guard<std::mutex> lg_mtx(collection_mutex);

    ++context_counter;

    contexts.insert(
        { context_counter, std::make_unique<Interpreter>(bulk_size, context_counter, io_ptr, iw_ptr) }
    );

    return context_counter;
}

// Обработка введённой строки.
void ContextCollector::handle_message(const handle_t context_id, const char* data, const std::size_t data_len)
{
    contexts_iter iter;

    {
        std::lock_guard<std::mutex> lg_mtx(collection_mutex);
        iter = contexts.find(context_id);
    }

    // Проверка, что передано корректное значение.
    if (iter == contexts.end()) {
        
        {
            std::lock_guard<std::mutex> lg(*console_mutex_ptr);
            std::cout << "Unable to handle message. Incorrect context id has been provided ! Value: " << context_id << std::endl;
        }

        return;
    }

    if (!iter->second) { return; }

    iter->second->handle_input(std::string(data, data + data_len));
}


void ContextCollector::stop_handling(const handle_t context_id)
{
    contexts_iter iter;

    {
        std::lock_guard<std::mutex> lg_mtx(collection_mutex);
        iter = contexts.find(context_id);
    }

    // Проверка, что передано корректное значение.
    if (iter == contexts.end()) {
        {
            std::lock_guard<std::mutex> lg(*console_mutex_ptr);
            std::cout << "Unable to stop handling. Incorrect context id has been provided ! Value: " << context_id << std::endl;
        }

        return;
    }

    if (!iter->second) { return; }

    // Завершаем текущий блок команд.
    iter->second->handle_eof();

    // Удаляем контекст из коллекции.
    {
        std::lock_guard<std::mutex> lg_mtx(collection_mutex);
        contexts.erase(context_id);
    }
}