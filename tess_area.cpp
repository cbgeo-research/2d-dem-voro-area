#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#define CSV_IO_NO_THREAD
#include "csv.h"
#include <unordered_map>

int main() {
  std::vector<double> radius;
  std::vector<std::pair<double, double>> Vert;
  std::unordered_map<unsigned int, std::pair<double, double>> vertices;
  std::unordered_map<unsigned int, std::vector<std::pair<double, double>>> connects;
  io::CSVReader<2> inC("bin/Connectivity.data");
  io::CSVReader<3> inV("bin/Vertices.data");
  io::CSVReader<1> inR("bin/Radius.data");
  double vertice_x, vertice_y,radi,dx,dy,density,void_area;
  double tess_area = 0.,g_area=0.; // area is of single cell;Area is the Tesselaton area
  signed int connect;
  unsigned int i = 1, j = 1, n = 0, cell, degree, ignored;

 while (inV.read_row(ignored, vertice_x, vertice_y))
   if (ignored==j){vertices.insert(std::make_pair(ignored, std::make_pair(vertice_x, vertice_y)));j++;}
 while (inR.read_row(radi))radius.emplace_back(radi);
 // try to keep i the same as cell, once the value of cell exceeds i, which means it is a new cell, i++, and store vertices of the previous cell into Vert; and flush the Vert; 
  while (inC.read_row(cell, connect))
    { if(i==cell && connect>0)
      {auto xandy =  vertices.find(connect)->second;
          Vert.emplace_back(xandy);}
     else if(i<cell)
     {connects.insert(std::make_pair(i, Vert));
      Vert.clear();i++;
      if (connect>0){auto xandy =  vertices.find(connect)->second;
        Vert.emplace_back(xandy);}}}
  // because only when cell>i the vertices can be stored, the vertices of the last cell should be inserted after the while loop.
  connects.insert(std::make_pair(i, Vert));  
for (auto it = connects.begin(); it!= connects.end(); ++it,++n) {
    double area = 0,length;
    std::vector<std::pair<double, double>> temp = it->second;
     for (auto m = temp.begin(); m!= temp.end(); ++m) {
      auto next = m + 1;
      if (m == (temp.end() - 1))next = temp.begin();
      dx=(next->first)-(m->first);
      dy=(next->second)-(m->second);
      length=std::pow((dx*dx+dy*dy),0.5);
      if(length>4*radius[n]){area=0;break;}
      area += ((m->first) * (next->second)) -((next->first) * (m->second));}
     g_area+=M_PI*std::pow(radius[n],2);
     tess_area +=std::fabs(area)/ 2;}
   void_area +=tess_area-g_area;
  density=g_area/tess_area;
  std::cout << "tess_area = "<<tess_area<<" grain area = " <<g_area<<" package density= "<<density;}
