//****Game Engine***
//Blake De Garza

//****Missles***
//Description: Fires a missle from the middle of the ship
//if the missle hits an enemy, enemy explodes!

#include "logo.h"
#include "Random.h"
struct State{
          unsigned long x;// x coordinate 
          unsigned long y;// y coordinate 
		      const unsigned char *image; //pointer to the image
          long life; //0=dead 1=alive
	};
  	typedef struct State STyp;
struct State1{
	        unsigned char x; //column of Enemy between 1-6
	        unsigned char y; //row of Enemy between 1-3
	        unsigned char shoot; //have they shot a missle
	        unsigned char clear; //dirty work
};
   typedef struct State1 ETyp;

extern const unsigned char Laser1[];
extern const unsigned char Laser0[];
extern const unsigned char SmallExplosion0[];
extern const unsigned char Missile0[];
extern const unsigned char Missile1[];
extern const unsigned char Bunker0[];
extern const unsigned char Bunker1[];
extern const unsigned char Bunker2[];
extern const unsigned char Bunker3[];
extern const unsigned char BigExplosion0[];
extern const unsigned char BigExplosion1[];
extern STyp EnemyA[];
extern STyp EnemyB[];
extern STyp Bunkers[];
extern STyp Player[];
extern  char string[];
extern unsigned long ADCMail;
unsigned long y;


unsigned long MissleDetect(unsigned long x,unsigned long y);
void MissleClear(unsigned char i);
void EnemyMissleDetect(unsigned long x, unsigned long y,unsigned char i);
void SexyScore(void);
unsigned short Ecount=0;

ETyp EnemyMissle[18]={ //these are the coordinates of the MISSLES
	  {8,30,0,1},
    {28,30,0,1},
    {48,30,0,1},
    {68,30,0,1},
    {88,30,0,1},
    {108,30,0,1},
    {8,40,0,1},
    {28,40,0,1},
    {48,40,0,1},
    {68,40,0,1},
    {88,40,0,1},
    {108,40,0,1},
    {8,50,0,1},
    {28,50,0,1},
    {48,50,0,1},
    {68,50,0,1},
    {88,50,0,1},
    {108,50,0,1},
               };
    
//**************User Shooter function*************
void MissleShoot(unsigned long x){
unsigned long f=88;
unsigned long z=0;
  
	for(y=88;y>10;y=y-8){
		z=MissleDetect(x,y);
		if(z==10){
			y=z;
		}
		RIT128x96x4_BMP(x+9,y,Laser0);
		if(y<=80){
			RIT128x96x4_BMP(x+9,f,Laser1);
			f=f-8;
		}
	}
	RIT128x96x4_BMP(x+9,17,Laser1);
}
 
//***********User Missile Detect*****************************
unsigned long MissleDetect(unsigned long x,unsigned long y){
	unsigned char i;
	x=x+9;
	for(i=0;i<18;i++){
	if((x/16)==((EnemyA[i].x)/16)){  //detecting x position with uncertainty of 8
		if((y/10) ==((EnemyA[i].y)/10)){  //detecting y position with uncertainty of 9
			if(EnemyA[i].life==1){
			EnemyA[i].life=0; //detected an enemy and successfully killed
			EnemyB[i].life=0;
			EnemyA[i].image=SmallExplosion0;
			EnemyB[i].image=SmallExplosion0;
			SexyScore();
			y=10;
			return(y);
			}
		} 
	 }
	}
	return(y);
	}

//*******EnemyMissleShoot*******
//**Clears the missle space behind it
//**If the selected enemy has no friends in front it will fire
//**Missle launches and continues to move upon every .5 second

	
	
