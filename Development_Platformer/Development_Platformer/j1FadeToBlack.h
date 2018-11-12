#ifndef __j1MODULEFADETOBLACK_H__
#define __j1MODULEFADETOBLACK_H__

#include "j1Module.h"

#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);

	bool IsFading() const;
	
private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;

	j1Module* to_disable;
	j1Module* to_enable;

public:
	fade_step GetStep() { return current_step; }
};

#endif // __j1MODULEFADETOBLACK_H__
