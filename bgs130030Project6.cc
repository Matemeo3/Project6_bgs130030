/*
 * name: Barrett Simmons
 * email: bgs130030@utdallas.edu
 * UTDID: bgs1300030
 * 
 */

#include <iostream>
#include "cdk.h"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <algorithm>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 18
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;

class BinaryFileHeader
{
public:
  uint32_t magicNumber; /* Should be 0xFEEDFACE */
  uint32_t versionNumber;
  uint64_t numRecords;
};

/*
 * Records in the file have a fixed length buffer
 * that will hold a C-Style string. This is the
 * size of the fixed length buffer.
 */
const int maxRecordStringLength = 25;
class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};
int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char 		*columnTitles[] = {"C0", "C1", "C2", "C3", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);
	
	stringstream buffer;
	BinaryFileHeader *myHeader = new BinaryFileHeader();
	ifstream binInfile ("cs3377.bin", ios::in | ios::binary);	
	binInfile.read((char*) myHeader, sizeof(BinaryFileHeader));
	
	//Matrix element 1,1
	buffer << hex << myHeader->magicNumber;
	string magicUpper = buffer.str();
	transform(magicUpper.begin(), magicUpper.end(), magicUpper.begin(), ::toupper);
	setCDKMatrixCell(myMatrix, 1, 1, ("Magic: 0x" + magicUpper).c_str());
	buffer.str("");
	
	//Matrix element 1,2
	buffer << myHeader->versionNumber;
	string versionNum = buffer.str();
	setCDKMatrixCell(myMatrix, 1, 2, ("Version: " + versionNum).c_str());
	buffer.str("");
	
	//Matrix element 1,3
	buffer << myHeader->numRecords;
	string numRec = buffer.str();
	setCDKMatrixCell(myMatrix, 1, 3, ("NumRecords: " + numRec).c_str());
	buffer.str("");
	
	/**
	*Loops through BinaryFileRecord class 
	*/
	BinaryFileRecord *myRecord = new BinaryFileRecord();
	for(unsigned int i = 2; i < myHeader->numRecords+2;i++)
	{
		binInfile.read((char*) myRecord, sizeof(BinaryFileRecord));
		buffer << dec << (int)myRecord->strLength;
		setCDKMatrixCell(myMatrix, i, 1, ("strlen: " + buffer.str()).c_str());
		setCDKMatrixCell(myMatrix, i, 2, myRecord->stringBuffer);
		buffer.str("");
	}
	
  /*
   * Display a message
   */ 
  
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
