Enactive Torch v.3.0 

Code for Data collection


The code in this repository is for research and data collection using an enactive torch (ET). 
It is not for commercial use.

For details on the ET see http://enactivetorch.wordpress.com/ and http://www.creative-robotics.com/et

This repository and its contents are not an offical part of the ET project.It is developed and maintained
independently by individuals interested in the research possiblitites of the ET.

The code and hardware for the ET are provided under creative commons license: http://creativecommons.org/licenses/by-nc-sa/3.0/

This repository has been set up to keep track of variations of ET code for research and data collection purposes.

It includes the original code for the ET v. 3.0 as well as the current form of the base data collection code.

The data collection code (ETDatacollection) is modified to output sensor readings at a faster sample rate along with a timestamp.
The button has also been repurposed for labeling data. Changes will be tracked with commit comments or in the 
code itself. 

Housekeeping:

When testing out code for new experimental setups use the 'Test Versions' folder to store your code. Once the code is stable
move it to the Code_Variants folder. Please either explain the functionality of your code in the initial code comments or add
a readme to its folders. If your variant code does not affect base functioning of the torch it can be merged with the ETDatacollection
version. Base functioning includes:

1) mapping the full range of the long range sensor to the motor 
2) printing each sensor reading at a speed of at least 120 htz. 
3) torch runs uninterupped for at least 60 minutes




 
