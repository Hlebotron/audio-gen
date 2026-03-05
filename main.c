#include <stdio.h>
#include <math.h>
#include <float.h>

#define SAMPLE_RATE 	(48e3)
#define BASE_FREQ	(440)
#define BUF_SIZE	(256)
#define VOLUME		(1.0)

#define FREQ1		(440)
#define FREQ2		(500)
#define SEMITONE(n) (BASE_FREQ * pow(2.0, ((float)(n))/12.0))

// typedef enum
// {
//     TYPE_SQUARE,
//     TYPE_SINE
// } sound_type;

typedef struct
{
    float (*fn) (float, int);
    float freq;
    float begin;
    float end;
} Note;

float square_wave(float freq, int time)
{
    float period = 1.0 / freq;
    float secs = ((float) time) / ((float)SAMPLE_RATE);
    return (fmod(secs, period) > (period/2) ? -1.0 : 1.0);
    /* return (fmod(angle, 2*M_PI) > M_PI) ? -1 : 1; */
}

float sine_wave(float freq, int time)
{
    float period = 1.0 / freq;
    float secs = ((float) time) / ((float)SAMPLE_RATE);
    return sin(2 * M_PI * freq / ((float)SAMPLE_RATE) * fmod((float)time, (float)SAMPLE_RATE));
    /* return (fmod(angle, 2*M_PI) > M_PI) ? -1 : 1; */
}

Note notes[] = {
    // Base
    {&square_wave, SEMITONE(0), 0.0, 2.0},
    {&square_wave, SEMITONE(-2), 2.0, 4.0},
    {&square_wave, SEMITONE(-4), 4.0, 6.0},
    {&square_wave, SEMITONE(-5), 6.0, 6.5},
    {&square_wave, SEMITONE(-10), 6.5, 7.0},
    {&square_wave, SEMITONE(-5), 7.0, 8.0},
    {&square_wave, SEMITONE(-7), 8.0, 10.0},
    {&square_wave, SEMITONE(-9), 10.0, 12.0},
    {&square_wave, SEMITONE(-10), 12.0, 14.0},
    {&square_wave, SEMITONE(-12), 14.0, 14.25},
    {&square_wave, SEMITONE(-13), 14.25, 14.5},
    {&square_wave, SEMITONE(-12), 14.5, 15.0},
    {&square_wave, SEMITONE(-10), 15.0, 16.0},
    
    
    {&square_wave, SEMITONE(3), 0.5, 0.75},
    {&square_wave, SEMITONE(2), 0.75, 1.0},
    {&square_wave, SEMITONE(3), 1.0, 1.25},
    {&square_wave, SEMITONE(5), 1.25, 1.5},
    {&square_wave, SEMITONE(7), 1.5, 2.0},
    

    {&square_wave, SEMITONE(7), 2.0, 2.25},
    {&square_wave, SEMITONE(5), 2.25, 2.5},
    {&square_wave, SEMITONE(7), 2.5, 2.75},
    {&square_wave, SEMITONE(8), 2.75, 3.0},
    {&square_wave, SEMITONE(10), 3.0, 4.0},


    {&square_wave, SEMITONE(12), 4.5, 4.75},
    {&square_wave, SEMITONE(10), 4.75, 5.0},
    {&square_wave, SEMITONE(12), 5.0, 5.25},
    {&square_wave, SEMITONE(14), 5.25, 5.5},
    {&square_wave, SEMITONE(15), 5.5, 6.0},
    {&square_wave, SEMITONE(14), 6.0, 7.0},
    {&square_wave, SEMITONE(10), 7.0, 8.0},

    {&square_wave, SEMITONE(8), 8.0, 8.5},
    {&square_wave, SEMITONE(7), 8.5, 9.0},
    {&square_wave, SEMITONE(8), 9.0, 9.5},
    {&square_wave, SEMITONE(12), 9.5, 10.0},
    {&square_wave, SEMITONE(10), 10.0, 10.5},
    {&square_wave, SEMITONE(8), 10.5, 11.0},
    {&square_wave, SEMITONE(7), 11.0, 11.5},
    {&square_wave, SEMITONE(3), 11.5, 12.0},

    {&square_wave, SEMITONE(5), 12.0, 13.0},
    {&square_wave, SEMITONE(3), 13.0, 13.5},
    {&square_wave, SEMITONE(7), 13.5, 14.0},
    {&square_wave, SEMITONE(5), 14.0, 15.0},
    {&square_wave, SEMITONE(7), 15.0, 16.0},
};



int play_freq(float freq, float duration)
{
    FILE* file = fopen("/home/sasha/Projects/audio/sound.raw", "ab");
    if(!file)
	return 1;
    float val[BUF_SIZE];
    for(int i = 0; i < SAMPLE_RATE * duration; i++)
    {
	val[i % BUF_SIZE] = (
	    /* sin(2 * M_PI * SEMITONE(3) / SAMPLE_RATE * fmod(i, SAMPLE_RATE)) + */
	    /* sin(2 * M_PI * SEMITONE(7) / SAMPLE_RATE * fmod(i, SAMPLE_RATE)) */
	    sine_wave(freq, i)
	    ) * VOLUME;
	if(i % BUF_SIZE == (BUF_SIZE - 1))
	    fwrite(val, sizeof(float), BUF_SIZE, file);
    }
    return 0;
}
int play_notes(Note* notes, size_t len)
{
    FILE* file = fopen("/home/sasha/Projects/audio/sound.raw", "ab");
    if(!file)
	return 1;

    float max = FLT_MIN;
    float min = FLT_MAX;
    Note current_note;
    for(int i = 0; i < len; i++)
    {
	current_note = notes[i];
	if(current_note.begin < min)
	    min = current_note.begin;

	if(current_note.end > max)
	    max = current_note.end;
    }

    float duration = max - min;
    float val[BUF_SIZE];
    float current_time;
    float (*current_fn) (float, int);
    for(int i = 0; i < SAMPLE_RATE * duration; i++)
    {
	current_time = i / SAMPLE_RATE;
	val[i % BUF_SIZE] = 0;
	for(int j = 0; j < len; j++)
	{
	    current_note = notes[j];
	    if(!(current_note.begin > current_time || current_note.end < current_time))
	    {
		current_fn = current_note.fn;
		current_fn(current_note.freq, i);
		// switch(current_note.type)
		// {
		// case TYPE_SQUARE:
		//     val[i % BUF_SIZE] += square_wave(current_note.freq, i);
		//     break;
		// case TYPE_SINE:
		//     val[i % BUF_SIZE] += sine_wave(current_note.freq, i);
		//     break;
		// }
	    }
	}
	
	if(i % BUF_SIZE == (BUF_SIZE - 1))
	    fwrite(val, sizeof(float), BUF_SIZE, file);
	
	if(i == (SAMPLE_RATE * duration - 1))
	    fwrite(val + (size_t)fmod(SAMPLE_RATE * duration, BUF_SIZE), sizeof(float), fmod(SAMPLE_RATE * duration, BUF_SIZE), file);
    }
    return 0;
}

int main()
{
    {
	FILE* file = fopen("/home/sasha/Projects/audio/sound.raw", "wb");
    }
    
    // play_freq(SEMITONE(0), 1.5);
    play_notes(notes, sizeof(notes) / sizeof(Note));

    /* play_freq(SEMITONE(3), 2.0); */
    /* play_freq(SEMITONE(0), 3.0); */
    
    return 0;
}
