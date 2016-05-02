import java.nio.ByteBuffer;

public class MasterPacket {
//    time at which to play the song
    private long timeToPlay;

//    the music to play
    private byte[] music;

    public long getTimeToPlay() {
        return timeToPlay;
    }

    public byte[] getMusic() {
        return music;
    }

    public MasterPacket(long timeToPlay, byte[] music) {
        this.timeToPlay = timeToPlay;
        this.music = music;
    }

    public void print(){
        System.out.println("<MasterPacket: timeToPlay=" + this.timeToPlay + ">");
    }

    public byte[] pack(){
        int length = Long.BYTES + music.length;
        ByteBuffer buffer = ByteBuffer.allocate(length);
        buffer.putLong(this.timeToPlay);
        buffer.put(this.music);

        return buffer.array();
    }

    public static MasterPacket unpack(byte[] packet){
        ByteBuffer buffer = ByteBuffer.wrap(packet);

        long timeToPlay = buffer.getLong();
        byte[] music = new byte[buffer.remaining()];
        buffer.get(music);

        return new MasterPacket(timeToPlay,music);
    }
}
