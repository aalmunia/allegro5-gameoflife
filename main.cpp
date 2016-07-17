#include <iostream>
#include <random>
#include <string>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

void nextGeneration(bool** pCells, bool** pNext, short int iTotalX, short int iTotalY, long int& iTotalAlive) {

	short int rowPrev = 0;
	short int rowNext = 0;
	short int colPrev = 0;
	short int colNext = 0;
	short int totalNeighbors = 0;

	for (short int i = 0; i < iTotalX; i++) {
		for (short int j = 0; j < iTotalY; j++) {

			pNext[i][j] = false;

			if (i == 0) {
				rowPrev = iTotalX - 1;
				rowNext = 1;
			}
			else {
				if (i == (iTotalX - 1)) {
					rowNext = 0;
					rowPrev = i - 1;
				}
				else {
					rowPrev = i - 1;
					rowNext = i + 1;
				}
			}

			if (j == 0) {
				colPrev = iTotalY - 1;
				colNext = 1;
			}
			else {
				if (j == (iTotalY - 1)) {
					colPrev = j - 1;
					colNext = 0;
				}
				else {
					colPrev = j - 1;
					colNext = j + 1;
				}
			}

			
			totalNeighbors += (!pCells[rowPrev][colPrev]) ? 0 : 1;
			totalNeighbors += (!pCells[rowPrev][j]) ? 0 : 1;
			totalNeighbors += (!pCells[rowPrev][colNext]) ? 0 : 1;
			totalNeighbors += (!pCells[i][colPrev]) ? 0 : 1;
			totalNeighbors += (!pCells[i][colNext]) ? 0 : 1;
			totalNeighbors += (!pCells[rowNext][colPrev]) ? 0 : 1;
			totalNeighbors += (!pCells[rowNext][j]) ? 0 : 1;
			totalNeighbors += (!pCells[rowNext][colNext]) ? 0 : 1;
			
			
			if (pCells[i][j] == true && (totalNeighbors == 2 || totalNeighbors == 3)) {
				pNext[i][j] = true;
				++iTotalAlive;
			}
			else if (pCells[i][j] == false && totalNeighbors == 3) {
				pNext[i][j] = true;
				++iTotalAlive;
			}

			totalNeighbors = 0;

		}
	}
}

void redrawCells(ALLEGRO_BITMAP* pBuffer, bool** pCells, bool** pNextCells, short int iWidth, short int iHeight, ALLEGRO_COLOR oCellColor, ALLEGRO_COLOR oBackColor) {
	al_lock_bitmap(pBuffer, al_get_bitmap_format(pBuffer), ALLEGRO_LOCK_WRITEONLY);
	al_set_target_bitmap(pBuffer);	
	for (short int i = 0; i < iWidth; i++) {
		for (short int j = 0; j < iHeight; j++) {			
			if (pCells[i][j] == true) {
				al_put_pixel(i, j, oCellColor);
			}
			else {
				al_put_pixel(i, j, oBackColor);
			}
		}
	}	
	al_unlock_bitmap(pBuffer);
}

void initCells(bool** pCells, bool** pNextCells, short int iAppWidth, short int iAppHeight, int fLifeScarcity) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1.0f, 1000.0f);
	for (short int i = 0; i < iAppWidth; i++) {
		pCells[i] = new bool[iAppHeight];
		pNextCells[i] = new bool[iAppHeight];
		for (short int j = 0; j < iAppHeight; j++) {
			pNextCells[i][j] = false;
			int fRandom = std::round(dist(mt));
			if (fRandom % fLifeScarcity == 0) {
				pCells[i][j] = true;
			}
			else {
				pCells[i][j] = false;
			}
		}
	}
}

void clearCells(bool** pCells, bool** pNextCells, short int iAppWidth, short int iAppHeight) {
	for (short int i = 0; i < iAppWidth; i++) {
		for (short int j = 0; j < iAppHeight; j++) {
			pCells[i][j] = false;
			pNextCells[i][j] = false;
		}
	}
}

void randomCells(bool** pCells, short int iAppWidth, short int iAppHeight, int fLifeScarcity) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1.0f, 1000.0f);
	for (short int i = 0; i < iAppWidth; i++) {
		for (short int j = 0; j < iAppHeight; j++) {
			int fRandom = std::round(dist(mt));
			if (fRandom % fLifeScarcity == 0) {
				pCells[i][j] = true;
			}
			else {
				pCells[i][j] = false;
			}
		}
	}
}

