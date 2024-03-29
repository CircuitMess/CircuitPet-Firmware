#ifndef CIRCUITPET_FIRMWARE_SPIFFSCHECKSUM_H
#define CIRCUITPET_FIRMWARE_SPIFFSCHECKSUM_H

#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
	{ "/GameScreens/Inst4.raw", 509441},
	{ "/GameScreens/Splash6.raw", 1560821},
	{ "/GameScreens/Inst5.raw", 1796954},
	{ "/GameScreens/Splash5.raw", 1708049},
	{ "/GameScreens/Splash2.raw", 795181},
	{ "/GameScreens/Inst6.raw", 1585545},
	{ "/GameScreens/Splash1.raw", 1128772},
	{ "/GameScreens/Inst1.raw", 1125606},
	{ "/GameScreens/Inst2.raw", 772851},
	{ "/GameScreens/Splash4.raw", 498985},
	{ "/GameScreens/Splash3.raw", 1255835},
	{ "/GameScreens/Inst3.raw", 1312456},
	{ "/OS/Level3.raw", 15342},
	{ "/OS/Level2.raw", 21195},
	{ "/OS/Level6.raw", 25140},
	{ "/OS/Level5.raw", 25140},
	{ "/OS/Level4.raw", 9948},
	{ "/OS/Level1.raw", 15654},
	{ "/OS/Logo.raw", 37530},
	{ "/Bg/Level3.raw", 6984607},
	{ "/Bg/Level2.raw", 5867326},
	{ "/Bg/Level6.raw", 4092103},
	{ "/Bg/settings.raw", 3343841},
	{ "/Bg/Level5.raw", 6852496},
	{ "/Bg/Level4.raw", 6945213},
	{ "/Bg/Level1.raw", 5771183},
	{ "/Home/06_general.gif", 807633},
	{ "/Home/rusty/06_general.gif", 707185},
	{ "/Home/rusty/06_look.gif", 1626668},
	{ "/Home/rusty/06_scratch.gif", 2056349},
	{ "/Home/rusty/04_scratch.gif", 1753053},
	{ "/Home/rusty/01_look.gif", 1120786},
	{ "/Home/rusty/04_general.gif", 599974},
	{ "/Home/rusty/04_look.gif", 957939},
	{ "/Home/rusty/01_scratch.gif", 1584190},
	{ "/Home/rusty/01_general.gif", 495805},
	{ "/Home/06_look.gif", 1329981},
	{ "/Home/06_scratch.gif", 2231670},
	{ "/Home/04_scratch.gif", 1867699},
	{ "/Home/01_look.gif", 867367},
	{ "/Home/04_general.gif", 659441},
	{ "/Home/04_look.gif", 1036984},
	{ "/Home/01_scratch.gif", 1521715},
	{ "/Home/01_general.gif", 626572},
	{ "/LevelUp/2.raw", 4520385},
	{ "/LevelUp/3.raw", 6014583},
	{ "/LevelUp/6.raw", 4406813},
	{ "/LevelUp/4.raw", 5630590},
	{ "/LevelUp/5.raw", 4999950},
	{ "/Pat2.gif", 1867699},
	{ "/intro.hs", 21831970},
	{ "/frame.raw", 4457023},
	{ "/MenuIcons/Locked6.raw", 230473},
	{ "/MenuIcons/Locked3.raw", 271830},
	{ "/MenuIcons/Locked2.raw", 237263},
	{ "/MenuIcons/settings.raw", 219220},
	{ "/MenuIcons/Icon1.raw", 282397},
	{ "/MenuIcons/Icon2.raw", 221044},
	{ "/MenuIcons/Icon4.raw", 275397},
	{ "/MenuIcons/Locked5.raw", 252058},
	{ "/MenuIcons/Locked4.raw", 249115},
	{ "/MenuIcons/Icon5.raw", 187931},
	{ "/MenuIcons/Icon3.raw", 360369},
	{ "/MenuIcons/Border.raw", 134832},
	{ "/MenuIcons/Icon6.raw", 209783},
	{ "/Stats/Battery.raw", 40647},
	{ "/Stats/Exp.raw", 29616},
	{ "/Stats/OilLevel.raw", 20032},
	{ "/Stats/Bar.raw", 46976},
	{ "/Stats/LongBar.raw", 198378},
	{ "/Stats/Happiness.raw", 20023},
	{ "/hatching.gif", 10257643},
	{ "/Games/Game3/Nut.raw", 35770},
	{ "/Games/Game3/Heart.raw", 5367},
	{ "/Games/Game3/Background.raw", 6158669},
	{ "/Games/Game3/DuckEat.gif", 602847},
	{ "/Games/Game3/BarFrame.raw", 175596},
	{ "/Games/Game3/Bomb.raw", 47674},
	{ "/Games/Game3/Screw.raw", 13779},
	{ "/Games/Game3/DuckWalk.gif", 1140649},
	{ "/Games/Game3/DuckEatBad.gif", 1350935},
	{ "/Games/Game3/Dynamite.raw", 22420},
	{ "/Games/Game3/DuckWin.gif", 1681991},
	{ "/Games/6/explosion.gif", 653699},
	{ "/Games/6/asteroidS.raw", 43126},
	{ "/Games/6/player.gif", 146135},
	{ "/Games/6/asteroidM.raw", 94446},
	{ "/Games/6/asteroidL.raw", 187161},
	{ "/Games/6/bg.raw", 2067594},
	{ "/Games/Game1/Arrow.raw", 12210},
	{ "/Games/Game1/OilyIdle.gif", 484721},
	{ "/Games/Game1/Background.raw", 5771183},
	{ "/Games/Game1/EmptyCan.raw", 39430},
	{ "/Games/Game1/Bar.raw", 824706},
	{ "/Games/Game1/OilyJump.gif", 974488},
	{ "/Games/Game1/OilyDone.gif", 2980060},
	{ "/Games/Game1/FullCan.raw", 43637},
	{ "/Games/Game4/TileBot2.raw", 111857},
	{ "/Games/Game4/ObstacleUnder2.raw", 118366},
	{ "/Games/Game4/Background.raw", 6640645},
	{ "/Games/Game4/DuckUnDucking.gif", 75499},
	{ "/Games/Game4/ObstacleUnder1.raw", 139314},
	{ "/Games/Game4/ObstacleOver1.raw", 175198},
	{ "/Games/Game4/DuckDucked.gif", 152613},
	{ "/Games/Game4/DuckDucking.gif", 74372},
	{ "/Games/Game4/DuckDown.gif", 370443},
	{ "/Games/Game4/DuckJump.gif", 109306},
	{ "/Games/Game4/DuckWalk.gif", 408450},
	{ "/Games/Game4/ObstacleOver2.raw", 88719},
	{ "/Games/Game4/TileTop1.raw", 84652},
	{ "/Games/Game4/TileBot1.raw", 112223},
	{ "/Games/Game4/ObstacleUnder3.raw", 96793},
	{ "/Games/Game4/ObstacleOver4.raw", 153903},
	{ "/Games/Game4/TileTop2.raw", 87277},
	{ "/Games/Game4/Goal.raw", 192950},
	{ "/Games/Game4/ObstacleOver3.raw", 132677},
	{ "/Games/Game4/DuckWin.gif", 585531},
	{ "/Games/Common/heart.raw", 5367},
	{ "/Games/Common/goblet.raw", 9073},
	{ "/Games/2/down3.raw", 223256},
	{ "/Games/2/down5.raw", 288417},
	{ "/Games/2/down2.raw", 220408},
	{ "/Games/2/down1.raw", 202959},
	{ "/Games/2/up1.raw", 235115},
	{ "/Games/2/up4.raw", 167073},
	{ "/Games/2/up2.raw", 167990},
	{ "/Games/2/down4.raw", 290503},
	{ "/Games/2/up3.raw", 174492},
	{ "/Games/2/duck.gif", 63947},
	{ "/Games/2/bg.raw", 5679655},
	{ "/Games/5/circ_b2.raw", 58376},
	{ "/Games/5/dance2.gif", 1140926},
	{ "/Games/5/starFull.raw", 38895},
	{ "/Games/5/note2.raw", 13824},
	{ "/Games/5/fail.gif", 1706996},
	{ "/Games/5/circ_p2.raw", 91720},
	{ "/Games/5/circ_y2.raw", 77416},
	{ "/Games/5/win.gif", 1055599},
	{ "/Games/5/dance4.gif", 1363956},
	{ "/Games/5/circ_b1.raw", 38476},
	{ "/Games/5/BarR.raw", 385540},
	{ "/Games/5/BarY.raw", 420634},
	{ "/Games/5/circ_y1.raw", 66816},
	{ "/Games/5/circ_p1.raw", 55020},
	{ "/Games/5/note3.raw", 11439},
	{ "/Games/5/note1.raw", 18594},
	{ "/Games/5/dance3.gif", 995869},
	{ "/Games/5/BarB.raw", 228282},
	{ "/Games/5/idle.gif", 977247},
	{ "/Games/5/BarP.raw", 316678},
	{ "/Games/5/starEmpty.raw", 31563},
	{ "/Games/5/dance1.gif", 2230188},
	{ "/Games/5/bg.raw", 5245733},
	{ "/Pat1.gif", 553292},
	{ "/death.g565", 380656},
	{ "/Test.raw", 1210091},
};

#endif //CIRCUITPET_FIRMWARE_SPIFFSCHECKSUM_H
