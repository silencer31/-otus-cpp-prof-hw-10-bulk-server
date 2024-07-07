#include "bulk_server.h"

#include <boost/algorithm/string.hpp>
#include <iostream>

void ClientSession::do_read()
{
	auto self(shared_from_this());

	socket_.async_read_some( // � ������ �������� async_read_some
		boost::asio::buffer(data_read, max_length),
		[this, self](boost::system::error_code errcode, [[maybe_unused]] std::size_t length)
		{
			// ������� �� ���������� ������� �� �������.
			if (errcode) {
				std::cout << " Session: " << session_id << ". Read data boost system error code: " << errcode.message() << std::endl;

				// �������� ������� � ������������� ���������� ������
				bulk_server_ptr->close_session(session_id);

				return;
			}

			//std::cout << "Session: " << session_id << ". Received: " << length << " bytes. Data: " << data_read << std::endl;

			//std::string str(data_read);
			// ��������� ���������� ������ ����� \n
			std::vector<std::string> strings;
			boost::split(strings, data_read, boost::is_any_of("\n"));

			for (const std::string& str : strings)
			{
				// ����� �� ��������� ������.
				if (0 == strcmp(str.data(), "exit")) {
					bulk_server_ptr->close_session(session_id);
					return;
				}

				// ����� �� ��������� ������.
				if (0 == strcmp(str.data(), "shutdown")) {
					//std::this_thread::sleep_for(std::chrono::seconds(1));
					bulk_server_ptr->shutdown_server(session_id);
					return;
				}

				// ���������� ������ � ��������.
				async::receive(handle, str.data(), strlen(str.data()));
			}
			
			// ������� ����� ��� ���������� ������� �� �������.
			clear_data_read();

			// ������� ��������� ������ ������.
			do_read();
		}
	);
}

// ���������� ������.
void ClientSession::shutdown()
{
	if (!socket_.is_open()) {
		return;
	}

	std::cout << "Session closing down process started. Session id: " << session_id << std::endl;

	boost::system::error_code ignore;

	socket_.shutdown(
		boost::asio::ip::tcp::socket::shutdown_both,
		ignore
	);

	socket_.close(ignore);

	std::cout << "Session closing down finished. Session id: " << session_id << std::endl;
}

// ������� ������ ��� ������ ������ �� ����.
void ClientSession::clear_data_read()
{
	for (int i = 0; i < max_length; ++i) {
		data_read[i] = 0;
	}
}
