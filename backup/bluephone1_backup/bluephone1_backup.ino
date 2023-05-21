

#define debug 0

// display stuff
#define IIC_SCL 12
#define IIC_SDA 11

const bool numbers[4][5][3] PROGMEM =
//bool numbers[4][5][3] = 
{
  {{0,1,0},
  {0,1,0},
  {0,1,0},
  {0,1,0},
  {0,1,0}},

  {{1,1,1},
  {0,0,1},
  {1,1,1},
  {1,0,0},
  {1,1,1}},

  {{1,1,1},
  {0,0,1},
  {1,1,1},
  {0,0,1},
  {1,1,1}},
  
  {{1,0,1},
  {1,0,1},
  {1,1,1},
  {0,0,1},
  {0,0,1}},
};

const bool coin_message[5][42] PROGMEM =
//bool coin_message[5][42] =
{
    {0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
    {0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
};

bool table[7][16] = 
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

//hello - do I need these here?
void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();
void update_screen();

//keypad stuff
bool button_status[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool button_status_last[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long debounce_time[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int debounce_delay = 10;
int rowPins[4] = {9, 7, 8, 6}; 
int colPins[3] = {5, 3, 4}; 
int followPin = 10;
int last_button = 0;

//coin stuff
#define solenoid_1 A2
#define solenoid_2 A0
#define coin_LED A1
#define coinPin 13
bool coin_status = 0;

//MIDI stuff
#if (debug == 0)
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

//note stuff
int octave1 = 0;
int octave2 = 0;
int scale[2][12] = {{0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19},{0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19}};
bool vibe = 0;
int key = 0;
int note1 = 48;
int note2 = 48;
int note3 = 48;
int note4 = 48;
int note5 = 0;
int note6 = 0;


//beat stuff
#define max_beats 64
#define drum_length 16
int count_in = 0;
bool drum_mute = 0;
bool chord_mute = 0;
bool lead_mute = 1;

bool drums[2][3][drum_length] = {
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }
};

int chords[2][64] =
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int leads[2][64] =
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int bpm = 120;
int bpm_delay = 15000/bpm;
long last_beat = 0;
int beat_count = 0;

bool beat_display[8] = {0,0,0,0,0,0,0,0};

int min_bpm = 40;
int max_bpm = 300;

//presets

#define number_of_chord_presets 9

int chord_preset = 0;
const int chord_presets[number_of_chord_presets][2][64] PROGMEM =
//int chord_presets[number_of_chord_presets][2][64] =
{
    //1 - DOO-WOP/CALL ME MAYBE
    {
        {
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        6, 6, 6, 6, 6, 6, 6, 6, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        4, 4, 4, 4, 4, 4, 4, 4, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        0, 0, 0, 0, 0, 0, 0, 0,
        },
        {
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5,  
        0, 0, 0, 0, 0, 0, 0, 0, 
        4, 4, 4, 4, 4, 4, 4, 4, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //2 - ANIME CHORDS
    {
        
        {   
        4, 4, 4, 4, 4, 4, 4, 4, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5,
        0, 0, 0, 0, 0, 0, 0, 0, 
        3, 3, 3, 3, 3, 3, 3, 3, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        6, 6, 6, 6, 6, 6, 6, 6, 
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //3 - KIDS
    {
        
        {   
        6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //4
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //5
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //6
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //7
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //8
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //9
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },
};

const int lead_presets[number_of_chord_presets][2][64] PROGMEM =
//int chord_presets[number_of_chord_presets][2][64] =
{
    //1 - DOO-WOP/CALL ME MAYBE
    {
        {
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        6, 6, 6, 6, 6, 6, 6, 6, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        4, 4, 4, 4, 4, 4, 4, 4, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        0, 0, 0, 0, 0, 0, 0, 0,
        },
        {
        1, 1, 1, 1, 1, 1, 1, 1, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5,  
        0, 0, 0, 0, 0, 0, 0, 0, 
        4, 4, 4, 4, 4, 4, 4, 4, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //2 - ANIME CHORDS
    {
        
        {   
        4, 4, 4, 4, 4, 4, 4, 4, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        5, 5, 5, 5, 5, 5, 5, 5,
        0, 0, 0, 0, 0, 0, 0, 0, 
        3, 3, 3, 3, 3, 3, 3, 3, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        6, 6, 6, 6, 6, 6, 6, 6, 
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //3 - KIDS
    {
        
        {   
        1, 	1, 	1, 	1, 	2, 	2, 	2, 	2, 	3, 	3, 	3, 	3, 	5, 	5, 	5, 	5, 	6, 	6, 	6, 	6, 	7, 	7, 	6, 	6, 	6, 	6, 	5, 	5, 	5, 	5, 	5, 	5, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	3, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2, 	2,
        },

        {   
        6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	6, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	4, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	1, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 	5, 
        },
    },

    //4
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //5
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //6
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //7
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //8
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },

    //9
    {
        
        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },

        {   
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        },
    },
};


#define number_of_drum_presets 9

int drum_preset = 0;
const bool drum_presets[number_of_drum_presets][3][drum_length] PROGMEM =
{
    //preset 1 - basic
    {
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},       
    },
    
    //preset 2 - double speed
    {
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0},       
    },

    //preset 3 - break beat
    {
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},       
    },

    //preset 4 - be my baby
    {
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},        
    },

    //preset 5 - disco
    {
        {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},       
    },

    //preset 6 - offbeats
    {
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},       
    },
    
    //preset 7 - hihats in threes
    {
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},       
    },

    //preset 8 - snare on each beat
    {
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},       
    },

    //preset 9 - half time
    {
        {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},       
    },
    

};

//mode stuff

int phone_number[8] = {0,0,0,0,0,0,0,0};
int phone_number_index = 0;
int mode = 0;
bool chord_track = 0;
bool lead_track = 0;
bool drum_track = 0;
int mode_shift = 0;

void reset()
{
    mode = 0;
    chord_track = 0;
    lead_track = 0;
    drum_track = 0;
    bpm = 120;
    phone_number_index = 0;
    
    //clear chords
    for (int i = 0; i < 64; i++)
    {
        chords[0][i] = 0;
        chords[1][i] = 0;
        leads[0][i] = 0;
        leads[1][i] = 0;
    }
    
    //clear drums
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < drum_length; j++)
        {
            drums[0][i][j] = 0;
            drums[1][i][j] = 0;
        }
    }
    clear_table();

    delay(500);
    digitalWrite(solenoid_1, 0);
}

void setup()
{
    //display stuff
    pinMode(IIC_SCL, OUTPUT);
    pinMode(IIC_SDA, OUTPUT);
    digitalWrite(IIC_SCL, LOW);
    digitalWrite(IIC_SDA, LOW);

    //keypad stuff
    pinMode(colPins[0], OUTPUT);
    pinMode(colPins[1], OUTPUT);
    pinMode(colPins[2], OUTPUT);
    pinMode(rowPins[0], INPUT);
    pinMode(rowPins[1], INPUT);
    pinMode(rowPins[2], INPUT);
    pinMode(rowPins[3], INPUT);
    pinMode(solenoid_1, OUTPUT);
    pinMode(solenoid_2, OUTPUT);
    pinMode(coin_LED, OUTPUT);
    pinMode(followPin, INPUT);
    pinMode(coinPin, INPUT_PULLUP);

    //coin stuff
    digitalWrite(solenoid_1, LOW);
    digitalWrite(solenoid_2, HIGH);
    digitalWrite(coin_LED, HIGH);

    #if (debug == 0)
    MIDI.begin(MIDI_CHANNEL_OMNI);   
    #endif

    #if (debug == 1)
    Serial.begin(9600);
    #endif

}

void loop()
{

//check buttons
button_check();

//check the beat
if (millis() - last_beat > bpm_delay)
{
    last_beat = millis();
    update_table();
    the_beat();
}
else

// records onto the previous beat every moment after the beat
// - hello, could do something like this, where late notes go forward and early notes go back
if (millis() - last_beat < (bpm_delay/2))
{
    check_record();
}

}

void clear_table()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            table[j][i] = 0;
        }
    }

}

