package benchmark;

import javax.sound.sampled.*;

public class Listener {
    public static void main(String args[]) throws Exception {
        AudioFormat format = Shouter.getFormat();
        TargetDataLine line = Listener.openLine(format);
        Listener.listen(line);
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

    public static void listen(TargetDataLine line){

        // Begin audio capture.
        line.start();

        int numBytesRead;
        byte[] data = new byte[line.getBufferSize() / 5];

        while (true) {
            // Read bytes from the TargetDataLine
            numBytesRead =  line.read(data, 0, data.length);

            // And print the bytes to stdout
            for (int i = 0; i < numBytesRead; i++){
                if (Math.abs(data[i]) > 120) {
                    long now = System.nanoTime();
                    System.out.println("at nanotime: " + now + " heard: " + data[i]);
                }
            }
        }
    }
}
