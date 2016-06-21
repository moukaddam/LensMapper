# LensMapper

Intro :
- A set of classes to handle the experimental data of measured magnetic field 
and simulated magnetic field (using comsol) and produce useful graphs for 
comparaison. The project also contains a routine to minimise the chi2 
to determine the magnetization of the permanent magnets used in the SPICE lenses

How to : 
- Typically several comsol files are produced by varying the magnetisation 
parameters of the inner and outer magnets, the code is executed manually on 
every comsol file in the list. The chi2 is printed at the end of the execution. 

Execution :
- input: 2 (+1) ascii files with a strict format: 
	+ simulated magnetic field
	+ measured experimental field
    + (measured experimental background)
    
e.g : 
./bin/mapperTool ./input/SPICEField3D.v51.3.21.TABLE ./input/ExpFieldLEL_2016Jun14_Inspect.dat  (no background subtraction) 
./bin/mapperTool ./input/SPICEField3D.v51.3.21.TABLE ./input/ExpFieldLEL_2016Jun14_Inspect.dat ./input/ExpBGroundField_2016Jun14.dat 

Output : 
- output: a root file with a list of graphs and histograms determined by the user	

--------------------------------------------------------------------------------
General Notes :

Hardware : 
- The mapper has 8 levels corresponding to the z direction, 
	+level 8 is the closest to the origine (reaction target) 
	+level 1 is quite shalow (in negative Z) and require a larger dimension 
	setting in the comsol simulation. Otherwise the generated chi2 = "inf". 
	
Measurment : 
- The magnetic field of earth (in Vancouver) is ~0.8 Gaus in magnitude.
Depending on the orientation of the mapper the contribution on every sensor 
is different, hence it's useful to make a "No Energy Lens" measurment 
and produce a background file. 
- When the cylotron will eventually add another contribution when it's ON.	
