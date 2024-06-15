#include "file_logger.h"

#include <fstream>
#include <sstream>

// Запись коллекции в файл с указанием временной метки.
void FileLogger::write_commands_to_file(const command_iterator& c_begin, const command_iterator& c_end, const file_time& ftime)
{
	const uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count();
	
	std::stringstream message;
	//filename << "bulk" << seconds << ".log";

	//std::ofstream file(filename.str(), std::ofstream::out);
	
	for (command_iterator comm_iter = c_begin; comm_iter != c_end; ++comm_iter) {
		message << (*comm_iter);
		if (comm_iter + 1 != c_end) {
			message << ", ";
		}
	}

	// Отправляем данные в потокобезопасную очередь.
	file_writer_ptr->add_data(handle_id, seconds, message.str());
}

// Реакция на появление данных в коллекторе.
void FileLogger::update()
{
	write_commands_to_file(collector_ptr->get_iter_begin(), collector_ptr->get_iter_end(), collector_ptr->get_file_time());
}