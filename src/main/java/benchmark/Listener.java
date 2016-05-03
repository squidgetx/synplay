package benchmark;

import javax.sound.sampled.*;

public class Listener {
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