void update_table()
{
    switch (mode)
    {
        case 0: //displays the "insert coin message"
            for (int i = 0; i < 16; i++)
            {
                for (int j = 1; j < 6; j++)
                {
                    if((beat_count + i)%64 < 42)
                    {
                      table[j][i] = pgm_read_byte(&coin_message[j-1][(beat_count + i)%64]);
                      //table[j][i] = coin_message[j-1][(beat_count + i)%64];
                    }
                    else
                    {
                      table[j][i] = 0;
                    }
                }
            }

        break;
        case 1:
            //phone number appears as it's being typed
        break;
        case 2:
        case 3:
        case 4:

            //displays the beat
            for (int i = 0; i < 4; i++)
            {
                if (beat_count % 4 == i)
                {
                    table[1][11+i] = 1;
                }
                else
                {
                    table[1][11+i] = 0;
                }            
            
                if ((beat_count%16)/4 == i)
                {
                    table[3][11+i] = 1;
                }
                else
                {
                    table[3][11+i] = 0;
                } 
                if ((beat_count/16) == i)
                {
                    table[5][11+i] = 1;
                }
                else
                {
                    table[5][11+i] = 0;
                }            
            }

        break;
        case 5:
            //control centre display
        break;
    }
    //every time the table updates, the screen updates
    update_screen();
}

