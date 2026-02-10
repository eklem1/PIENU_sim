# PIENU MC simulation

Code to run the PIENU full detector MC, as well as the conversion 'MC to data'.

## MC
Run macros: pienu_2012.mac & pimue_2012.mac
* Main difference to change between decays is `/radiative/mu_or_e 0` for pienu, `1` for pimue
* Set `/tracking/verbose 2` to get full tracking printed to terminal - use only for small runs

### Compiling
    source setup.sh
    Then inside pienu_build/:
        cmake ..
        make

### Versions
There are some version of MC building files used to run different versions of the MC.
* Geometry:
  * WorldConstructor.ccAir : Sets the NaI calo, Al layer and Al-oxide and to air material - essentially removing them
  * WorldConstructor.ccCsIAir : Additionally also sets the CsI crystals to air

### MCtoData
This code takes the raw root file from the MC, and converts it into a root file that imitates the real data root file, that can then be used for analysis.

Main code: macro/MCtree.cc

### Compiling
    source setup_macro.sh
    Then inside macro/:
        make


