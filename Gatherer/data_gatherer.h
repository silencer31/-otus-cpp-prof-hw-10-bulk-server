#pragma once

#include "../Interfaces/interface_gatherer.h"
#include "../thread_writers/thread_console_writer.h"
#include "../thread_writers/thread_file_writer.h"

#include <vector>
#include <string>
#include <chrono>

using file_time = std::chrono::system_clock::time_point;

class DataGatherer final : public IGatherer
{
public:
	DataGatherer() = delete;

	explicit DataGatherer(const std::shared_ptr<IWriter>& io_ptr,
		const std::shared_ptr<IWriter>& iw_ptr)
		: console_writer_ptr(io_ptr)
		, file_writer_ptr(iw_ptr)
	{}

	/**
	* ���������� ����� ������ ������.
	* @param hid ����� ���������.
	* @param time �����.
	* @param data ������ ������.
	* @param bulk_size ����������� �� ���-�� ������ ������.
	*/
	void add_one_piece(const std::uint32_t& handle_id, const std::string& data,
		 const std::size_t& bulk_size) override;

	/**
	* ��������� �� ����� ��� ������������ ������.
	*/
	void flush_pieces() override;

private: // Data
	const std::shared_ptr<IWriter> console_writer_ptr;
	const std::shared_ptr<IWriter> file_writer_ptr;

	std::vector<std::string> data_pieces;

	std::mutex data_mutex;
	
	std::uint32_t handle_id_first; // handle_id ����, ��� ������� ������ ������ ������.
	file_time first_command_time;  // time ����, ��� ������� ������ ������ ������.
};