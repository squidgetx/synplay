Usage
=====

The C++ code all lives in the `synplay/` directory. To run the master and client, they obviously must first be built:

```
make -B
```

where `-B` ensures that all headers and rebuilt, if necessary.

The master is run as follows:

```
./bin/master filename [ADDR PORT]+
```

where filename is the file to send. `(ADDR,PORT)` are pairs of IP address, port to which the file should be sent. Any number of address, port pairs may be specified. 

Note that only the audio formats supported by `libsndfile` are supported. Specifically, WAV files are supported, whereas MP3 are not. More details can be found at [http://www.mega-nerd.com/libsndfile/](http://www.mega-nerd.com/libsndfile/).

To run the client, 

```
./bin/client PORT
```

where `PORT` is the port on which the client will listen for audio packets.

Dependencies
============

- libsndfile
- libportaudio
- boost
- asio (*vendored*)

These dependencies are available on `apt`, `dnf`, or the `brew` package manager.

JACK
====

By default, this program attempts to use JACK, a low latency audio layer. Please be sure to start JACK with a default sample rate of 44.1kHZ! ex: `jackd -d alsa -r 44100`.
