g++ tess_area.cpp -o bin/tess_area
g++ table_voronoi.cpp -o bin/table
cd bin/
for i in $(find . -iname "*.dat"); do awk '{print $3 " " $4}' $i > $i.txt; done
for i in $(find . -iname "*.txt" | awk -F/ '{print $0}' | sort); do ./table $i; ./tess_area; done

