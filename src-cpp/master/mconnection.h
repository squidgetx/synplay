#ifndef M_CONNECTION_H
#define M_CONNECTION_H

struct MConnection {
  enum MConnectionState {
    NAKED = 0,
    SENT_INITIAL_TIMESYNC = 1,
    SENT_FINAL_TIMESYNC = 2,
    SENDING_DATA = 3
  };

  asio::deadline_timer *timer;
  MConnectionState state;

  MConnection() : state(NAKED), timer(nullptr) {};
};

#endif
