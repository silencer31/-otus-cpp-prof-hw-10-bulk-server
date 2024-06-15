#include "console.h"

#include <sstream>
#include <iostream>

// Вывод данных в консоль.
void Console::output_to_console(const command_iterator& c_begin, const command_iterator& c_end)
{
	std::stringstream message;

	for (command_iterator comm_iter = c_begin; comm_iter != c_end; ++comm_iter) {
		message << (*comm_iter);
		if (comm_iter + 1 != c_end) {
			message << ", ";
		}
	}

	// Отправляем данные в потокобезопасную очередь.
	console_writer_ptr->add_data(message.str());
}

// Реакция на появление данных в коллекторе.
void Console::update()
{
	output_to_console(collector_ptr->get_iter_begin(), collector_ptr->get_iter_end());
}