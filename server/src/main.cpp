#include <iostream>
#include <thread>
#include <list>
#include <asio.hpp>
#include <spdlog/spdlog.h>
#include "GameLiftManager.hpp"
#include "ClientManager.hpp"
#include "data/Loader.hpp"
#include "world/World.hpp"

using asio::ip::tcp;

int main(int argc, char* argv[]) {
#ifdef LOG_STDOUT
    auto logger = spdlog::stdout_logger_mt("default", true);
#else
    auto logger = spdlog::basic_logger_mt("default", "./logs/main.log", true);
#endif
    logger->set_level(spdlog::level::debug);

    PacketData::Initialize();

    try
    {
        int port = 9000;
        if (argc == 2) {
            port = atoi(argv[1]);
        }

        logger->info("Listening on port {}", port);

        GGameLiftManager.reset(new GameLiftManager);

        if (!GGameLiftManager->InitializeGameLift(port)) {
            logger->error("Failed to initialize GameLift!");
            return -1;
        }

        asio::io_service io_service;

        data::Loader loader("./data");
        std::shared_ptr<world::World> world = std::make_shared<world::World>(io_service, loader);
        // Start a set dungeon this time
        world->start("tower");

        tcp::endpoint endpoint(tcp::v4(), port);

        ClientManager srv(io_service, endpoint, world->handler());
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    GGameLiftManager->OnProcessTerminate();

    return 0;
}