package benchmark;

import javax.sound.sampled.TargetDataLine;

public class ListenRunner implements Runnable {

    private TargetDataLine line;

    public ListenRunner(TargetDataLine line) {
        this.line = line;
    }

    public void run() {
        // Begin audio capture.
        this.line.start();

        int numBytesRead;
        byte[] data = new byte[this.line.getBufferSize() / 5];

        while (true) {
            // Read bytes from the TargetDataLine
            numBytesRead =  this.line.read(data, 0, data.length);

            // And print the bytes to stdout
            for (int i = 0; i < numBytesRead; i++){
                if (Math.abs(data[i]) > 120) {
                    long now = System.currentTimeMillis();
                    System.out.println("at nanotime: " + now + " heard: " + data[i]);
                }
            }
        }
    }
}