void the_beat()
{       
    switch(mode)
    {
        case 1: // if phone number is entered, count in
            if(phone_number_index == 8)
            {
                table[0][count_in] = 1;
                table[6][16-count_in] = 1;
              
                if(count_in % 4 == 0)
                {
                    for(int i = 0; i<16; i++)
                    {
                        for(int j = 1; j<6; j++)
                        {
                            table[j][i] = 0;
                        }           
                    }

                    if(count_in == 0)
                    { 
                        for(int i = 0; i<3; i++)
                        {
                            for(int j = 0; j<5; j++)
                            {
                                table[j+1][i+1] = pgm_read_byte(&numbers[0][j][i]);
                                //table[j+1][i+1] = numbers[0][j][i];
                            }           
                        }
                    }
                    else
                    {
                        for(int i = 0; i<3; i++)
                        {
                            for(int j = 0; j<5; j++)
                            {
                                table[j+1][i+count_in] = pgm_read_byte(&numbers[count_in/4][j][i]);
                                //table[j+1][i+count_in] = numbers[count_in/4][j][i];
                            }           
                        }                    
                    } 
                }

                /*if(count_in == 4)
                {
                    MIDI.sendNoteOff(note5, 0, 6);
                }*/
                
                if(count_in == 16)
                {
                    phone_number_index = 0;
                    beat_count = max_beats-1;
                    mode = 2;
                    clear_table();
                    count_in = 0;
                }
                else
                {
                    count_in++;
                }
            }
        break;
        
        
        case 2:
        case 3:
        case 4:
        case 5:
        #if (debug == 0)
            
            //play chord sequence if no other chord is playing
             
            if ((last_button == 0 && button_status[11] == 0) || mode != 2)
            {
                if (48 + scale[vibe + chords[chord_track][beat_count] - 1] + octave1*12 + key != note1)
                {
                    MIDI.sendNoteOff(note1, 0, 2);
                    chord_visualisation(chords[chord_track][beat_count], 1);

                    note1 = 48 + scale[vibe][chords[chord_track][beat_count] - 1] + octave1*12 + key;
                    note2 = 48 + scale[vibe][chords[chord_track][beat_count] + 1] + octave1*12 + key;
                    note3 = 48 + scale[vibe][chords[chord_track][beat_count] + 3] + octave1*12 + key;

                    /*if (chords[chord_track][beat_count] > 0 && chord_mute == 0)
                    {
                        MIDI.sendNoteOn(note1, 127, 2);
                        MIDI.sendNoteOn(note2, 127, 3);
                        MIDI.sendNoteOn(note3, 127, 4);
                    }*/
                }
            }

            if ((last_button == 0 && button_status[11] == 0) || mode != 3)
            {
                if (48 + scale[vibe][leads[lead_track][beat_count] - 1] + octave2*12 + key != note4)
                { 
                    MIDI.sendNoteOff(note4, 0, 5);

                    note4 = 48 + scale[vibe][leads[lead_track][beat_count] - 1] + octave2*12 + key;

                    /*if (leads[lead_track][beat_count] > 0 && lead_mute == 0)
                    {
                        MIDI.sendNoteOn(note4, 127, 5);
                    }*/
                }
            }

            //play drums
            for(int i = 0; i < 3; i++)
                if (mode == 4 && button_status[i+1] == 1)
                {
                    MIDI.sendNoteOn(i, 127, 10);
                }
                else if (mode != 4 || button_status[4 + i] == 0)
                { 
                    if (drums[drum_track][2-i][beat_count%drum_length] == 1 && drum_mute == 0)
                    {
                        MIDI.sendNoteOn(i, 127, 10);
                    }
                }

              if (chords[chord_track][beat_count] > 0 && chord_mute == 0)
                    {
                        MIDI.sendNoteOn(note1, 127, 2);
                        MIDI.sendNoteOn(note2, 127, 3);
                        MIDI.sendNoteOn(note3, 127, 4);
                    }
              if (leads[lead_track][beat_count] > 0 && lead_mute == 0)
                    {
                        MIDI.sendNoteOn(note4, 127, 5);
                    }
              
        #endif
        break;
    }
  
    beat_count++;
    if (beat_count == max_beats)
    {
        beat_count = 0;
    }
}

