
# Bayesian Network Learning for Medical Diagnosis
## Overview
Course Assignment 4 for 2302-COL333
This project focuses on learning parameters for a Bayesian Network that models the relationships between diseases and observed symptoms in medical diagnosis. The goal is to handle missing values in health records and compute accurate parameters for effective diagnosis.

## Structure
### Bayesian Network
The Bayesian Network structure includes eight diagnoses: hypovolemia, left ventricular failure, anaphylaxis, insufficient analgesia, pulmonary embolus, intubation, kinked tube, and disconnection. Observable nodes include CVP, PCWP, History, TPR, Blood Pressure, CO, HR BP, HR EKG, HR SAT, SaO2, PAP, MV, Min Vol, Exp CO2, FiO2, and Pres.

### Input Format
Utilize the alarm.bif network with most probability values as -1.
Datafile (records.dat) contains patient records with features in the same order as the .bif network.
### Output Format
Replace all -1s in the conditional probability tables with computed probability values (up to four decimal places). Output a complete alarm.bif network.

## How to Use
Clone the repository.
Run the provided .bif parser.
Use the learning algorithm to handle missing values and compute parameters.
Output the learned network for medical diagnosis based on health records.

## Files Provided
records.dat (Dataset file)
Startup code file
Format checker for output validation
alarm.bif (structure to learn)
gold_alarm.bif (true parameters)