void copyCells(bool** pCells, bool** pNextCells, short int iAppWidth, short int iAppHeight) {
	for (short int i = 0; i < iAppWidth; i++) {
		memcpy(pCells[i], pNextCells[i], (iAppHeight * sizeof(bool)));		
	}
	
}

unsigned int randr(unsigned int min, unsigned int max) {
	double scaled = (double)rand() / RAND_MAX;
	return (max - min + 1)*scaled + min;
}

bool isPatternStable(long int* iAliveHistory, short int iSize) {
	int iStable = 0;
	for (short int i = 0; i < iSize; i++) {
		for (short int j = 0; j < iSize; j++) {
			if (iAliveHistory[i] == iAliveHistory[j]) {
				++iStable;
			}
		}		
	}
	if (iStable >= (iSize * 10)) {
		return true;
	}
	else {
		return false;
	}
}

bool areIdenticalCellMap(bool** pCells, bool** pNextCells, short int iAppWidth, short int iAppHeight) {
	bool bReturn = true;
	for (short int i = 0; i < iAppWidth; i++) {
		for (short int j = 0; j < iAppHeight; j++) {
			if (pCells[i][j] != pNextCells[i][j]) {
				return false;
			}
		}
	}
	return bReturn;
}

// COPIED FROM http://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

