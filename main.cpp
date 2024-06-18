#include "bulk_server.h"

#include <iostream>

#ifdef WIN32
#define appname "bulk_server.exe"
#else
#define appname "./bulk_server"
#endif

namespace ba = boost::asio;

int main(int argc, char* argv[])
{
    std::locale::global(std::locale(""));

    if (argc != 3) {
        std::cout << "Usage: " << appname << " <port> <bulk_size>" << std::endl;
        return -1;
    }

    int port = std::atoi(argv[1]);

    if (port <= 0) {
        std::cerr << "Incorrect port value\n";
        return -1;
    }

    int bulk_size = std::atoi(argv[2]);

    if (bulk_size <= 0) {
        std::cerr << "Incorrect bulk_size value\n";
        return -1;
    }

    try
    {
        ba::io_context io_context;

        // Создание сервера.
        std::shared_ptr<BulkServer> task_server_ptr = std::make_shared<BulkServer>(
            io_context,
            static_cast<unsigned short>(port),
            bulk_size
        );

        // 
        std::cout << "___ io run ___ waiting" << std::endl;
        io_context.run();
        std::cout << "___ io run ___ finished" << std::endl;

    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}