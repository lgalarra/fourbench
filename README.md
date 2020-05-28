# fourbench
A benchmark for metadata-augmented RDF datasets. It consists of two tools:

- A data generator that takes an RDF file as input and augments it with synthetic provenance information.

- A query generator that builds SPARQL queries on provenance-augmented datasets

## Compiling

### Data generator (written in C++)

#### Requirements

- gcc
- automake
- boostlib >= 1.32

#### Instructions to compile
- Clone this repository
- Run:
  - cd PATH_TO_REPOSITORY_CLONE
  - ./configure
  - make

#### Instructions to run 

The executable lies in src/4Bench. Run ./4Bench -h to see a description of the input arguments, which includes a configuration file .ini. If this file is not provided, the program uses a default configuration which is output at runtime. 

The file src/4bench_output provides the example of a typical output. 

### Query generator (written in Java)

#### Requirements
- Java >= 8
- Maven

#### Instructions to compile
- Run:
  - cd PATH_TO_REPOSITORY_CLONE/querygenerator
  - mvn compile

#### Instruction to run

- Run the class java.aau.cs.qweb.fourbench (Maven does not generate jar at the moment)
