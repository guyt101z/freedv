class MyPortaudioClass
{
[snip]

int myMemberCallback(const void *input, void *output,
  unsigned long frameCount,
  const PaStreamCallbackTimeInfo* timeInfo,
  PaStreamCallbackFlags statusFlags);

static int myPaCallback(
  const void *input, void *output,
  unsigned long frameCount,
  const PaStreamCallbackTimeInfo* timeInfo,
  PaStreamCallbackFlags statusFlags,
  void *userData )
{
  return ((MyPortaudioClass*)userData)
     ->myMemberCallback(input, output, frameCount, timeInfo, statusFlags);
}

[snip]
};
