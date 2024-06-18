#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>
#include <utility>

using boost::asio::ip::tcp;


class BulkServer;

using bulk_server_shared = std::shared_ptr<BulkServer>;

/**
* @brief Класс - сессия пользователя. Позволяет пользователям подключаться к серверу в рамках отдельной сессии.
*/
class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:

	ClientSession(
		const bulk_server_shared& ts_ptr,
		tcp::socket socket, int s_id)
		: bulk_server_ptr(ts_ptr)
		, socket_(std::move(socket))
		, session_id(s_id)
	{}

	~ClientSession() {
		shutdown();
	}

	/**
	* Начало сессии пользователя.
	*/
	void start() {
		clear_data_read();
		do_read();
	}

	/**
	* Выключение сессии.
	*/
	void shutdown();

private: // methods

	/**
	* Асинхронное чтение данных из сокета.
	*/
	void do_read();

	/**
	* Очистка буфера для приема данных по сети.
	*/
	void clear_data_read();


private: // data
	tcp::socket socket_;
	int session_id; // Идентификатор сессии.

	const bulk_server_shared bulk_server_ptr; // Для связи с сервером, создавшим данную сессию.

	enum { max_length = 1024 };

	char data_read[max_length]; // Для получения данных из сети.

	bool shutdown_session_flag{false}; // Флаг, что завершается работа сессии.
};

using session_shared = std::shared_ptr<ClientSession>;
