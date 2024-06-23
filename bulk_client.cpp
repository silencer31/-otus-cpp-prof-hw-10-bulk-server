#include <iostream>
#include <thread>
#include <boost/asio.hpp>

//#include "structs.h"

using boost::asio::ip::tcp;

namespace ba = boost::asio;

class BulkClient
{
public:
    BulkClient() = delete;

    BulkClient(const std::string& ip_addr, const unsigned short port)
        : _end_point(ba::ip::tcp::endpoint(ba::ip::address::from_string(ip_addr), port))
        , _sock(ba::ip::tcp::socket(_io_context))
    {
        connected = connect_to_server();
    }

    ~BulkClient() {
        disconnect();
    }
    
    /**
    * Узнать, удалось ли подключиться.
    */
    bool is_connected() const {
        return connected;
    }

    /**
    * Отправка данных.
    * @param data_ptr отправляемые данные.
    * @param data_size размер данных.
    */
    bool send_message(const char* data_ptr, const std::size_t data_size)
    {
        if (data_size == 0) {
            last_error = std::string("Empty data to send");
            return false;
        }

        bytes_written = 0;

        try {
            bytes_written = ba::write(_sock, ba::buffer(data_ptr, data_size));
        }
        catch (const boost::system::system_error& ex) {
            last_error = std::string("boost exception: ") + ex.what();
            return false;
        }
        catch (const std::exception& ex) {
            last_error = std::string("std exception: ") + ex.what();
            return false;
        }

        // Проверяем, сколько байт удалось записать.
        if (bytes_written == 0) {
            last_error = "Zero bytes were written";
            return false;
        }

        return true;
    }

private:
    /**
    * Подключение к серверу.
    */
    bool connect_to_server()
    {
        try {
            _sock.connect(_end_point);
        }
        catch (const boost::system::system_error& ex) {
            last_error = std::string("boost exception: ") + ex.what();
            return false;
        }
        catch (const std::exception& ex) {
            last_error = std::string("std exception: ") + ex.what();
            return false;
        }

        return true;
    }

    /**
    * Отключение от сервера.
    */
    void disconnect()
    {
        if (!_sock.is_open()) {
            return;
        }

        boost::system::error_code ignore;

        _sock.shutdown(
            boost::asio::ip::tcp::socket::shutdown_both,
            ignore
        );

        _sock.close(ignore);

        connected = false;
    }

private:
    ba::io_context _io_context;
    ba::ip::tcp::endpoint _end_point;
    ba::ip::tcp::socket _sock;

    bool connected;

    std::string last_error; // Ошибка, полученная в процессе сетевого взаимодействия.
    
    std::size_t bytes_written; // Сколько байт удалось отправить.

    //BulkQueue mWriteMsgs;
};

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: bulk_client <host> <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[2]);

    if (port <= 0) {
        std::cerr << "Incorrect port value\n";
        return -1;
    }

    const std::string server_addr(argv[1]);
        
    BulkClient bulk_client(server_addr, static_cast<unsigned short>(port));

    if ( !bulk_client.is_connected()) {
        std::cerr << "Unable to connect to server " << server_addr << ". Port: " << port << std::endl;
        return -1;
    }

    bulk_client.send_message("a", 1);

        /*
        client.write("a");
        client.write("b");
        client.write("\n");
        client.write("c");
        client.write("\n");
        client.write("d");
        client.write("\n");
        client.write("\n");
        client.write("\n");
        client.write("f");
        client.write("\n");
        client.write("g");
        client.write("{");
        client.write("\n");
        client.write("h");
        client.write("\n");
        client.write("i");
        client.write("\n");
        client.write("j");
        client.write("\n");
        client.write("k");
        client.write("\n");
        client.write("\n");
        client.write("l");
        client.write("\n");*/

   


    return 0;
}
