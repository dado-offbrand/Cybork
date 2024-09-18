from pathlib import Path
import playsound
import openai
import speech_recorder
import time
import os

# ai voice recording
recorded_voice_path = "generated_speech/speech.wav"
speech_recorder.record_audio(recorded_voice_path)

recorded_voice_audio = open(recorded_voice_path, "rb")
transcript = openai.audio.transcriptions.create(
  model="whisper-1",
  file=recorded_voice_audio
)
recorded_voice_audio.close()
recorded_voice_text = transcript.text
print(f'RECOGNIZED: "{recorded_voice_text}"\n')

completion = openai.chat.completions.create(
  model="gpt-4o-mini",
  temperature=1.5,
  max_tokens=128,
  messages=[
    {"role": "system", "content": "Give an out of pocket, insane, but short response."},
    {"role": "user", "content": recorded_voice_text}
  ]
)
generated_response = completion.choices[0].message.content
print(f'RESPONSE: "{generated_response}"')

# ai speech synthesis
synthesis_path = "generated_speech/speech.mp3"

with openai.audio.speech.with_streaming_response.create(
  model="tts-1",
  voice="echo",
  input=generated_response,
  response_format="mp3"
) as response:
    response.stream_to_file(synthesis_path)

print("RESPONDING: Saying something horrifying.")
playsound.playsound(os.path.dirname(__file__) + "/" + synthesis_path)