void EnemyMissleShoot(void){
	  unsigned char i;
	  unsigned char t;
	  for(i=0;i<18;i++){
			
			if(EnemyMissle[i].shoot==1){
				
	       

				if(EnemyA[i].y==20){
			     if((EnemyA[i].life ==1)&&(EnemyA[i+6].life==0)&&(EnemyA[i+12].life==0)){
						
						RIT128x96x4_BMP(EnemyMissle[i].x,EnemyMissle[i].y,Missile0);
						EnemyMissle[i].y=EnemyMissle[i].y+9;
					if(EnemyMissle[i].y>=39){
					    RIT128x96x4_BMP(EnemyMissle[i].x,EnemyMissle[i].y-16,Missile1);	
				}
			}
		}
		if(EnemyA[i].y==30){
			if((EnemyA[i].life ==1)&&(EnemyA[i+6].life==0)){
					
					RIT128x96x4_BMP(EnemyMissle[i].x,EnemyMissle[i].y,Missile0);
					EnemyMissle[i].y=EnemyMissle[i].y+9;
					if(EnemyMissle[i].y>=49){
					    RIT128x96x4_BMP(EnemyMissle[i].x,EnemyMissle[i].y-16,Missile1);	
					}
					
				}
			}
	  if((EnemyA[i].y==40)&&(EnemyA[i].life==1)){
				RIT128x96x4_BMP(EnemyMissle[i].x,EnemyMissle[i].y,Missile0);
				EnemyMissle[i].y=EnemyMissle[i].y+9;
				if(EnemyMissle[i].y>=59){
				      RIT128x96x4_BMP(EnemyMissle[i].x,EnemyMissle[i].y-16,Missile1);	
					}
			}

			if(EnemyMissle[i].shoot==1 && (EnemyA[i].life==0) && (EnemyMissle[i].clear==1)){
				for(t=50;t<88;t=t+9){
			 RIT128x96x4_BMP(EnemyMissle[i].x,t,Missile1);
		 }
		 EnemyMissle[i].clear=0;
	 }
	 EnemyMissleDetect(EnemyMissle[i].x,EnemyMissle[i].y,i);
	 
	} //end of outside if statement (for checking if shooting)
	   
	   
	     MissleClear(i);
	} //end of outside for loop
}

//******Enemy Missle Clear*********
//clears the previous missile in order to prevent smearing
void MissleClear(unsigned char i){ 

	   if(EnemyMissle[i].y>96 && i<6){
				EnemyMissle[i].shoot=0;
			  EnemyMissle[i].y=30;
		 }
		 if(EnemyMissle[i].y>96 && i<12 && i>=6){
			  EnemyMissle[i].shoot=0;
			  EnemyMissle[i].y=40;
		 }
		 if(EnemyMissle[i].y>96 && i<18 && i>=12){
			 EnemyMissle[i].shoot=0;
			 EnemyMissle[i].y=50;
		 }
	 }		
	 
	 
	 //*******Enemy Missile Detection
	 //**Detects for Bunkers and User
void EnemyMissleDetect(unsigned long x, unsigned long y,unsigned char j){
	  unsigned char i;
	//------Detect User to fuck his shit up--------------- 
	if((x/14)==(ADCMail/14)){
		if((y/8)==(95/8)){
			if(Player[0].life==0){
				Player[0].image=BigExplosion0;
			}
			if(Player[0].life==1){
				Player[0].life--;
			}
			if(Player[0].life==2){
				Player[0].life--;
			}
		}
	}
	
	//------Detect Bunker Hits----------------------------
	for(i=0;i<3;i++){
	if((x/18)==((Bunkers[i].x)/18)){  //detecting x position with uncertainty of 8
		if((y/5) ==((Bunkers[i].y)/5)){  //detecting y position with uncertainty of 9
			if(Bunkers[i].life==1){
				Bunkers[i].life=0;
				Bunkers[i].image=Bunker3;
				RIT128x96x4_BMP(x,y,Missile1);
				EnemyMissle[j].y=50;
				EnemyMissle[j].shoot=0;
				RIT128x96x4_BMP(x,y-9,Missile1);
			 }
			if(Bunkers[i].life==2){
				Bunkers[i].life=1;
				Bunkers[i].image=Bunker2;
				RIT128x96x4_BMP(x,y,Missile1);
		    EnemyMissle[j].y=50;
				EnemyMissle[j].shoot=0;
				RIT128x96x4_BMP(x,y-9,Missile1);
				
				
			}			 
			if(Bunkers[i].life==3){
			 Bunkers[i].life=2;
			 Bunkers[i].image=Bunker1;
			 EnemyMissle[j].y=50;
			 EnemyMissle[j].shoot=0;
			 RIT128x96x4_BMP(x,y-9,Missile1);
			 
			}
			}
		 } 
    }
   }  
 	 
//********Evaluate Score and Determine ASCII value******

void SexyScore(void){
	unsigned char s;
	unsigned long score=0;
	unsigned char convert;
	for(s=0;s<18;s++){
		if(s<6 && EnemyA[s].life==0){
			score=score+30;
		}
		if((s<12 && s>= 6)&& EnemyA[s].life==0){
			score=score+20;
		}
		if((s<18 && s>=12) && EnemyA[s].life==0){
			score=score+10;
		}
	}
	convert=(score/100)+0x30;
	score=score%100;
	string[6]=convert;
	convert=(score/10)+0x30;
	score=score%10;
	string[7]=convert;
	string[8]=score+0x30;
	
	  
}