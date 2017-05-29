//
// Created by ldavid on 5/22/17.
//

#include <spdlog/spdlog.h>
#include "PacketDispatcher.hpp"

void PacketDispatcher::dispatch(pb::Message& msg)
{
    auto logger = spdlog::get("shared");

    {
        std::lock_guard<std::mutex> guard(mAddedAccess);
        for (handlers_map::value_type to_add : mAdded) {
            mHandlers.insert(to_add);
        }
        mAdded.clear();
    }

    {
        std::lock_guard<std::mutex> guard(mRemovedAccess);
        for (size_t to_remove : mRemoved) {
            auto it = std::find_if(
                mHandlers.begin(), mHandlers.end(),
                [&](auto pair) {
                    return pair.second->id == to_remove;
                });

            mHandlers.erase(it);
        }
        mRemoved.clear();
    }


    const std::type_info& type_info = typeid(msg);

    auto pair = mHandlers.equal_range(type_info.hash_code());
    for (auto it = pair.first; it != pair.second; ++it) {
        auto& func = *(it->second);
        func(msg);
    }
}
