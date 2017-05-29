//
// Created by ldavid on 5/12/17.
//

#include "World.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <spdlog/spdlog.h>

using namespace world;

World::World(asio::io_service &io_service, data::Loader &loader)
    : mLoader(loader)
      , mWorkQueue(io_service)
      , mDungeon(nullptr)
      , mTickTimer(io_service, boost::posix_time::milliseconds(TICK_DURATION))
      , mHandler(*this)
      , mTickId(0)
{

}

void World::start(const std::string &dungeonName)
{
    mDungeon = mLoader.getDungeon(dungeonName);

    std::shared_ptr<World> self = shared_from_this();
    for (data::Level::Ptr level : mDungeon->getLevels()) {
        mLevels.push_back(std::make_shared<Level>(level, self));
    }

    mTickTimer.async_wait(mWorkQueue.wrap(std::bind(&World::doTick, this)));
}

void World::doTick()
{
    auto logger = spdlog::get("main");

    mTickId++;

    for (auto &level : mLevels) {
        level->doTick(mTickId);
    }

    mHandler.update(mTickId);

    nextTick();
}

void World::nextTick()
{
    mTickTimer.expires_at(mTickTimer.expires_at() + boost::posix_time::milliseconds(TICK_DURATION));
    mTickTimer.async_wait(mWorkQueue.wrap(std::bind(&World::doTick, this)));
}

std::vector<std::shared_ptr<Level>> &World::levels()
{
    return mLevels;
}

Handler &World::handler()
{
    return mHandler;
}
