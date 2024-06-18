#include "bulk_server.h"

#include "async.h"

#include <iostream>


BulkServer::BulkServer(boost::asio::io_context& io_context, const unsigned short port, const int bulk_size)
	: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
	do_accept();

	std::cout << "bulk size " << bulk_size << std::endl;
}


void BulkServer::do_accept()
{
	acceptor_.async_accept( // ���� ����� �������� ����������, ��������� ��������� �������.
		[this](boost::system::error_code errcode, tcp::socket socket)
		{
			if (shutdown_flag) {
				std::cout << "\n -- connection rejected. Server is shutting down!" << std::endl;
				return;
			}

			// ������� ���� �� ������ boost.
			if (errcode) {
				std::cout << "\n -- connection rejected. Boost system error: " << errcode.message() << std::endl;
				do_accept(); // ������������� �� �������� ���������� ����������.
				return;
			}

			std::cout << "\n -- connection accepted" << std::endl;

			// ������� ���������� ������ � ��������� ����� ������.
			session_shared session_ptr = std::make_shared<ClientSession>(
				shared_from_this(),
				std::move(socket),
				session_number // ������ ������ ����� ���� �����.
			);

			// ��������� ����� ������ � ��������� ������ �������.
			sessions[session_number] = session_ptr;

			session_number++;

			// ������ ������ ������.
			session_ptr->start();

			// ������������� �� �������� ���������� ����������.
			do_accept();
		}
	);

	std::cout << "\nwaiting for new connection" << std::endl;
}

// �������� ������� �� ���������� �������.
void BulkServer::shutdown_server(int session_id)
{
	std::cout << "Task server: Exit request received from session: " << session_id << std::endl;

	shutdown_flag = true;

	// � ����� ��������� ��� ������.
	for (const auto& [id, session_ptr] : sessions) {
		if (id != session_id) { // ���������� ������, �� ������� ������ ������ �� ����������.
			session_ptr->shutdown();
		}
	}

	// ��������� ����� ����� ����������.
	acceptor_.cancel();
	//acceptor_.close();
}

// �������� ������.
void BulkServer::close_session(int session_id)
{
	std::cout << "Task server: Session will be closed: " << session_id << std::endl;

	sessions[session_id]->shutdown();
	sessions.erase(session_id);
	session_number--;
}