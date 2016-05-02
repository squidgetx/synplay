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

        int PACKET_SIZE = 1024;
        byte[] audioSegment = new byte[PACKET_SIZE];

//        connect to client socket
        int PORT_NUM = 6000;
        InetAddress addr = InetAddress.getByName(args[1]);
        DatagramSocket socket = new DatagramSocket();
        socket.connect(addr,PORT_NUM);

        boolean first = true;
        while (din.read(audioSegment) != -1) {
            long playAt = -1;
            if (first) {
                first = false;
                long now = System.currentTimeMillis();
                playAt = now + 1000;
            }

            MasterPacket masterPacket = new MasterPacket(playAt, audioSegment);
            byte[] payload = masterPacket.pack();

            masterPacket.print();

            DatagramPacket packet = new DatagramPacket(payload, payload.length);
            socket.send(packet);

        }

    }
}
