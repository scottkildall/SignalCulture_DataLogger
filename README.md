MIS Content Manager
written by Scott Kildall
for the Exploratorium
skildall@exploratorium.edu

**Configuration**
Mac and Windows, should cross-compile
For Mac, use JRE 1.6
For Win, use JRE 1.8

** BitBucket Configuration in Eclipse **
http://crunchify.com/how-to-configure-bitbucket-git-repository-in-you-eclipse/

**Overview**
This is a Java application, which uses the Processing libraries to display content related to live microscopy samples from the Bio lab at the Exploratorium. Samples might include, but are not limited to ZebraFish, Stem Cells and Amoeba.

Installing Java for Windows
This requires the JRE (Java Runtime Environment)
Java Install for Windows
http://www.oracle.com/technetwork/java/javase/downloads/index.html
Java SE 8u45
Click on JRE
Download Windows x64 for the .exe file
After downloading, run the installer

**Exporting**
The application will be a runnable JAR. The host machine requires JRE (Java Runtime Environment) for this to work properly. JDK is only used for development, so this is note needed.

To export a JAR in Eclipse:
File->Export
Under Java, choose Runnable JAR, click NEXT

**Playig QuickTime videos**
You need to import these three .jar files, in the Processing libs
jne.jar
gstreamer-java.jar
video.jar


Then: import all of the native .dll (Win)and .dylib (Mac) files that are in these subdirectories. Right now, I have this set up so that we these are in the root project folder, but eventually they should be in the appropriate sub-directory.

*important* choose Package required libraries into generated JAR

** Installing Eclipse Development Environment **


** Installing Eclipse Development Environment **
- First install JRE
- Then, download and install Eclispe for Java
- Import GIT repo using these instrucitons
http://crunchify.com/how-to-configure-bitbucket-git-repository-in-you-eclipse/
- Setup run/launch configurations
MIS_ContentManager will read in JSON files for configuration and will work in tandem with other MIS-based applications. It uses the Processing libraries to handle all drawing.

** Current Bugs **
ControlP5 list box won't issue values after adding items. Try to solve or re-generate control after each one?

** Quick Shortcuts (copy and paste into code) **
gc.errorLogger.writeLine("error");
gc.statusLogger.writeLine("status");
