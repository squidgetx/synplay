#ifndef MASTER_H
#define MASTER_H

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>

#include "asio.hpp"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include <sndfile.hh>
#include "util/constants.h"
#include "master/mconnection.h"

#define TP_BUFFER_SIZE (2 + 5 * 8)

class Master
{
  public:
    Master(std::string& filename,std::vector<asio::ip::udp::endpoint>& remote_endpts);
    ~Master ();

    void run();

  private:
    asio::io_service io_service;
    asio::ip::udp::socket socket;
    SndfileHandle file;

    std::unordered_map<std::shared_ptr<asio::ip::udp::endpoint>, MConnection> connections;

    // Audio sampling parameters
    // The initial offset from the master time to use for the clients.
    // Should account for maxium network latency to a client.
    const mtime_offset_t STREAM_OFFSET = 1000;

    // Stream start time
    mtime_t stream_start;
    // The number of frames already sent by the client.
    std::size_t n_frames_sent = 0;

    uint8_t tp_buffer[TP_BUFFER_SIZE];
    int16_t data_buffer[SAMPLES_PER_PACKET];
    uint16_t synced;
    uint16_t outstanding_packets;

    bool isDone = false;

    void send_data(std::shared_ptr<asio::ip::udp::endpoint> remote_endpt, asio::const_buffer& buf);
    void send_data();

    void send_timesync();

    void send_initial_timesync(asio::ip::udp::endpoint& remote_endpt, int16_t attempt = 0);
    void send_final_timesync(asio::ip::udp::endpoint& remote_endpt, TPacket *tp, int16_t attempt = 0);

    void receive_initial_timesync_reply(asio::ip::udp::endpoint& remote_endpt, int16_t attempt);
    void receive_final_timesync_reply(asio::ip::udp::endpoint& remote_endpt, TPacket *tp, int16_t attempt);

    void receive_everything();

    template <typename WriteHandler>
asio::deadline_timer *start_timer(asio::ip::udp::endpoint& remote_endpt, int16_t attempt, WriteHandler handler);
};

#endif
