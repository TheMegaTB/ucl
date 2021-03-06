#ifndef PROTOMESH_TRANSMISSION_HPP
#define PROTOMESH_TRANSMISSION_HPP

#include <vector>
#include <memory>

using namespace std;

namespace ProtoMesh::communication::transmission {
    enum class ReceiveResult {
        OK,
        NoData,
        Timeout
    };

#define TRANSMISSION_HANDLER_T shared_ptr<ProtoMesh::communication::transmission::TransmissionHandler>

    class TransmissionHandler {
    public:
        TransmissionHandler() = default;

        ~TransmissionHandler() = default;

        virtual void send(vector<uint8_t> message)= 0;
        virtual ReceiveResult recv(vector<uint8_t> *buffer, unsigned int timeout_ms)= 0;
    };

    class NetworkStub : public TransmissionHandler {
        vector<vector<uint8_t>> queue;
    public:
        explicit NetworkStub() = default;

        void addMessageToIncomingQueue(std::vector<uint8_t> message) { queue.push_back(message); }

        void send(std::vector<uint8_t> message) override {}
        ReceiveResult recv(std::vector<uint8_t>* buffer, unsigned int timeout_ms) override {
            if (queue.empty()) return ReceiveResult::NoData;
            *buffer = queue[0];
            queue.erase(queue.begin());
            return ReceiveResult::OK;
        }
    };
}

#endif // PROTOMESH_TRANSMISSION_HPP