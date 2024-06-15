#include "interpreter.h"

// Обработка введённой строки.
void Interpreter::handle_input(const std::string& command)
{
	if (command.empty()) {
		collector_ptr->handle_input(InputType::END_INPUT, "");
		return;
	}

	if (0 == command.compare("{")) {
		collector_ptr->handle_input(InputType::OPEN_BRACKET, "");
		return;
	}

	if (0 == command.compare("}")) {
		collector_ptr->handle_input(InputType::CLOSE_BRACKET, "");
		return;
	}

	collector_ptr->handle_input(InputType::COMMAND, command);
}

// Обработка полученного EOF.
void Interpreter::handle_eof()
{
	collector_ptr->handle_input(InputType::END_INPUT, "");
}