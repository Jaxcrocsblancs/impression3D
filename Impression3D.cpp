// Impression3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>

int main() {
	int origineX = 80;
	int origineY = 80;
	float origineZ = 0.2;
	int width = 20;
	int ecart = 8;
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

	std::vector<float> decalage45;
	std::vector<int> signe45;
	std::vector<float> decalage45m;
	std::vector<int> signe45m;
	std::vector<float> decalage0;
	std::vector<int> signe0;
	for (int i = 0; i < 2 * width / ecart; i++) {
		decalage0.push_back(ecart * i);
		decalage45.push_back(ecart * i);
		decalage45m.push_back(ecart * i);
		signe0.push_back(1);
		signe45.push_back(1);
		signe45m.push_back(1);
	}
	float pas = 0.0;
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
		
		pas += 0.2;
		if (pas > ecart) pas = 0;
		float decal, xs, xf, ys, yf, tailleDiagonale;
		for (int nb = 0; nb < (2 * width) / ecart; nb++) {
			// Remplissage 45
			decal = decalage45[nb] + pas;
			xs = (decal < width) ? origineX : origineX + decal - width;
			ys = (decal < width) ? origineY + width - decal : origineY;
			xf = (decal < width) ? origineX + decal : origineX + width;
			yf = (decal < width) ? origineY + width : origineY + width - (decal - width);

			if (nb % 2 == 0) {
				float ts = xs;
				xs = xf;
				xf = ts;
				ts = ys;
				ys = yf;
				yf = ts;
			}

			file << "G1 X" << xs << " Y" << ys << " Z" << i << " E" << e << " F1200" << std::endl;
			tailleDiagonale = sqrt((xs - xf) * (xs - xf) + (ys - yf) * (ys - yf));
			e += (deltaE / width) * tailleDiagonale;
			file << "G1 X" << xf << " Y" << yf << " Z" << i << " E" << e << " F1200" << std::endl;
		}
		for (int nb = 0; nb < (2 * width) / ecart; nb++) {
			// Remplissage -45
			decal = decalage45[nb] + pas;
			xs = (decal < width) ? origineX + decal : origineX + width;
			ys = (decal < width) ? origineY : origineY + decal - width;
			xf = (decal < width) ? origineX : origineX + decal - width;
			yf = (decal < width) ? origineY + decal : origineY + width;

			if (nb % 2 == 0) {
				float ts = xs;
				xs = xf;
				xf = ts;
				ts = ys;
				ys = yf;
				yf = ts;
			}

			file << "G1 X" << xs << " Y" << ys << " Z" << i << " E" << e << " F1200" << std::endl;
			tailleDiagonale = sqrt((xs - xf) * (xs - xf) + (ys - yf) * (ys - yf));
			e += (deltaE / width) * tailleDiagonale;
			file << "G1 X" << xf << " Y" << yf << " Z" << i << " E" << e << " F1200" << std::endl;
		}

		for (int nb = 0; nb < (float)width / (float)ecart; nb++) {
			// Replissage 0
			decal = decalage0[nb] + pas;
			if (decal + pas < width) {
				xs = origineX;
				ys = origineY + decal;
				xf = origineX + width;
				yf = origineY + decal;

				if (nb % 2 == 0) {
					float ts = xs;
					xs = xf;
					xf = ts;
					ts = ys;
					ys = yf;
					yf = ts;
				}

				file << "G1 X" << xs << " Y" << ys << " Z" << i << " E" << e << " F1200" << std::endl;
				tailleDiagonale = sqrt((xs - xf) * (xs - xf) + (ys - yf) * (ys - yf));
				e += (deltaE / width) * tailleDiagonale;
				file << "G1 X" << xf << " Y" << yf << " Z" << i << " E" << e << " F1200" << std::endl;
			}
		}
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