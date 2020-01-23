// Impression3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

int main() {
	int origineX = 80;
	int origineY = 80;
	float origineZ = 0.2;
	int width = 10;
	float deltaE = width * 0.2 * 0.4 / (3.141592654 * 1.75 * 1.75 / 4);

	float e = 0;

	std::ofstream file;
	file.open("square.gcode");
	// Ecriture de l'entete
	file << "G21" << std::endl; //Dimension en mm 
	file << "M106 S204; turn fan on" << std::endl;
	file << "M190 S50; wait for bed temperature to be reached" << std::endl;
	file << "M104 S210; set temperature" << std::endl;
	file << "G28; home all axes" << std::endl;
	file << "G92 E0" << std::endl;
	file << "M109 S210; wait for temperature to be reached" << std::endl;
	file << "G90; use absolute coordinates" << std::endl;
	file << "G92 E0" << std::endl;
	file << "M82; use absolute distances for extrusion" << std::endl;
	file << "G92 E0" << std::endl;
	file << "M106 S204; turn fan on" << std::endl;

	// La trait de caca de base 
	file << "G1 X" << origineX << " Y" << 0 << " Z0.2 E" << (deltaE/width)*origineX << " F1200" << std::endl;
	file << "G92 E0" << std::endl;

	// Aller a la position de base
	file << "G1 X" << origineX << " Y" << origineY << " Z0.2 E0 F1200" << std::endl;

	// On fais la base
	for (float i = 0; i < width; i += 0.2) {
		file << "G0 X" << origineX + i << " Y" << origineY << " Z" << origineZ << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX + i  << " Y" << origineY + width << " Z" << origineZ << " E" << e << " F1200" << std::endl;
		i += 0.2;
		if (i < width) {
			file << "G0 X" << origineX + i << " Y" << origineY + width << " Z" << origineZ << " F1200" << std::endl;
			e += deltaE;
			file << "G1 X" << origineX + i << " Y" << origineY << " Z" << origineZ << " E" << e << " F1200" << std::endl;
		}
	}

	// on retoune a l'origine 
	file << "G0 X" << origineX << " Y" << origineY << " Z0.2 F1200" << std::endl;

	float decalage45 = 4;
	int signe45 = 1;
	float decalage45m = 4;
	int signe45m = 1;
	float decalage0 = 4;
	int signe0 = 1;
	// On remplis 
	for (float i = origineZ; i <= width; i+=0.2) {
		// Faire les bords
		file << "G1 X" << origineX << " Y" << origineY << " Z" << i << " E" << e << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX << " Y" << origineY + width << " Z" << i << " E" << e << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX + width << " Y" << origineY + width << " Z" << i << " E" << e << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX + width << " Y" << origineY << " Z" << i << " E" << e << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX << " Y" << origineY << " Z" << i << " E" << e << " F1200" << std::endl;

		// Remplissage 45
		decalage45 = (decalage45 + signe45 * 0.2);
		if (origineX + decalage45 > origineX + width)
			signe45 = -1;
		if (origineX + decalage45 < origineX)
			signe45 = 1;

		file << "G1 X" << origineX + decalage45 << " Y" << origineY << " Z" << i << " E" << e << " F1200" << std::endl;
		float tailleDiagonale = sqrt(
			((origineX+width)-(origineX+decalage45)) * ((origineX + width) - (origineX + decalage45)) +
			((origineY + width - decalage45) - (origineY)) * ((origineY + width - decalage45) - (origineY))
		);
		e += (deltaE/width) * tailleDiagonale;
		file << "G1 X" << origineX + width << " Y" << origineY + width - decalage45 << " Z" << i << " E" << e << " F1200" << std::endl;

		// Remplissage -45
		decalage45m = (decalage45m + signe45m * 0.2);
		if (origineX + decalage45m > origineX + width)
			signe45m = -1;
		if (origineX + decalage45m < origineX)
			signe45m = 1;

		file << "G1 X" << origineX + decalage45m << " Y" << origineY << " Z" << i << " E" << e << " F1200" << std::endl;
		tailleDiagonale = sqrt(
			((origineX + decalage45m) - (origineX)) * ((origineX + decalage45m) - (origineX)) +
			((origineY + decalage45m) - (origineY)) * ((origineY + decalage45m) - (origineY))
		);
		e += (deltaE / width) * tailleDiagonale;
		file << "G1 X" << origineX << " Y" << origineY + decalage45m << " Z" << i << " E" << e << " F1200" << std::endl;

		// Remplissage 0
		decalage0 = (decalage0 + signe0 * 0.2);
		if (origineY + decalage0 > origineY + width)
			signe0 = -1;
		if (origineY + decalage0 < origineY)
			signe0 = 1;

		file << "G1 X" << origineX << " Y" << origineY + decalage0 << " Z" << i << " E" << e << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX + width << " Y" << origineY + decalage0 << " Z" << i << " E" << e << " F1200" << std::endl;
	}

	// On fais le chapeau
	for (float i = 0.2; i < width; i += 0.2) {
		file << "G0 X" << origineX + i << " Y" << origineY << " Z" << width << " F1200" << std::endl;
		e += deltaE;
		file << "G1 X" << origineX + i << " Y" << origineY + width << " Z" << width << " E" << e << " F1200" << std::endl;

		file << "G0 X" << origineX + i << " Y" << origineY << " Z" << width << " F1200" << std::endl;
		
	}

	file << "G92 E0" << std::endl;
	file << "; < / layer>" << std::endl;
	file << "G92 E0" << std::endl;
	file << "M107; fan off" << std::endl;
	file << "M104 S0; turn off temperature" << std::endl;
	file << "M140 S0" << std::endl;
	file << "M107" << std::endl;
	file << "G1 X0; move back X to origin" << std::endl;
	file << "M84; disable motors" << std::endl;
}
