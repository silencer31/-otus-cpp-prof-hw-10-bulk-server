#pragma once

#include "client_session.h"

using boost::asio::ip::tcp;

using session_map = std::map<int, session_shared>; // Для коллекции сессий пользователей.


/**
* @brief Класс - сервер обработки сетевых подключений и управления сессиями пользователей.
*/
class BulkServer : public std::enable_shared_from_this<BulkServer>
{
public:
	BulkServer() = delete;

	BulkServer(boost::asio::io_context& io_context, const unsigned short port, const int bulk_size);

	/**
	* Получена команда на выключение сервера.
	* @param session_id уникальный id сессии, из которой получена команда.
	*/
	void shutdown_server(int session_id);

	/**
	* Закрытие сессии.
	* @param session_id уникальный id сессии.
	*/
	void close_session(int session_id);
	

private: // methods
	/**
	* Приступить к ожиданию новых подключений.
	*/
	void do_accept();

private: // data
	int session_number{0};   // Кол-во сессий.

	bool shutdown_flag{false}; // Флаг, что нужно завершать работу сервера.	

	tcp::acceptor acceptor_;

	session_map sessions; // Коллекция сессий.			
};