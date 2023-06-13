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
The application itself is a QT 6.4 application (LGPL license details below). To build it, you need to install the Qt framework, which can be downloaded at https://www.qt.io/download-open-source. After installation, use the Qt Creator IDE to open asclepius.pro project file in the cpp folder. Please set the build and configuration directories to the bin folder. Then hit play. 	

## Running RoA
* In the settings dialog on the "R Server tab", please specify the correct hostname+port of the Plumber instance hosting the R code.
* Create a new project in RoA and specify the database name used in the PostgreSQL instance.

## Managing data
To add data to the PostgreSQL instance, use a tool like DBeaver (see https://dbeaver.io/). Connect to the PostgreSQL instance (using the credentials used in the small docker scripts earlier). Next, create a new database and two new tables, one called "main" and another called "types". Put your data in the main table and specify the types (numeric, integer, factor) for each variable in the main table in the types table. The types table needs to have two columns for this, namely "variable" and "type". For example, data, check the data folder.

## Virtual appliance
For convenience, a virtual appliance file is made available via DropBox that can be imported into software like VirtualBox. The file includes a pre-installed version of RoA inside a virtual machine running a Linux desktop. The virtual appliance can be downloaded here: https://www.dropbox.com/sh/99awsatroanuxsn/AAD-RPjOKS_673sxXLiZR9uba?dl=0 .

## Qt license obligation
* https://www.qt.io/licensing/open-source-lgpl-obligations

Developing with LGPL
* These four degrees of freedom are very important for the success of the open-source Qt Project, and it is important that all users of Qt under LGPL adhere to these and fully meet all the requirements set by the LGPL. It is recommended that a thorough legal analysis is conducted when choosing to use any open-source licenses. In many cases, the LGPL is a viable solution to use, but it is important that the freedoms of the LGPL are not restricted from the user of an application or device using an LGPL library such as Qt, which may be difficult to achieve in some use cases. For such cases the commercial licensing option is typically a better choice. The following requirements should be fulfilled when, for example, creating an application or a device with Qt using the LGPL: Complete corresponding source code of the library used with the application or the device built using LGPL, including all modifications to the library, should be delivered with the application (or alternatively provide a written offer with instructions on how to get the source code). It should be noted that the complete corresponding source code has to be delivered even if the library has not been modified at all.
* In case of dynamic linking, it is possible, but not mandatory, to keep application source code proprietary as long as it is “work that uses the library” – typically achieved via dynamic linking of the library. In case of static linking of the library, the application itself may no longer be “work that uses the library” and thus become subject to LGPL. It is recommended to either link dynamically, or provide the application source code to the user under LGPL.
* The user is allowed to change and re-link the library used in the application or device – including reverse engineering. With LGPLv3 it is explicitly stated that the user also needs to be able to run the re-linked binary, and that sufficient installation information must be provided. In practice, this forbids the creation of closed devices, also known as tivoization.
* If the application or device is not fully following all requirements of the LGPL, it is not allowed to be distributed at all. This includes, for example, possible patent license restricting distribution of the application in which case there is no right to distribute.
* The freedoms provided by the LGPL license cannot be in any way negotiated upon or restricted from any of the recipients – i.e. it is not possible to make such terms for an application or a device using the LGPL library that would in any way restrict the rights provided by the LGPL. For example some means of distribution, such as online application stores, may have rules that are in conflict with LGPL, in which case those cannot be used with the LGPL licensing option of Qt.
* The user of an application or device created with LGPL library has to be notified of their rights by providing a copy of the LGPL license text to the user and displaying a prominent notice about using the LGPL library – i.e. it is not allowed to hide the fact the LGPL library is used.
* The LGPLv3 doesn’t forbid anyone from implementing any kind of DRM technology. However, if someone breaks the DRM, the license says that he will be free to distribute his software. For more details, see the (L)GPLv3 license FAQ.
* The LGPLv3 contains explicit patent clauses in order to prevent people from trying to enforce patent claims against other licensees of the open source libraries. The clauses are rather complex but extremely relevant especially for larger corporations with software patents. So, it is always recommended to go through the patent clauses carefully with a legal counsel to make sure this would not be an issue if deciding to use LGPLv3. For more information and details, please see the (L)GPLv3 FAQ.
This is not a complete list of LGPL requirements – please refer to the LGPL for complete list of requirements – it is always recommended to contact an experienced legal counsel to clarify detailed licensing suitability. Please note that The Qt Company does not offer legal guidance on whether or not a certain license is suited for your need, or if your application or device meets the LGPL requirements. In case there is any doubt whether or not the requirements of LGPL can be met in full, the commercial licensing option of Qt is often the best choice.

