# voro-ps

The only inputfile is the DEM000*.dat, you have to delete other files ended in .txt and .data under the folder. Then run
sh voro-area.sh

(As in the voro-area.sh, the first step is to compile *.cpp to generate excutable file table and tess_area;
Print the radii of grains to Radius.data;
Print the x and y coordinates of the grains to DEM000*.dat.txt, noted that it should be a comma-delimited parser;
Sort the DEM000*.dat.txt and run "table DEM000*.dat.txt" to get the Vertices.data, Degree.data, and Connectivity.data for each inputfile;
Run "tess_area DEM000*.dat.txt" to compute the packing density as well as plotting the tesslation;
Converting the postscripts to png files)
