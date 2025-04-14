#include "Sprite.h"

Sprite::Sprite(int spr){
	ypos = spr & 0b11111111;
	xpos = spr >> 8 & 0b11111111;
	tindex = spr >> 16 & 0b11111111;
	flags = spr >> 24;
	 

}


bool Sprite::isVoid() const{
	return !(ypos || xpos || tindex || flags);
}

Sprite::Sprite(){
	ypos = 0;
	xpos = 0;
	tindex = 0;
	flags = 0;
}
