#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SAMPLE_RATE (44100)
#define CHANNELS (2)
#define SAMPLE_SIZE (2)
#define PACKET_TIME_MS (10)

#define FRAMES_PER_PACKET (SAMPLE_RATE * PACKET_TIME_MS / 1000) 
#define SAMPLES_PER_PACKET (FRAMES_PER_PACKET * CHANNELS)
#define SHORTS_PER_PACKET SAMPLES_PER_PACKET

#define SEND_SPEED_FACTOR (2)
// How fast to send packets; if this = 2 for instance then we 
// send packets twice as fast as the rate of song audio

#endif
