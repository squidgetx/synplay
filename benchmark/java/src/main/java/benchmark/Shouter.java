package benchmark;

import javax.sound.sampled.*;
import java.lang.*;

public class Shouter {

    public static AudioFormat getFormat() {
      // Decide on a standard audio format to use
      AudioFormat decodedFormat = new AudioFormat(AudioFormat.Encoding.PCM_SIGNED,
              44100,
              16,
              2,
              4,
              44100,
              false);
      return decodedFormat;
    }

    public static void main(String args[]) throws Exception {
        SourceDataLine audioOut = openLine(getFormat());

//        construct and run the thread with max priority
        ShoutRunner sr = new ShoutRunner(audioOut);
        Thread thread = new Thread(sr);
        thread.setPriority(Thread.MAX_PRIORITY);
        thread.start();
    }

    public static SourceDataLine openLine(AudioFormat format) throws LineUnavailableException {
        SourceDataLine line;
        DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);
        if (!AudioSystem.isLineSupported(info)) {
            // Handle the error ...
            throw new LineUnavailableException();
        }

        // Obtain and open the line.
        line = (SourceDataLine) AudioSystem.getLine(info);
        line.open(format);
        line.start();
        return line;
    }

}
