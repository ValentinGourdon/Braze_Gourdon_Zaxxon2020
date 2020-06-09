# Prerequis
Download SFML 2.5.1 here https://www.sfml-dev.org/download-fr.php
Install it on your computer

# Visual Studio parameters
Right-click on the project -> Properties.
In VC++ Directories add to "Include directories" the SFML include path (ex : C:\SFML-2.5.1\include)
and add to "Library directories" the SFML lib path. (ex : C:\SFML-2.5.1\lib)

Still in the project's Properties click on Linker -> Input.
Add winmm.lib as an additionnal dependencies. (Click on the arrow and on modify to open a new window. Type winmm.lib in the field and click on OK). 

Zaxxon2020 is ready to start.
