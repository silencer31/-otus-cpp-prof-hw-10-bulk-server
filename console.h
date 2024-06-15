#pragma once

#include "collector.h"
#include "thread_writers/thread_console_writer.h"

/**
* @brief Класс для вывода коллекции команд в консоль.
*/
class Console : public Observer {
public:
	Console() = delete;

	explicit Console(const std::shared_ptr<Collector>& cltr_ptr, const std::shared_ptr<IWriter>& io_ptr)
		: collector_ptr(cltr_ptr)
		, console_writer_ptr(io_ptr)
	{}

	~Console() = default;

	/**
	* Вывод данных в консоль.
	* @param c_begin итератор на начало данных
	* @param c_end итератор на окончание данных
	*/
	void output_to_console(const command_iterator& c_begin, const command_iterator& c_end);

	/**
	* Реакция на появление данных в коллекторе.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
	const std::shared_ptr<IWriter>	 console_writer_ptr;
};