void button_check()
{
    //check keypad
    for(int i = 0; i < 3; i++)
    {
        digitalWrite(colPins[i], LOW);
        for(int j = 0; j < 4; j++)
        {
            button_status[3*j + i + 1] = !digitalRead(rowPins[j]);
            if (button_status[3*j + i + 1] != button_status_last[3*j + i + 1])
            {
                if ((millis() - debounce_time[3*j + i + 1]) > debounce_delay)
                {
                    button_update(3*j + i + 1, button_status[3*j + i + 1]);
                    button_status_last[3*j + i + 1] = button_status[3*j + i + 1];
                    debounce_time[3*j + i + 1] = millis();
                }
            }
        }
        digitalWrite(colPins[i], HIGH);
    }
    
    //check follow-on
    button_status[0] = !digitalRead(followPin);
    if (button_status[0] != button_status_last[0])
    {
        if ((millis() - debounce_time[0]) > debounce_delay)
        {
            button_update(0, button_status[0]);
            button_status_last[0] = button_status[0];
            debounce_time[0] = millis();
        }
    }

    //check coin
    button_status[13] = digitalRead(coinPin);
    if (button_status[13] != button_status_last[13])
    {
        if ((millis() - debounce_time[13]) > debounce_delay*2)
        {
            button_update(13, button_status[13]);
            button_status_last[13] = button_status[13];
        }
    }
}

void check_record()
{
    int record_beat = beat_count - 1;
    if (record_beat == -1)
    {
        record_beat = 63;
    }
    
    switch (mode)
    {
        case 2:
            if(button_status[11] == 1)
            {
                chords[chord_track][record_beat] = last_button;
            }
        break;

        case 3:
            if(button_status[11] == 1)
            {
                leads[lead_track][record_beat] = last_button;
            }
        break;

        case 4:
            for (int i = 1; i < 4; i++)
            {
                if(button_status[11] == 1 && button_status[i] == 1)
                {
                    drums[drum_track][3-i][record_beat%drum_length] = 1;              
                }    
                else if (button_status[11] == 1 && button_status[3+i] == 1) 
                {
                    drums[drum_track][3-i][record_beat%drum_length] = 0;
                }
            }

    }
}


void phone_number_check()
{
    bpm = 100*phone_number[0] +  10*phone_number[1] +  phone_number[2];
    
    if (bpm > max_bpm)
    {
        bpm = max_bpm;
    } 
    if (bpm < min_bpm)
    {
        bpm = min_bpm;
    } 
    bpm_delay = 15000/bpm;


    key = phone_number[3];
    if (key > 7)
    {
        key = 7;
    } 

    if (phone_number[4] == 0)
    {
        vibe = 0; 
    }
    else
    {
        vibe = 1;
    }

    drum_preset = phone_number[6];
    load_preset(0, drum_preset, 0);
    
    drum_preset = phone_number[7];
    load_preset(0, drum_preset, 1);
    
    chord_preset = phone_number[5];
    load_preset(1, chord_preset, 0);
}

void load_preset(int in, int pr, int tr)
{ // in is the instrument (0 for drums, 1 for chords), pr is the preset (1-9 or 0 for nothing, tr is the track (0 or 1).
    if (pr > 0)
    {
        if (in == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < drum_length; j++)
                {
                    drums[tr][i][j] = pgm_read_byte(&drum_presets[pr-1][i][j]);
                }
            }
        }

        if (in == 1)
        {
            for (int i = 0; i < 64; i++)
            {
                chords[0][i] = pgm_read_word(&chord_presets[pr-1][0][i]);
                chords[1][i] = pgm_read_word(&chord_presets[pr-1][1][i]);
                leads[0][i] = pgm_read_word(&lead_presets[pr-1][0][i]);
                leads[1][i] = pgm_read_word(&lead_presets[pr-1][1][i]);
                //chords[i] = chord_presets[pr-1][tr][i];
            }
        }
    }
    else
    {
        if (in == 0)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < drum_length; j++)
                    {
                        drums[tr][i][j] = 0;
                    }
                }
            }

            if (in == 1)
            {
                for (int i = 0; i < 64; i++)
                {
                    chords[0][i] = 0;
                    chords[1][i] = 0;
                    leads[0][i] = 0;
                    leads[1][i] = 0;
                }
            }
    }

}

