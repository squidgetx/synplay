#include <stdio.h>
#include <sys/time.h>
#include <portaudio.h>

#define NUM_SECONDS   (1)
#define SAMPLE_RATE   (44100)


long get_millisecond_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

struct uData {
  long startTime;
};

static int pacallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    /* Cast data passed through stream to our structure. */
    (void) inputBuffer; /* Prevent unused variable warning. */
    float *out = (float*)outputBuffer;
    struct uData *data = (struct uData *)userData;
    unsigned int i;

    data->startTime = get_millisecond_time();
    for(i = 0; i<framesPerBuffer; i++ )
    {
      *out = 1.0;
      out++;
    }

    return paComplete;
}

int main(void)
{
    PaStream *stream;
    PaError err;

    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    struct uData data;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                2,          /* stereo output */
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                4,        /* frames per buffer */
                                pacallback,
                                &data );

    if (err != paNoError) goto error;
    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    /* Sleep for several seconds. */
    Pa_Sleep(NUM_SECONDS*1000);

    printf("Sent impulse at %ld.\n", data.startTime);

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
