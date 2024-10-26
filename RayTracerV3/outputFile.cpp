#include "outputFile.h"

void createPPMFile(const std::string& filename, const int imageWidth, const int imageHeight) {
    std::string newfile = filename; //Copy filename
    newfile.pop_back(); newfile.pop_back(); newfile.pop_back(); //Remove txt
    newfile.append("ppm"); //Add PPM
    std::ofstream outfile(newfile); //Open the newfile to write information
    outfile << "P3" << "\n" << imageWidth << " " << imageHeight << "\n" << "255\n"; //Write PPM header on newfile
    outfile.close(); //Close the newfile
}

void writeColor(const std::string& filename, const Vec3& colorToWrite) {
    std::ofstream outfile(filename); //Open the newfile to write information
    outfile << int(colorToWrite.getX() * 255) << " " << int(colorToWrite.getY() * 255) << " " << int(colorToWrite.getZ() * 255) << "\n"; // add color
    outfile.close(); //Close the newfile
}