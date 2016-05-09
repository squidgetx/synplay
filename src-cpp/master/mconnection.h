#ifndef M_CONNECTION_H
#define M_CONNECTION_H

typedef enum {
  NAKED = 0,
  SENT_INITIAL_TIMESYNC = 1,
  SENT_FINAL_TIMESYNC = 2,
  SENDING_DATA = 3
} MConnectionState;

typedef struct mconnection {
  asio::deadline_timer timer;
  MConnectionState state;
  MConnection() {
    timer = NULL;
    state = NAKED;
  };
} MConnection;

#endif
