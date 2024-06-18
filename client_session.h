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
* @brief ����� - ������ ������������. ��������� ������������� ������������ � ������� � ������ ��������� ������.
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
	* ������ ������ ������������.
	*/
	void start() {
		clear_data_read();
		do_read();
	}

	/**
	* ���������� ������.
	*/
	void shutdown();

private: // methods

	/**
	* ����������� ������ ������ �� ������.
	*/
	void do_read();

	/**
	* ������� ������ ��� ������ ������ �� ����.
	*/
	void clear_data_read();


private: // data
	tcp::socket socket_;
	int session_id; // ������������� ������.

	const bulk_server_shared bulk_server_ptr; // ��� ����� � ��������, ��������� ������ ������.

	enum { max_length = 1024 };

	char data_read[max_length]; // ��� ��������� ������ �� ����.

	bool shutdown_session_flag{false}; // ����, ��� ����������� ������ ������.
};

using session_shared = std::shared_ptr<ClientSession>;
