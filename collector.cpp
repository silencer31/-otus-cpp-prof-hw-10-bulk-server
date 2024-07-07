#include "collector.h"


void Collector::handle_input(InputType input_type, const std::string& command)
{
	switch (input_type) {
	case InputType::COMMAND:
		// Коллекция имеет динамический размер.
		if (storage_type == StorageType::DYNAMIC_T) {
			if (commands_collection.size() + 1 == commands_collection.capacity()) {
				commands_collection.reserve(commands_collection.size() * 2);
			}

			commands_collection.push_back(command);
			
			return;
		}
		
		// Коллекция имеет статический размер.
		// Отправляем одну порцию данных в сборщик порций.
		gatherer_ptr->add_one_piece(handle_id, command, collection_max_size);


		/*
		commands_collection.push_back(command);

		// Коллекция ещё не используется.
		if (storage_type == StorageType::UNKNOWN_T) {
			storage_type = StorageType::STATIC_T;
			first_command_time = std::chrono::system_clock::now();
		}
		
		// Коллекция имеет статический размер.
		if (storage_type == StorageType::STATIC_T) {
			// Проверяем, стал ли добавленный элемент крайним.
			if (commands_collection.size() == collection_max_size) {
				flush_collection(true);
			}
		}
		*/
		break;
	case InputType::OPEN_BRACKET:
		open_brackets_number++;

		// Проверяем, начался ли динамический блок.
		if (open_brackets_number == 1) {
			flush_collection(true);
			storage_type = StorageType::DYNAMIC_T;
			first_command_time = std::chrono::system_clock::now();
		}

		break;
	case InputType::CLOSE_BRACKET:
		// Если открывающих скобок ещё не было, закрывающую игнорируем.
		if (open_brackets_number == 0) {
			return;
		}

		close_brackets_number++;
		
		// Конец динамического блока.
		if (close_brackets_number == open_brackets_number) {
			flush_collection(false);
			reset_collection();
		}

		break;
	case InputType::END_INPUT:
		flush_collection(false);
		reset_collection();
		break;
	case InputType::NO_INPUT:
		break;
	default:
		break;
	}
}


void Collector::flush_collection(bool clear_flag)
{
	if (commands_collection.empty()) { return; }

	notify(); // Оповещаем обозревателей.

	if (clear_flag) {
		commands_collection.clear();
	}
}


void Collector::reset_collection()
{
	commands_collection.clear();
	open_brackets_number = 0;
	close_brackets_number = 0;
	storage_type = StorageType::UNKNOWN_T;
}