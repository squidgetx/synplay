package benchmark;

import javax.sound.sampled.*;

public class Listener {
    public static void main(String args[]) throws Exception {
        AudioFormat format = Shouter.getFormat();
        TargetDataLine line = Listener.openLine(format);

//        construct and run the thread with max priority
        ListenRunner lr = new ListenRunner(line);
        Thread thread = new Thread(lr);
        thread.setPriority(Thread.MAX_PRIORITY);
        thread.start();
    }

    public static TargetDataLine openLine(AudioFormat format) throws LineUnavailableException {
        TargetDataLine line;
        DataLine.Info info = new DataLine.Info(TargetDataLine.class, format);

        if (!AudioSystem.isLineSupported(info)) {
            throw new LineUnavailableException();
        }

        // Obtain and open the line.
        line = (TargetDataLine) AudioSystem.getLine(info);
        line.open(format);

        return line;
    }

}
