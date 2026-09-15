#include "Skins.h"
Skins::Skins(SDL_Renderer* render) : render(render)
{
	for (int i = 0; i < SkinsCount; i++)
		for (int j = 0; j < Move::Count; j++)
			skins[i][j] = new Animation(render);

	skins[0][idle_horiz]->Add("animations\\walk\\sprite_sheets\\to_idle.png", { 0, 0, 89, 57 }, { 2, 1, 20, 2.3 }, once);
	skins[0][idle_horiz]->Add("animations\\idle\\sprite_sheets\\idle.png", { 0, 0, 459, 54 }, { 10, 1, 110, 2.3 });
	skins[0][walk_horiz]->Add("animations\\walk\\sprite_sheets\\from_idle.png", { 0, 0, 89, 57 }, { 2, 1, 20, 2.3 }, once);
	skins[0][walk_horiz]->Add("animations\\walk\\sprite_sheets\\walk.png", { 0, 0, 179, 347 }, { 4, 6, 60, 2.3 });
	skins[0][walk_up]->Add("animations\\walk\\sprite_sheets\\walk.png", { 0, 0, 179, 347 }, { 4, 6, 70, 2.3 });
	skins[0][walk_down]->Add("animations\\walk\\sprite_sheets\\walk.png", { 0, 0, 179, 347 }, { 4, 6, 70, 2.3 });

	skins[1][idle_horiz]->Add("CatPackFree\\idle.png", { 0, 0, 319, 31 }, { 10, 1, 150, 3 });
	skins[1][walk_horiz]->Add("CatpackFree\\Box3.png", { 0, 0, 127, 31 }, { 4, 1, 200, 3 });
	skins[1][walk_up] = nullptr;
	skins[1][walk_down] = nullptr;

	skins[2][idle_horiz]->Add("Sprout_Lands\\Characters\\Basic.png", { 0, 0, 95, 47 }, { 2, 1, 1000, 3 });
	skins[2][walk_horiz]->Add("Sprout_Lands\\Characters\\Basic.png", { 0, 96, 191, 143 }, { 4, 1, 150, 3 });
	skins[2][walk_up]->Add("Sprout_Lands\\Characters\\Basic.png", { 0, 48, 191, 95 }, { 4, 1, 150, 3 });
	skins[2][walk_down]->Add("Sprout_Lands\\Characters\\Basic.png", { 0, 0, 191, 47 }, { 4, 1, 150, 3 });

	skins[3][idle_horiz]->Add("The_Female_Adventurer\\Idle\\Idle.png", { 0, 0, 383, 63 }, { 8, 1, 200, 3.5 });
	skins[3][walk_horiz]->Add("The_Female_Adventurer\\Walk\\walk.png", { 0, 64, 383, 127 }, { 8, 1, 120, 3.5 });
	skins[3][walk_up]->Add("The_Female_Adventurer\\Walk\\walk.png", { 0, 192, 383, 255 }, { 8, 1, 120, 3.5 });
	skins[3][walk_down]->Add("The_Female_Adventurer\\Walk\\walk.png", { 0, 0, 383, 63 }, { 8, 1, 120, 3.5 });

	skins[4][idle_horiz]->Add("FreeCatCharacterAnimations\\idle.png", { 0, 0, 255, 31 }, {8, 1, 140, 4});
	skins[4][idle_horiz]->Add("FreeCatCharacterAnimations\\jump.png", { 0, 0, 127, 31 }, {4, 1, 200, 4}, once);
	skins[4][walk_horiz]->Add("FreeCatCharacterAnimations\\run.png", { 0, 0, 319, 31 }, {10, 1, 90, 4});
	skins[4][walk_up] = nullptr;
	skins[4][walk_down] = nullptr;

	skins[5][idle_horiz]->Add("Goldie_pack\\move.png", { 64, 120, 127, 159 }, {2, 1, 250, 3}, once);
	skins[5][idle_horiz]->Add("Goldie_pack\\move.png", { 64, 40, 127, 79 }, {2, 1, 250, 3});
	skins[5][walk_horiz]->Add("Goldie_pack\\move.png", { 0, 280, 127, 319 }, {4, 1, 150, 3});
	skins[5][walk_up]->Add("Goldie_pack\\move.png", { 0, 200, 127, 239 }, {4, 1, 150, 3});
	skins[5][walk_down]->Add("Goldie_pack\\move.png", { 0, 160, 127, 199 }, {4, 1, 150, 3});



}
Animation* Skins::getAnimation(int num_skin, Move move) const
{
	return skins[num_skin][move];
}
Skins::~Skins()
{
	for (int i = 0; i < SkinsCount; i++)
		for (int j = 0; j < Move::Count; j++)
			delete skins[i][j];
}