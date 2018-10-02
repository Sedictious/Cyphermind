#include <gb/gb.h>
#include <time.h>
#include <stdio.h>

#include "tilemaps/arrow.c"
#include "tilemaps/alphabet.c"

void init();
void checkInput();
void updateSwitches();

//arrow position
UINT8 arrow_pos[0x02];

//sprite indeces
UINT8 sprites[0x05];

//holds password
UINT8 password[0x05];

UINT8 i; UINT8 j;

UINT8 correct_tls; 
UINT8 displaced_tls; 
UINT8 alpha_tls;

unsigned char tile_map[] = {0x00};


void clrScrn(){
	tile_map[0x00] = -1;
	
	for (i=0;i<20;i++){
		for (j=0;j<18;j++){
			set_bkg_tiles(i,j,1,1,tile_map);
		}
	}
}

void chkPswd(){
	correct_tls = 0;
	displaced_tls = 0;
	alpha_tls = 0;
	
	for (i=0; i<5; i++){
		if (password[i]/9 == sprites[i]/9){
			alpha_tls++;
		}
		
		if (password[i]==sprites[i]){
			correct_tls++;
			sprites[i] = -1;
		}
	}
	
	for (i=0; i<5; i++){
		if (sprites[i] != -1){
			for (j=0; j<5; j++){
				if (password[i]==sprites[j] && sprites[j]!=-1){
					displaced_tls++;
					sprites[i] = -1;
					break;
				}
			}
		}
	}
}

void setPswd(){
	printf("Set password:");
	for (i=0; i<5; i++){
		waitpad(J_START);
		printf("*");
		
		j = clock(NULL)%18;
		if (j<0)
			j*=-1;

		password[i] = j;
		
		delay(200);
	}
	waitpad(J_START);
}

void mvUp(){
	move_sprite(0, arrow_pos[0], arrow_pos[1]-16);
	arrow_pos[1] -= 16;
	for (i=0;i<5;i++){
		move_sprite(i+1, 8+24*i, arrow_pos[1]-8);
		
		tile_map[0] = sprites[i];
		set_bkg_tiles(3*i, arrow_pos[1]/8, 1, 1, tile_map);

		set_sprite_tile(i+1, 1);
	}
	chkPswd();
		tile_map[0] = (correct_tls == 0)?19:correct_tls + 8;
		set_bkg_tiles(15, arrow_pos[1]/8, 1, 1, tile_map);
		
		tile_map[0] = (displaced_tls ==0)?19:displaced_tls + 8;
		set_bkg_tiles(16, arrow_pos[1]/8, 1, 1, tile_map);
		
		tile_map[0] = (alpha_tls==0)?19:alpha_tls + 8;
		set_bkg_tiles(17, arrow_pos[1]/8, 1, 1, tile_map);	
		
	for(i=0; i<5; i++){
		
		sprites[i] = 0;
	}
	
}


void main() {
	
	init();
	tile_map[0] = 0;
	
	while(1) {
		
		checkInput();
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();
		delay(100);
	}
	
}

void init() {
	
	DISPLAY_ON;	
	setPswd();
	set_bkg_data(0,20,alphabet); 
	tile_map[0] = 3;
	//set_bkg_tiles( 0, 0, 1,20, bgmap); 
	set_bkg_tiles(0,0,1,1,tile_map);
		
	clrScrn();
	
	
	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, arrow);
	
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	set_sprite_tile(0,0);
	
	arrow_pos[0] = 8;
	arrow_pos[1] = 149;
	move_sprite(0, 8, 149);
	
	//Load alphabet sprites
	set_sprite_data(1, 19, alphabet);
	for (i=0; i<5; i++){
		set_sprite_tile(i+1,1);
		move_sprite(i + 1, 8+24*i, 141);
		sprites[i] = 0;
	}


}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

void checkInput() {

	if (joypad() & J_LEFT && arrow_pos[0]>=10) {
		arrow_pos[0]-=24;
		move_sprite(0, arrow_pos[0], arrow_pos[1]);
	}	

	else{
		if (joypad() & J_RIGHT && arrow_pos[0]<=100){
		arrow_pos[0]+=24;
		move_sprite(0, arrow_pos[0], arrow_pos[1]);
	}}
	
	//Whenever up or down is pressed, change the corresponding sprite
	if (joypad() & J_UP) {
		i = (arrow_pos[0] + 17)/24;
		sprites[i - 1] = (sprites[i - 1] + 1)%18;
		set_sprite_tile(i, sprites[i - 1] + 1);
	}
	
	else{
		if (joypad() & J_DOWN) {
			i = (arrow_pos[0] + 17)/24;
			
			if (sprites[i-1]==0){sprites[i-1] = 17;}
			else{sprites[i - 1] = (sprites[i - 1] - 1)%18;}
			set_sprite_tile(i, (sprites[i - 1] + 1));

		}
	}
	
	if (joypad() & J_A){
		if(arrow_pos[1]>=40){
			mvUp();
		}
		else{
			mvUp();
			HIDE_SPRITES;
			tile_map[0] = password[0];
			set_bkg_tiles(0, 0, 1, 1, tile_map);
	
			tile_map[0] = password[1];
			set_bkg_tiles(1, 0, 1, 1, tile_map);
	
			tile_map[0] = password[2];
			set_bkg_tiles(2, 0, 1, 1, tile_map);
	
			tile_map[0] = password[3];
			set_bkg_tiles(3, 0, 1, 1, tile_map);
	
			tile_map[0] = password[4];
			set_bkg_tiles(4, 0, 1, 1, tile_map);
			waitpad(J_START);
		}
	}
	
}
