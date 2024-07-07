#pragma once

#include "Observ/observer.h"
#include "Gatherer/data_gatherer.h"

#include <memory>

using command_iterator = std::vector<std::string>::const_iterator;

// ��� �������� ������.
enum class InputType {
	NO_INPUT,
	COMMAND,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	END_INPUT
};

// ������� ��� ��������� ������.
enum class StorageType {
	UNKNOWN_T,
	STATIC_T,
	DYNAMIC_T
};

/**
* @brief �����, ���������� ��������� ������ � ����������� ��.
*/
class Collector : public Observable {
public:
	Collector() = delete;

	explicit Collector(const std::size_t& bulk_size, const std::uint32_t& hid,
		const std::shared_ptr<IGatherer>& ig_ptr)
		: collection_max_size(bulk_size)
		, handle_id(hid)
		, gatherer_ptr(ig_ptr)
	{
		commands_collection.reserve(collection_max_size);
	}


	~Collector() {
		flush_collection(true);
	}
	
	/**
	* ��������� �����.
	* @param input_type ��� �������� ������
	* @param command ������
	*/
	void handle_input(InputType input_type, const std::string& command);

	/**
	*  ����� ���� ��������� � ������� � ������ � ���� ����� ���������� ������������.
	*  @param clear_flag - ������� �� ��������� ����� ������.
	*/
	void flush_collection(bool clear_flag);

	/**
	* ���������� ��������� � ������ ����� ����������� ������.
	*/
	void reset_collection();

	/**
	* @return id ���������.
	*/
	std::uint32_t get_handle_id() {
		return handle_id;
	}

	/**
	* ������� ��� �������� ����������� �������� ������ � ������ ������������.
	* @return ����� �������� �����.
	*/
	file_time get_file_time() {
		return first_command_time;
	}

	/**
	* @return ����������� �������� �� ������ ���������.
	*/
	command_iterator get_iter_begin() {
		return commands_collection.cbegin();
	}

	/**
	* @return ����������� �������� �� ����� ���������.
	*/
	command_iterator get_iter_end() {
		return commands_collection.end();
	}

private:
	const std::size_t collection_max_size;
	const std::uint32_t handle_id;

	const std::shared_ptr<IGatherer> gatherer_ptr;

	StorageType storage_type{ StorageType::UNKNOWN_T };
	
	int open_brackets_number{ 0 };
	int close_brackets_number{ 0 };

	std::vector<std::string> commands_collection;

	file_time first_command_time;
};