void button_update(int button, bool state)
{  
    
    if(button_status[0] == 1 && state == 1)
    {
        if(button == 1)
        {
            mode = 4;
            clear_table();
        }
        if(button == 2)
        {
            mode = 2;
            clear_table();
        }
        if(button == 3)
        {
            mode = 3;
            clear_table();
        }
        if(button == 4)
        {
            drum_mute = !drum_mute;
        }
        if(button == 5)
        {
            chord_mute = !chord_mute;
        }
        if(button == 6)
        {
            lead_mute = !lead_mute;
        }
        if(button == 7)
        {
            drum_track = !drum_track;
        }
        if(button == 8)
        {
            chord_track = !chord_track;
        }
        if(button == 9)
        {
            lead_track = !lead_track;
        }
        if(button == 10)
        {
            key--;
        }
        if(button == 11)
        {
            key++;
        }
        if(button == 12)
        {
            digitalWrite(solenoid_1, state);
            reset();
        }

        mode_shift = button;
    }
    else
    {       
        switch (mode) {
            case 0: //if coin detected, move to mode 1
                if (state == 1 && button == 13)
                {
                    mode = 1;
                    MIDI.sendNoteOn(0, 127, 6);
                    clear_table();
                }
            break;
            
            case 1: //waits for the phone number to be keyes in
                if ((state == 1 && phone_number_index < 8)&&(button > 0 && button < 13))
                {
                    phone_number[phone_number_index] = button;
                    if (phone_number[phone_number_index] == 11)
                    {
                        phone_number[phone_number_index] = 0;
                    }
                    table[3][phone_number_index + 4] = 1;
                    update_table();
                    phone_number_index++;
                    if (phone_number_index == 8)
                    {
                        clear_table();
                        phone_number_check();
                    }

                note5 = button;

                MIDI.sendNoteOn(note5, 127, 6);
                }
                else
                {
                MIDI.sendNoteOff(note5, 0, 6);
                }


            break;

            case 2: //buttons for the 2 synth modes
            case 3:
                switch (button) {
                    case 9:
                        if (button_status[11] == 0)
                        {
                            if (mode == 2 && state == 1)
                            {
                                octave1++;
                                if (octave1 > 2)
                                {
                                    octave1 = 2;
                                }
                            }
                            
                            if (mode == 3 && state == 1)
                            {
                                octave2++;
                                if (octave2 > 2)
                                {
                                    octave2 = 2;
                                }
                            }
                        }
                    break;
            
                    case 10:
                        if (button_status[button] == 1 && button_status[11] == 0)
                        {
                            if (mode == 2)
                            {
                                chord_track = !chord_track;
                                lead_track = chord_track;
                            }
                            else
                            {
                                lead_track = !lead_track;
                                chord_track = lead_track;
                            }
                        }
                    break;

                    case 12:
                        if (button_status[11] == 0)
                        {
                            if (mode == 2 && state == 1)
                            {
                                octave1--;
                                if (octave1 < -2)
                                {
                                    octave1 = -2;
                                }
                            }
                            
                            if (mode == 3 && state == 1)
                            {
                                octave2--;
                                if (octave2 < -2)
                                {
                                    octave2 = -2;
                                }
                            }
                        }
                    break;
            
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:

                        if (mode == 2)
                        {
                            chord_visualisation(button, state);
                        }
                        else
                        {
                            note_visualisation(button, state);
                        }

                        if (state == 1)
                        {
                            /* #if (debug == 0)
                            if (mode == 2)
                            {
                                //MIDI.sendNoteOff(note1, 0, 2);
                                //MIDI.sendNoteOff(note2, 0, 3);
                                //MIDI.sendNoteOff(note3, 0, 4);
                            }
                            if (mode == 3)
                            {
                                MIDI.sendNoteOff(note4, 0, 5);
                            }
                            #endif

                            */ //check if any of this is needed. if not, delete
                            
                            #if (debug == 0)
                            if (mode == 2)
                            {
                                note1 = 48 + scale[vibe][button - 1] + octave1*12 + key;
                                note2 = 48 + scale[vibe][button + 1] + octave1*12 + key;
                                note3 = 48 + scale[vibe][button + 3] + octave1*12 + key;
                                MIDI.sendNoteOn(note1, 127, 2);
                                MIDI.sendNoteOn(note2, 127, 3);
                                MIDI.sendNoteOn(note3, 127, 4);
                            }
                            
                            if (mode == 3)
                            {
                                note4 = 48 + scale[vibe][button - 1] + octave2*12 + key;
                                MIDI.sendNoteOn(note4, 127, 5);
                            }
                            #endif

                            last_button = button;
                        }
                        else
                        {
                            if (button == last_button)
                            {
                                #if (debug == 0)
                                if (mode == 2)
                                {
                                    MIDI.sendNoteOff(note1, 0, 2);
                                    //MIDI.sendNoteOff(note2, 0, 3);
                                    //MIDI.sendNoteOff(note3, 0, 4);
                                }
                                else
                                {
                                    MIDI.sendNoteOff(note4, 0, 5);
                                }
                                #endif

                                last_button = 0;
                            }
                        }

                    break;
                }
            break;

            case 4:
                switch (button) {
                    case 7:
                        for (int i = 0; i < drum_length; i++)
                        {
                            drums[drum_track][2][i] = pgm_read_byte(&drum_presets[0][2][i]);
                        }
                    break;

                    case 8:
                        for (int i = 0; i < drum_length; i++)
                        {
                            drums[drum_track][1][i] = pgm_read_byte(&drum_presets[0][1][i]);
                        }
                    break;

                    case 9:
                        if (state == 1)
                        {
                            bpm++;
                            bpm_delay = 15000/bpm;
                        }
                    break;

                    case 10:
                        if (state == 1)
                        {
                            drum_track = !drum_track;
                        }
                    break;

                    case 12:
                        if (state == 1)
                        {
                            bpm--;
                            bpm_delay = 15000/bpm;
                        }
                    break;

                }
            break;
        
    }
  }
}

