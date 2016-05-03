#include <stdio.h>
#include <sys/time.h>
#include <portaudio.h>

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define THRESHOLD (120)

long get_millisecond_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

struct uData {
  long recvTime;
  unsigned long frames;
};

static int pacallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    /* Cast data passed through stream to our structure. */
    (void) outputBuffer; /* Prevent unused variable warning. */
    short *in = (short*)inputBuffer;
    struct uData *data = (struct uData *)userData;
    unsigned int i;

    for(i = 0; i<framesPerBuffer; i++ )
    {
      data->frames++;
      printf("\t%d\n", *in);
      if (*in > THRESHOLD) {
        data->recvTime = get_millisecond_time();
        return paAbort;
      }

      in++;
    }

    return paContinue;
}

int main(void)
{
    PaStream *stream;
    PaError err;

    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    struct uData data;
    data.frames = 0;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                2,
                                0,
                                paInt16,
                                SAMPLE_RATE,
                                4,        /* frames per buffer */
                                pacallback,
                                &data );

    if (err != paNoError) goto error;
    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    /* Sleep for several seconds. */
    Pa_Sleep(NUM_SECONDS*1000);

    printf("Frames: %ld. Received impulse at %ld.\n", data.frames, data.recvTime);

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
    Pa_Terminate();
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
