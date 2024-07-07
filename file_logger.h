#pragma once

#include "collector.h"
//#include "thread_writers/thread_file_writer.h"

/**
* @brief Класс для записи коллекции команд в файл.
*/
class FileLogger : public Observer {
public:
	FileLogger() = delete;

	explicit FileLogger(const std::shared_ptr<Collector>& cltr_ptr,
		const std::shared_ptr<IWriter>& iw_ptr)
	: collector_ptr(cltr_ptr)
	, file_writer_ptr(iw_ptr)
	{}
	
	~FileLogger() = default;

	/**
	* Обработка введённой строки.
	* @param c_begin итератор на начало данных
	* @param c_end итератор на окончание данных
	* @param ftime время
	*/
	void write_commands_to_file(const command_iterator& c_begin, const command_iterator& c_end, const file_time& ftime);

	/**
	* Реакция на появление данных в коллекторе.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
	const std::shared_ptr<IWriter>	 file_writer_ptr;
};