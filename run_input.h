#include "interpreter.h"

#include <sstream>

/**
* Создание экземпляров классов и проверка работы.
*/
void run_commands_input(int number)
{
	std::shared_ptr<Collector>    collector_ptr   = std::make_shared<Collector>(static_cast<size_t>(number));
	std::unique_ptr<Interpreter>  interpreter_ptr = std::make_unique<Interpreter>(collector_ptr);

	// Проверка работы.

	// Ручной ввод.
	std::string text;
	
	while (std::getline(std::cin, text)) {
		interpreter_ptr->handle_input(text);
	}
	
	if (std::cin.eof()) {
		interpreter_ptr->handle_eof();
	}
	
	// Ввод из набора.
	/*
	std::istringstream input;
	input.str("1\n{\n2\n3\n4\n5\n6\n}\n7\n{\n8\n");

	for (std::string line; std::getline(input, line); )
	{
		interpreter_ptr->handle_input(line);
	}

	if (input.eof()) {
		interpreter_ptr->handle_eof();
	}
	*/
}