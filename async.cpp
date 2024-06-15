#include "async.h"

namespace async {
    // Хранитель контекстов.
    ContextCollector context_collector;

    // Создает контекст для реализации многопоточной работы.
    handle_t connect(std::size_t bulk) {
        return context_collector.add_context(bulk);
    }

    // Обработка данных указанным контекстом.
    void receive(handle_t handle, const char* data, std::size_t size) {
        context_collector.handle_message(handle, data, size);
    }

    // Прекращение обработки данных.
    void disconnect(handle_t handle) {
        context_collector.stop_handling(handle);
    }
}