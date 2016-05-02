import org.tritonus.share.sampled.file.AudioOutputStream;

import javax.sound.sampled.AudioFormat;
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

    public static int audioFormatLength(){
        return 2 * Float.BYTES + 5 * Integer.BYTES;
    }
    public byte[] packAudioFormat(AudioFormat format){

        ByteBuffer buffer = ByteBuffer.allocate(MasterPacket.audioFormatLength());

        AudioFormat.Encoding encoding = format.getEncoding();
        if (encoding.equals(AudioFormat.Encoding.ALAW)){
            buffer.putInt(0);
        } else if (encoding.equals(AudioFormat.Encoding.PCM_FLOAT)){
            buffer.putInt(1);
        } else if (encoding.equals(AudioFormat.Encoding.PCM_SIGNED)){
            buffer.putInt(2);
        } else if (encoding.equals(AudioFormat.Encoding.PCM_UNSIGNED)){
            buffer.putInt(3);
        } else if (encoding.equals(AudioFormat.Encoding.ULAW)){
            buffer.putInt(4);
        }

        buffer.putFloat(format.getSampleRate());
        buffer.putInt(format.getSampleSizeInBits());
        buffer.putInt(format.getChannels());
        buffer.putInt(format.getFrameSize());
        buffer.putFloat(format.getFrameRate());
        buffer.putInt((format.isBigEndian() ? 1 : 0));

        return buffer.array();
    }

    public static AudioFormat unpackAudioFormat(byte[] packet){

        ByteBuffer buffer = ByteBuffer.wrap(packet);

        AudioFormat.Encoding encoding = AudioFormat.Encoding.PCM_SIGNED;
        int encodingInt = buffer.getInt();

        switch (encodingInt){
            case 0:
                encoding = AudioFormat.Encoding.ALAW;
                break;
            case 1:
                encoding = AudioFormat.Encoding.PCM_FLOAT;
                break;
            case 2:
                encoding = AudioFormat.Encoding.PCM_SIGNED;
                break;
            case 3:
                encoding = AudioFormat.Encoding.PCM_UNSIGNED;
                break;
            case 4:
                encoding = AudioFormat.Encoding.ULAW;
                break;
        }

        float sampleRate = buffer.getFloat();
        int sampleSizeInBits = buffer.getInt();
        int channels = buffer.getInt();
        int frameSize = buffer.getInt();
        float frameRate = buffer.getFloat();
        boolean bigEndian = (buffer.getInt() == 1);

        return new AudioFormat(encoding,sampleRate,sampleSizeInBits,channels,frameSize,frameRate,bigEndian);
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
