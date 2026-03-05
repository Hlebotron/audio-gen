#include <stdio.h>
#include <math.h>

#define SAMPLE_RATE 	(48e3)
#define BASE_FREQ	(440)
#define BUF_SIZE	(256)
#define VOLUME		(1.0)

#define FREQ1		(440)
#define FREQ2		(500)

double square_wave(float freq, int time)
{
    float period = 1.0 / freq;
    float secs = ((float) time) / ((float)SAMPLE_RATE);
    return (fmod(secs, period) > (period/2) ? -1.0 : 1.0);
    /* return (fmod(angle, 2*M_PI) > M_PI) ? -1 : 1; */
}

#define SEMITONE(n) (BASE_FREQ * pow(2.0, ((float)(n))/12.0))

int play_freq(float freq, float duration)
{
    /* printf("Sematones: 3: %f; 7: %f", ), SEMITONE(7)); */
    FILE* file = fopen("/home/sasha/Projects/audio/sound.raw", "ab");
    if(!file)
	return 1;
    float val[BUF_SIZE];
    for(int i = 0; i < SAMPLE_RATE * duration; i++)
    {
	val[i % BUF_SIZE] = (
	    /* sin(2 * M_PI * SEMITONE(3) / SAMPLE_RATE * fmod(i, SAMPLE_RATE)) + */
	    /* sin(2 * M_PI * SEMITONE(7) / SAMPLE_RATE * fmod(i, SAMPLE_RATE)) */
	    square_wave(freq, i)
	    ) * VOLUME;
	if(i % BUF_SIZE == (BUF_SIZE - 1))
	    fwrite(val, sizeof(float), BUF_SIZE, file);
    }
    return 0;
}
/* int play_sounds() */

int main()
{
    {
	FILE* file = fopen("/home/sasha/Projects/audio/sound.raw", "wb");
    }
    play_freq(SEMITONE(0), 1.5);
    play_freq(SEMITONE(3), 2.0);
    /* play_freq(SEMITONE(0), 3.0); */
    return 0;
}
