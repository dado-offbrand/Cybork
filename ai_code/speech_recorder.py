import pyaudio
import webrtcvad
import numpy as np
from scipy.io.wavfile import write

# constants
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK_DURATION_MS = 30
CHUNK = int(RATE * CHUNK_DURATION_MS / 1000)

# init vad
vad = webrtcvad.Vad()
vad.set_mode(1) # 0: Normal, 1: Low Bitrate, 2: Aggressive, 3: Very Aggressive

# init misc audio
audio = pyaudio.PyAudio()
stream = audio.open(format=FORMAT, channels=CHANNELS, rate=RATE, input=True, frames_per_buffer=CHUNK)
print("Began listening")

# main function, does everything
def record_audio(filename):
    frames = []
    recording = False
    silence_count = 0

    try:
        while True:
            # Read raw audio data from the microphone
            buffer = stream.read(CHUNK, exception_on_overflow=False)

            if vad.is_speech(buffer, RATE):
                if not recording:
                    #print("Speech detected, recording...")
                    recording = True

                silence_count = 0
                frames.append(buffer)

            else:
                if recording:
                    # Increment silence counter
                    silence_count += 1

                    # If silence lasts for more than a few chunks, stop recording
                    if silence_count > 30:  # Adjust for how many chunks of silence you want before stopping
                        #print("Silence detected, stopping recording...")
                        break

    except KeyboardInterrupt:
        pass

    # Stop recording and save the file
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # Save audio data to a .wav file
    audio_data = b''.join(frames)
    audio_array = np.frombuffer(audio_data, dtype=np.int16)
    write(filename, RATE, audio_array)
    #print(f"Audio saved to {filename}\n")