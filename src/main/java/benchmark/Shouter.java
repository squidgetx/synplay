package benchmark;

import javax.sound.sampled.*;

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

    }

    public static void openLine(AudioFormat format) throws LineUnavailableException {
        TargetDataLine line;
        DataLine.Info info = new DataLine.Info(TargetDataLine.class, format);
        if (!AudioSystem.isLineSupported(info)) {
            // Handle the error ...
            throw new LineUnavailableException();
        }

        // Obtain and open the line.
        line = (TargetDataLine) AudioSystem.getLine(info);
        line.open(format);
    }

}
