#include "predictor.h"

/////////////////////////////////////////////////////////////
// 2bitsat
/////////////////////////////////////////////////////////////

const int NUM_ENTRIES = 8192 / 2;
int * memory_2bsat;

void InitPredictor_2bitsat() {
	memory_2bsat = new int[NUM_ENTRIES];

	for (int i = 0; i < NUM_ENTRIES; i++)
	{
		memory_2bsat[i] = 1;
	}

}

bool GetPrediction_2bitsat(UINT32 PC) {
	
	int index = PC & 0x00000FFF;
	int state = memory_2bsat[index];

	if (state > 1) {
		return TAKEN;
	} else {
		return NOT_TAKEN;
	}
}

void UpdatePredictor_2bitsat(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int index = PC & 0x00000FFF;
	int state = memory_2bsat[index];

	if (resolveDir) memory_2bsat[index] = state + 1 > 3 ? 3 : state + 1;
	else memory_2bsat[index] = state - 1 < 0 ? 0 : state - 1;
}

/////////////////////////////////////////////////////////////
// 2level
/////////////////////////////////////////////////////////////

const int NUM_BHT_ENTRIES = 512;
const int NUM_PHT_TABLES = 8;
const int NUM_PHT_ENTRIES = 64;

const UINT32 BHT_MASK = 0x00000FF8;
const UINT32 PHT_MASK = 0x00000007;

uint8_t* bht_memory_2lev;
int ** pht_memory_2lev;

void InitPredictor_2level() {
	bht_memory_2lev = new uint8_t[NUM_BHT_ENTRIES];

	pht_memory_2lev = new int*[NUM_PHT_TABLES];

	for (int i = 0; i < NUM_PHT_TABLES; i++) {
		pht_memory_2lev[i] = new int[NUM_PHT_ENTRIES];
		for (int j = 0; j < NUM_PHT_ENTRIES; j++) {
			pht_memory_2lev[i][j] = 1;
		}
	}
}

bool GetPrediction_2level(UINT32 PC) {

	int bht_index = (PC & BHT_MASK) >> 3;
	int pht_index = PC & PHT_MASK;

	uint8_t pattern = bht_memory_2lev[bht_index];
	int state = pht_memory_2lev[pht_index][pattern];

	if (state > 1) {
		return TAKEN;
	}
	else {
		return NOT_TAKEN;
	}
}

void UpdatePredictor_2level(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int bht_index = (PC & BHT_MASK) >> 3;
	int pht_index = PC & PHT_MASK;

	uint8_t pattern = bht_memory_2lev[bht_index];

	int state = pht_memory_2lev[pht_index][pattern];

	if (resolveDir) pht_memory_2lev[pht_index][pattern] = state + 1 > 3 ? 3 : state + 1;
	else pht_memory_2lev[pht_index][pattern] = state - 1 < 0 ? 0 : state - 1;

	pattern <<= 1;
	if (resolveDir) pattern &= 0b00111111;
	else pattern &= 0b00111110;
	
	bht_memory_2lev[bht_index] = pattern;
}

/////////////////////////////////////////////////////////////
// openend
/////////////////////////////////////////////////////////////

void InitPredictor_openend() {

}

bool GetPrediction_openend(UINT32 PC) {

  return TAKEN;
}

void UpdatePredictor_openend(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {

}

