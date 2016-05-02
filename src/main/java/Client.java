import java.net.*;
import javax.sound.sampled.*;
import java.io.*;

public class Client {
    public static void main(String args[]) throws Exception {

      // Hackaround audio format
        File file = new File("yellow.mp3");
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
        AudioFormat format = din.getFormat();

        // test
        DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);
        SourceDataLine audioLine = (SourceDataLine) AudioSystem.getLine(info);
        audioLine.open(format);
        audioLine.start();


        int PORT_NUM = 6000;
        //DatagramSocket socket = new DatagramSocket(PORT_NUM);
        MulticastSocket socket = new MulticastSocket(PORT_NUM);
        InetAddress group = InetAddress.getByName(args[0]);
        socket.joinGroup(group);

        socket.setReceiveBufferSize(25000000);


        DatagramPacket packet = new DatagramPacket(new byte[1024],1024);

        int packets = 0;
        while(true) {
          socket.receive(packet);
          packets++;
          byte[] requestData = packet.getData();

          MasterPacket masterPacket = MasterPacket.unpack(requestData);
          masterPacket.print();

          audioLine.write(masterPacket.getMusic(), 0, masterPacket.getMusic().length);
          System.out.println(packets + " pakcets received\r\n");
        }
/*
        long sleepTime = masterPacket.getTimeToPlay() - System.currentTimeMillis();
        Thread.sleep(sleepTime);

        System.out.println("Waking up at: " + System.currentTimeMillis());
        System.out.println(new String(masterPacket.getMusic()));
        audioLine.start();
        */

    }
}
