T_Pont topologie[NBPONTS] = 
{
	{0,1}, // pont 0 : relie les îles 0 et 1
	{1,2}, //1
	{2,3}, //2
	{3,4}, //3 
	{0,5}, //4
	{1,6}, //5
	{2,7}, //6
	{3,8}, //7
	{4,9}, //8
	{5,6}, //9
	{6,7}, //10
	{7,8}, //11
	{8,9}, //12
	{5,10}, //13 
	{6,11}, //14
	{7,12}, //15
	{8,13}, //16
	{9,14}, //17
	{10,11}, //18
	{11,12}, //19
	{12,13}, //20
	{13,14}, //21
	{10,15}, //22
	{11,16}, //23 
	{12,17}, //24
	{13,18}, //25
	{14,19}, //26
	{15,16}, //27
	{16,17}, //28
	{17,18}, //29
	{18,19}, //30
	{15,20}, //31
	{16,21}, //32
	{17,22}, //33 
	{18,23}, //34
	{19,24}, //35
	{20,21}, //36
	{21,22}, //37
	{22,23}, //38
	{23,24} //39
}; 
 
T_Position positionInitiale =
{
0, // numCoup
ROU, // trait 
AUCUN, // premier roi bloqué 
{ // ponts 
	PRESENT, //0
	PRESENT, //1
	PRESENT, //2
	PRESENT, //3 
	PRESENT, //4
	PRESENT, //5
	PRESENT, //6
	PRESENT, //7
	PRESENT, //8
	PRESENT, //9
	PRESENT, //10
	PRESENT, //11
	PRESENT, //12
	PRESENT, //13 
	PRESENT, //14
	PRESENT, //15
	PRESENT, //16
	PRESENT, //17
	PRESENT, //18
	PRESENT, //19
	PRESENT, //20
	PRESENT, //21
	PRESENT, //22
	PRESENT, //23 
	PRESENT, //24
	PRESENT, //25
	PRESENT, //26
	PRESENT, //27
	PRESENT, //28
	PRESENT, //29
	PRESENT, //30
	PRESENT, //31
	PRESENT, //32
	PRESENT, //33 
	PRESENT, //34
	PRESENT, //35
	PRESENT, //36
	PRESENT, //37
	PRESENT, //38
	PRESENT //39
},
// pour chaque pion : ile, nbvoisins, isole 
{{UNKNOWN,UNKNOWN,UNKNOWN},{UNKNOWN,UNKNOWN,UNKNOWN},{UNKNOWN,UNKNOWN,UNKNOWN}},
{{UNKNOWN,UNKNOWN,UNKNOWN},{UNKNOWN,UNKNOWN,UNKNOWN},{UNKNOWN,UNKNOWN,UNKNOWN}}
};


T_Position positionInitialeStandard =
{
0,		// numCoup 
ROU,	// trait
AUCUN, // premier roi bloqué 
{	// ponts 
	PRESENT, //0
	PRESENT, //1
	PRESENT, //2
	PRESENT, //3 
	PRESENT, //4
	PRESENT, //5
	PRESENT, //6
	PRESENT, //7
	PRESENT, //8
	PRESENT, //9
	PRESENT, //10
	PRESENT, //11
	PRESENT, //12
	PRESENT, //13 
	PRESENT, //14
	PRESENT, //15
	PRESENT, //16
	PRESENT, //17
	PRESENT, //18
	PRESENT, //19
	PRESENT, //20
	PRESENT, //21
	PRESENT, //22
	PRESENT, //23 
	PRESENT, //24
	PRESENT, //25
	PRESENT, //26
	PRESENT, //27
	PRESENT, //28
	PRESENT, //29
	PRESENT, //30
	PRESENT, //31
	PRESENT, //32
	PRESENT, //33 
	PRESENT, //34
	PRESENT, //35
	PRESENT, //36
	PRESENT, //37
	PRESENT, //38
	PRESENT //39
},
// pour chaque pion : ile, nbvoisins, isole 
{{6,4,FAUX},{7,4,FAUX},{8,4,FAUX}},
{{16,4,FAUX},{17,4,FAUX},{18,4,FAUX}}
};
