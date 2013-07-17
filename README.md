SNPduo
======

## Background
The [snpduo tool](http://www.plosone.org/article/info:doi/10.1371/journal.pone.0006711) is designed to calculate the global identity-by-state statistics for pairs of samples. It was originally developed in the [lab of Dr. Jonathan Pevsner](http://www.pevsnerlab.kennedykrieger.org) at the Kennedy Krieger Institute in Baltimore.

SNPduo can aid in the discovery of undocumented or misdocumented familial relationships, identical samples, sample mislabeling and population structure in SNP genotyping datasets. For compatibility with existing software and data formats, snpduo is designed to accept many of the same formats as [PLINK](http://pngu.mgh.harvard.edu/~purcell/plink/download.shtml). The software supports data with many individuals (thousands in a single experiment) and many markers (1M+). Summaries of the IBS between individuals are useful because they do not rely on calculations of identity-by-descent (IBD) probability, are more computationally tractable and require no prior familial relationship information. An additional IBS state described by the tool is IBS2* ("IBS two star"), where two heterozygous calls are compared to each other.

## Installation

#### Linux
Unpack the code archive. Edit the Makefile CXXFLAGS as needed, i.e. to include -m64 or specific architecture specifications. Running the 'make' command will build the software. The software can be installed with 'make install' or removed with 'make uninstall'.

#### Windows
Windows compilation is best performed manually and requires a C++ compiler. Unpack the archive and navigate to the code directory. Compile using 'g++ -O3 -Wall -o snpduo *.cpp'. Include any other appropriate C++ flags. If desired copy to a folder in the system PATH for easy command-line access. The '-m64' flag should be included if 64-bit compilation is desired.

## Input Formats
There are multiple input formats that SNPduo can read. All formats should be white-space delimited (space, tab, or mixed) and end in newline ‘\n’ characters. Lines ending only in ‘\r’ WILL NOT read properly, though ‘\r\n’ will. This may require altering file format on Macs.

#### PED/MAP Format
The ped/map format requires two separate files: a map file with information for each marker on a separate line and a ped file with information about each individual on a line. The order of markers in the ped file and in the map file MUST be the same.

###### MAP File
The '.map' file contains the information about the location of markers in the dataset. The required columns are chromosome (1-26, X, Y, M are acceptable), RSID or SNP ID, centiMorgan location, and physical location. Only the chromosome and position are used in SNPduo. The other columns are imported for PLINK compatibility. The map file can contain only the chromosome, RSID and physical position if the “--map3” switch is specified at run time.

###### PED File
The '.ped' file contains sample information, their familial relationships and their marker genotypes. The required fields are family id, individual id, mother id, father id, sex, and disease status. Absent father and mother information should each be reported as 0 (and thus you shouldn’t use 0 as an individual ID). Sex should be specified as 0=unknown, 1=male, 2=female. Disease status should be specified as an integer as well. After the disease status should be the genotypes. Individual SNP alleles can be separated by white space or not, i.e. an “AA” genotype can be represented as “AA” or “A A”. The parsing algorithm will treat both correctly. Uncalled markers have the genotype “00”, and should be coded as either “00” or “0 0” depending on the overall file format. NC, NoCall or other variations will not work.

#### TFAM/TPED Format
The tfam and tped files transposed files that contain the same information that would be contained in a map/ped file combination. A tfam file contains individual information on familial relationships and disease information, where one line represents the information for one individual. A tped file combines the map information and marker information.

###### TFAM File
A tfam file contains the first six columns of a ped file (family id, individual id, father id, mother id, sex, disease status). The same coding requirements apply to the tfam file as did to the ped file. One individual should be included per line.

###### TPED File
The first four columns of a tped file are the same as a map file (chromosome, rsid, cM position, physical position). The following columns are whitespace separated genotypes. Whitespace consists of tabs and spaces in this context. Individual alleles can be grouped into a genotype or separated by whitespace also. The information for one marker for all individuals is on one line. The cM position can again be left out if “--map3” is specified. The order of individual genotypes in the tped file MUST be in the same order as the individuals in the tfam file.

## Usage

#### Data Input

###### PED/MAP
Ped/map files with the same basename are specified for input as “--file filenamebase”. This will automatically open both the “filenamebase.map” and “filenamebase.ped”. The ped and mapfiles can be specified separately using “--ped pedfilename.ped --map mapfilename.map”.

###### TPED/TFAM
Transposed format is read similarly as “--tfile filenamebase”, which will automatically read “filenamebase.tped” and “filenamebase.tfam”. They also can be specified separately as “--tped tpedfile.tped --tfam tfamfile.tfam”.

###### PLINK Genome Input
An alternative input is the “.genome” output file from PLINK. Use “--genome filename.genome” to read in the PLINK genome output IBS data. Expected relationships cannot be derived using this method at this time.

#### Recoding data

###### Input conversion
The standard map/ped and tped/tfam files can be interconverted. To convert a tped/tfam fileset into ped/map use “--tfile myfilenamebase --recode”. For coding ped/map to tfam/tped “--file myfilenamebase --recode --transpose” is used instead. In all cases the output separates alleles by whitespace, i.e. “AA” genotype becomes “A A” in the file to maintain compatibility with PLINK. Similarly the genetic position is always printed. If it was not specified in the input 0 is used at all positions for the genetic distance.

###### SNPduoWeb Visualization Tool
The web-based SNPduo visualization tool ([SNPduoWeb](https://github.com/RobersonLab/snpduoweb)) can use data exported from the command-line SNPduo IBS summarization tool. To export data from ped/map to a SNPduoWeb compatible format use “--file myfilenamebase --recode --webDuo”. It is critical to note that the conversion to SNPduoWeb format recodes the genotypes to an AA/AB/BB convention, regardless of the input. The ‘A’ allele is the allele with the highest frequency in the input file, and the ‘B’ allele has the minor allele frequency in the input file.

#### Analysis Options

###### Output file names
There are several different analysis options in SNPduo. The output basename for all output files (including recoded ped/map/tped/tfam files) defaults to “snpduo”. To specify a different output base name invoke “--out myoutcustombase” at run time.

###### IBS counts only
The snpduo “--counts” option finds the count of each IBS state (IBS0, IBS1, IBS2, IBS2\*) for each pairwise comparison to a comma-separated text file with the extension “.count”.

###### IBS summaries
The summary calculations (described below) can be printed to a file (.summary) as using the “--summary” switch. This summary information is useful for downstream analysis using direct plotting, clustering analysis, or principal components analysis. Summary information calculated includes the mean of identity-by-state, the standard deviation of identity-by-state, the informative percent of SNPs: (IBS2\* + IBS0) / (IBS0 + IBS1 + IBS2), IBS2\* percent: (IBS2\*) / (IBS0 + IBS1 + IBS2) and IBS2\* percent of informative: (IBS2\*) / (IBS0 + IBS2\*).

## Examples

Recoding a ped/map to a tped/tfam. Input myInput.ed and inputMapFile.map. Output newBaseName.tfam and newBaseName.tped.

```snpduo --out newBaseName --ped myInput.ped --map inputMapFile --recode```

Get IBS counts only. Inputs are data.ped and data.map. Output getCounts.count

```snpduo --out getCounts --counts --file data```

Get IBS summary information. Inputs are data.tfam and data.tped. Output test.count and test.summary.

```snpduo --out test --counts --summary --tfile data```
