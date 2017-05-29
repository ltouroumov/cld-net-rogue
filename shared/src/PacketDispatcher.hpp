//
// Created by ldavid on 5/22/17.
//

#include <google/protobuf/message.h>
#include <map>
#include <mutex>

#ifndef PREZ_GAME_PACKETDISPATCHER_HPP
#define PREZ_GAME_PACKETDISPATCHER_HPP

namespace pb = google::protobuf;

class PacketDispatcher
{
public:
    class base_msg_handler
    {
    public:
        const size_t id;
        base_msg_handler(size_t id) : id(id) {}

        virtual void operator()(pb::Message &) = 0;
    };

    template<typename MsgType>
    class msg_handler : public base_msg_handler
    {
    public:
        typedef std::function<void(MsgType &)> func_type;


        msg_handler(size_t id, func_type handler)
            : base_msg_handler(id), m_handler(handler)
        {}

        void operator()(pb::Message &msg)
        {
            m_handler(dynamic_cast<MsgType &>(msg));
        }

    private:
        func_type m_handler;
    };

    void dispatch(pb::Message &msg);

    template<typename MsgType>
    size_t wire(std::function<void(MsgType &)> handler)
    {
        std::lock_guard<std::mutex> guard(mAddedAccess);

        size_t msgType = typeid(MsgType).hash_code();
        msg_handler<MsgType> *gen_handler = new msg_handler<MsgType>(++mNextHandlerId, handler);
        mAdded.insert(handlers_map::value_type(msgType, gen_handler));
        return gen_handler->id;
    }

    void unwire(size_t handler_id)
    {
        std::lock_guard<std::mutex> guard(mRemovedAccess);
        mRemoved.insert(handler_id);
    }

private:
    typedef std::multimap<size_t, base_msg_handler *> handlers_map;

    // typedef std::function<void(pb::Message&)> handler_type;
    handlers_map mHandlers;
    std::set<size_t> mRemoved;
    std::set<handlers_map::value_type> mAdded;
    size_t mNextHandlerId = 0;

    std::mutex mRemovedAccess;
    std::mutex mAddedAccess;
};


#endif //PREZ_GAME_PACKETDISPATCHER_HPP
