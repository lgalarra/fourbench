# fourbench
A benchmark for metadata-augmented RDF datasets

## Compiling

### Data generator (written in C++)

#### Requirements

- gcc
- automake
- boostlib >= 1.32

#### Instructions
- Clone this repository
- Run:
  - cd PATH_TO_REPOSITORY_CLONE
  - ./configure
  - make

### Query generator (written in Java)

#### Requirements
- Java >= 8
- Maven

#### Instructions
- Run:
 - cd PATH_TO_REPOSITORY_CLONE/querygenerator
 - mvn compile
