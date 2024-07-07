#include "data_gatherer.h"

#include <sstream>

void DataGatherer::add_one_piece(const std::uint32_t& handle_id, const std::string& data,
	 const std::size_t& bulk_size)
{
	// ���� ������ bulk_size ����� 1, ����� ���������� � ������� ��� ������ � ����� � ��������� �������.
	if (bulk_size == 1) {
		console_writer_ptr->add_data(data);
		first_command_time = std::chrono::system_clock::now();
		const uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(first_command_time.time_since_epoch()).count();
		file_writer_ptr->add_data(handle_id, seconds, data);
		return;
	}
	
	{
		std::lock_guard<std::mutex> lg(data_mutex);

		// ���� ��������� ������ ������ ����,
		// ����������� ����� � ���������� handle_id � time ����, ��� ������� ������ ������.
		if (data_pieces.empty()) {
			data_pieces.reserve(bulk_size);
			handle_id_first = handle_id;
			first_command_time = std::chrono::system_clock::now();
		}

		// ��������� ��������� ������ ������.
		data_pieces.push_back(data);

		// ��������� ���������� �� ���������� ������.
		if (data_pieces.size() != bulk_size) {
			return;
		}

		// ���������� ������.
		std::stringstream message;

		for (auto comm_iter = data_pieces.cbegin(); comm_iter != data_pieces.cend(); ++comm_iter) {
			message << (*comm_iter);
			if (comm_iter + 1 != data_pieces.cend()) {
				message << ", ";
			}
		}

		// ���������� ������ � ������� ������ � �������.
		console_writer_ptr->add_data(message.str());

		const uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(first_command_time.time_since_epoch()).count();

		// ���������� ������ � ������� ������ � �����.
		file_writer_ptr->add_data(handle_id_first, seconds, message.str());

		// ������� ��������� ��� ������ ������.
		data_pieces.clear();
	}
}

void DataGatherer::flush_pieces()
{
	{
		std::lock_guard<std::mutex> lg(data_mutex);

		// ���� ��������� ������ ������ ���� �������.
		if (data_pieces.empty()) { return; }

		// ���������� ������.
		std::stringstream message;

		for (auto comm_iter = data_pieces.cbegin(); comm_iter != data_pieces.cend(); ++comm_iter) {
			message << (*comm_iter);
			if (comm_iter + 1 != data_pieces.cend()) {
				message << ", ";
			}
		}

		// ���������� ������ � ������� ������ � �������.
		console_writer_ptr->add_data(message.str());

		const uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(first_command_time.time_since_epoch()).count();

		// ���������� ������ � ������� ������ � �����.
		file_writer_ptr->add_data(handle_id_first, seconds, message.str());

		// ������� ��������� ��� ������ ������.
		data_pieces.clear();
	}
}