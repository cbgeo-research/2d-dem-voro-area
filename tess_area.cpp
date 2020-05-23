#define CSV_IO_NO_THREAD
#include "csv.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char** argv) {
  std::string filename;
  if (argc == 2) {
    filename = argv[1];
  } else {
    std::cerr << "Incorret number of arguments." << std::endl;
    std::exit(EXIT_FAILURE); }
  
  std::vector<double> radius;
  std::vector<std::pair<double, double>> Vert;
  std::vector<std::pair<double, double>> grain_xy;
  std::unordered_map<unsigned int, std::pair<double, double>> vertices;
  std::unordered_map<unsigned int, std::vector<std::pair<double, double>>>
      connects;
  io::CSVReader<2> inG(filename);
  io::CSVReader<2> inC("Connectivity.data");
  io::CSVReader<3> inV("Vertices.data");
  io::CSVReader<1> inR("Radius.data");
  double vertice_x, vertice_y, radi,grain_x,grain_y, dx, dy, density, void_area, wd = 0.,ht = 0.;
  double tess_area = 0.,
         g_area = 0.;  // area is of single cell;Area is the Tesselaton area
  signed int connect;
  unsigned int i = 1, j = 1, n = 0, cell, degree, ignored;

  while (inV.read_row(ignored, vertice_x, vertice_y))
    if (ignored == j) {
      vertices.insert(std::make_pair(ignored, std::make_pair(vertice_x, vertice_y)));j++;}
// only store the positive vertice id numbering
  
  while (inR.read_row(radi)) radius.emplace_back(radi);
  while (inG.read_row(grain_x,grain_y))grain_xy.emplace_back(std::make_pair(grain_x, grain_y)) ;
// Maximum x and y; used to set boundaries of the postscripts as 1.2 times of 10000*x and 10000*y;
  for (auto it = grain_xy.begin(); it != grain_xy.end(); ++it) {
    if ((it->first)>wd)wd=(it->first);
    if ((it->second)>ht)ht=(it->second);}
  wd = 12000*wd; ht=12000*ht;

  while (inC.read_row(cell, connect)) {
    if (i == cell && connect > 0) {
// only concern vertices inside the domain whose id numbering is greater than 0;
      auto xandy = vertices.find(connect)->second;
      Vert.emplace_back(xandy);
    } else if (i < cell) {
      connects.insert(std::make_pair(i, Vert));
      Vert.clear();i++;
// Insert the vector of vertices into the map of connects with the index i and flush the vector to store vertices of the next cell;
      if (connect > 0) {auto xandy = vertices.find(connect)->second;
        Vert.emplace_back(xandy); }}}
  connects.insert(std::make_pair(i, Vert));// The vertices of the last cell should be inserted after the while loop;

//Write PostScript File and compute area of tesselation
  std::string fn1 = filename;
  fn1.erase(fn1.find_last_of("."), std::string::npos);
  fn1.append(".ps");
  std::cout<<fn1;
  std::ofstream fout;
  fout.open(fn1.c_str(),std::ios::out);
  float margin=1,hrx1=wd,hry2=ht;
  fout<<"%!PS-Adobe-3.0 EPSF-3.0 \n";
  fout<<"%%BoundingBox: "<<-margin<<" "<<-margin<<" "<<hrx1 + margin<<" "<<hry2 + margin<<std::endl;
  fout<<"%%Creator: Amber SpongeBob \n";
  fout<<"%%Title: Voronoi Tessellation \n";
  
// Calculate the area of each cell stored in connects map
  for (auto it = connects.begin(); it != connects.end(); ++it, ++n) {
    fout<<"newpath \n";
    double area = 0, length,dg_area;
    std::vector<std::pair<double, double>> temp = it->second;
// Enumerate the vertices vector by "it"
    dg_area = M_PI * std::pow(radius[n], 2);
    for (auto m = temp.begin(); m != temp.end(); ++m) {
      auto next = m + 1;
      if (m == (temp.end() - 1)) next = temp.begin();
      dx = (next->first) - (m->first);
      dy = (next->second) - (m->second);
      length = std::pow((dx * dx + dy * dy), 0.5);
      if (length > 4 * radius[n]) {
        area = 0;
        dg_area=0;
        break;}
      area += ((m->first) * (next->second)) - ((next->first) * (m->second));
      fout<<(m->first)*10000<<"\t"<<(m->second)*10000;
      if(m==temp.begin())fout<<" moveto \n";
      else fout<<" lineto \n";
// Output the coordinates of vertices of eacn cell whose length is less than 4 times of grain radius and indice is positive;
    }
    g_area +=dg_area;
    tess_area += std::fabs(area) / 2;
    fout<<"closepath \n";
    fout<<"1 setlinewidth 1 0 0 setrgbcolor \n";
    fout<<"stroke \n";
    fout<<"newpath "<<(grain_xy[n].first)*10000<<" "<<(grain_xy[n].second)*10000<<" "<<radius[n]*7000<<" 0.0 setlinewidth 0.4 setgray 0 360 arc gsave fill grestore"<<std::endl;
  }
  fout.close();
  void_area += tess_area - g_area;
  density = g_area / tess_area;

  std::ofstream outfile;
  outfile.open("packing_density.txt", std::ios_base::app); // append instead of overwrite
  outfile << "tess_area = " << tess_area << " grain area = " << g_area
          << " package density= " << density << "\n";
  outfile.close();
 
}
