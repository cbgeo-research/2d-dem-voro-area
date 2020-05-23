g++ tess_area.cpp -o ./tess_area
g++ table_voronoi.cpp -o ./table
awk '{print $2}' DEM000002.dat> Radius.data
for i in $(find . -iname "*.dat"); do awk '{print $3" , " $4}' $i > $i.txt; done
for i in $(find . -iname "*.txt" | awk -F/ '{print $0}' | sort); do ./table $i; ./tess_area $i; done
for i in $(find . -iname "*.ps"); do convert -colorspace sRGB -density 600 $i -background white -flatten -resize 1863x2636 -units pixelsperinch -density 224.993 $i.png; done


