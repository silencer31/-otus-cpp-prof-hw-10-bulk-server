#pragma once

#include <vector>
#include <string>
#include <memory>
#include <chrono>

#include "Observ/observer.h"

using command_iterator = std::vector<std::string>::const_iterator;
using file_time = std::chrono::system_clock::time_point;

// Тип введённых данных.
enum class InputType {
	NO_INPUT,
	COMMAND,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	END_INPUT
};

// Текущий тип хранилища команд.
enum class StorageType {
	UNKNOWN_T,
	STATIC_T,
	DYNAMIC_T
};

/**
* @brief Класс, содержащий коллекцию команд и управляющий ею.
*/
class Collector : public Observable {
public:
	Collector() = delete;

	explicit Collector(const std::size_t bulk_size)
		: collection_max_size(bulk_size)
	{
		commands_collection.reserve(collection_max_size);
	}


	~Collector() {
		flush_collection(true);
	}
	
	/**
	* Обработка ввода.
	* @param input_type Тип введённых данных
	* @param command данные
	*/
	void handle_input(InputType input_type, const std::string& command);

	/**
	*  Вывод всей коллекции в консоль и запись в файл через оповещение наблюдателей.
	*  @param clear_flag - очищать ли коллекцию после вывода.
	*/
	void flush_collection(bool clear_flag);

	/**
	* Подготовка коллекции к новому вводу неизвестных данных.
	*/
	void reset_collection();

	/**
	* Функция для проверки корректного создания файлов в рамках тестирования.
	* @return время создания файла.
	*/
	file_time get_file_time() {
		return first_command_time;
	}

	/**
	* @return константный итератор на начало коллекции.
	*/
	command_iterator get_iter_begin() {
		return commands_collection.cbegin();
	}

	/**
	* @return константный итератор на конец коллекции.
	*/
	command_iterator get_iter_end() {
		return commands_collection.end();
	}

private:
	const std::size_t collection_max_size;
	
	StorageType storage_type{ StorageType::UNKNOWN_T };
	
	int open_brackets_number{ 0 };
	int close_brackets_number{ 0 };

	std::vector<std::string> commands_collection;

	file_time first_command_time;
};