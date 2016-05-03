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
      byte[] data = {127, 127, 127, 127};
      // record write time!
      long time = System.nanoTime();
      audioOut.write(data, 0, 4);
      System.out.println("Impulse sent at " + time);
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
