# Rod of Asclepius (RoA)
RoA is a proof-of-concept application that provides visual analytics support for deconfounded causal inference in observational studies.

## Dependencies
* The system was designed on a Linux environment, so it is also easiest to use a Linux environment. However, on Windows, some alternative options are to install Windows Substem for Linux (see https://learn.microsoft.com/en-us/windows/wsl/install) or Cygwin (see https://www.cygwin.com/?ref=itsfoss.com).
* The application expects the communicate with a running instance of Plumber hosting the R code (see https://www.rplumber.io/).
* The Plumber instance communicates with a running PostgreSQL database (see https://www.postgresql.org/).
* RoA depends on the Open Graph Drawing Framework (OGDF) library (see https://ogdf.uos.de/releases/). A pre-compiled version (.so) for Linux is added in the lib folder.

Plumber and PostgreSQL need to be installed inside a working docker environment. See https://www.docker.com/ for instructions on how to set up this environment. Next, use the build and management scripts in the docker folder to build the docker images and run instances of them before starting RoA.
	* docker/postgresql/install.sh
	* docker/postgresql/up.sh
	* docker/rbase/cleanbuild.sh
	* docker/plumber/cleanbuild.sh
	* docker/plumber/up.sh (and down.sh to stop)

## Building RoA
The application itself is a QT 6.4 application. To build it, you need to install the Qt framework, which can be downloaded at https://www.qt.io/download-open-source. After installation, use the QT Creator IDE to open asclepius.pro project file in the cpp folder. Please set the build and configuration directories to the bin folder. Then hit play.

## Running RoA
* In the settings dialog on the "R Server tab", please specify the correct hostname+port of the Plumber instance hosting the R code.
* Create a new project in RoA and specify the database name used in the PostgreSQL instance.

## Managing data
To add data to the PostgreSQL instance, use a tool like DBeaver (see https://dbeaver.io/). Connect to the PostgreSQL instance (using the credentials used in the small docker scripts earlier). Next, create a new database and two new tables, one called "main" and another called "types". Put your data in the main table and specify the types (numeric, integer, factor) for each variable in the main table in the types table. The types table needs to have two columns for this, namely "variable" and "type". For example, data, check the data folder.

## Virtual appliance