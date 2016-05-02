import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Master {
    public static void main(String[] args) throws Exception {
        String filename = args[0];
        int PORT_NUM = 6000;
        InetAddress addr = InetAddress.getByName(args[1]);

        DatagramSocket socket = new DatagramSocket();
        socket.connect(addr,PORT_NUM);

        long now = System.currentTimeMillis();
        long playAt = now + 1000;

        String toSend = "this is the first message";
        byte[] bytes = toSend.getBytes();
        DatagramPacket packet = new DatagramPacket(bytes,bytes.length);

        socket.send(packet);
    }
}