std::string ReplaceString(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void ReplaceStringInPlace(std::string& subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

int main(short int argc, char** argv) {

	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_MONITOR_INFO oMonitorInfo;
	al_get_monitor_info(0, &oMonitorInfo);

	short int iDisplayWidth = oMonitorInfo.x2 * 0.70f;
	short int iDisplayHeight = oMonitorInfo.y2 * 0.70f;
	short int iAppWidth = iDisplayWidth * 0.95f;
	short int iAppHeight = iDisplayHeight * 0.95f;
	short int iMarginHorizontal = (iDisplayWidth - iAppWidth) / 2;
	short int iMarginVertical = ((iDisplayHeight - iAppHeight) / 2);
	int iGenerations = 0;
	short int iFPS = 30;
	float iLifeMin = 3.0f;
	float iLifeMax = 37.0f;	
	short int iFontSize = (iDisplayWidth > 1024) ? 12 : 10;
	if (iDisplayWidth < 800) {
		iFontSize = 8;
	}
	long int iSimulations = 1;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(iLifeMin, iLifeMax);
	
	int iLifeScarcity = std::round(dist(mt));

	bool** pCells = new bool*[iAppWidth];
	bool** pNextGenCells = new bool*[iAppWidth];

	initCells(pCells, pNextGenCells, iAppWidth, iAppHeight, iLifeScarcity);

	ALLEGRO_DISPLAY* pDisplay = al_create_display(iDisplayWidth, iDisplayHeight);	
	ALLEGRO_EVENT_QUEUE* pQueue = al_create_event_queue();	
	ALLEGRO_TIMER* pTimer = al_create_timer(1.0f / iFPS);
	ALLEGRO_TIMER* pSecondBySecondTimer = al_create_timer(1.0f);
	ALLEGRO_BITMAP* pBuffer = al_create_bitmap(iAppWidth, iAppHeight);
	ALLEGRO_COLOR oBackgroundColor = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR oCellColor = al_map_rgb(randr(150, 255), randr(150, 255), randr(150, 255));
	ALLEGRO_FONT* oFont = al_load_ttf_font("VeraMono.ttf", iFontSize, 0);
	ALLEGRO_FONT* oFontLarge = al_load_ttf_font("VeraMono.ttf", (iFontSize * 3), 0);

	al_inhibit_screensaver(true);
	
	al_register_event_source(pQueue, al_get_keyboard_event_source());
	al_register_event_source(pQueue, al_get_mouse_event_source());	
	al_register_event_source(pQueue, al_get_timer_event_source(pTimer));
	al_register_event_source(pQueue, al_get_timer_event_source(pSecondBySecondTimer));
	al_set_target_backbuffer(pDisplay);
	al_clear_to_color(oBackgroundColor);
	al_flip_display();

	al_start_timer(pTimer);
	al_start_timer(pSecondBySecondTimer);

	ALLEGRO_EVENT oEvent;

	short int iBufferUsed = 0;
	short int iBufferDrawn = 0;
	bool bRedraw = false;
	std::string sHeaderStatistics = "GEN  [GENXXXXX]     FPS  [FPSXXXXX]     CELLS  [CELLSXXXXX]    GENS/S  [GENSSXXXXX]    SCARCTY  [SCARXXXXX]    TIME  [TIMEXXXXX]";
	std::string sHeaderStats = "";
	/* std::string sHeaderText_2 = "";
	std::string sHeaderText_3 = "";
	std::string sHeaderText_4 = "";
	std::string sHeaderText_5 = "";
	std::string sHeaderText_6 = ""; */
	std::string sCountdownText = "";
	std::string sSimulations = "";
	std::string sStats = "CELLS: ";

	sStats.append(std::to_string((iAppWidth * iAppHeight)));
	sStats.append(", MAP SIZE (KB): ");
	sStats.append(std::to_string((iAppWidth * iAppHeight * sizeof(bool)) / 1024));
	sStats.append("  (SPACE) Pause (C)olor, (R)eload, (S)carcity, (F) +1 FPS, (G) -1 FPS, (ESC) Exit");

	long int iTotalAlive = 0;
	int iPatternStableBuffer = (iFPS * 4);
	long int* iTotalPatternStable = new long int[iPatternStableBuffer];
	short int iTotalPatternCounter = 0;
	long int iSecondsRunning = 0;

	float fPosText2 = (iAppWidth * 0.15);
	float fPosText3 = (iAppWidth * 0.30);
	float fPosText4 = (iAppWidth * 0.50);	
	float fPosText5 = (iAppWidth * 0.70);
	float fPosText6 = (iAppWidth * 0.85);

	float fPosTextSim = (iAppWidth * 0.75);

	bool bPatternIsStable = false;
	int iCountdownSeconds = 10;

	bool bDrawingOn = false;
	bool bTimerStopped = false;

	ALLEGRO_COLOR oRandColor = al_map_rgb(randr(0, 255), randr(0, 255), randr(0, 255));

	while (true) {
		
		al_wait_for_event(pQueue, &oEvent);

		if (oEvent.type == ALLEGRO_EVENT_TIMER) {
			if (!bTimerStopped) {
				if (oEvent.timer.source == pTimer) {

					iTotalAlive = 0;
					redrawCells(pBuffer, pCells, pNextGenCells, iAppWidth, iAppHeight, oCellColor, oBackgroundColor);
					nextGeneration(pCells, pNextGenCells, iAppWidth, iAppHeight, iTotalAlive);
					al_set_target_backbuffer(pDisplay);
					al_clear_to_color(oBackgroundColor);
					al_draw_bitmap(pBuffer, iMarginHorizontal, iMarginVertical, 0);

					sHeaderStats = ReplaceString(sHeaderStatistics, "[GENXXXXX]", std::to_string(iGenerations));
					sHeaderStats = ReplaceString(sHeaderStats, "[FPSXXXXX]", std::to_string(iFPS));
					sHeaderStats = ReplaceString(sHeaderStats, "[CELLSXXXXX]", std::to_string(iTotalAlive));
					sHeaderStats = ReplaceString(sHeaderStats, "[SCARXXXXX]", std::to_string(iLifeScarcity));
					sHeaderStats = ReplaceString(sHeaderStats, "[TIMEXXXXX]", std::to_string(iSecondsRunning));
					if (iGenerations > 0 && iSecondsRunning > 0) {
						sHeaderStats = ReplaceString(sHeaderStats, "[GENSSXXXXX]", std::to_string(iGenerations / iSecondsRunning));
					}
					else {
						sHeaderStats = ReplaceString(sHeaderStats, "[GENSSXXXXX]", "0");
					}					
					sSimulations = "SIMS ";
					sSimulations.append(std::to_string(iSimulations));
					int iLengthSims = al_get_text_width(oFont, sSimulations.c_str());
					int iLengthStats = al_get_text_width(oFont, sHeaderStats.c_str());
					al_draw_text(oFont, oCellColor, ((iAppWidth - iLengthStats) / 2), 1.0f, 0, sHeaderStats.c_str());
					al_draw_text(oFont, oCellColor, (iDisplayWidth - (iLengthSims + 25.0f)), (iAppHeight + iMarginVertical + 5.0f), 0, sSimulations.c_str());
					al_draw_text(oFont, oCellColor, 25.0f, (iAppHeight + iMarginVertical + 5.0f), 0, sStats.c_str());

					if (bPatternIsStable == true) {
						sCountdownText.clear();
						sCountdownText.append("PATTERN STABILIZED, RESTARTING IN... ");
						int iLengthStr = al_get_text_width(oFontLarge, sCountdownText.c_str());
						sCountdownText.append(std::to_string(iCountdownSeconds));
						al_draw_text(oFontLarge, oRandColor, ((iAppWidth - iLengthStr) / 2), (iAppHeight * 0.45f), 0, sCountdownText.c_str());
					}

					al_flip_display();
					++iGenerations;
					copyCells(pCells, pNextGenCells, iAppWidth, iAppHeight);

					if (iTotalPatternCounter == iPatternStableBuffer) {
						bPatternIsStable = isPatternStable(iTotalPatternStable, iPatternStableBuffer);
						delete iTotalPatternStable;
						iTotalPatternStable = new long int[iPatternStableBuffer];
						iTotalPatternCounter = 0;
					}
					iTotalPatternStable[iTotalPatternCounter] = iTotalAlive;
					++iTotalPatternCounter;
				}

				if (oEvent.timer.source == pSecondBySecondTimer) {
					if (bPatternIsStable == true) {
						if (iCountdownSeconds > 1) {
							--iCountdownSeconds;
						}
						else {
							bPatternIsStable = false;
							iTotalPatternCounter = 0;
							iGenerations = 0;
							iSecondsRunning = 0;
							iCountdownSeconds = 10;
							++iSimulations;
							clearCells(pCells, pNextGenCells, iAppWidth, iAppHeight);
							randomCells(pCells, iAppWidth, iAppHeight, iLifeScarcity);
						}
					}
					else {
						iCountdownSeconds = 10;
					}
					++iSecondsRunning;
					oRandColor = al_map_rgb(randr(0, 255), randr(0, 255), randr(0, 255));
				}
			}
		}

		if (oEvent.type == ALLEGRO_EVENT_KEY_DOWN) {
			
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {				
				break;
			}

			if (oEvent.keyboard.keycode == ALLEGRO_KEY_SPACE) {
				if (!bTimerStopped) {					
					bTimerStopped = true;
					al_stop_timer(pTimer);
				}
				else {					
					bTimerStopped = false;
					al_start_timer(pTimer);
				}				
			}

			if (oEvent.keyboard.keycode == ALLEGRO_KEY_R) {
				bPatternIsStable = false;
				iTotalPatternCounter = 0;
				iGenerations = 0;
				iSecondsRunning = 0;
				iCountdownSeconds = 10;
				++iSimulations;
				clearCells(pCells, pNextGenCells, iAppWidth, iAppHeight);
				randomCells(pCells, iAppWidth, iAppHeight, iLifeScarcity);
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_S) {
				bPatternIsStable = false;
				iTotalPatternCounter = 0;
				iGenerations = 0;
				iSecondsRunning = 0;
				iCountdownSeconds = 10;
				iLifeScarcity = randr(iLifeMin, iLifeMax);
				++iSimulations;
				clearCells(pCells, pNextGenCells, iAppWidth, iAppHeight);
				randomCells(pCells, iAppWidth, iAppHeight, iLifeScarcity);				
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_M) {
				bPatternIsStable = false;
				iTotalPatternCounter = 0;
				iGenerations = 0;
				iSecondsRunning = 0;
				iCountdownSeconds = 10;
				iLifeScarcity = iLifeMin;
				++iSimulations;
				clearCells(pCells, pNextGenCells, iAppWidth, iAppHeight);
				randomCells(pCells, iAppWidth, iAppHeight, iLifeScarcity);
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_N) {
				bPatternIsStable = false;
				iTotalPatternCounter = 0;
				iGenerations = 0;
				iSecondsRunning = 0;
				iCountdownSeconds = 10;
				iLifeScarcity = iLifeMax;
				++iSimulations;
				clearCells(pCells, pNextGenCells, iAppWidth, iAppHeight);
				randomCells(pCells, iAppWidth, iAppHeight, iLifeScarcity);
			}

			if (oEvent.keyboard.keycode == ALLEGRO_KEY_F) {
				++iFPS;				
				al_set_timer_speed(pTimer, (1.0f / iFPS));
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_G) {
				if(iFPS > 3) {
					--iFPS;
				}
				al_set_timer_speed(pTimer, (1.0f / iFPS));
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_C) {				
				int iRCell = randr(0, 255);
				int iGCell = randr(0, 255);
				int iBCell = randr(0, 255);
				oCellColor = al_map_rgb(iRCell, iGCell, iBCell);
			}
		}

	}	// End main loop

	al_destroy_event_queue(pQueue);	
	al_destroy_display(pDisplay);

	delete iTotalPatternStable;
	for (short int i = 0; i < iAppWidth; i++) {
		delete pCells[i];
		delete pNextGenCells[i];
	}
	delete[] pCells;
	delete[] pNextGenCells;
	return 0;
}