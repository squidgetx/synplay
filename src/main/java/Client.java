import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Client {
    public static void main(String args[]) throws Exception {
        int PORT_NUM = 6000;
        DatagramSocket socket = new DatagramSocket(PORT_NUM);

        DatagramPacket packet = new DatagramPacket(new byte[1024],1024);

        socket.receive(packet);
        byte[] requestData = packet.getData();

        MasterPacket masterPacket = MasterPacket.unpack(requestData);
        masterPacket.print();

        long sleepTime = masterPacket.getTimeToPlay() - System.currentTimeMillis();
        Thread.sleep(sleepTime);

        System.out.println("Waking up at: " + System.currentTimeMillis());
        System.out.println(new String(masterPacket.getMusic()));
    }
}
