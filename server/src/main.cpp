#include <iostream>
#include <thread>
#include <list>
#include <asio.hpp>
#include <spdlog/spdlog.h>
#include "Config.hpp"
#include "GameLiftManager.hpp"
#include "ClientManager.hpp"
#include "data/Loader.hpp"
#include "world/World.hpp"

using asio::ip::tcp;

int main(int argc, char* argv[]) {
    auto mainLogger = spdlog::stdout_logger_mt("main", true);
    mainLogger->set_level(spdlog::level::debug);

    auto sharedLogger = spdlog::stdout_logger_mt("shared", true);
    sharedLogger->set_level(spdlog::level::debug);

    PacketData::Initialize();

    try
    {
        int port = 9000;
        if (argc == 2) {
            port = atoi(argv[1]);
        }

        mainLogger->info("Listening on port {}", port);

        GGameLiftManager.reset(new GameLiftManager);

        if (!GGameLiftManager->InitializeGameLift(port)) {
            mainLogger->error("Failed to initialize GameLift!");
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

    return 0;
}