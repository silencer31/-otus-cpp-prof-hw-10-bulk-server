//#include "client_session.h"
#include "bulk_server.h"

#include <iostream>

void ClientSession::do_read()
{
	auto self(shared_from_this());

	socket_.async_read_some( // У сокета вызываем async_read_some
		boost::asio::buffer(data_read, max_length),
		[this, self](boost::system::error_code errcode, std::size_t length)
		{
			if (errcode) {
				std::cout << " Session: " << session_id << ". Read data boost system error code: " << errcode.message() << std::endl;

				// Сообщить серверу о необходимости выключения сессии
				bulk_server_ptr->close_session(session_id);

				return;
			}

			std::cout << "Session: " << session_id << ". Received: " << length << " bytes\n" << data_read << std::endl;

			// Очищаем буфер для следующего запроса от клиента.
			clear_data_read();

			// Ожидаем очередную порцию данных.
			do_read();
		}
	);
}

// Выключение сессии.
void ClientSession::shutdown()
{
	if (!socket_.is_open()) {
		return;
	}

	std::cout << "Shutdown process started. Session: " << session_id << std::endl;

	boost::system::error_code ignore;

	socket_.shutdown(
		boost::asio::ip::tcp::socket::shutdown_both,
		ignore
	);

	socket_.close(ignore);

	std::cout << "Shutdown finished. Session: " << session_id << std::endl;
}

// Очистка буфера для приема данных по сети.
void ClientSession::clear_data_read()
{
	for (int i = 0; i < max_length; ++i) {
		data_read[i] = 0;
	}
}
