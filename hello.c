
/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/

#include "neslib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <nes.h>
#include "game1TitleScreen.h"
// CHR data
//#resource "game1TitleScreen.chr"
//#link "game1ileset.s"

#define SFX_START    0
#define FP_BITS  4

const unsigned char palTitle[16]={ 0x0F, 0x16, 0x38, 0x30, 0x0F, 0x27, 0x30, 0x16 , 0x0F, 0x16, 0x27, 0x38, 0x0F, 0x30, 0x30, 0x2D};

// setup Famitone library
//#link "famitone2.s"
void __fastcall__ famitone_update(void);
//#link "music_aftertherain.s"
extern char after_the_rain_music_data[];
//#link "demosounds.s"
extern char demo_sounds[];
static unsigned char bright;
static unsigned char frame_cnt;
static unsigned char wait;
static int iy,dy;
static unsigned char i;



// link the pattern table into CHR ROM
//#link "chr_generic.s"


// draw a message on the screen
const char *breakfast[] = {
    "Bacon and Eggs ",
    "Cereal ",
    "Pancakes ",
    "Chicken and Waffles ",
    "Scrambled Eggs ",
    "Bacon Omelette ",
    "Oatmeal ",
    "Banana Bread ",
    "French Toast ",
    "Cinnamon Rolls ",
    "Migas ",
    "Breakfast Tacos ",
    "Hot Pockets ",
    "Hash Browns ",
    "Eggs Benedict ",
    "Crepe ",
    "Sausage and Eggs ",
    "Breakfast Burritos "
  };

  const char *lunch[] = {
    "Corn Dogs ",
    "Cheesesteak ",
    "Mac and Cheese ",
    "Burritos ",
    "Hot dogs ",
    "Fried Chicken ",
    "Fish and chips ",
    "Turkey ",
    "Meatloaf ",
    "Tacos ",
    "Tamales ",
    "Nachos ",
    "Salad ",
    "Peanut Butter and Jelly Sandwich ",
    "a Sandwich ",
    "Grilled Cheese ",
    "Quesadillas ",
    "Chicken Soup "
  };

  const char *dinner[] = {
    "Burgers and French Fries ",
    "Pizza ",
    "Sushi ",
    "Wings ",
    "Lobster ",
    "Chicken Cordon Bleu ",
    "Oysters ",
    "Ribs ",
    "Barbecue ",
    "Ramen ",
    "Gumbo ",
    "Dumplings ",
    "Lasagna ",
    "Spaghetti and Meatballs ",
    "Mole ",
    "Fajitas ",
    "Chili Dogs ",
    "Steak and Mashed Potatoes "
  };

void put_str(unsigned int adr, const char *str) {
  vram_adr(adr);        // set PPU read/write address
  vram_write(str, strlen(str)); // write bytes to PPU
}

void pal_fade_to(unsigned to)
{
  if(!to) music_stop();

  while(bright!=to)
  {
    delay(4);
    if(bright<to) ++bright; else --bright;
    pal_bright(bright);
  }

  if(!bright)
  {
    ppu_off();
    set_vram_update(NULL);
    scroll(0,0);
  }
}

void title_screen(void)
{
  scroll(-8,240);//title is aligned to the color attributes, so shift it a bit to the right

  vram_adr(NAMETABLE_A);
  vram_unrle(game1TitleScreen);

  vram_adr(NAMETABLE_C);//clear second nametable, as it is visible in the jumping effect
  vram_fill(0,1024);

  pal_bg(palTitle);
  pal_bright(4);
  ppu_on_bg();
  delay(20);//delay just to make it look better

  iy=240<<FP_BITS;
  dy=-8<<FP_BITS;
  frame_cnt=0;
  wait=160;
  bright=4;

  while(1)
  {
    ppu_wait_frame();

    scroll(-8,iy>>FP_BITS);

    if(pad_trigger(0)&PAD_START) break;

    iy+=dy;

    if(iy<0)
    {
      iy=0;
      dy=-dy>>1;
    }

    if(dy>(-8<<FP_BITS)) dy-=2;

    if(wait)
    {
      --wait;
    }
    else
    {
      pal_col(2,(frame_cnt&32)?0x0f:0x20);//blinking press start text
      ++frame_cnt;
    }
  }

  scroll(-8,0);//if start is pressed, show the title at whole
  sfx_play(SFX_START,0);

  for(i=0;i<16;++i)//and blink the text faster
  {
    pal_col(2,i&1?0x0f:0x20);
    delay(4);
  }

  pal_fade_to(0);
}

 

void recommend(unsigned int choice)
{
  int randomNum1 = rand()%17;
  vram_adr(NTADR_A(2,14));
  vram_write("You should try ",14);
  if(choice==1)
  put_str(NTADR_A(17,14),breakfast[randomNum1]);
   if(choice==2)
  put_str(NTADR_A(17,14),lunch[randomNum1]);
  if (choice==3)
  put_str(NTADR_A(17,14),dinner[randomNum1]);
    
}


void main(void) {

 unsigned int choice =0;

  // set palette colors
  pal_col(0,0x16);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white

  // write text to name table
  //title_screen();
  vram_write("HELLO, SENIOR PROJECT!", 22);	// write bytes to video RAM
  vram_adr(NTADR_A(2,2));
  vram_write("Welcome to The Hunger Game!",28);
  vram_adr(NTADR_A(2,6));
  vram_write("What are you craving?",21);
  vram_adr(NTADR_A(4,8));
  vram_write("Breakfast? Press A",18);
  vram_adr(NTADR_A(4,9));
  vram_write("Lunch?     Press B",18);
  vram_adr(NTADR_A(4,10));
  vram_write("Dinner?    Press Start",22);
  vram_adr(NTADR_A(2,14));
 

 
  /*
  put_str(NTADR_A(2,19),"Are you still hungry?");
  vram_adr(NTADR_A(4,21));
  vram_write("Yes?",4);
  vram_adr(NTADR_A(4,22));
  vram_write("No?",4);*/
  
  
  famitone_init(after_the_rain_music_data);
  sfx_init(demo_sounds);
  // set music callback function for NMI
  nmi_set_callback(famitone_update);
  // play music
  music_play(0);
  // enable PPU rendering (turn on screen)
  ppu_on_all();
  
      while(1) {
      
    if (pad_trigger(0) & PAD_A){
      ppu_off();
      recommend(1);
      ppu_on_all();
      //sfx_play(0,0);
	//break;
    }
    if (pad_trigger(0) & PAD_B){
      ppu_off();
      recommend(2);
      ppu_on_all();
      //sfx_play(0,0);
	//break;
    }
    if (pad_trigger(0) & PAD_START){
      ppu_off();
      recommend(3);
      ppu_on_all();
      //sfx_play(0,0);
	//break;
    }

      };


  // infinite loop
    while(1) {
      
    // poll controller 0
    char pad = pad_poll(0);
    // play sounds when buttons pushed
    if (pad & PAD_A) {
      sfx_play(0,0);
    }
    if (pad & PAD_START) {
      sfx_play(0,0);
    }
    if (pad & PAD_B) {
      sfx_play(0,0);
    }

    
  }
}