void update_screen()
{
    IIC_start();
    IIC_send(0x40);// set the address plus 1 automatically
    IIC_end();
    IIC_start();
    IIC_send(0xc0);// set the initial address as 0
    for (int i = 15; i > -1; i--)
    {
        IIC_send(table[6][i]+2*table[5][i]+4*table[4][i]+8*table[3][i]+16*table[2][i]+32*table[1][i]+64*table[0][i]);
    }
    IIC_end();

    IIC_start();
    IIC_send(0x8A);// set the brightness display
    IIC_end();
}

void IIC_start()
{
    digitalWrite(IIC_SCL, LOW);
    delayMicroseconds(3);
    digitalWrite(IIC_SDA, HIGH);
    delayMicroseconds(3);
    digitalWrite(IIC_SCL, HIGH);
    delayMicroseconds(3);
    digitalWrite(IIC_SDA, LOW);
    delayMicroseconds(3);
}

void IIC_send(unsigned char send_data)
{
    for (char i = 0; i < 8; i++) 
    {
        digitalWrite(IIC_SCL, LOW);
        delayMicroseconds(3);
        if (send_data & 0x01)
        {
            digitalWrite(IIC_SDA, HIGH);
        }
        else
        {
            digitalWrite(IIC_SDA, LOW);
        }
        delayMicroseconds(3);
        digitalWrite(IIC_SCL, HIGH);
        delayMicroseconds(3);
        send_data = send_data >> 1;
    }
}

void IIC_end()
{
    digitalWrite(IIC_SCL, LOW);
    delayMicroseconds(3);
    digitalWrite(IIC_SDA, LOW);
    delayMicroseconds(3);
    digitalWrite(IIC_SCL, HIGH);
    delayMicroseconds(3);
    digitalWrite(IIC_SDA, HIGH);
    delayMicroseconds(3);
}

void note_visualisation(int note, bool state)
{
    if (mode == 2 || mode == 3)
    {
        table[3][note] = state;
    }
}

void chord_visualisation(int note, bool state)
{
    if (mode == 2 || mode == 3)
    {
        if (state == 1)
        {
            for(int i = 1; i < 6; i++)
            {
              for (int j = 1; j < 8; j++)
              {
                  if (i != 3)
                  table[i][j] = 0;
              }
            }
        
        }
        if (note > 0)
        {
        // visualisation of the root note being played
            for(int i = 1; i < 6; i++)
            {
              if (i != 3)
              {
                  table[i][note] = state;
              }
            }

            // visualisation of the other notes being played
            for(int i = 2; i < 5; i++)
            {
              int t = note + 2;
              if (t > 7)
              {
                t = t - 7;
              }

              int f = note + 4;
              if (f > 7)
              {
                f = f - 7;
              }
              
              if (i != 3)
              {
                  table[i][t] = state;
                  table[i][f] = state;
              }
            }
        }
    }
}

