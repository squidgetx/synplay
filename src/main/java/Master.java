import javax.sound.sampled.*;
import java.net.*;
import java.io.*;

public class Master {
    public static void main(String[] args) throws Exception {

//        taken from Jon Skeet's StackOverflow answer:
// http://stackoverflow.com/questions/938304/how-to-get-audio-data-from-a-mp3
        File file = new File(args[0]);
        AudioInputStream in= AudioSystem.getAudioInputStream(file);
        AudioInputStream din = null;
        AudioFormat baseFormat = in.getFormat();
        AudioFormat decodedFormat = new AudioFormat(AudioFormat.Encoding.PCM_SIGNED,
                baseFormat.getSampleRate(),
                16,
                baseFormat.getChannels(),
                baseFormat.getChannels() * 2,
                baseFormat.getSampleRate(),
                false);
        din = AudioSystem.getAudioInputStream(decodedFormat, in);

        // test
        /*
        AudioFormat format = din.getFormat();
        DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);
        SourceDataLine audioLine = (SourceDataLine) AudioSystem.getLine(info);
        audioLine.open(format);
        audioLine.start();

        int BUFSIZE = 4096;
        byte[] buffer = new byte[BUFSIZE];
        int read = -1;

        while ((read = din.read(buffer)) != -1) {
          audioLine.write(buffer, 0, read);
        }
        */
        

        int PACKET_SIZE = 1024;
        byte[] audioSegment = new byte[PACKET_SIZE];

//        connect to client socket
        int PORT_NUM = 6000;
        InetAddress addr = InetAddress.getByName(args[1]);
        DatagramSocket socket = new DatagramSocket();
        socket.connect(addr,PORT_NUM);

        boolean first = true;
        int packets = 0;
        while (din.read(audioSegment) != -1) {
            
            long playAt = -1;
            if (first) {
                first = false;
                long now = System.currentTimeMillis();
                playAt = now + 1000;
            }

            MasterPacket masterPacket = new MasterPacket(packets, playAt, audioSegment);
            byte[] payload = masterPacket.pack();

            masterPacket.print();

            DatagramPacket packet = new DatagramPacket(payload, payload.length);
            socket.send(packet);
            packets++;

        }

        System.out.println(packets + " packets sent");

    }
}
