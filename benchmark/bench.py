"""
PyAudio Example: Make a wire between input and output (i.e., record a
few samples and play them back immediately).

This is the callback (non-blocking) version.
"""

import pyaudio
import time

WIDTH = 2
CHANNELS = 2
RATE = 44100

def callback(in_data, frame_count, time_info, status):
    return (bytes(bytearray((255, 255, 255, 255,)*frame_count)), pyaudio.paContinue)

def main():
    p = pyaudio.PyAudio()
    stream = p.open(format=p.get_format_from_width(WIDTH),
            channels=CHANNELS,
            rate=RATE,
            output=True,
            stream_callback=callback)

    print stream.is_active()

    time.sleep(10)

    stream.stop_stream()
    stream.close()

    p.terminate()

if __name__ == '__main__':
    main()

