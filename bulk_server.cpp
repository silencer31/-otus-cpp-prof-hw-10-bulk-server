#include "bulk_server.h"

#include "async.h"

#include <iostream>


BulkServer::BulkServer(boost::asio::io_context& io_context, const unsigned short port, const int bs)
	: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	, bulk_size(bs)
{
	//std::cout << "Bulk_Constructor" << std::endl;

	do_accept();
}

BulkServer::~BulkServer()
{
	std::cout << "Bulk_Destructor" << std::endl;
}

void BulkServer::do_accept()
{
	acceptor_.async_accept( // Если будет входящее соединение, выполнить следующую функцию.
		[this](boost::system::error_code errcode, tcp::socket socket)
		{
			if (shutdown_flag) {
				std::cout << "\n -- connection rejected. Server is shutting down!" << std::endl;
				return;
			}

			// Смотрим была ли ошибка boost.
			if (errcode) {
				std::cout << "\n -- connection rejected. Boost system error: " << errcode.message() << std::endl;
				do_accept(); // Переключаемся на ожидание следующего соединения.
				return;
			}

			std::cout << "\n -- connection accepted" << std::endl;

			// Создаем клиентскую сессию и запускаем прием данных.
			session_shared session_ptr = std::make_shared<ClientSession>(
				shared_from_this(),
				std::move(socket),
				session_number, // Каждая сессия знает свой номер.
				bulk_size
			);

			// Добавляем новую сессию в коллекцию сессий сервера.
			sessions[session_number] = session_ptr;

			session_number++;

			// Начало работы новой сессии - нового контекста обработки данных.
			session_ptr->start();

			// Переключаемся на ожидание следующего соединения.
			do_accept();
		}
	);

	std::cout << "\nwaiting for new connection" << std::endl;
}

// Получена команда на выключение сервера.
void BulkServer::shutdown_server(int session_id)
{
	std::cout << "Bulk server: Exit request received from session id: " << session_id << std::endl;

	shutdown_flag = true;

	// В цикле завершаем все сессии.
	for (const auto& [id, session_ptr] : sessions) {
		if (id != session_id) { // Пропускаем сессию, от которой пришел сигнал на завершение.
			session_ptr->shutdown();
		}
	}

	// Отключаем прием новых соединений.
	acceptor_.cancel();
	//acceptor_.close();
}

// Закрытие сессии.
void BulkServer::close_session(int session_id)
{
	std::cout << "Bulk server: Session will be closed: " << session_id << std::endl;

	sessions[session_id]->shutdown();
	sessions.erase(session_id);
	session_number--;
}