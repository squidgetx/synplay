package benchmark;

import javax.sound.sampled.SourceDataLine;

public class ShoutRunner implements Runnable {

    private SourceDataLine line;

    public ShoutRunner(SourceDataLine line) {
        this.line = line;
    }

    public void run() {
        byte[] data = {127, 127, 127, 127};
        // record write time!
        long time = System.currentTimeMillis();
        this.line.write(data, 0, 4);
        System.out.println("Impulse sent at " + time);
    }
}
